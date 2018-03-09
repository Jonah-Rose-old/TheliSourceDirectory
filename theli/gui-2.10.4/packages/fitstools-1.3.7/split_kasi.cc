#include <CCfits>
#include <thread>
#include <fitsio.h>
#include <fstream>
#include <unistd.h>
#include "fitstools.h"
#include "statistics.h"
#include <semaphore.h>

using namespace std;


sem_t semaphor; // control the number of threads reading simultaneously

void preprocess_thread(int, int, bool, bool, float, float, 
		       string, string, string, string, myFITS&);

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image\n";
    cerr << "          [-n number of CPUs to use; default: 1]\n";
    cerr << "          [-o [y/n] (overscan correction; default: y)]\n";
    cerr << "          [-g [y/n] (gain correction; default: y)]\n";
    cerr << "      Preprocesses KASI images, and extracts individual chips.\n";
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
  string gainchoice = "y", overscanchoice = "y";
  bool dogain = true, dooverscan = true;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  // read the command line arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
	  break;
        case 'g': gainchoice = argv[++i];
	  break;
        case 'o': overscanchoice = argv[++i];
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

  // read the FITS header and the data block

  myFITS image(input_image);

  //  checkfile(input_image);
  string call = programcall(argv, argc);

  //  myFITS image(input_image);
  int n_ext = image.numExt();

  // the base of the output image name
  basename(input_image, ".fits", output_base);

  // Global header keywords we will propagate
  float exptime = atof(image.readKey("EXPTIME").c_str());
  float airmass;
  string airmass_str = image.readKey("AIRMASS").c_str();
  if (airmass_str == "NaN") airmass = 1.0;
  else airmass = atof(airmass_str.c_str());

  string dateobs = image.readKey("DATE-OBS");
  string object  = image.readKey("OBJECT");
  // replace whitespace by underscores
  reducewhitespace(object, "_", " ");
  string filter  = image.readKey("FILTER");
  // Keep the first word of the filter string, only
  vector<string> filterwords = getword(filter,' ');
  filter = filterwords[0];
  
  vector<thread> mythreads;
  int t, ex_start, ex_end;
  // create arguments and launch threads;
  // threads work on different file extensions

  sem_init(&semaphor, 0, 1);

  for(t=0; t<NUM_THREADS; t++) {
    ex_start = t * (n_ext / NUM_THREADS);
    if (t <  NUM_THREADS-1) ex_end = (t + 1) * (n_ext / NUM_THREADS) - 1;
    if (t == NUM_THREADS-1) ex_end = n_ext - 1;
    mythreads.push_back( thread( preprocess_thread, ex_start, ex_end, dogain,
				 dooverscan, exptime, airmass, dateobs,
				 object, filter, output_base, ref(image)));
  }
  
  // wait for threads to finish
  for (t=0; t<NUM_THREADS; t++)
    mythreads[t].join();

  return 0;
}


//***************************************************************************************
void preprocess_thread(int ex_start, int ex_end, bool dogain, bool dooverscan, 
		       float exptime, float airmass, string dateobs, string object, string filter, 
		       string output_base, myFITS &image)
{

  long i,j,k,t,ex;
  long n_chip, m_chip, ccdnum = 1, ichip, jchip;
  long amp_count = 0;

  vector<float> data_part;
  vector<float> data_pasted;

  // Header handling: obtain the geometries from the ExtHDUs
  for (ex=ex_start; ex<=ex_end; ex++) {

    // read the image geometry, gain, bias and data sections; extensions start with 1, not 0
    sem_wait(&semaphor);
    long n = image.readLongExtKey("NAXIS1", ex+1);
    long m = image.readLongExtKey("NAXIS2", ex+1);
    if (ex>=0  && ex<=7)  ccdnum = 1;
    if (ex>=8  && ex<=15) ccdnum = 2;
    if (ex>=16 && ex<=23) ccdnum = 3;
    if (ex>=24 && ex<=31) ccdnum = 4;

    /*
    float gaina    = image.readExtKey("GAINA", ex+1) / 4.0; // DECam average gain is around 4.0
    float gainb    = image.readExtKey("GAINB", ex+1) / 4.0;
    float saturata = image.readExtKey("SATURATA", ex+1) * gaina;
    float saturatb = image.readExtKey("SATURATB", ex+1) * gainb;
    // Choose the lower saturation value
    float saturate = (saturata < saturatb) ? saturata : saturatb;
    */

    // extract the vertices from the "[XMIN:XMAX,YMIN:YMAX]" strings
    string biassec_str = image.readStringExtKey("BIASSEC", ex+1);
    string datasec_str = image.readStringExtKey("DATASEC", ex+1);

    // split_datasection() appends to the vectors
    vector<long> biassec, datasec;
    split_datasection(biassec_str, biassec);
    split_datasection(datasec_str, datasec);

    // Read the data section
    vector<float> data;
    image.readExtData(data, ex+1);
    sem_post(&semaphor);

    // initialise 'overscan' to zero (in case we don't do it)
    long dim_oscan = (biassec[1] - biassec[0] + 1) * (biassec[3] - biassec[2] + 1);
    vector<float> oscan(dim_oscan,0.);

    //****************************************
    // Obtain OVERSCAN and GAIN if requested.
    // Otherwise default to 0.0 and 1.0
    //****************************************
    float median_overscan = 0.;
    
    // Overscan correction?
    if (dooverscan) {
      t = 0;
      for (j=biassec[2]-1; j<biassec[3]-1; j++) {
	for (i=biassec[0]-1; i<biassec[1]-1; i++) {
	  oscan[t++] = data[i+n*j];
	}
      }
      median_overscan = median(oscan,dim_oscan);
    }

    // Gain correction?
    double gain;
    if (!dogain) {
      gain = 1.0;
    }

    //************************************************
    // OVERSCAN and GAIN correction
    //************************************************
    for (j=0; j<m; j++) {
      for (i=datasec[0]-1; i<=datasec[1]-1; i++) {
	data[i+n*j] = (data[i+n*j] - median_overscan) * gain;
      }
    }

    //****************************************
    // EXTRACT CORRECTED DATA
    //****************************************

    // Extract the corrected data section
    long dim_n = datasec[1] - datasec[0] + 1;
    long dim_m = datasec[3] - datasec[2] + 1;

    n_chip = 8*dim_n;
    m_chip = dim_m;
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
    // Paste the amplifiers
    //****************************************
    
    // reset the vectors when starting a new detector
    if (ex==0 || ex==8 || ex==16 || ex==24) { 
      data_pasted.resize(n_chip*m_chip);
      amp_count = 0;
    }

    for (j=0; j<m; j++) {
      jchip = j;
      for (i=0; i<n; i++) {
	ichip = i+ex*amp_count;
	if (j>=datasec[2]-1 && j<=datasec[3]-1 &&
	    i>=datasec[0]-1 && i<=datasec[1]-1) {
	  data_pasted[ichip+n_chip*jchip] = data[i+n*j];
	}
      }
    }


    //****************************************
    // WRITE CORRECTED IMAGE
    //****************************************
    if (ex==0 || ex==8 || ex==16 || ex==24) { 
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
      imageout.addKey("AIRMASS",  airmass,  "Mean airmass for the observation");
      for (i=1; i<=50; i++) imageout.addKey("DUMMY"+NumberToString(i), 0.0, "");
      sem_post(&semaphor);
    }
  }
}

