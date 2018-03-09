#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -l line mask width (e.g.: 25)\n";
    cerr << "          [-b box mask width  (e.g.: 50)]\n";
    cerr << "      Creates a mask image, \"gpc1_mask.fits\", for GPC1 chip-stage images\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int w1, w2, num, xmin, xmax, ymin, ymax, box;
  long n, m, i, j, l, o;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  w1 = 25;
  w2 = 50;

  box = 0;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'l': w1 = atoi(argv[++i]);
	break;
      case 'b': w2 = atoi(argv[++i]);
	box = 1;
	break;
      }
    }
  }


  int x[] = {0, 598, 1206, 1814, 2422, 3030, 3638, 4246, 4840};
  int y[] = {0, 607, 1217, 1827, 2438, 3047, 3657, 4267, 4863};

  num = 9;

  // the GPC1 chip size
  n = 4846;
  m = 4868;

  // initialise the mask image to 1
  vector<float> data_out(n*m,1.);

  // mask the mask image
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      for (l=0; l<num; l++) {
	// mask the columns
	xmin = x[l] - w1;
	xmax = x[l] + w1;
	if (xmin < 0) xmin = 0;
	if (xmax > n) xmax = n;
	if (xmin == 0) xmax = w1 / 2;
	if (xmax == n) xmin = xmax - w1 / 2;
	if (i>=xmin && i<=xmax) data_out[i+n*j] = 0.;

	// mask the rows
	ymin = y[l] - w1;
	ymax = y[l] + w1;
	if (ymin < 0) ymin = 0;
	if (ymax > m) ymax = m;
	if (ymin == 0) ymax = w1 / 2;
	if (ymax == m) ymin = ymax - w1 / 2;
	if (j>=ymin && j<=ymax) data_out[i+n*j] = 0.;
      }
      if (box == 1) {
	for (l=0; l<num; l++) {
	  xmin = x[l] - w2;
	  xmax = x[l] + w2;
	  if (xmin < 0) xmin = 0;
	  if (xmax > n) xmax = n;
	  if (xmin == 0) xmax = w2;
	  if (xmax == n) xmin = xmax - w2;
	  for (o=0; o<num; o++) {
	    ymin = y[o] - w2;
	    ymax = y[o] + w2;
	    if (ymin < 0) ymin = 0;
	    if (ymax > m) ymax = m;
	    if (ymin == 0) ymax = w2;
	    if (ymax == m) ymin = ymax - w2;
	    if (i>=xmin && i<=xmax && j>=ymin && j<=ymax) data_out[i+n*j] = 0.;
	  }
	}
      }
    }
  }

  writeImage("gpc1_mask.fits", data_out, n, m, -32);

  return 0;
}
