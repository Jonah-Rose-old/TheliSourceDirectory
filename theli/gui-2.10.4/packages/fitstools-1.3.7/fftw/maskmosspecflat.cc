#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"
#include "histogram.h"
#include "fitgauss1d.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image" << endl;
    cerr << "           -o output_image" << endl;
    cerr << "           -c dispersion direction" << endl;
    cerr << "          [-s skip n pixels at the spatial dimension edges (default: 3)]" << endl;
    cerr << "          [-l lower intensity cutoff (automatic if unspecified)]" << endl;
    cerr << "          [-r number of pixels cut (default: 0)]" << endl;
    cerr << "          [-k kappa value for sigma clipping (default: 4)]" << endl;
    cerr << "          [-v (verbose)]\n" << endl;
    exit(1);
  }
}

bool verbose = false;

void filter_image(const vector<float> &, vector<float> &, const string,
		  const long, const long);
void make_cutoff(const vector<float> &, vector<float> &, const float,
		 const string, const long, const long, const float, const long);
float get_lowercutoff(const vector<float> &, const float,
		      const string, const long, const long);
void report(vector<float> &, const string, const long, const long);

//**********************************************************************
int main(int argc, char *argv[])
{
  long n, m, i, restriction = 0, skip = 3;
  string input_image, output_image, dispdirection, call;
  float kappa = 4.0, manualcutoff = 0.0;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'c': dispdirection = argv[++i];
	break;
      case 'r': restriction = atol(argv[++i]);
	break;
      case 's': skip = atol(argv[++i]);
	break;
      case 'k': kappa = atof(argv[++i]);
	break;
      case 'l': manualcutoff = atof(argv[++i]);
	break;
      case 'v': verbose = true;
	break;
      }
    }
  }

  checkfile(input_image);
  call = programcall(argv, argc);

  // read the FITS data block
  myFITS image_in(input_image);
  n = image_in.naxis1;
  m = image_in.naxis2;

  // estimate the lower cutoff
  double lowercutoff = get_lowercutoff(image_in.data, manualcutoff, 
				       dispdirection, n, m);

  vector<float> data_filtered(n*m,0);

  // apply a gradient filter to the data
  filter_image(image_in.data, data_filtered, dispdirection, n, m);

  // do some statistics on the filtered image and determine useful cutoffs
  // to keep the true edges, only
  make_cutoff(image_in.data, data_filtered, kappa, dispdirection, n, m, 
	      lowercutoff, skip);

  report(data_filtered, dispdirection, n, m);

  writeImage(output_image, input_image, data_filtered, call);

  vector<float> comparison(n*m,0.);
  for (long j=0; j<m; j++) {
    for (long i=0; i<n; i++) {
      data_filtered[i+n*j] = data_filtered[i+n*j] > 0 ? 65535 : 0.;
      if (dispdirection.compare("y") == 0) {
	if (j<m/2) comparison[i+n*j] = image_in.data[i+n*j];
	else comparison[i+n*j] = data_filtered[i+n*j];
      }
      if (dispdirection.compare("x") == 0) {
	if (i<n/2) comparison[i+n*j] = image_in.data[i+n*j];
	else comparison[i+n*j] = data_filtered[i+n*j];
      }
    }
  }

  writeImage("mapcomp.fits", input_image, comparison, call);

  return 0;
}

//********************************************************
// filter the image with a linear gradient filter
//********************************************************
void filter_image(const vector<float> &data, vector<float> &data_filtered,
		  const string dispdirection, const long n, const long m)
{
  // linear gradient filter
  vector<float> filter(3,0.);
  vector<float> filter1(2,0.);

  // 3-element filter
  filter[0] =  1.;
  filter[1] =  0.;
  filter[2] = -1.;

  // 2-element filter
  filter1[0] =  1.;
  filter1[1] = -1.;

  // filtering
  float sum = 0;
  long i = 0;
  long j = 0;

  if (dispdirection.compare("x") == 0) {
    for (j=1; j<m-1; j++) {
      for (i=0; i<n; i++) {
	sum  = data[i+n*(j-1)];
	sum += data[i+n*(j+0)];
	//	sum += data[i+n*(j+1)];
	data_filtered[i+n*j]  = data[i+n*(j-1)] * filter1[0];
	data_filtered[i+n*j] += data[i+n*(j+0)] * filter1[1];
	//	data_filtered[i+n*j] += data[i+n*(j+1)] * filter[2];
	data_filtered[i+n*j] /= sum;
      }
    }
  } else {
    for (j=0; j<m; j++) {
      for (i=1; i<n-1; i++) {
	sum  = data[i-1+n*j];
	sum += data[i+0+n*j];
	sum += data[i+1+n*j];
	data_filtered[i+n*j]  = data[i-1+n] * filter[0];
	data_filtered[i+n*j] += data[i+0+n] * filter[1];
	data_filtered[i+n*j] += data[i+1+n] * filter[2];
	data_filtered[i+n*j] /= sum;
      }
    }
  }
}

