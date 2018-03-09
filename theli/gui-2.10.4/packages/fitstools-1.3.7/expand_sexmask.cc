#include <cstring> // for strcpy()
#include <fstream>
#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;


//************************************************************
void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input mask image (sextractor segmentation map or objects image)\n";
    cerr << "           -c input mask catalog (sextractor objects catalog)\n";
    cerr << "           -r ellipse scaling parameter (default value: 3.0)\n";
    cerr << "           -f expansion kernel width (default: 1)\n";
    cerr << "           -o output_image\n";
    cerr << "           -w weight_image\n";
    cerr << "    Masks an ellipse around each object detection. The ellipse can be scaled\n";
    cerr << "    using the R parameter.\n";
    cerr << "    Before masking, each object can optionally be convolved with a small kernel\n";
    cerr << "    to add some extra leverage.\n";
    cerr << "    If the centroid of the object is in a weighted area, it is ignored (filtering spurious detections).\n\n";
    exit(1);
  }
}


//************************************************************
int main(int argc, char *argv[])
{
  int n, m, width = 1;
  long i, j, k, l, ik, jl;
  float r = 3.0;
  string input_image, output_image, weight_image;
  char input_catalog[FILEMAX];

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'w': weight_image = argv[++i];
	break;
      case 'c': strcpy(input_catalog, argv[++i]);
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'f': width = atol(argv[++i]);
	break;
      case 'r': r = atof(argv[++i]);
	break;
      }
    } 
  }

  // Read the FITS image
  checkfile(input_image);
  string call = programcall(argv, argc);

  myFITS image_in(input_image);
  myFITS weight_in(weight_image);
  n = image_in.naxis1;
  m = image_in.naxis2;

  vector<float> imagecopy(image_in.data);

  // Spatially convolve the FITS image with a small kernel to provide some extra safety
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      // spatial convolution with a box filter
      for (k=-width; k<=width; k++) {
	ik = i+k;
	// respect image boundaries
	if (ik < 0) ik = 0;
	if (ik >= n) ik = n-1;
	for (l=-width; l<=width; l++) {
	  jl = j+l;
	  // respect image boundaries
	  if (jl < 0) jl = 0;
	  if (jl >= m) jl = m-1;
	  imagecopy[i+n*j] += image_in.data[ik+n*jl];
	}
      }
      imagecopy[i+n*j] = imagecopy[i+n*j] > 0. ? 1. : 0.;  
    }
  }

  image_in.destroy();

  // Read the object catalog
  vector<float> x;
  vector<float> y;
  vector<float> cxx;
  vector<float> cyy;
  vector<float> cxy;
  vector<float> aimg;
  vector<float> bimg;
  float dx, dy;
  float xtmp, ytmp, cxxtmp, cyytmp, cxytmp, atmp, btmp;
  ifstream input(input_catalog);
  if (!input.is_open()) {
    cerr <<  "ERROR: Could not open " << input_catalog << "!\n";
    exit (1);
  }
  else {
    while ( input.good() )
      {
	input >> xtmp >> ytmp >> cxxtmp >> cyytmp >> cxytmp >> atmp >> btmp;
	// only include objects if they are not "just" outside the image
	// (excluding spurious sextractor detections that appear for very low thresholds)
	if (weight_in.data [ floor(xtmp) + n*(floor(ytmp))] != 0) {
	  x.push_back(xtmp);
	  y.push_back(ytmp); 
	  cxx.push_back(cxxtmp);
	  cyy.push_back(cyytmp);
	  cxy.push_back(cxytmp);
	  aimg.push_back(atmp);
	  bimg.push_back(btmp);
	}
      }
    input.close();
  }

  long nobj = x.size();

  long imin, imax, jmin, jmax;
  for (k=0; k<nobj; k++) {

    // do not process hot pixels or extremely compact sources
    if (bimg[k] < 1.0) continue;

    // the max area containing the object
    imin = (x[k] - aimg[k]*r > 0) ? int(x[k] - aimg[k]*r) : 0;
    imax = (x[k] + aimg[k]*r < n) ? int(x[k] + aimg[k]*r) : n-1;
    jmin = (y[k] - aimg[k]*r > 0) ? int(y[k] - aimg[k]*r) : 0;
    jmax = (y[k] + aimg[k]*r < m) ? int(y[k] + aimg[k]*r) : m-1;
    for (j=jmin; j<=jmax; j++) {
      for (i=imin; i<=imax; i++) {
	dx = float(i) - x[k];
	dy = float(j) - y[k];
	if (cxx[k] * dx * dx +
	    cyy[k] * dy * dy + 
	    cxy[k] * dx * dy <= r*r)
	  imagecopy[i+n*j] = 1.;
      }
    }
  }

  writeImage(output_image, input_image, imagecopy, call);

  return 0;
}
