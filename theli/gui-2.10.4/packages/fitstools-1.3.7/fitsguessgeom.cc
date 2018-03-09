#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

void scan(myFITS&, int);

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i image \n";
    cerr << "          [-x fits_extension (for multi-extension fits files)] \n";
    cerr << "          [-n (just report the number of extensions found)] \n\n";
    cerr << "  PURPOSE: Guesses which pixels on a detector receive light.\n";
    cerr << "           Works best with a flatfield with ~20 kADU.\n";
    cerr << "  OUTPUT:  CUTON-X SIZE-X CUTON-Y SIZE-Y NAXIS1 NAXIS2 CRPIX1 CRPIX2\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  long i;
  int next, s, extension = -1, extonly = 0;
  string input_image;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'x': extension = atoi(argv[++i]);
	break;
      case 'n': extonly = 1;
	break;
      }
    }
  }

  checkfile(input_image);

  // read the FITS image
  myFITS image(input_image, false);
  
  // how many extensions does the image have
  next = image.numExt();

  if (extonly == 1) {
    cout << next << endl;
    return 0;
  }

  // loop over the extensions
  if (next>0) {
    for (s=1;s<=next;s++) {
      if (extension == -1) scan(image, s);
      else if (extension == s) scan(image, s);
    }
  }

  if (next == 0) scan(image, 0);

  return 0;
}


//*****************************************************************
void scan(myFITS &image, int extension)
{
  long n, m, i, j, u, k, xmin, xmax, ymin, ymax;
  float crpix1, crpix2;
  float meanval, contrast, highthresh = 65535., val, mval;
  
  contrast = 3.0;

  // No extension
  if (extension == 0) {
    n = image.naxis1;
    m = image.naxis2;
    crpix1 = image.readKeyFailsafe("CRPIX1", n/2.);
    crpix2 = image.readKeyFailsafe("CRPIX2", m/2.);
    image.readData();
  }
  // Extensions
  else {
    n = image.eNaxis1(extension);
    m = image.eNaxis2(extension);
    crpix1 = image.readExtKey("CRPIX1", extension, n/2.);
    crpix2 = image.readExtKey("CRPIX2", extension, m/2.);
    image.readExtData(extension);
  }

  vector<double> row(n,0);
  vector<double> col(m,0);
  
  meanval = median(image.data, n*m, "fast");
  xmin = 0;
  xmax = 0;
  ymin = 0;
  ymax = 0;

  // extract rows, average with sigma-rejection
  for (j=0; j<m; j++)  {
    u = 0;
    for (i=0; i<n; i++) {
      if (image.data[i+n*j] < highthresh) row[u] = image.data[i+n*j];
      else row[u] = 0.;
      u++;
    }
    col[j] = mean_iter(row, n, 1.5, 2, -60000.);
  }
  // scan the bottom
  for (k=0; k<m-3; k++) {
    val = (col[k]+col[k+1])/(col[k+2]+col[k+3]);
    mval = 0.5*(col[k] + col[k+1]);
    if ( val >= 0.99 && val <= 1.01 && mval > meanval/contrast) {
      ymin = k+2;
      break;
    }
  }
  // scan the top
  for (k=m-4; k>=0; k--) {
    val = (col[k]+col[k+1])/(col[k+2]+col[k+3]);
    mval = 0.5*(col[k] + col[k+1]);
    if (val >= 0.99 && val <= 1.01 && mval > meanval/contrast) {
      ymax = k+3;
      break;
    }
  }
  
  // extract columns, average with sigma-rejection
  for (i=0; i<n ; i++) {
    u = 0;
    for (j=0; j<m; j++) {
      if (image.data[i+n*j] < highthresh) col[u] = image.data[i+n*j];
      else {
	col[u] = 0.;
      }
      u++;
    }
    row[i] = mean_iter(col, m, 1.5, 2, -60000.);
  }
  // scan the left edge
  for (k=0; k<n-3; k++) {
    val = (row[k]+row[k+1])/(row[k+2]+row[k+3]);
    mval = 0.5*(row[k] + row[k+1]);
    if (val >= 0.99 && val <= 1.01 && mval > meanval/contrast) {
      xmin = k+2;
      break;
    }
  }
  // scan the right edge
  for (k=n-4; k>=0; k--) {
    val = (row[k]+row[k+1])/(row[k+2]+row[k+3]);
    mval = 0.5*(row[k] + row[k+1]);
    if (val >= 0.99 && val <= 1.01 && mval > meanval/contrast) {
      xmax = k+3;
      break;
    }
  }
  
  // print the result
  if (xmin == 0) xmin = 1;
  if (xmax == 0) xmax = n;
  if (ymin == 0) ymin = 1;
  if (ymax == 0) ymax = m;

  long xext = xmax - xmin;
  long yext = ymax - ymin;

  if (xext % 2 != 0) xext -= 1;
  if (yext % 2 != 0) yext -= 1;

  cout << xmin << " " << xext << " " << ymin << " " << yext << " " << n << " " 
       << m << " " << crpix1 << " " << crpix2 << endl;
}
