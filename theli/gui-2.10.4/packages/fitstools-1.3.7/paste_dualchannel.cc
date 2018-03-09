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
    cerr << "           -m mode (MOSAIC_II or SOI)\n";
    cerr << "          [-v (do no overscan correction; e.g. for pre-2009 MOSAIC data)\n";
    cerr << "      Pastes together CCD images taken with 2 readout channels.\n";
    cerr << "      Includes overscan correction (unless the -v option is provided)\n";
    cerr << "      and gain normalisation.\n\n";
    exit(1);
  }
}

void get_overscan_overall(vector<float>&data, int n, int m, int xmin, int xmax, float &overscan);
void set_instrument_params(string mode);

float overscan1, overscan2;
int ovscan_min1, ovscan_min2, ovscan_max1, ovscan_max2;
int im_xmin1, im_xmin2, im_xmax1, im_xmax2;
int numtotchannels;
vector<float> gain;

int main(int argc, char *argv[])
{
  bool do_oscan = true;
  long n_new, m_new, ii1, ii2, i, j, k, l, m, n1, n2, k1, k2;
  string input_image, output_image, mode;
  int e1, e2;
  float crpix1, crpix2;
  float crpix11, crpix12;
  double cd11, cd12, cd21, cd22;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'm': mode = argv[++i];
            break;
        case 'v': do_oscan = false;
	    break;
      }
    }
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image(input_image);
  FITS image_in(input_image, Read);

  // create the output image;
  output_image = "!" + output_image;
  auto_ptr<FITS> imageout_ptr(new FITS(output_image, Write));

  // Load instrument-specific parameters
  set_instrument_params(mode);

  vector<float> imsec1, imsec2;
  vector<float> tmpdata;

  for (l=1; l<=numtotchannels/2; l++) {
    e1 = 2*l-1;
    e2 = e1+1;
    imsec1.clear();
    imsec2.clear();
    image.readExtData(imsec1, e1);
    image.readExtData(imsec2, e2);

    // the image geometry
    n1 = image.readLongExtKey("NAXIS1", e1);
    n2 = image.readLongExtKey("NAXIS1", e2);
    m  = image.readLongExtKey("NAXIS2", e1);

    k1 = 0;
    k2 = 0;

    // get the median overscan for each section
    if (do_oscan) {

      get_overscan_overall(imsec1, n1, m, ovscan_min1, ovscan_max1, overscan1);
      get_overscan_overall(imsec2, n2, m, ovscan_min2, ovscan_max2, overscan2);      

      n_new = im_xmax1 - im_xmin1 + im_xmax2 - im_xmin2 + 2;
      m_new = m;

      tmpdata.resize(n_new*m_new,0);

      // correct the image regions for overscan and gain
      for (j=0; j<m_new; j++) {
	for (i=0; i<n_new; i++) {
	  k = i+n_new*j;
	  // first section
	  if (i <= im_xmax1 - im_xmin1) {
	    ii1 = i + im_xmin1 - 1;
	    k1 = ii1 + n1*j;
	    tmpdata[k] = (imsec1[k1] - overscan1) * gain[e1-1];
	  }
	  // second section
	  if (i > im_xmax1 - im_xmin1) {
	    ii2 = i - (im_xmax1 - im_xmin1) + im_xmin2 - 2;
	    k2 = ii2 + n2*j;
	    tmpdata[k] = (imsec2[k2] - overscan2) * gain[e2-1];
	  }
	}
      }
    }

    else {
      n_new = n1+n2;
      m_new = m;
      tmpdata.resize(n_new*m_new,0);

      // correct the image regions for gain
      for (j=0; j<m_new; j++) {
	for (i=0; i<n_new; i++) {
	  k = i+n_new*j;
	  // first section
	  if (i < n1) {
	    k1 = i + n1*j;
	    tmpdata[k] = imsec1[k1] * gain[e1-1];
	  }
	  // second section
	  if (i >= n1) {
	    k2 = i + n2*j;
	    tmpdata[k] = imsec2[k2] * gain[e2-1];
	  }
	}
      }
    }

    // Add an image extension to the output image
    addImageExtension(imageout_ptr, tmpdata, l, n_new, m_new);

    // read original extension keywords
    crpix11 = image.readExtKey("CRPIX1", e1);
    crpix12 = image.readExtKey("CRPIX1", e2);
    crpix1 = (crpix11 > crpix12) ? crpix11 : crpix12;
    crpix2 = image.readExtKey("CRPIX2", e1);
    cd11   = image.readDoubleExtKey("CD1_1", e1);
    cd12   = image.readDoubleExtKey("CD1_2", e1);
    cd21   = image.readDoubleExtKey("CD2_1", e1);
    cd22   = image.readDoubleExtKey("CD2_2", e1);

    // Set the pointer to the new extension and add the keywords
    ExtHDU &imext = imageout_ptr->extension(l);
    
    imext.addKey("BSCALE", 1.0, "");
    imext.addKey("BZERO", 0.0, "");
    imext.addKey("INHERIT", true, "");
    imext.addKey("EXTVER", l, "");
    imext.addKey("CRPIX1", crpix1, "");
    imext.addKey("CRPIX2", crpix2, "");
    imext.addKey("CD1_1", cd11, "");
    imext.addKey("CD1_2", cd12, "");
    imext.addKey("CD2_1", cd21, "");
    imext.addKey("CD2_2", cd22, "");
  }

  // We have to explicitly close the image. 
  // Otherwise the header keys are not written to the last extension! (Bug in ccfits?)
  imageout_ptr->destroy();
  image_in.destroy();

  return 0;
}