//********************************************************
// kappa-sigma clipping for the filtered image
//********************************************************
void make_cutoff(const vector<float> &data, vector<float> &data_filtered, 
		 const float kappa, const string dispdirection, 
		 const long n, const long m, const float lowercutoff,
		 const long skip)
{
  vector<float> sample;

  // First, extract a cut perpendicular to the dispersion direction
  // near the center of the image
  if (dispdirection.compare("x") == 0) {
    for (long j=0; j<m; j++) 
      sample.push_back(data_filtered[n/2+n*j]);
  } else {
    for (long i=0; i<n; i++) 
      sample.push_back(data_filtered[i+n*m/2]);
  }

  // make a histogram for the sample (take the central 90% so we don't get
  // confused by high outliers)
  float binsize = 0.0002;     // empirically determined
  float fraction = 0.90;
  histogram histo(sample, binsize, fraction);

  // fit a Gaussian to the distribution
  double b_init = histo.peakval();
  double x0_init = histo.peakpos();
  double sigma_init = histo.fwhmguess() / (2.*sqrt(2.*log(2.)));
  vector<float> fitresult(3,0.);
  do_gauss1d_fit(histo.bins, histo.counts, b_init, x0_init, sigma_init, 
		 fitresult, verbose);

  // Mask the image
  float min_allowed_val = fitresult[1] - kappa*fitresult[2];
  float max_allowed_val = fitresult[1] + kappa*fitresult[2];

  long k = 0;
  for (long j=0; j<m; j++) {
    for (long i=0; i<n; i++) {

      // thresholding
      if (data_filtered[k] < min_allowed_val || 
	  data_filtered[k] > max_allowed_val || 
	  data[k] < lowercutoff)
	data_filtered[k] = 0.;
      else
	data_filtered[k] = 1.;

      // optional edge filtering
      if (dispdirection.compare("x") == 0) {
	if (j<skip || j>=m-skip) data_filtered[k] = 0.;
      }
      if (dispdirection.compare("y") == 0) {
	if (i<skip || i>=n-skip) data_filtered[k] = 0.;
      }

      k++;
    }
  }

  // Mask orphans (1 pixel wide unmasked areas)
  for (long j=1; j<m-1; j++) {
    for (long i=1; i<n-1; i++) {
      if (dispdirection.compare("y") == 0) {
	if (data_filtered[i-1+n*j] == 0. && 
	    data_filtered[i+n*j] == 1. &&
	    data_filtered[i+1+n*j] == 0.) {
	  data_filtered[i+n*j] = 0.;
	}
      }
      else {
	if (data_filtered[i+n*(j-1)] == 0 && 
	    data_filtered[i+n*j] == 1. &&
	    data_filtered[i+n*(j+1)] == 0.) {
	  data_filtered[i+n*j] = 0.;
	}
      }
    }
  }
}


//*****************************************************************
// what are the typical brightest and darkest values of the spec flat;
//*****************************************************************
float get_lowercutoff(const vector<float> &data, const float manualcutoff, 
		      const string dispdirection, const long n, const long m) {

  // First, extract a cut perpendicular to the dispersion direction
  // near the center of the image
  vector<float> sample;
  if (dispdirection.compare("x") == 0) {
    for (long j=0; j<m; j++) 
      sample.push_back(data[n/2+n*j]);
  } else {
    for (long i=0; i<n; i++) 
      sample.push_back(data[i+n*m/2]);
  }

  // sort the data
  sort(sample.begin(), sample.end());
 
  // get the means of the uppermost and lowermost 1% values
  vector<float>bsample;
  vector<float>fsample;
  for (unsigned long i=0.99*sample.size(); i<sample.size(); i++)
    bsample.push_back(sample[i]);
  for (unsigned long i=0; i<0.01*sample.size(); i++)
    fsample.push_back(sample[i]);

  float typical_faint  = mean(fsample, fsample.size());
  float typical_bright = mean(bsample, bsample.size());

  // automatic cut-off: 10% of the brightest value
  float lowercutoff = 0.1*typical_bright < typical_faint ? 
		      typical_faint : 0.1*typical_bright;

  // manual override:
  if (manualcutoff != 0.) lowercutoff = manualcutoff;

  if (verbose)
    cout << "Typical faintest and brightest values, and lower cutoff:" << 
      typical_faint << " " << typical_bright << " " << lowercutoff << endl;

  return lowercutoff;
}


//********************************************************
// report slit positions
//********************************************************
void report(vector<float> &data_filtered, const string dispdirection, 
	    const long n, const long m)
{
  long maxpos;
  if (dispdirection.compare("x") == 0) maxpos = n;
  else maxpos = m;

  vector<float> sample;
  if (dispdirection.compare("x") == 0) {
    for (long j=0; j<m; j++) 
      sample.push_back(data_filtered[n/2+n*j]);
  } else {
    for (long i=0; i<n; i++) 
      sample.push_back(data_filtered[i+n*m/2]);
  }

  // step through the sample and ID the slit positions
  long i0=0, i1=0, i2=0;
  long begin=0, end=0;
  for (unsigned long i=0; i<sample.size(); i++) {
    if (i>0) i0 = i-1;
    if (i<sample.size()-1) i2=i+1;
    i1=i;

    if (sample[i0]==0 && sample[i1]==1) begin=i1;
    if (sample[i1]==1 && sample[i2]==0) {
      end = i1;
      if (dispdirection.compare("x") == 0)
	cout << 1 << " " << maxpos << " " << begin+1 << " " << end+1 << endl;
      else
	cout << begin+1 << " " << end+1 << " " << 1 << " " << maxpos << endl;
    }
  }
}

