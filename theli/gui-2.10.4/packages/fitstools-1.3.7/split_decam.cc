#include <CCfits>
#include <thread>
#include <fitsio.h>
#include <fstream>
#include <unistd.h>
#include "fitstools.h"
#include "statistics.h"
#include <semaphore.h>

using namespace std;

typedef struct {
  vector<int> vic_number, src_number;
  vector<string> vic_channel, src_channel;
  vector<float> x, xerr, srcnl, p1, p2, p3;
} _xtalk_;

sem_t semaphor; // control the number of threads reading simultaneously

void read_xtalk(string, _xtalk_&);

void read_linearity(string, vector< vector<double>>&, vector< vector<double>>&, vector< vector<double>>&);

void preprocess_thread(int, int, long, bool, bool, bool, bool, 
		       double, float, float, string, string, string, 
		       string, _xtalk_&, vector< vector<double>>&, 
		       vector< vector<double>>&, vector<int>&, myFITS&);

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image\n";
    cerr << "           -p path to xtalk config file\n";
    cerr << "           -q path to linearity FITS table\n";
    cerr << "          [-n number of CPUs to use; default: 1]\n";
    cerr << "          [-o [y/n] (overscan correction; default: y)]\n";
    cerr << "          [-g [y/n] (gain correction; default: y)]\n";
    cerr << "          [-x [y/n] (xtalk correction; default: y)]\n";
    cerr << "          [-l [y/n] (nonlinearity correction; default: y)]\n";
    cerr << "      Preprocesses DECAM images, and extracts individual chips.\n";
    exit(1);
  }
}