//***************************************************************
// Calculates a global overscan
//***************************************************************
void get_overscan_overall(vector<float>&data, int n, int m, int xmin, int xmax, float &overscan)
{
  long i, j, k, w;

  w = (xmax - xmin + 1) * m;

  vector<float> oscan(w,0);

  k = 0;
  for (j=0; j<m; j++) {
    for (i=xmin; i<=xmax; i++) {
      oscan[k++] = data[i+n*j];
    }
  }
  overscan = median(oscan, w, "fast");
}  


//***************************************************************
// Set instrument specific parameters
//***************************************************************
void set_instrument_params(string mode) 
{
  // sorry, these are hard-coded

  if (mode.compare("MOSAIC_II") == 0) {
    numtotchannels = 16;
    gain.resize(numtotchannels,0);

    // Gain ratios within the chip
    gain[0]  = 1.0;
    gain[1]  = 1.0357;
    gain[2]  = 1.2440;
    gain[3]  = 1.0;
    gain[4]  = 1.1259;
    gain[5]  = 1.0;
    gain[6]  = 1.0460;
    gain[7]  = 1.0;   
    gain[8]  = 1.0358;
    gain[9]  = 1.0;
    gain[10] = 1.0468;
    gain[11] = 1.0;
    gain[12] = 0.9023;
    gain[13] = 1.0;
    gain[14] = 0.9762;
    gain[15] = 1.0;
    
    // Overscan regions
    ovscan_min1 = 1070;
    ovscan_max1 = 1100;
    ovscan_min2 = 10;
    ovscan_max2 = 40;
    
    // Image regions
    im_xmin1 = 25;
    im_xmax1 = 1048;
    im_xmin2 = 65;
    im_xmax2 = 1088;
  }

  if (mode.compare("SOI") == 0) {
    numtotchannels = 4;
    gain.resize(numtotchannels,0);

    // Gain ratios within the chip
    gain[0]  = 1.00000;
    gain[1]  = 1.00000;
    gain[2]  = 1.00000;
    gain[3]  = 1.01333;
    
    // Overscan regions
    ovscan_min1 = 10;
    ovscan_max1 = 20;
    ovscan_min2 = 10;
    ovscan_max2 = 20;
    
    // Image regions
    im_xmin1 = 28;
    im_xmax1 = 539;
    im_xmin2 = 28;
    im_xmax2 = 539;
  }
}
