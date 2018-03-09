#include <CCfits>
#include <fitsio.h>
#include <tiffio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE:  %s \n" << argv[0] << endl;
    cerr << "           -i input_TIFF_image\n";
    cerr << "           -o output_FITS_image\n\n";
    cerr << "  WARNING: This programme will only read grey-level 16-bit TIFF images!\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  TIFF    *intiff;  // pointer to the TIFF file, defined in tiffio.h
  string  image_out;
  tdata_t *line_buf;
  tsize_t lsize;
  uint16 sample_per_pix;
  int bit;
  uint32 n, m, p, shift;
  long i, j;
  char input_image[4096]; 
  string output_image, input_image_string;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': strcpy(input_image, argv[++i]);
            break;
        case 'o': output_image = argv[++i];
            break;
      }
    }
  }

  input_image_string = input_image;

  // read the TIFF image
  intiff = TIFFOpen(input_image, "r");
  if (intiff) {
    
    TIFFGetField(intiff, TIFFTAG_IMAGEWIDTH, &n);
    TIFFGetField(intiff, TIFFTAG_IMAGELENGTH, &m);
    TIFFGetField(intiff, TIFFTAG_BITSPERSAMPLE, &bit);
    TIFFGetField(intiff, TIFFTAG_SAMPLESPERPIXEL, &sample_per_pix);

    lsize = TIFFScanlineSize(intiff);
    
    line_buf = (tdata_t*) _TIFFmalloc(lsize);
    if(line_buf == NULL) {
      cerr <<  "Can't run _TIFFmalloc(" << lsize << ")!\n";
      return 1;
    }

    valarray<float> data_out(n*m);

    for (i=0; i<m; ) {
      for (j=0; (j<m) && (i<m); j++, i++) {
	if(TIFFReadScanline(intiff, line_buf, i, 0) == -1)
	  cerr <<  "Can't read line of tiff, line = " << i << endl;
	for (p=0; p<n; p++) {              
	  shift = (p*sample_per_pix)*2;
	  data_out[j*n+p] = *((unsigned short *)((unsigned char *) line_buf+shift));
	}
      }
    }  

    long naxis = 2;
    long fpixel = 1;
    long naxes[2] = {n, m};

    // overwrite existing file if the file already exists.
    output_image = "!" + output_image;

    // create the output FITS file
    try {
      auto_ptr<FITS> imageout_ptr( new FITS(output_image, -32, naxis, naxes));
      PHDU &imageout = imageout_ptr->pHDU();
      imageout.addKey("EXPTIME", 1.0, "");
      imageout.addKey("ORIGFILE", input_image_string, "");
      imageout.addKey("DUMMY001", 0.0, "");
      imageout.addKey("DUMMY002", 0.0, "");
      imageout.addKey("DUMMY003", 0.0, "");
      imageout.addKey("DUMMY004", 0.0, "");
      imageout.addKey("DUMMY005", 0.0, "");
      imageout.addKey("DUMMY006", 0.0, "");
      imageout.addKey("DUMMY007", 0.0, "");
      imageout.addKey("DUMMY008", 0.0, "");
      imageout.addKey("DUMMY009", 0.0, "");
      imageout.write(fpixel,n*m,data_out);
    }
    catch (FitsException&) {
      cerr << "ERROR in writeImage(): Could not write " << output_image << ")!" << endl;
    }
    

    TIFFClose(intiff);
    intiff = NULL;
  }
  
  return 0;
}