//***************************************************************
int main(int argc, char *argv[])
{
  long i;
  int ncpu, NUM_THREADS, nthreads_user = 1;
  string input_image, output_image;
  string output_base;
  string gainchoice = "y", overscanchoice = "y", xtalkchoice = "y", linearitychoice = "y";
  bool dogain = true, dooverscan = true, doxtalk = true, dolinearity = true;
  string xtalkfile, linearityfile;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  // read the command line arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
	  break;
        case 'p': xtalkfile = argv[++i];
	  break;
        case 'q': linearityfile = argv[++i];
	  break;
        case 'g': gainchoice = argv[++i];
	  break;
        case 'o': overscanchoice = argv[++i];
	  break;
        case 'x': xtalkchoice = argv[++i];
	  break;
        case 'l': linearitychoice = argv[++i];
	  break;
        case 'n': nthreads_user = atoi(argv[++i]);
	  break;
      }
    }
  }

  // multi-threading
  ncpu = sysconf(_SC_NPROCESSORS_ONLN);
  NUM_THREADS = nthreads_user;
  if (nthreads_user > 1) {
    if (nthreads_user > ncpu) NUM_THREADS = ncpu;
    else NUM_THREADS = nthreads_user;
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

  if (xtalkchoice.compare("y") == 0) doxtalk = true;
  else if (xtalkchoice.compare("n") == 0) doxtalk = false;
  else {
    cerr << "ERROR: Invalid choice for the xtalk correction. Must be 'y' or 'n'!" << endl;
    return (1);
  }

  if (linearitychoice.compare("y") == 0) dolinearity = true;
  else if (linearitychoice.compare("n") == 0) dolinearity = false;
  else {
    cerr << "ERROR: Invalid choice for the linearity correction. Must be 'y' or 'n'!" << endl;
    return (1);
  }

  if (xtalkfile.empty()) {
     cerr << "ERROR: No crosstalk input file was defined (option -p)!" << endl;
     return (1);
  }
   
  if (linearityfile.empty()) {
     cerr << "ERROR: No linearity input file was defined (option -q)!" << endl;
     return (1);
  }

  // Read the xtalk matrix
  _xtalk_ xtalk;
  read_xtalk(xtalkfile, xtalk);
  // How many xtalk channels do we have?
  int n_xtalk = xtalk.x.size();

  // Read the linearity table
  vector< vector<double> > ADU(62), ADU_LINEAR_A(62), ADU_LINEAR_B(62);
  read_linearity(linearityfile, ADU, ADU_LINEAR_A, ADU_LINEAR_B);

  // read the FITS header and the data block
  checkfile(input_image);
  string call = programcall(argv, argc);

  //  cout << "Opening DECam image" << endl;
  myFITS image(input_image);
  // 62 DECam science detectors (2 of which are dead as of Nov 2013)
  // Ignore extensions 63-70 (OI/guide chips)
  int n_ext = 62;

  // the base of the output image name
  basename(input_image, ".fits", output_base);

  // Global header keywords we will propagate
  float exptime = atof(image.readKey("EXPTIME").c_str());
  float airmass;
  string airmass_str = image.readKey("AIRMASS").c_str();
  if (airmass_str == "NaN") airmass = 1.0;
  else airmass = atof(airmass_str.c_str());

  double mjdobs  = image.mjdobs();
  string dateobs = image.readKey("DATE-OBS");
  // Remove the last seven digits from dateobs (fractions of seconds)
  dateobs.erase(dateobs.length()-7,7);
  string object  = image.readKey("OBJECT");
  // replace whitespace by underscores
  reducewhitespace(object, "_", " ");
  string filter  = image.readKey("FILTER");
  // Keep the first word of the filter string, only
  vector<string> filterwords = getword(filter,' ');
  filter = filterwords[0];
  
  int ex;

  // Which CCDNUM is in which extension? Needed for interchip crosstalk correction
  vector<int> channelmap;
  for (ex=0; ex<n_ext; ex++) {
    channelmap.push_back(image.readLongExtKey("CCDNUM", ex+1));
  }

  vector<thread> mythreads;
  int t, ex_start, ex_end;
  // create arguments and launch threads;
  // threads work on different file extensions

  sem_init(&semaphor, 0, 1);

  for(t=0; t<NUM_THREADS; t++) {
    ex_start = t * (n_ext / NUM_THREADS);
    if (t <  NUM_THREADS-1) ex_end = (t + 1) * (n_ext / NUM_THREADS) - 1;
    if (t == NUM_THREADS-1) ex_end = n_ext - 1;
    mythreads.push_back( thread( preprocess_thread, ex_start, ex_end, n_xtalk, 
				 dogain, dooverscan, doxtalk, dolinearity, 
				 mjdobs, exptime, airmass, dateobs, object, filter, 
				 output_base, ref(xtalk), ref(ADU_LINEAR_A), ref(ADU_LINEAR_B),
				 ref(channelmap), ref(image)));
  }
  
  // wait for threads to finish
  for (t=0; t<NUM_THREADS; t++)
    mythreads[t].join();

  return 0;
}

