#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

typedef struct {
  long naxis1;
  long naxis2;
  long biasxmin;
  long biasxmax;
  long dataxmin;
  long dataxmax;
  long ccdxmin;
  long ccdxmax;
  long xsize;
  long chipx;
  long chipy;
  float crpix1;
  float gain;
  int binx;
} _extensiongeometry_;


void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image\n";
    cerr << "          [-o [y/n] (overscan correction; default: y)]\n";
    cerr << "          [-g [y/n] (gain correction; default: y)]\n";
    cerr << "          [-d (print some debugging information)]\n\n";
    cerr << "      Preprocesses PAUCAM data taken with multiple readout ports.\n";
    cerr << "      The extensions of the readouts are pasted\n";
    cerr << "      back together as individual chips, by default correcting for overscan\n";
    cerr << "      and gain (options -o and -g), relying on correct FITS headers).\n";
    exit(1);
  }
}


void preprocess(vector<float>&, long, long, long, long, float, float, bool, bool);


//***************************************************************
int main(int argc, char *argv[])
{
  long i, j, k;
  int n_chip = 18, gap = 50;
  string input_image, output_image, biassec, datasec, ccdsec, binstring, filter;
  string output_base, gainchoice = "y", overscanchoice = "y";
  string instrument;
  bool dogain = true, dooverscan = true, debug = false;
  
  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  // read the command line arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'd': debug = true;
            break;
        case 'g': gainchoice = argv[++i];
            break;
        case 'o': overscanchoice = argv[++i];
            break;
      }
    }
  }

  // check arguments for consistency
  if (gainchoice.compare("y") == 0) dogain = true;
  else if (gainchoice.compare("n") == 0) dogain = false;
  else {
    cerr << "ERROR: Invalid choice for the gain correction. Must be 'y' or 'n'!" << endl;
    return (1);
  }

  if (overscanchoice.compare("y") == 0) dooverscan = true;
  else if (overscanchoice.compare("n") == 0) dooverscan = false;
  else {
    cerr << "ERROR: Invalid choice for the overscan correction. Must be 'y' or 'n'!" << endl;
    return (1);
  }

  // read the FITS header and the data block
  checkfile(input_image);
  string call = programcall(argv, argc);

  myFITS image(input_image);
  int n_ext = image.numExt();

  // hardcoding number of readout ports (not contained in FITS header)
  int n_read = 4;

  // the base of the output image name
  basename(input_image, ".fits", output_base);

  if (debug) {
    cout << "Processing image:              " << input_image << endl;
    cout << "Number of FITS extensions:     " << n_ext << endl;
  }

  // exit if number of extensions is not what is expected (72)
  if (n_ext != 72) {
    cerr << "ERROR! 72 extensions expected, but " << n_ext << " were found!" << endl;
    return (1);
  }
  
  // an array containing all the gains
  vector<float> gains(n_ext,0.);

  // an array with the reference pixels per chip
  vector<float> vec_crpix1(n_read,0.);

  // a structure containing the relevant extension gemoetries
  _extensiongeometry_ *extgeom = new _extensiongeometry_[n_ext];

  int part = 0;
  int chip_count = 0;
  int chip_count_old = 0;
  int ccdnum = 0;
  int ex;
  long xsize = 0;

  // if we don't have information where on the detector the data section is:
  // (in that case we assume data sections are read out / stored left to right)
   long xstep = 0;           

  vector<float> data_corrected;
  vector<float> data_part;

  // Header handling: obtain the geometries from the ExtHDUs
  for (ex=0; ex<n_ext; ex++) {
    // read the image geometry, gain, bias and data sections; extensions start with 1, not 0
    extgeom[ex].naxis1 = image.readLongExtKey("NAXIS1", ex+1);
    extgeom[ex].naxis2 = image.readLongExtKey("NAXIS2", ex+1);
    extgeom[ex].crpix1 = image.readExtKey("CRPIX1", ex+1);
    extgeom[ex].gain   = image.readExtKey("GAIN", ex+1);

    // MUST declare vertices here (split_datasection() appends to the vector)
    vector<long> biasvert, datavert, ccdvert;

    // extract the vertices from the "[XMIN:XMAX,YMIN:YMAX]" strings
    biassec = image.readStringExtKey("BIASSEC", ex+1);
    datasec = image.readStringExtKey("DATASEC", ex+1);
    ccdsec  = image.readStringExtKey("CCDSEC", ex+1);
    extgeom[ex].binx = 4234 / extgeom[ex].naxis2; // sorry, hardcoded, no binning info in header!
    split_datasection(biassec, biasvert);
    split_datasection(datasec, datavert);
    split_datasection(ccdsec, ccdvert);
    extgeom[ex].biasxmin = biasvert[0];
    extgeom[ex].biasxmax = biasvert[1];
    extgeom[ex].dataxmin = datavert[0];
    extgeom[ex].dataxmax = datavert[1];
    extgeom[ex].ccdxmin  = ccdvert[0];
    extgeom[ex].ccdxmax  = ccdvert[1];
    extgeom[ex].xsize    = datavert[1] - datavert[0] + 1;
    //    extgeom[ex].chipy    = datavert[3] - datavert[2] + 1;
    extgeom[ex].chipy    = 4224; // wrong DATASEC keyword

    // If data is binned
    if (extgeom[ex].binx != 1) {
      if (ccdvert[0] % 2 != 0) extgeom[ex].ccdxmin = ccdvert[0] / extgeom[ex].binx + 1;
      else extgeom[ex].ccdxmin = ccdvert[0] / extgeom[ex].binx;
      if (ccdvert[1] % 2 != 0) extgeom[ex].ccdxmax = ccdvert[1] / extgeom[ex].binx + 1;
      else extgeom[ex].ccdxmax = ccdvert[1] / extgeom[ex].binx;
    }
    // If data is unbinned
    else {
      extgeom[ex].ccdxmin = ccdvert[0];
      extgeom[ex].ccdxmax = ccdvert[1];
    }

    // recollect the gains
    gains[ex] = extgeom[ex].gain;
  }

  

  // maximum gain
  float gainmax = max(gains, 4);

  // Determine the x-dimension of each of the chips (with overscans removed)
  k = 0;
  for (j=0; j<n_chip; j++) {
    xsize = 0;
    for (i=0; i<n_read; i++) xsize += extgeom[i+n_read*j].xsize;
    for (i=0; i<n_read; i++) extgeom[k++].chipx = xsize;
  }

  if (debug) {
    printf("EXT\tNAXIS1 NAXIS2 GAIN BIAS_XMIN BIAS_XMAX DATA_XMIN DATA_XMAX CCDXMIN CCDXMAX XSIZE BIN CHIPX CHIPY\n");
    for (ex=0; ex<n_ext; ex++) {
      printf("%d\t%ld\t%ld\t%f\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%d\t%ld\t%ld\n", ex+1, 
	     extgeom[ex].naxis1, extgeom[ex].naxis2, extgeom[ex].gain, 
	     extgeom[ex].biasxmin, extgeom[ex].biasxmax,
	     extgeom[ex].dataxmin, extgeom[ex].dataxmax,
	     extgeom[ex].ccdxmin, extgeom[ex].ccdxmax,
	     extgeom[ex].xsize, extgeom[ex].binx,
	     extgeom[ex].chipx, extgeom[ex].chipy);
    }
  }

  // Now we know how large the pasted images have to be; start with chip 1
  vector<float> data_pasted((extgeom[0].chipx+2*gap/extgeom[0].binx)*extgeom[0].chipy, 0.);

  vector<float> data_mosaic;

  double mjdobs = image.mjdobs();

  // Data handling, loop over extensions
  for (ex=0; ex<n_ext; ex++) {

    // figure out the chip number
    chip_count = image.readExtKey("AMPID", ex+1) - 1;
    ccdnum = image.readExtKey("CCDNUM", ex+1);

    // skip the currently badly distorted CCds
    if (ccdnum > 8) continue;
    //    cout << chip_count << " " << ccdnum << endl;
    //    chip_count = (ex*n_chip) / n_ext;

    // update the vector size to accomodate the pasted chip
    if (chip_count != chip_count_old)
      data_pasted.resize(extgeom[ex].chipx*extgeom[ex].chipy);

    chip_count_old = chip_count;

    // read the data
    image.readExtData(data_corrected, ex+1);

    // overscan and gain correction
    preprocess(data_corrected, extgeom[ex].naxis1, extgeom[ex].naxis2, 
	       extgeom[ex].biasxmin, extgeom[ex].biasxmax, extgeom[ex].gain, 
	       gainmax, dogain, dooverscan);

    // collect the corrected data
    if (part < n_read) {
      k = 0;
      data_part.resize(extgeom[ex].xsize*extgeom[ex].naxis2);
      for (j=0; j<extgeom[ex].naxis2; j++) {
	for (i=extgeom[ex].dataxmin-1; i<extgeom[ex].dataxmax; i++) {
	  data_part[k++] = data_corrected[i+extgeom[ex].naxis1*j];
	}
      }

      // paste the amplifiers
      k = 0;
      for (j=0; j<extgeom[ex].naxis2; j++) {
       	for (i=xstep; i<xstep+extgeom[ex].xsize; i++) {
	  //for (i=extgeom[ex].ccdxmin-1; i<extgeom[ex].ccdxmax; i++) {
	  data_pasted[i+extgeom[ex].chipx*j] = data_part[k++];
	}
      }

      // the reference pixel
      vec_crpix1[part] = extgeom[ex].crpix1;

      xstep += extgeom[ex].xsize;
      part++;
    }

    // write the pasted image
    if (part == n_read) {
      output_image = output_base+"_"+NumberToString(ccdnum)+".fits";
      //      writeImage(output_image, data_pasted, extgeom[ex].chipx, extgeom[ex].naxis2, -32);
      writeImage(output_image, data_pasted, extgeom[ex].chipx, extgeom[ex].chipy, -32);

      // update the header
      double cd11     = image.readDoubleExtKey("CD1_1", ex+1);
      double cd12     = image.readDoubleExtKey("CD1_2", ex+1);
      double cd21     = image.readDoubleExtKey("CD2_1", ex+1);
      double cd22     = image.readDoubleExtKey("CD2_2", ex+1);
      double crval1   = image.readDoubleExtKey("CRVAL1", ex+1);
      double crval2   = image.readDoubleExtKey("CRVAL2", ex+1);
      float  crpix2   = image.readExtKey("CRPIX2", ex+1);
      float  equinox  = 2000.0;
      float  exptime  = atof(image.readKey("EXPTIME").c_str());
      float  airmass  = atof(image.readKey("AIRMASS").c_str());
      cout.precision(12);
      string object   = image.readKey("OBJECT");
      // replace whitespace by underscores
      reducewhitespace(object, "_", " ");
      string filter   = image.readKey("FTRAY");
      string dateobs  = image.readKey("DATE-OBS");
      string timeobs  = image.readKey("UTSTART");
      string ctype1   = image.readStringExtKey("CTYPE1", ex+1);
      string ctype2   = image.readStringExtKey("CTYPE2", ex+1);
      //      string radecsys = image.readStringExtKey("RADECSYS", ex+1);
      string radecsys = "FK5"; // sorry, hardcoded, not available in header
      myFITS imageout(output_image, false, "Write");

      /*
      float crpix1 = 0;
      if (chip_count == 0) crpix1 = max(vec_crpix1,n_read);
      if (chip_count == 1) crpix1 = max(vec_crpix1,n_read);
      if (chip_count == 2) crpix1 = max(vec_crpix1,n_read);
      */

      float crpix1 = max(vec_crpix1,n_read);

      // construct a simple filter keyword ...
      imageout.addKey("RADECSYS", radecsys, "Coordinate system reference");
      imageout.addKey("CTYPE1", ctype1, "R.A. in tangent plane projection");
      imageout.addKey("CTYPE2", ctype2, "DEC. in tangent plane projection");
      imageout.addKey("CRVAL1", crval1, "Reference coordinate of first axis");
      imageout.addKey("CRVAL2", crval2, "Reference coordinate of second axis");
      imageout.addKey("CRPIX1", crpix1, "Reference pixel of first axis");
      imageout.addKey("CRPIX2", crpix2, "Reference pixel of second axis");
      imageout.addKey("CD1_1", cd11, "Linear projection matrix");
      imageout.addKey("CD1_2", cd12, "Linear projection matrix");
      imageout.addKey("CD2_1", cd21, "Linear projection matrix");
      imageout.addKey("CD2_2", cd22, "Linear projection matrix");
      imageout.addKey("FILTER", filter, "Filter");
      imageout.addKey("OBJECT", object, "Object name");
      imageout.addKey("EXPTIME", exptime , "Exposure time [seconds]");
      imageout.addKey("EQUINOX", equinox, "Equinox of coordinate system");
      imageout.addKey("DATE-OBS", dateobs+"T"+timeobs , "UT Date of observation");
      imageout.addKey("MJD-OBS", mjdobs, "MJD of start of obseration");
      imageout.addKey("AIRMASS", airmass, "Mean airmass for the observation");
      for (i=1; i<=50; i++) imageout.addKey("DUMMY"+NumberToString(i), 0.0, "");

      part = 0;
       xstep = 0;
    }

  }

  delete [] extgeom;

  return 0;
}

