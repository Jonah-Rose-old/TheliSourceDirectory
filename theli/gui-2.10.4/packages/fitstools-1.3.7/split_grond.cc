#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

typedef struct {
  long naxis1;
  long naxis2;
  long biasaxmin;
  long biasaxmax;
  long dataaxmin;
  long dataaxmax;
  long biasbxmin;
  long biasbxmax;
  long databxmin;
  long databxmax;
  long dataaymin;
  long dataaymax;
  long databymin;
  long databymax;
  long xsize;
  long ysize;
  float crpix1;
  float crpix2;
} _extensiongeometry_;


void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image\n\n";
    cerr << "      Preprocesses optical and NIR GROND images. The dual readouts in each of the four\n";
    cerr << "      optical extensions are pasted back together as individual chips, correcting\n";
    cerr << "      for overscan, relying on correct FITS headers).\n\n";
    cerr << "      The NIR channel is cut into the three filter sections.\n\n";
    exit(1);
  }
}


void preprocess(vector<float>&, long, long, long, long, float, float, bool, bool);
void get_overscan_overall(vector<float>&, int, int, int, int, float&);


//***************************************************************
int main(int argc, char *argv[])
{
  long i, j, k;
  string input_image, output_image, biasseca, biassecb, dataseca, datasecb, filter;
  float overscana, overscanb, binx, biny;
  string instrument;
  
  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  // read the command line arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
      }
    }
  }

  // read the FITS header and the data block
  checkfile(input_image);
  string call = programcall(argv, argc);

  myFITS image(input_image);
  instrument = image.readKey("INSTRUME");

  // is this an optical or a NIR image?
  // naxis = 0: OPTICAL
  // naxis = 2: NIR
  int naxis = atoi(image.readKey("NAXIS").c_str());
  
  //************************************************
  // OPTICAL DATA
  //************************************************

  if (naxis == 0) {
    int n_ext = image.numExt();
    
    // how many amps were used per detector
    int n_read = atoi(image.readKey("HIERARCH ESO DET OUTPUTS").c_str());
    
    // exit if number of amplifiers is not what is expected (8)
    if (n_read != 8) {
      cerr << "ERROR: 8 amplifiers expected. This image has " << n_read << "amps instead. Exit." << endl;
      return 1;
    }
    
    // exit if number of extensions is not what is expected (4)
    if (n_ext != 4) {
      cerr << "ERROR: 4 extenions expected. This image has " << n_ext << " extensions instead. Exit." << endl;
      return 1;
    }
    
    // an array with the reference pixels per chip
    vector<float> vec_crpix1(n_read,0.);
    
    // a structure containing the relevant extension gemoetries
    _extensiongeometry_ *extgeom = new _extensiongeometry_[n_ext];
    
    // Now we know how large the pasted images have to be; start with chip 1
    vector<float> data_pasted, data_corrected;
    
    // gains, hardcoded (Filters in extensions 1-4: i, z, g, r)
    vector<float> gainA(4,1.0);
    vector<float> gainB(4,0.0);
    gainB[0] = 0.9867;
    gainB[1] = 0.9042;
    gainB[2] = 1.0571;
    gainB[3] = 0.9954;
    
    int ex;
    
    // Loop over the extensions
    for (ex=0; ex<n_ext; ex++) {
      
      // read the image geometry, gain, bias and data sections; extensions start with 1, not 0
      extgeom[ex].naxis1 = image.readLongExtKey("NAXIS1", ex+1);
      extgeom[ex].naxis2 = image.readLongExtKey("NAXIS2", ex+1);
      extgeom[ex].crpix1 = image.readExtKey("CRPIX1", ex+1);
      extgeom[ex].crpix2 = image.readExtKey("CRPIX2", ex+1);
      
      // MUST declare vertices here (split_datasection() appends to the vector)
      vector<long> biasverta, dataverta;
      vector<long> biasvertb, datavertb;
      
      // extract the vertices from the "[XMIN:XMAX,YMIN:YMAX]" strings
      biasseca = image.readStringExtKey("BSECA", ex+1);
      biassecb = image.readStringExtKey("BSECB", ex+1);
      dataseca = image.readStringExtKey("DSECA", ex+1);
      datasecb = image.readStringExtKey("DSECB", ex+1);
      binx     = image.readExtKey("CDELT1", ex+1);
      biny     = image.readExtKey("CDELT2", ex+1);
      if (binx != biny || binx != 1.0) {
	cerr << "ERROR: Binning factor " << binx << " unsupported. Exit." << endl;
	return 1;
      }
      split_datasection(biasseca, biasverta);
      split_datasection(biassecb, biasvertb);
      split_datasection(dataseca, dataverta);
      split_datasection(datasecb, datavertb);
      
      extgeom[ex].biasaxmin = biasverta[0]-1;
      extgeom[ex].biasaxmax = biasverta[1]-1;
      extgeom[ex].biasbxmin = biasvertb[0]-1;
      extgeom[ex].biasbxmax = biasvertb[1]-1;
      extgeom[ex].dataaxmin = dataverta[0]-1;
      extgeom[ex].dataaxmax = dataverta[1]-1;
      extgeom[ex].databxmin = datavertb[0]-1;
      extgeom[ex].databxmax = datavertb[1]-1;
      extgeom[ex].dataaymin = dataverta[2]-1;
      extgeom[ex].dataaymax = dataverta[3]-1;
      extgeom[ex].databymin = datavertb[2]-1;
      extgeom[ex].databymax = datavertb[3]-1;
      extgeom[ex].xsize     = dataverta[1] - dataverta[0] + 1 + datavertb[1] - datavertb[0] + 1;
      extgeom[ex].ysize     = dataverta[3] - dataverta[2] + 1;
      
      data_pasted.resize(extgeom[ex].xsize*extgeom[ex].ysize);
      
      // read the data
      image.readExtData(data_corrected, ex+1);
      
      // get a global overscan value for each readout channel
      get_overscan_overall(data_corrected, extgeom[ex].naxis1, extgeom[ex].naxis2,
			   extgeom[ex].biasaxmin, extgeom[ex].biasaxmax, overscana);
      get_overscan_overall(data_corrected, extgeom[ex].naxis1, extgeom[ex].naxis2,
			   extgeom[ex].biasbxmin, extgeom[ex].biasbxmax, overscanb);
      
      // do the overscan correction and paste the dual channels back together
      k = 0;
      for (j=0; j<extgeom[ex].naxis2; j++) {
	for (i=0; i<extgeom[ex].naxis1; i++) {
	  // first section
	  if (i >= extgeom[ex].dataaxmin && i <= extgeom[ex].dataaxmax && 
	      j >= extgeom[ex].dataaymin && j <= extgeom[ex].dataaymax)
	    data_pasted[k++] = (data_corrected[i+extgeom[ex].naxis1*j] - overscana) * gainA[ex];
	  // second section
	  if (i >= extgeom[ex].databxmin && i <= extgeom[ex].databxmax &&
	      j >= extgeom[ex].databymin && j <= extgeom[ex].databymax)
	    data_pasted[k++] = (data_corrected[i+extgeom[ex].naxis1*j] - overscanb) * gainB[ex];
	}
      }
      
      // write the pasted image
      
      // update the header
      double cd11     = image.readDoubleExtKey("CD1_1", ex+1);
      double cd12     = image.readDoubleExtKey("CD1_2", ex+1);
      double cd21     = image.readDoubleExtKey("CD2_1", ex+1);
      double cd22     = image.readDoubleExtKey("CD2_2", ex+1);
      float  crpix1   = image.readExtKey("CRPIX1", ex+1);
      float  crpix2   = image.readExtKey("CRPIX2", ex+1);
      float  exptime  = image.readExtKey("EXPTIME", ex+1);
      double crval1   = atof(image.readKey("CRVAL1").c_str());
      double crval2   = atof(image.readKey("CRVAL2").c_str());
      float  equinox  = atof(image.readKey("EQUINOX").c_str());
      float  airmass  = atof(image.readKey("AIRMASS").c_str());
      double mjdobs   = atof(image.readKey("MJD-OBS").c_str());
      string object   = image.readKey("OBJECT");
      // replace whitespace by underscores
      reducewhitespace(object, "_", " ");
      string filter   = image.readStringExtKey("SUBSET", ex+1);
      // remove whitespace
      removeallwhitespace(filter);
      // construct a simple filter keyword ...
      if (filter.compare("grond_g") == 0) filter = "g";
      if (filter.compare("grond_r") == 0) filter = "r";
      if (filter.compare("grond_i") == 0) filter = "i";
      if (filter.compare("grond_z") == 0) filter = "z";
      
      string dateobs  = image.readKey("DATE-OBS");
      string ctype1   = image.readStringExtKey("CTYPE1", ex+1);
      string ctype2   = image.readStringExtKey("CTYPE2", ex+1);
      string radecsys = image.readStringExtKey("RADESYS", ex+1);
      
      output_image = "GROND."+filter+"."+dateobs+"_1.fits";
      writeImage(output_image, data_pasted, extgeom[ex].xsize, 
		 extgeom[ex].ysize, -32);
      
      myFITS imageout(output_image, false, "Write");
      
      imageout.addKey("INSTRUME", instrument, "Instrument");
      imageout.addKey("RADECSYS", radecsys, "Coordinate system reference");
      imageout.addKey("CTYPE2", ctype2, "DEC. in tangent plane projection");
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
      imageout.addKey("DATE-OBS", dateobs, "UT Date of observation");
      imageout.addKey("MJD-OBS", mjdobs, "MJD of start of obseration");
      imageout.addKey("AIRMASS", airmass, "Mean airmass for the observation");
      for (i=1; i<=50; i++) imageout.addKey("DUMMY"+NumberToString(i), 0.0, "");
    }
    
    delete [] extgeom;
  }
  
  //************************************************
  // NIR DATA
  //************************************************

  if (naxis == 2) {
    int chips;
    int xmin, xmax, ymin, ymax;
    vector<float> data_corrected;
    
    for (chips=0; chips<3; chips++) {
      // J-band
      if (chips == 0) {
	filter="J";
	xmin = 29; xmax = 1013; ymin = 6; ymax = 1014;
      }
      // H-band
      if (chips == 1) {
	filter = "H";
	xmin = 1045; xmax = 2044; ymin = 12; ymax = 980;
      }
      // K-band
      if (chips == 2) {
	filter = "K";
	xmin = 2054; xmax = 3062; ymin = 12; ymax = 1024;
      }
      
      // write the pasted image
      
      // update the header
      //double mjdobs   = atof(image.readKey("MJD-OBS").c_str());
      cout.precision(12);
      double mjdobs;
      image.readmyKey("MJD-OBS",mjdobs);
      cout << mjdobs << endl;

      mjdobs   = atof(image.readKey("CRVAL1").c_str());
      cout << mjdobs << endl;

      double crval1   = atof(image.readKey("CRVAL1").c_str());
      double crval2   = atof(image.readKey("CRVAL2").c_str());
      double cd11     = atof(image.readKey(filter+"_CD1_1").c_str());
      double cd12     = atof(image.readKey(filter+"_CD1_2").c_str());
      double cd21     = atof(image.readKey(filter+"_CD2_1").c_str());
      double cd22     = atof(image.readKey(filter+"_CD2_2").c_str());
      float crpix1    = atof(image.readKey(filter+"_CRPIX1").c_str());
      float crpix2    = atof(image.readKey(filter+"_CRPIX2").c_str());
      float equinox   = atof(image.readKey("EQUINOX").c_str());
      float airmass   = atof(image.readKey("AIRMASS").c_str());
      float dit       = atof(image.readKey("HIERARCH ESO DET DIT").c_str());
      float ndit      = atof(image.readKey("HIERARCH ESO DET NDIT").c_str());
      string object   = image.readKey("OBJECT");
      string dateobs  = image.readKey("DATE-OBS");
      string ctype1   = image.readKey("CTYPE1");
      string ctype2   = image.readKey("CTYPE2");
      string radecsys = image.readKey("RADESYS");
      float exptime   = dit*ndit;

      // replace whitespace by underscores
      reducewhitespace(object, "_", " ");

      // read the corresponding data section
      data_corrected.resize((xmax-xmin+1)*(ymax-ymin+1));
      k = 0;
      for (j=0; j<image.naxis2; j++) {
	for (i=0; i<image.naxis1; i++) {
	  if (i>=xmin-1 && i<xmax && j>=ymin-1 && j<ymax) {
	    // I know it's weird but these data have to be multiplied by -1 ...
	    data_corrected[k++] = -1. * image.data[i+image.naxis1*j] * ndit;
	  }
	}
      }

      output_image = "GROND."+filter+"."+dateobs+"_1.fits";
      writeImage(output_image, data_corrected, xmax-xmin+1, ymax-ymin+1, -32);
      
      myFITS imageout(output_image, false, "Write");
      
      imageout.addKey("INSTRUME", instrument, "Instrument");
      imageout.addKey("RADECSYS", radecsys, "Coordinate system reference");
      imageout.addKey("CTYPE2", ctype2, "DEC. in tangent plane projection");
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
      imageout.addKey("DATE-OBS", dateobs, "UT Date of observation");
      imageout.addKey("MJD-OBS", mjdobs, "MJD of start of obseration");
      imageout.addKey("AIRMASS", airmass, "Mean airmass for the observation");

      for (i=1; i<=50; i++) imageout.addKey("DUMMY"+NumberToString(i), 0.0, "");
    }
  }
  
  return 0;
}

  
//***************************************************************
// Calculates a global overscan for each readout channel
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
