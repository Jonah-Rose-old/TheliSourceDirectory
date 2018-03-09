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
    cerr << "           -t threshold (default: 55000)\n\n";
    exit(1);
  }
}


int main(int argc, char *argv[])
{
  int n, m;
  long i, j, k, count, npixlarge;
  int bloomed;
  float threshold, temp;
  string input_image, output_image;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  threshold = 55000.;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 't': threshold = atof(argv[++i]);
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

  npixlarge = (n+2) * (m+2);

  // arrays holding bloomed pixels
  vector<float> imagelarge(npixlarge,0);
  vector<float> imagemodlarge(npixlarge,0);

  // wrap a 1 pixel wide border with value -1000 around the image
  // thus we don't suffer from cumbersome border effects
  for (j=0; j<m+2; j++) {
    for (i=0; i<n+2; i++)  {
      k = i + (n+2) * j;
      if (i>0 && i<n+1 && j>0 && j<m+1) 
	imagelarge[k] = image.data[(i-1)+n*(j-1)];
      else imagelarge[k] = -1000.;
      imagemodlarge[k] = imagelarge[k];
    }
  }

  // detect bloomed pixels
  vector<int> mask(npixlarge,0);
  vector<int> maskexpand(npixlarge,0);

  for (i=0; i<n+2; i++)  {
    for (j=0; j<m+2; j++)  {
      k = i + (n+2) * j;
      if (imagemodlarge[k] > threshold) mask[k] = 1;
      else mask[k] = 0;
      if (imagemodlarge[k] < -999.) mask[k] = -1000;
      maskexpand[k] = mask[k];
    }
  }

  // expand bloomed regions by 1 pixel to each side
  // in order to deal with affected neighbouring pixels
  // and pixels at the end of spikes that are only partially bloomed
  
  for (i=1; i<n+1; i++)  {
    for (j=1; j<m+1; j++)  {
      if (mask[i-1+(n+2)*(j+1)] == 1 || 
	  mask[i  +(n+2)*(j+1)] == 1 || 
	  mask[i+1+(n+2)*(j+1)] == 1 ||
	  mask[i-1+(n+2)*j]     == 1 ||                      
	  mask[i+1+(n+2)*j]     == 1 ||
	  mask[i-1+(n+2)*(j-1)] == 1 || 
	  mask[i  +(n+2)*(j-1)] == 1 || 
	  mask[i+1+(n+2)*(j-1)] == 1 ) {
	maskexpand[i+(n+2)*j] = 2;
      }
    }
  }

  for (i=1; i<n+1; i++)  {
    for (j=1; j<m+1; j++)  {
      if (mask[i+(n+2)*j] == 1) maskexpand[i+(n+2)*j] = 1;
    }
  }

  for (i=1; i<n+1; i++)  {
    for (j=1; j<m+1; j++)  {
      if (maskexpand[i+(n+2)*j] == 2) maskexpand[i+(n+2)*j] = 1;
    }
  }

  // estimate the true value of a bloomed pixel by averaging
  // over unaffected neighbouring pixels

  temp = 0;
  bloomed = 1;
  count = 0;

  while (bloomed == 1) {
    bloomed = 0;
    
    for (i=1; i<n+1; i++)  {
      for (j=1; j<m+1; j++)  {
	if (maskexpand[i+(n+2)*j] == 1) {
	  temp = 0;
	  count = 0;
	  bloomed = 1;
	  
	  if (imagelarge[(i-1)+(n+2)*(j-1)] < threshold && 
	      maskexpand[(i-1)+(n+2)*(j-1)] == 0) {
	    temp += imagelarge[(i-1)+(n+2)*(j-1)];
	    count++;
	  }
	  if (imagelarge[i+(n+2)*(j-1)] < threshold && 
	      maskexpand[i+(n+2)*(j-1)] == 0) {
	    temp += imagelarge[i+(n+2)*(j-1)];
	    count++;
	  }
	  if (imagelarge[(i+1)+(n+2)*(j-1)] < threshold && 
	      maskexpand[(i+1)+(n+2)*(j-1)] == 0) {
	    temp = temp + imagelarge[(i+1)+(n+2)*(j-1)];
	    count++;
	  }
	  if (imagelarge[(i-1)+(n+2)*j] < threshold && 
	      maskexpand[(i-1)+(n+2)*j] == 0) {
	    temp += imagelarge[(i-1)+(n+2)*j];
	    count++;
	  }
	  if (imagelarge[(i+1)+(n+2)*j] < threshold && 
	      maskexpand[(i+1)+(n+2)*j] == 0) {
	    temp += imagelarge[(i+1)+(n+2)*j];
	    count++;
	  }
	  if (imagelarge[(i-1)+(n+2)*(j+1)] < threshold && 
	      maskexpand[(i-1)+(n+2)*(j+1)] == 0) {
	    temp += imagelarge[(i-1)+(n+2)*(j+1)];
	    count++;
	  }
	  if (imagelarge[i+(n+2)*(j+1)] < threshold && 
	      maskexpand[i+(n+2)*(j+1)] == 0) {
	    temp += imagelarge[i+(n+2)*(j+1)];
	    count++;
	  }
	  if (imagelarge[(i+1)+(n+2)*(j+1)] < threshold && 
	      maskexpand[(i+1)+(n+2)*(j+1)] == 0) {
	    temp += imagelarge[(i+1)+(n+2)*(j+1)];
	    count++;
	  }
	  
	  if (count > 0) {
	    imagemodlarge[i+(n+2)*j] = temp / count;
	  }
	}
      }
    }
    
    //    detect remaining bloomed pixels
    for (i=0; i<n+2; i++)  {
      for (j=0; j<m+2; j++)  {
	k = i + (n+2)*j;
	imagelarge[k] = imagemodlarge[k];
	if (imagelarge[k] > threshold) maskexpand[k] = 1;
	else maskexpand[k] = 0;
	if (imagelarge[k] < -999.) {
	  maskexpand[k] = -1000;     
	}
      }
    }
  }

  // remove the 1 pixel wide wrapper around the image
  vector<float> data_out(n*m,0);

  for (i=1; i<n+1; i++)  {
    for (j=1; j<m+1; j++)  {
      data_out[(i-1) + n*(j-1)] = imagelarge[i+(n+2)*j];
    }
  }
  
  writeImage(output_image, input_image, data_out, call);

  return 0;
}