//***************************************************************
// Overscan correction and gain normalization
//***************************************************************
void preprocess(vector<float>&data, long n, long m, long xmin, long xmax, 
		float gain, float gainmax, bool dogain, bool dooverscan)
{
  long i, j, t = 0;
  float overscan = 0.;
  
  long dim = xmax - xmin + 1;

  // initialise 'overscan' to zero (in case we don't do it)
  vector<float> oscan(dim,0.);

  // reset gain and gainmax if not requested
  if (!dogain) {
    gain = 1.0;
    gainmax = 1.0;
  }

  // subtract the overscan, then do the gain normalization
  for (j=0; j<m; j++) {
    if (dooverscan) {
      // get the median overscan for each line (if not, 'overscan' is initialised to zero)
      for (i=xmin-1; i<xmax-1; i++) oscan[t++] = data[i+n*j];
      overscan = median(oscan, dim);
      t = 0;
    }
    for (i=0; i<n; i++) data[i+n*j] = (data[i+n*j] - overscan) * gain / gainmax;
  }
  

  // gain normalization, then subtract the overscan
  /*
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) data[i+n*j] = data[i+n*j] * gain / gainmax;

    // get the median overscan for each line
    if (dooverscan) {
      for (i=xmin-1; i<xmax-1; i++) oscan[t++] = data[i+n*j];
      overscan = median(oscan, dim);
      t = 0;
    }
    // subtract overscan
    for (i=0; i<n; i++) data[i+n*j] -= overscan;
  }
  */
}