//***************************************************************************************
void preprocess_thread(int ex_start, int ex_end, long n_xtalk, 
		       bool dogain, bool dooverscan, bool doxtalk, bool dolinearity, 
		       double mjdobs, float exptime, float airmass, string dateobs, string object, string filter, 
		       string output_base, _xtalk_ &xtalk, 
		       vector<vector<double>> &ADU_LINEAR_A, vector<vector<double>> &ADU_LINEAR_B,
		       vector<int> &channelmap, myFITS &image)
{

  long i,j,k,t,ex;
  long n_ext = channelmap.size();

  vector<float> data_part;

  // Header handling: obtain the geometries from the ExtHDUs
  for (ex=ex_start; ex<=ex_end; ex++) {
    // read the image geometry, gain, bias and data sections; extensions start with 1, not 0
    sem_wait(&semaphor);
    long n         = image.readLongExtKey("NAXIS1", ex+1);
    long m         = image.readLongExtKey("NAXIS2", ex+1);
    long ccdnum    = image.readLongExtKey("CCDNUM", ex+1);
    float gaina    = image.readExtKey("GAINA", ex+1) / 4.0; // DECam average gain is around 4.0
    float gainb    = image.readExtKey("GAINB", ex+1) / 4.0;
    float saturata = image.readExtKey("SATURATA", ex+1) * gaina;
    float saturatb = image.readExtKey("SATURATB", ex+1) * gainb;
    // Choose the lower saturation value
    float saturate = (saturata < saturatb) ? saturata : saturatb;

    // extract the vertices from the "[XMIN:XMAX,YMIN:YMAX]" strings
    string biasseca_str = image.readStringExtKey("BIASSECA", ex+1);
    string dataseca_str = image.readStringExtKey("DATASECA", ex+1);
    string biassecb_str = image.readStringExtKey("BIASSECB", ex+1);
    string datasecb_str = image.readStringExtKey("DATASECB", ex+1);
    string datasec_str  = image.readStringExtKey("DATASEC", ex+1);

    // split_datasection() appends to the vectors
    vector<long> biasseca, dataseca, biassecb, datasecb, datasec;
    split_datasection(biasseca_str, biasseca);
    split_datasection(biassecb_str, biassecb);
    split_datasection(dataseca_str, dataseca);
    split_datasection(datasecb_str, datasecb);
    split_datasection(datasec_str,  datasec);

    // Read the data section
    vector<float> data(n*m,0);
    image.readExtData(data, ex+1);
    sem_post(&semaphor);

    // initialise 'overscan' to zero (in case we don't do it)
    long dim_oscana = (biasseca[1] - biasseca[0] + 1) * (biasseca[3] - biasseca[2] + 1);
    long dim_oscanb = (biassecb[1] - biassecb[0] + 1) * (biassecb[3] - biassecb[2] + 1);
    vector<float> oscana(dim_oscana,0.);
    vector<float> oscanb(dim_oscanb,0.);

    //****************************************
    // XTALK CORRECTION
    //****************************************
    if (doxtalk) {

      long ii = 0; // Inverted index
      for (t=0; t<n_xtalk; t++) {

	if (xtalk.vic_number[t] == ccdnum) {
	  // Dumping data from offending chip into data_offender
	  // Assuming it's the same chip
	  vector<float> data_offender(data);
	  
	  // If offending channel is not in the same chip
	  if (xtalk.src_number[t] != ccdnum) {
	    // in which extension is that CCDnum?
	    int source_channel = 0;
	    for (k=0; k<n_ext; k++) {
	      if (channelmap[k] == xtalk.src_number[t]) source_channel = channelmap[k];
	    }
	    // Dumping data from offending chip into data_offender
	    sem_wait(&semaphor);
	    image.readExtData(data_offender, source_channel+1);
	    sem_post(&semaphor);
	  }
	  
	  // Correct channel A with channel B
	  if (xtalk.vic_channel[t] == "A" && xtalk.src_channel[t] == "B") {
	    for (j=0; j<m; j++) {
	      ii = datasecb[1] - 1;
	      for (i=dataseca[0]-1; i<=dataseca[1]-1; i++) {
		data[i+n*j] -= data_offender[(ii--)+n*j] * xtalk.x[t];
	      }
	    }
	  }
	  // Correct channel B with channel A (ignoring 2nd order effects as A is already corrected)
	  if (xtalk.vic_channel[t] == "B" && xtalk.src_channel[t] == "A") {
	    for (j=0; j<m; j++) {
	      ii = dataseca[1] - 1;
	      for (i=datasecb[0]-1; i<=datasecb[1]-1; i++) {
		data[i+n*j] -= data_offender[(ii--)+n*j] * xtalk.x[t];
	      }
	    } 
	  }
	}
      }
    }

    //****************************************
    // Obtain OVERSCAN and GAIN if requested.
    // Otherwise default to 0.0 and 1.0
    //****************************************
    float median_overscan_a = 0.;
    float median_overscan_b = 0.;
    
    // Overscan correction?
    if (dooverscan) {
      t = 0;
      for (j=biasseca[2]-1; j<biasseca[3]-1; j++) {
	for (i=biasseca[0]-1; i<biasseca[1]-1; i++) {
	  oscana[t++] = data[i+n*j];
	}
      }
      t = 0;
      for (j=biassecb[2]-1; j<biassecb[3]-1; j++) {
	for (i=biassecb[0]-1; i<biassecb[1]-1; i++) {
	  oscanb[t++] = data[i+n*j];
	}
      }
      median_overscan_a = median(oscana,dim_oscana);
      median_overscan_b = median(oscanb,dim_oscanb);
    }

    // Gain correction?
    if (!dogain) {
      gaina = gainb = 1.0;
    }

    //************************************************
    // OVERSCAN, GAIN and NONLINEARITY correction
    //************************************************
    long data_nonlin;
    for (j=0; j<m; j++) {
      for (i=dataseca[0]-1; i<=dataseca[1]-1; i++) {
	data_nonlin = data[i+n*j];
	if (data_nonlin >= 0 && data_nonlin <= 65535 && dolinearity) 
	  // Nonlinearity data available
	  data[i+n*j] = (ADU_LINEAR_A[ex][data_nonlin] - median_overscan_a) * gaina;
	else 
	  // Nonlinearity data NOT available, or nonlinearity correction not requested
	  data[i+n*j] = (data_nonlin - median_overscan_a) * gaina;
      }
      for (i=datasecb[0]-1; i<=datasecb[1]-1; i++) {
	data_nonlin = data[i+n*j];
	if (data_nonlin >= 0 && data_nonlin <= 65535 && dolinearity) 
	  // Nonlinearity data available
	  data[i+n*j] = (ADU_LINEAR_B[ex][data_nonlin] - median_overscan_b) * gainb;
	else 
	  // Nonlinearity data NOT available, or nonlinearity correction not requested
	  data[i+n*j] = (data_nonlin - median_overscan_b) * gainb;
      }
    }

    //****************************************
    // EXTRACT CORRECTED DATA
    //****************************************

    // Extract the corrected data section
    long dim_n = datasec[1] - datasec[0] + 1;
    long dim_m = datasec[3] - datasec[2] + 1;
    data_part.resize(dim_n*dim_m);

    k = 0;
    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	if (j>=datasec[2]-1 && j<=datasec[3]-1 &&
	    i>=datasec[0]-1 && i<=datasec[1]-1) {
	  data_part[k++] = data[i+n*j];
	}
      }
    }

    //****************************************
    // WRITE CORRECTED IMAGE
    //****************************************

    // write the corrected image
    string output_image = output_base+"_"+NumberToString(ccdnum)+".fits";
    sem_wait(&semaphor);
    writeImage(output_image, data_part, dim_n, dim_m, -32);

    // update the header
    double cd11    = image.readDoubleExtKey("CD1_1", ex+1);
    double cd12    = image.readDoubleExtKey("CD1_2", ex+1);
    double cd21    = image.readDoubleExtKey("CD2_1", ex+1);
    double cd22    = image.readDoubleExtKey("CD2_2", ex+1);
    double crval1  = image.readDoubleExtKey("CRVAL1", ex+1);
    double crval2  = image.readDoubleExtKey("CRVAL2", ex+1);
    float  crpix1  = image.readExtKey("CRPIX1", ex+1);
    float  crpix2  = image.readExtKey("CRPIX2", ex+1);
    float  equinox = image.readExtKey("EQUINOX", ex+1);
    string ctype1  = "RA---TAN";
    string ctype2  = "DEC--TAN";
    string radecsys = image.readStringExtKey("RADESYS", ex+1);
    myFITS imageout(output_image, false, "Write");
    
    imageout.addKey("RADECSYS", radecsys, "Coordinate system reference");
    imageout.addKey("CTYPE1",   ctype1,   "R.A. in tangent plane projection");
    imageout.addKey("CTYPE2",   ctype2,   "DEC. in tangent plane projection");
    imageout.addKey("CRVAL1",   crval1,   "Reference coordinate of first axis");
    imageout.addKey("CRVAL2",   crval2,   "Reference coordinate of second axis");
    imageout.addKey("CRPIX1",   crpix1,   "Reference pixel of first axis");
    imageout.addKey("CRPIX2",   crpix2,   "Reference pixel of second axis");
    imageout.addKey("CD1_1",    cd11,     "Linear projection matrix");
    imageout.addKey("CD1_2",    cd12,     "Linear projection matrix");
    imageout.addKey("CD2_1",    cd21,     "Linear projection matrix");
    imageout.addKey("CD2_2",    cd22,     "Linear projection matrix");
    imageout.addKey("FILTER",   filter,   "Filter combination");
    imageout.addKey("OBJECT",   object,   "Object name");
    imageout.addKey("EXPTIME",  exptime , "Exposure time [seconds]");
    imageout.addKey("EQUINOX",  equinox,  "Equinox of coordinate system");
    imageout.addKey("DATE-OBS", dateobs,  "UT Date of observation");
    imageout.addKey("MJD-OBS",  mjdobs,   "MJD of start of obseration");
    imageout.addKey("AIRMASS",  airmass,  "Mean airmass for the observation");
    imageout.addKey("SATURATE", saturate, "Lowest Saturation value for channel A/B");
    for (i=1; i<=50; i++) imageout.addKey("DUMMY"+NumberToString(i), 0.0, "");
    sem_post(&semaphor);
  }
}


