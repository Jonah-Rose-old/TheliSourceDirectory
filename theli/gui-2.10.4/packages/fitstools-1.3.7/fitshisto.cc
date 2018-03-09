#include <cstring> // for strcpy()
#include <fstream> // for std::ofstream file
#include <iomanip>
#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -o output_file \n";
    cerr << "           -l high-level (0.99)\n";
    cerr << "           -b black-level (0.01)\n";
    cerr << "           -v verbose\n";
    cerr << "  PURPOSE: Histogram of fitsimages, cuts 20%% as frame \n";

    exit(1);
  }
}

void  clear_histo(long array[4096]){
  int i;
  for (i=0; i<4096; i++) array[i] = 0;
}

int main(int argc, char *argv[])
{
  int verbose=0;

  int n, m, n0, m0, nmax, mmax ;  // imagewidth  and -height
  float frame=0.2;                      // 20% cut off
  long i, j;                           // some Counter 
  long k1=0, k2=0; // k1 input pixel counter within frame, k2 processed pixel counter
  long t,sum=0;    // t interval counter, sum  input pixel minus white  pixel
  float  level, white99,count_white, count_black;  // pixel value, upper white interval level
  float  black1;          // pixel value, lower black interval level
  string input_image; // filename string input image file
  char out1[200];         // filename string output histogram file
  float high_level=0.99;    // numerical percent level of white, default, 
  float black_level=0.01;  // numerical percent level of black, default, 
                          // depending on commandline parameters l, b

  long histo[4096];       // histogram-Array, used twice: first whole image levels
			  // second levels below high level
  
  float maximum=-100000.0, minimum=100000.0, mean=0;  // image statistic levels 
  float range,  step;   // histogram  dimensions
                          // range = max -min or white99 - min
                          // step = histogram width of a single step

  int interval=256;      // histogram intervals, up to 4096, saves processing time 
  int stretch=4;          // stretch 1 bis 16 um den Weisspunkt zu praezisieren
                          // first extends on stretch * interval for better whitepoint
                          // calculation (512-2048), then reduces back to 256 for
                          // output interval (256)

  interval=interval*stretch;

  // end variable declaration and default definition ............................
  
  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': strcpy(out1,argv[++i]);
	break;
      case 'l': high_level = atof(argv[++i]);
	break;
      case 'b': black_level = atof(argv[++i]);
	break;
      case 'v': verbose=1;++i;
	break;
      }
    }
  }

  checkfile(input_image);

  if (verbose) cout << "step 1: File IO\n";

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  ofstream file(out1);

  if (!file.is_open()) {
    cerr << "ERROR: Could not open " << out1 << endl;
    return 1;
  }

  clear_histo(histo);

  n0 = frame*n;         // cut frame off at bottom
  m0 = frame*m;         // cut frame off at left
  nmax = n*(1.0-frame); // cut frame off at top
  mmax = m*(1.0-frame); // cut frame off at right
  
  if (verbose) cout << "step 2: Check Max, Min, Mean of whole image\n";
  
  // first run: max, min, mean, 
  for (j=m0; j<mmax; j++) {
    for (i=n0; i<nmax; i++) {
      level = image.data[i+n*j];
      if ( level > maximum ) maximum = level;
      else {
	if ( level < minimum ) minimum = level;
      }
      mean += level;
      k1++;
    }
  }
  mean /= k1;
  range = maximum - minimum;
  step = range / (interval - 1);

  if (verbose) printf("        max %8.2f min %8.2f mean %8.2f range %8.2f step %8.2f\n", maximum, minimum, mean, range, step);

  // total histogram .........................................
  if (verbose) cout << "step 3: histogram round 1\n";
  if (verbose) printf("        M %i Mmax %i M0 %i N %i Nmax %i N %i \n",m,mmax,m0,n,nmax,n0);

  for (j=m0; j<mmax; j++) {
    for (i=n0; i<nmax; i++) {
      level = image.data[i+n*j];
      if (level < maximum && level > minimum){
	t = (int)((level-minimum)/range *(interval-1)); //%interval;
	histo[t]++;
      }
      // rounding errors: max value may be larger than last step, likewise for min value
      else {
	if (level >= maximum) histo[interval-1]++;
	if (level <= minimum) histo[0]++;
      }
      k2++;
    }
    if (verbose && (j%100==0)) cout << ".";
  }

  if (verbose ) cout << endl;

  // get upper level where whitepoint fits to high_level ............ 
  if (verbose) cout << "step 4a: search whitepoint\n";

  sum = k1;   // number of pixels within frame
  count_white = k1*high_level;   // total pixels below hih-level
  white99 = interval-1; // auf default max setzen
  if (verbose) cout << "step 4b: count down to whitepoint\n";
  for (i=interval-1; i>=0; i--){
    sum -= histo[i];
    if (sum <= count_white) {
      white99 = i;
      break;
    }
  }

  printf("         Whitepoint (%5.1f %% )interval: %8.2f of %5i\n", 
	 high_level*100.0, white99, interval-1);

  // calculating the level
  white99 = minimum + (white99) * step; //changed in true levels

  printf("         Whitepoint-Level: %8.2f\n",white99);

  // get lower level where blackpoint fits to black_level ............
  if (verbose) cout << "step 5a: search blackpoint\n";

  sum = 0;
  count_black = k1*black_level;
  black1 = 0; // default min val

  if (verbose) cout << "step 5b: count up to blackpoint\n";

  for (i=0; i<=interval; i++){
    sum += histo[i];
    if (sum >= count_black){
      black1 = i;
      break;
    }
  }
  printf("         Blackpoint (%5.1f %% )interval: %8.2f of %5i\n",black_level*100.0,black1,interval-1);

  // calculating the level
  black1 = minimum + (black1) * step; // in true levels

  printf("         Blackpoint-Level: %8.2f\n",black1);

  //----------------------------------------------------
  // second run , reduce histogramm  up to whitepoint........................
  if (verbose) cout << "step 6: run 2., final histogram in 256 steps\n";
  interval /= stretch;
  k2 = 0;
  clear_histo(histo);
  // new range and steps
  range = white99 - black1;
  step = range / (interval-1);
  if (verbose) printf("        Range %8.2f step %8.2f\n", range, step);
  for (j=m0; j<mmax; j++) {
    for (i=n0; i<nmax; i++) {
      level = image.data[i+n*j];
      if (level < white99 && level > black1){
	t = (int)((level-black1)/range *(interval-1));// %interval;
	histo[t]++ ;
      }
      else{
	// accomodate rounding errors
	if (level >= white99)  histo[interval-1]++;
	if (level <= black1)   histo[0]++;
      }
      k2++;
    }
    if (verbose && (j%100==0)) cout << ".";
  }
  if (verbose ) cout << endl;

  if (verbose)  printf("        Counts in: %li  out: %li\n",k1,k2);
  if (verbose)  printf("        active pixel %4.2f%% \n",k2*1.0/k1*100.0);
  
  file << setiosflags(ios::fixed) << setprecision(2) << "# 8-Bit-Histogramm for " << input_image << endl;
  file << "# whitelevel | blacklevel: " << high_level << " | " << black_level << endl;
  file << "# max:   " << maximum << endl;
  file << "# min:   " << minimum << endl;
  file << "# mean:  " << mean << " (N = )" << k1 << ")" << endl; 
  file << "# white: " << white99 << endl;
  file << "# black: " << black1 << "\n" << endl;
  file << "# step;<;interval;counts" << endl;

  if(verbose) cout << "step 7: output 256 histogram data to file\n";
  for (i=0; i<interval; i++) {
    file << i << " < " << (i+1)*step+black1 << " " << histo[i] << endl;
  }
  file.close();

  return 0;
}
