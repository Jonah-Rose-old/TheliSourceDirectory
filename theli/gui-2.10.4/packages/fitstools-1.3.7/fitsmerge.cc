#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -j replacement_image \n";
    cerr << "           -o output_image \n";
    cerr << "           -x x_cen y_cen (centre of the replaced area)\n";
    cerr << "           -r radius of the replaced area [pixels] \n";
    cerr << "           [-a autoscale]\n";
    cerr << "           [-s scaling factor (override autoscale, default: 1.0)]\n";
    cerr << "           [-n (normalises the image if set)]\n\n";
    cerr << "  PURPOSE: Replaces part of the input_image by the same part\n";
    cerr << "           of the (optionally scaled) replacement_image.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int n, m, nc, mc, radius, xcen, ycen;
  bool norm, manualscale, autoscale;
  long i, j, k;
  float xi, yi, dist, normfactor, scale, autotmp;
  string input_image1, input_image2, output_image;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  scale = 1.0;
  norm = false;
  autoscale = false;
  manualscale = false;
  xcen = 0;
  ycen = 0;
  radius = 0;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image1 = argv[++i];
            break;
        case 'j': input_image2 = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'x': xcen = atoi(argv[++i]) - 1;
            ycen = atoi(argv[++i]) - 1;
            break;
        case 'r': radius = atoi(argv[++i]);
            break;
        case 'a': autoscale = true;
            break;
        case 's': scale = atof(argv[++i]);
	    manualscale = true;
            break;
        case 'n': norm = true;
            break;
      }
    }
  }

  // override autoscale
  if (manualscale) autoscale = false;

  checkfile(input_image1);
  checkfile(input_image2);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image1(input_image1);
  myFITS image2(input_image2);
  n  = image1.naxis1;
  m  = image2.naxis2;
  nc = image2.naxis1;
  mc = image2.naxis2;

  if (n != nc || m != mc) {
    cerr << "ERROR: Images do not have the same dimensions!\n";
    return 1;
  }
  
  if (autoscale) {
    k = 0;
    autotmp = 0.;
    for (i=0; i<n; i++) {
      xi = (float) i - xcen;
      for (j=0; j<m; j++) {
	yi = (float) j - ycen;
	dist = sqrt(xi*xi+yi*yi);
	if (dist<radius && dist>radius-2.) {
	  autotmp += (image1.data[i+n*j] / image2.data[i+n*j]);
	  k++;
	}
      }	   
    }
    scale = autotmp / (float) k;
    cout << "Autoscaling factor: " << scale << endl;
  }
  
  vector<float> data_out(image1.data);

  for (i=0; i<n; i++) {
    xi = (float) i - xcen;
    for (j=0; j<m; j++) {
      yi = (float) j - ycen;
      dist = sqrt(xi*xi+yi*yi);
      if (dist <= radius) data_out[i+n*j] = scale * image2.data[i+n*j];
    }
  }
  
  // get the normalisation factor (if requested)
  if (norm) {
    normfactor = 0.;
    for (k=0; k<n*m; k++) {
      normfactor += data_out[k];
    }
    // normalise the image
    for (k=0; k<n*m; k++) {
      data_out[k] /= normfactor;
    }
  }

  writeImage(output_image, input_image1, data_out, call);

  return 0;
}