//****************************************************************
void read_xtalk(string xtalkfile, _xtalk_ &xtalk)
{
  string vic_tmp, src_tmp;
  float x_tmp, xerr_tmp, srcnl_tmp, p1_tmp, p2_tmp, p3_tmp;

  // Read the file; a typical line looks like this:
  // ccd26B ccd26A -5.36e-04 6.86e-06 4.373232e+04 -6.703301e-04 3.463778e-08 9.838246e-13
  ifstream input(xtalkfile.c_str());
  if (!input.is_open()) {
    cerr <<  "ERROR: Could not open " << xtalkfile << "!\n";
    exit (1);
  }
  else {
    //Parse out the comment header line
    input.ignore (numeric_limits<streamsize>::max(),'\n');
    // Read the actual data
    while ( input.good() ) {
      input >> vic_tmp >> src_tmp >> x_tmp >> xerr_tmp >> srcnl_tmp >> p1_tmp >> p2_tmp >> p3_tmp;
      // Remove the 'ccd' substring
      vic_tmp = RemoveChars(vic_tmp,"cd");
      src_tmp = RemoveChars(src_tmp,"cd");
      // Channel A or Channel B?
      size_t vic_channelpos = vic_tmp.find("A");
      size_t src_channelpos = src_tmp.find("A");
      if (vic_channelpos != string::npos) xtalk.vic_channel.push_back("A");
      else xtalk.vic_channel.push_back("B");
      if (src_channelpos != string::npos) xtalk.src_channel.push_back("A");
      else xtalk.src_channel.push_back("B");
      // Extract chip number
      xtalk.vic_number.push_back( atoi( vic_tmp.erase( vic_tmp.length()-1,1).c_str() ) );
      xtalk.src_number.push_back( atoi( src_tmp.erase( src_tmp.length()-1,1).c_str() ) );
      // Get the rest
      xtalk.x.push_back(x_tmp);
      xtalk.xerr.push_back(xerr_tmp);
      xtalk.srcnl.push_back(srcnl_tmp);
      xtalk.p1.push_back(p1_tmp);
      xtalk.p2.push_back(p2_tmp);
      xtalk.p3.push_back(p3_tmp);
    }
  }
  input.close();
}


//****************************************************************
void read_linearity(string linearityfile, 
		    vector< vector<double>> &ADU, 
		    vector< vector<double>> &ADU_LINEAR_A, 
		    vector< vector<double>> &ADU_LINEAR_B)
{
  int ex;
  auto_ptr<FITS> pInfile(new FITS(linearityfile.c_str(),Read,false));
  
  for (ex=1; ex<=62; ex++) {
    // select the extension
    ExtHDU& table = pInfile->extension(ex);
    long nrows = table.rows();
    // read the data
    table.column("ADU").read( ADU[ex-1], 1, nrows);
    table.column("ADU_LINEAR_A").read( ADU_LINEAR_A[ex-1], 1, nrows);
    table.column("ADU_LINEAR_B").read( ADU_LINEAR_B[ex-1], 1, nrows);
  }
}
