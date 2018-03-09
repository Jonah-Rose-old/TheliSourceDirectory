#include <CCfits>
#include <fitsio.h>
#include "statistics.h"
#include "fitstools.h"
#include "tiffio.h"

using namespace std;

// compiled with
// gcc fits2tiff.c -L. -leclipse -lqfits -ltiff -lm -O3 -o fits2tiff

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image\n";
    cerr << "           -o output_image\n";
    cerr << "           [-r min max (dynamic range used)] (all)\n";
    cerr << "           [-b bitpix for TIFF (8 or 16)] (16)\n";
    cerr << "           [-g background greylevel in percent] (0.0)\n";
    cerr << "           [-e gamma exponent] (1.0)\n";
    cerr << "           [-z (do a ds9-style zscale-ing)\n";
    cerr << "           [-c zscale contrast (1.0)\n";
    cerr << "           [-s shrink dynamic range by this fraction (0.0)\n\n";
    cerr << "  EXAMPLE:\n";
    cerr << "  fits2tiff -i in.fits -o out.tif -b 16 -r -0.03 10\n\n";
    cerr << "  Converts the FITS range [-0.03, 10] to a 16 bit\n";
    cerr << "  TIFF image. Lower and higher values are clipped.\n\n";
    cerr << "  The -z (zscale) option determines the lower and upper\n";
    cerr << "  thresholds automatically. The contrast can be increased\n";
    cerr << "  with -c (lowering the upper threshold). The background\n";
    cerr << "  brightness can be reduced with -s, which increases the\n";
    cerr << "  lower threshold (the dynamic range is shrunk by the\n";
    cerr << "  given fraction).\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  TIFF     *outtiff;             // pointer to the TIFF file, defined in tiffio.h
  uint16   *outbuf16, *outb16;   // further TIFF pointers
  uint8    *outbuf8,  *outb8;    // further TIFF pointers

  int row, column, bit, zscaleing;
  long n, m, i, j, k, ic, iter;
  float blowup, grey, gamma, gmaxlevel, shrink, contrast;
  float minthresh, maxthresh;
  vector<float> zscale(2,0);
  string input_image;
  char output_image[4096];
  
  gamma     = 1.0;
  grey      = 0.0;
  bit       = 16;
  minthresh = 0.;
  maxthresh = 0.;
  zscaleing = 0;
  shrink    = 0.;
  contrast  = 1.0;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
	    break;
        case 'o': strcpy(output_image, argv[++i]);
	    break;
	case 'b': bit = atoi(argv[++i]);
	    break;
	case 'g': grey = atof(argv[++i]);
	    break;
	case 'e': gamma = atof(argv[++i]);
	    break;
	case 'r': minthresh = atof(argv[++i]);
	          maxthresh = atof(argv[++i]);
	    break;
        case 'z': zscaleing = 1;
	    break;
	case 'c': contrast = fabs(atof(argv[++i]));
	    break;
	case 's': shrink = atof(argv[++i]);
	    if (shrink > 1.) shrink = 0.;
	    break;
      }
    }
  }

  checkfile(input_image);
  
  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  vector< vector<long> > imtiff(n);
  for(i=0; i<n; i++) {
    imtiff[i].resize(m,0);
  }
  
  // if dynamic range not provided by user, then get min and max from FITS
  if (minthresh == 0. && maxthresh == 0. && zscaleing == 0) {
    minthresh = maxthresh = image.data[0];
    for (k=0; k<n*m; k++)  {
      if (image.data[k] < minthresh) minthresh = image.data[k];
      if (image.data[k] > maxthresh) maxthresh = image.data[k];
    }
  }

  // if z-scale is requested
  if (zscaleing == 1) {
    get_zscale(image.data, n, m, zscale, shrink, contrast);
    minthresh = zscale[0];
    maxthresh = zscale[1];
    cout << "# zscale min max\n";
    cout << minthresh << " " << maxthresh << endl;
  }

  // apply gamma correction if requested
  if ( gamma != 1.0 ) {
    if (n>4000) {
      cerr <<  "                                    \r";
      cerr <<  "-- Applying Gamma scaling ...\r";
    }
    gamma = 1./gamma;
    gmaxlevel = pow(maxthresh,gamma);
    for (k=0; k<n*m; k++)
      image.data[k] = pow(image.data[k], gamma) / gmaxlevel * maxthresh;
  }

  // clipping min and max values
  for (k=0; k<n*m; k++) {
    if (image.data[k] <= minthresh) image.data[k] = minthresh;
    if (image.data[k] >= maxthresh) image.data[k] = maxthresh;
  }
  
  // rescale to 8/16 bit range and make positive
  // avoid saturation by subtracting the additive grey level in blowup
  if (n>4000) {
    ic = 0;
    fprintf(stderr, "                                    \r");
    fprintf(stderr, "-- Converting:  0%c\r", 37);
  }
  
  if (bit == 8) {
    grey = grey / 100. * 253.;
    blowup = (253. - grey) / (maxthresh - minthresh);
  }
  else {
    grey = grey / 100. * 65000;
    blowup = (65000. - grey) / (maxthresh - minthresh);
  }
  
  for (i=0; i<n; i++)  {
    for (j=0; j<m; j++)  {
      image.data[i+n*j] = blowup * (image.data[i+n*j] - minthresh) + grey;
      // flipping TIFF in y dir
      imtiff[i][m-j-1] = (long) image.data[i+n*j];
    }
    if (n>4000) {
      iter = ceil((float)(n-1)/(float)10);
      if(i%iter==0) {
	fprintf(stderr,"-- Converting:  %ld%c\r", ic*10,37);
	ic++;
      }
    }
  }
  if (n>4000) fprintf(stderr,"-- Writing FITS as TIFF ...\r");
  
  outtiff = TIFFOpen(output_image, "w");
  TIFFSetField(outtiff, TIFFTAG_IMAGEWIDTH, n);
  TIFFSetField(outtiff, TIFFTAG_IMAGELENGTH, m);
  TIFFSetField(outtiff, TIFFTAG_COMPRESSION, 1);
  if (bit == 8)  TIFFSetField(outtiff, TIFFTAG_BITSPERSAMPLE, 8);
  if (bit == 16) TIFFSetField(outtiff, TIFFTAG_BITSPERSAMPLE, 16);
  TIFFSetField(outtiff, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(outtiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  TIFFSetField(outtiff, TIFFTAG_PLANARCONFIG, 1);
  TIFFSetField(outtiff, TIFFTAG_SOFTWARE, "fits2tiff by Mischa Schirmer");
  TIFFSetField(outtiff, TIFFTAG_IMAGEDESCRIPTION, "Created by THELI");
  
  if (bit == 8) {
    outbuf8 = (uint8 *)_TIFFmalloc(TIFFScanlineSize(outtiff));
    for (row=0; row<m; row++) {
      outb8 = outbuf8;
      for (column=0; column<n; column++) {
	*outb8++ = (uint8) (imtiff[column][row]);
      }
      TIFFWriteScanline(outtiff, outbuf8, row, 0);
    }
    TIFFClose(outtiff);
    _TIFFfree(outbuf8);
  }
  
  else if (bit == 16) {
    outbuf16 = (uint16 *)_TIFFmalloc(TIFFScanlineSize(outtiff));
    for (row=0; row<m; row++) {
      outb16 = outbuf16;
      for (column=0; column<n; column++) {
	*outb16++ = (uint16) (imtiff[column][row]);
      }
      TIFFWriteScanline(outtiff, outbuf16, row, 0);
    }
    TIFFClose(outtiff);
    _TIFFfree(outbuf16);
  }
  
  fprintf(stderr, "                                    \r");

  return (0);
}
