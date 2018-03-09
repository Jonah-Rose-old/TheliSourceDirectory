#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -o output_image \n";
    cerr << "           [-l min_value (default: 20000)]\n";
    cerr << "           [-m min max (manual thresholds, override)]\n";
    cerr << "           [-r additional blooming margin (default: 0)]\n";
    cerr << "           [-s smooth]\n";
    cerr << "           [-v verbose]\n\n";
    cerr << "    Detects blooming spikes and creates a mask.\n";
    cerr << "    The characteristic blooming value is automatically\n";
    cerr << "    detected. The minimum value is used to reject pixels\n";
    cerr << "    with smaller values (usually sky background) from the\n";
    cerr << "    statistics to speed up calculations.\n";
    cerr << "    The range of pixel values in a blooming spike is determined\n";
    cerr << "    automatically. It can be extended by the additional margin.\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  long n, m, nhist, minindex_int, maxindex_int, verbose, smooth;
  long min_int, max_int, manualoverride;
  long i, j, k, l, npix1, npix2;
  float minval, maxval, histstep, maxhist, range, minoverride, maxoverride;
  float bloomval, bloommin, bloommax, bloommaxindex;
  string input_image, output_image;

  minval = 20000.;
  verbose = 0;
  smooth = 0;
  minindex_int = 0;
  maxindex_int = 0;
  range = 0.;
  manualoverride = 0;
  minoverride = 0.;
  maxoverride = 0.;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'l': minval = atof(argv[++i]);
	break;
      case 'r': range = atof(argv[++i]);
	break;
      case 'm': minoverride = atof(argv[++i]);
	maxoverride = atof(argv[++i]);
	manualoverride = 1;
	break;
      case 'v': verbose = 1;
	break;
      case 's': smooth = 1;
	break;
      }
    }
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS image
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  histstep = 1000.;
  maxval   = max(image.data,n*m);
  if (maxval > 200000.) maxval = 200000.;

  // masking
  l = 0;
  for (k=0; k<n*m; k++) {
    if (image.data[k] < minval) image.data[k] = 0.;
    else l++;
  }

  npix1 = l;

  if (npix1 == 0) {
    cerr << "WARNING: No pixels above the minimum threshold (option -l) of " << minval << endl;
    cerr << "No blooming detected.\n";

    for (i=0; i<n*m; i++) image.data[i] = 0.;
    writeImage(output_image, input_image, image.data, call);
    return 0;
  }

  // detect value of blooming spike
  vector<float> pixels(npix1,0);
  vector<long> pixelcoords(npix1,0);

  l = 0;
  for (k=0; k<n*m; k++) {
    if (image.data[k] > 0) {
      pixels[l] = image.data[k];
      pixelcoords[l] = k;
      l++;
    }
  }

  if (manualoverride == 0) {
    
    nhist = ceil( (maxval - minval) / histstep);
    vector<float> hist(nhist,0);

    for (j=0; j<npix1; j++) {
      for (k=0; k<nhist; k++) {
	if (pixels[j] >= minval + histstep * k && 
	    pixels[j] < minval + histstep * (k+1)) {
	  hist[k]++;
	}
      }
    }
    
    if (smooth == 1) smooth_array(hist,nhist,2.);
    
    if (verbose == 1) {
      cout << "Histogram index --- ADU\n\n";
      for (i=0; i<nhist; i++) {
	cout << (long) hist[i] << " " << (long) (minval + i * histstep) << endl;
      }
    }
    
    maxhist = max(hist, nhist);
    bloommaxindex = maxindex(hist,nhist);
    bloomval = minval + bloommaxindex * histstep;
    
    // detect a reasonable dynamic range for the blooming spike
    max_int = (int) bloommaxindex;
    i = 1;
    while (hist[max_int] > 0.1 * maxhist && max_int < nhist) {
      max_int = (int) bloommaxindex + i;
      if (max_int>nhist-1) {
	max_int = nhist-1;
	break;
      }
      maxindex_int = max_int;
      i++;
    }
    
    min_int = bloommaxindex;
    i = 1;
    while (hist[min_int] > 0.1 * maxhist && min_int > 0) {
      min_int = (int) bloommaxindex - i;
      if (min_int<0) {
	min_int = 0;
	break;
      }
      minindex_int = min_int;
      i++;
    }
    
    bloommin = minval + (minindex_int - 1) * histstep - range;
    bloommax = minval + (maxindex_int + 1) * histstep + range;
    
    bloommax = bloomval + 1.5 * (bloommax - bloomval);
  }
  else {
    bloommin = minoverride;
    bloommax = maxoverride;
  }
  
  cout << "Blooming range: " << (long) bloommin << " " << (long) bloommax << endl;
  
  // keep only pixels with values within the blooming range
  l = 0;
  for (i=0; i<npix1; i++) {
    if (pixels[i] >= bloommin && pixels[i] <= bloommax) {
      l++;
    }
  }
  
  npix2 = l;
  
  cout << "Number of possibly bloomed pixels: " << npix2 << endl;
  
  vector<float> pixels2(npix2,0);
  vector<long> pixelcoords2(npix2,0);
  
  l = 0;
  for (i=0; i<npix1; i++) {
    if (pixels[i] >= bloommin && pixels[i] <= bloommax) {
      pixels2[l] = pixels[i];
      pixelcoords2[l] = pixelcoords[i];
      l++;
    }
  }
  
  for (i=0; i<n*m; i++)
    image.data[i] = 0.;
  for (i=0; i<npix2; i++)
    image.data[pixelcoords2[i]] = 1.;
  
  writeImage(output_image, input_image, image.data, call);

  return 0;
}
