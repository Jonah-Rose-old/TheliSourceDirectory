#include <cstring>  // for strcmp()
#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
//#include "statistics.h"

using namespace std;

float calc_mean(vector<float> const &data, const long start, 
		const long end, const int masked, const float mask);
float calc_noise(vector<float> const &data);
float calc_sum(vector<float> const &data);

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -o output_image \n";
    cerr << "           -b binsize (or different binning along x and y: binx biny)]\n";
    cerr << "           [-m mask value]\n";
    cerr << "           [-n (noise map)]\n\n";
    cerr << "           [-s straight sum, no mean, no outlier rejection]\n\n";
    cerr << "  PURPOSE: Creates a binned image using outlier rejection.\n";
    cerr << "           Pixels with values of -m can be ignored on an optional basis.\n";
    cerr << "           If the -n option is given, we assume we are binning a noise map.\n\n";

    exit(1);
  }
}


int main(int argc, char *argv[])
{
  int n, m, nb, mb, binx, biny, it, jt, masked, noisemap, straightsum = 0;
  long i, j, k, bsq;
  float mask;
  string input_image, output_image;
  double crpix1, crpix2, cd11, cd12, cd21, cd22;

  masked = 0;
  mask = 0.;
  binx = 1;
  biny = 1;
  noisemap = 0;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i],"-i") == 0) input_image = argv[++i];
      if (strcmp(argv[i],"-o") == 0) output_image = argv[++i];
      if (strcmp(argv[i],"-m") == 0) {
	mask = atof(argv[++i]);
	masked = 1;
      }
      // -b can take either one or two numbers
      if (strcmp(argv[i],"-b") == 0) {
	binx = atoi(argv[++i]);
	if (i <= argc-2) {
	  if (atoi(argv[i+1]) == 0) biny = binx;
	  else biny = atoi(argv[++i]);
	}
	else biny = binx;
      }
      if (strcmp(argv[i],"-n") == 0) noisemap = 1;
      if (strcmp(argv[i],"-s") == 0) straightsum = 1;
    }
  }

  if (binx < 1 || biny < 1) {
    cerr << "ERROR: Invalid binsize: " << binx << "x" << biny << endl;
    return 1;
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  nb = floor(n/binx);
  mb = floor(m/biny);
  
  bsq = binx * biny;
  
  vector<float> image_binned(nb*mb,0);
  vector<float> data_out(nb*mb,0);
  vector<float> chunk(bsq,0);

  // Do the binning
  for (j=0; j<mb; j++) {
    for (i=0; i<nb; i++) {
      k = 0;
      for (jt=j*biny;jt<(j+1)*biny;jt++) {
	for (it=i*binx;it<(i+1)*binx;it++) {
	  chunk[k++] = image.data[it+n*jt];
	}
      }
      if (straightsum == 0) {
	// sort the array, then take the middle 80% values (outlier rejection)
	sort(chunk.begin(), chunk.end());
	if (noisemap == 0) 
	  image_binned[i+nb*j] = calc_mean(chunk, 0.1*bsq, 0.9*bsq, masked, mask);
	if (noisemap == 1)
	  image_binned[i+nb*j] = calc_noise(chunk);
      }
      else {
	  image_binned[i+nb*j] = calc_sum(chunk);
      }
    }
  }
  
  for (k=0; k<nb*mb; k++) {
    data_out[k] = image_binned[k];
  }
  
  writeImage(output_image, input_image, data_out, nb, mb, call);

  crpix1 = image.readKeyFailsafe("CRPIX1", nb/2.) / binx;
  crpix2 = image.readKeyFailsafe("CRPIX2", mb/2.) / biny;
  cd11   = image.readKeyFailsafe("CD1_1", -0.001) * binx;
  cd12   = image.readKeyFailsafe("CD1_2", 0.000) * binx;
  cd21   = image.readKeyFailsafe("CD2_1", 0.000) * biny;
  cd22   = image.readKeyFailsafe("CD2_2", 0.001) * biny;

  // update header
  myFITS imageout(output_image, false, "Write");
  imageout.addKey("CRPIX1", crpix1, "Reference pixel of first axis");
  imageout.addKey("CRPIX2", crpix2, "Reference pixel of second axis");
  imageout.addKey("CD1_1", cd11, "Linear projection matrix");
  imageout.addKey("CD1_2", cd12, "Linear projection matrix");
  imageout.addKey("CD2_1", cd21, "Linear projection matrix");
  imageout.addKey("CD2_2", cd22, "Linear projection matrix");

  return 0;
}

//************************************************************
float calc_mean(vector<float> const &data, const long start, 
		const long end, const int masked, const float mask)
{
  long i, numpixels, naned;
  float mean;
  
  mean = 0.;
  numpixels = 0;
  naned = 0;
  
  for (i = start; i < end; i++)  {
    if (masked == 1) {
      if (data[i] != mask && isnan(data[i]) == 0) {
	mean += data[i];
	numpixels++;
      }
      if (isnan(data[i]) != 0) naned++;
    }
    else {
      if (isnan(data[i]) == 0) {
	mean += data[i];
	numpixels++;
      }
      else naned++;
    }
  }
  
  if (numpixels != 0.)
    mean /= numpixels;
  else {
    if (masked == 0) mean = 0.;
    if (masked == 1) mean = mask;
  }
  
  if (naned > (end-start) / 2)
    mean = sqrt(-1); // return NaN
  
  return (mean);
}


//************************************************************
float calc_noise(vector<float> const &data)
{
  ulong i;
  float noise;
  
  noise = 0.;

  for (i=0; i<data.size(); i++) {
    noise += data[i]*data[i];
  }
  
  return (sqrt(noise));
}


//************************************************************
float calc_sum(vector<float> const &data)
{
  ulong i;
  float sum;
  
  sum = 0.;

  for (i=0; i<data.size(); i++) {
    sum += data[i];
  }
  
  return sum;
}
