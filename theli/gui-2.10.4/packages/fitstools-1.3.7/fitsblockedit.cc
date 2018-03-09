#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE:  " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -o output_image \n";
    cerr << "           -x xmin xmax ymin ymax \n";
    cerr << "           [-v value; default: 0.0]\n";
    cerr << "           [-r radius (if option -c is given)]\n";
    cerr << "           [-c xcen ycen (if option -r is given)]\n";
    cerr << "           [-n (invert the selected region)]\n";
    cerr << "           [-a ]\n";
    cerr << "           [-m ]\n\n";
    cerr << "      Sets pixels inside the defined section\n";
    cerr << "      to the value given with the -v option.\n";
    cerr << "      If -c and -r is used, the masked region will be circular.\n";
    cerr << "      If -n is given, the selection is inverted.\n";
    cerr << "      Is the -a (-m) option given, then the value\n";
    cerr << "      given does not replace the section of the\n";
    cerr << "      image, but is added (multiplied) to it.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int xcen, ycen, xmin, xmax, ymin, ymax, add, mult, inverse, box, circle;
  long n, m, i, j;
  float value, rad, dist;
  string input_image, output_image;
  
  value = 0.;
  xcen = ycen = 0;
  xmin = xmax = ymin = ymax = 0;
  add   = 0;
  mult  = 0;
  box   = 0;
  rad   = 0.;
  circle  = 0;
  inverse = 0;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'x': xmin = atoi(argv[++i]) - 1;
	    xmax  = atoi(argv[++i]);
	    ymin  = atoi(argv[++i]) - 1;
	    ymax  = atoi(argv[++i]);
	    box = 1;
            break;
        case 'c': xcen = atoi(argv[++i]) - 1;
	    ycen  = atoi(argv[++i]) - 1;
	    circle = 1;
            break;
	case 'v': value = atof(argv[++i]);
            break;
	case 'r': rad = atof(argv[++i]);
	    circle = 1;
            break;
	case 'a': add = 1;
            break;
	case 'm': mult = 1;
            break;
	case 'n': inverse = 1;
            break;
      }
    }
  }

  // error handling
  if (box == 1 && circle == 1) {
    cerr << "ERROR: Either a box (option -x) or a circle (options -c and -r) must be selected!\n";
    exit (1);
  }

  if (add == 1 && mult == 1) {
    cerr << "ERROR: Only one of options -a or -m can be given at the same time!\n";
    exit (1);
  }

  if (box == 1) {
    if (xmin==0 && xmax==00 && ymin==0 && ymax==0) {
      cerr << "ERROR: No image section specified (0 0 0 0).\n";
      exit (1);
    }
    if (xmin>xmax || ymin>ymax) {
      cerr << "ERROR: xmin must be smaller than xmax; same for ymin and ymax!\n";
      exit (1);
    }
  }

  if (circle == 1 && rad == 0.) {
    cerr << "ERROR: No radius specified!\n";
    exit (1);
  }

  if (circle == 1 && (xcen == 0 || ycen == 0)) {
    cerr << "ERROR: No centre specified for the circle!\n";
    exit (1);
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS image
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  // Modify the specified sections
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      // mask a rectangle
      if (box == 1) {
	// regular pattern
	if (inverse == 0) {
	  if (i>=xmin && i<xmax && j>=ymin && j<ymax) {
	    if (add==0 && mult == 0) image.data[i+n*j] = value;
	    if (add==1) image.data[i+n*j] += value;
	    if (mult==1) image.data[i+n*j] *= value;
	  }
	}
	// inverted pattern
	else {
	  if (!(i>=xmin && i<xmax && j>=ymin && j<ymax)) {
	    if (add==0 && mult == 0) image.data[i+n*j] = value;
	    if (add==1) image.data[i+n*j] += value;
	    if (mult==1) image.data[i+n*j] *= value;
	  }
	}
      }

      // mask a circle
      if (circle == 1) {
	dist = (float) (xcen-i)*(xcen-i) + (ycen-j)*(ycen-j);
	// regular pattern
	if (inverse == 0) {
	  if (dist <= rad*rad) {
	    if (add==0 && mult == 0) image.data[i+n*j] = value;
	    if (add==1) image.data[i+n*j] += value;	  
	    if (mult==1) image.data[i+n*j] *= value;	  
	  }
	}
	// inverted pattern
	else {
	  if (dist >= rad*rad) {
	    if (add==0 && mult == 0) image.data[i+n*j] = value;
	    if (add==1) image.data[i+n*j] += value;	  
	    if (mult==1) image.data[i+n*j] *= value;	  
	  }
	}
      }
    }
  }

  writeImage(output_image, input_image, image.data, call);
  
  return 0;
}
