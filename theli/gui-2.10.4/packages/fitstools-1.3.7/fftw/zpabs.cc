#include <iomanip>
#include <cstdlib>
#include <cstring> 
#include <cstdio>  
#include <cmath>   
#include <sstream> 
#include <fstream> 
#include <iostream>
#include <vector>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_multifit.h>
#include "statistics.h"

using namespace std;

typedef struct {
  float airmass;
  float color;
  float magdiff;
  float weight;
  bool flag;
  bool flag_old;
} photsource;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "          -i Input catalog (instrumental mag, standard mag, colorterm, airmass)\n";
    cerr << "         [-e extinction coefficient (if fixed; must be negative; default: -0.1)]\n";
    cerr << "         [-c color term (if fixed; default: 0.0)]\n";
    cerr << "         [-z ZPmin ZPmax (lower and upper boundaries for the ZP)]\n";
    cerr << "         [-k kappa (rejection threshold in sigma; default: 2.0)]\n";
    cerr << "         [-t threshold (cut-off non-photometric data; default: 0.1 mag below the fit)]\n";
    cerr << "         [-p output absolute path for data file (default: ./)]\n";
    cerr << "         [-m max_iter (maximum number of iterations; default: 10)]\n";
    cerr << "         [-s convergence criterion (max change in ZP, EXT and COL; default: 0.01)]\n";
    cerr << "  PURPOSE: Calculates a multivariable linear fit for the photometric solution.\n\n";
  }

  exit (0);
}

void do_fit(photsource*, int, float*, float*, float*, const float, const float, int);
int get_ngood(photsource*, int);
bool reject_outliers(photsource*, int, float, float, float, float, float, float*);
void write_result(const photsource*, const string, const int, const int, const float, 
		  const float, const float, const float, const string);


//******************************************************************
int main(int argc, char *argv[])
{
  int i;

  char input_file[FILEMAX];

  float ZP = 0.;
  float EXTFIX = -0.1;
  float COLFIX = 0.;
  float extcoeff, colcoeff;
  float zpmin = 0.;
  float zpmax = 100.;
  float kappa = 2.0;
  float nonphot_cutoff = 0.1;
  float convergence = 0.01;
  string path = "./";
  int max_iter = 10;

  // print usage if no arguments were given
  if (argc == 1) usage(0, argv);

  // read the arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch (tolower((int)argv[i][1])) {
      case 'i': strcpy(input_file, argv[++i]);
        break;
      case 'e': EXTFIX = atof(argv[++i]);
        break;
      case 'c': COLFIX = atof(argv[++i]);
        break;
      case 'z': zpmin = atof(argv[++i]);
	zpmax = atof(argv[++i]);
	break;
      case 'k': kappa = atof(argv[++i]);
        break;
      case 't': nonphot_cutoff = atof(argv[++i]);
        break;
      case 's': convergence = atof(argv[++i]);
        break;
      case 'm': max_iter = atoi(argv[++i]);
        break;
      case 'p': path = argv[++i];
        break;
      }
    }
  }

  float instmag_tmp, instmag_err_tmp, stdmag_tmp, color_tmp, airmass_tmp;

  // Count how many sources we have
  ifstream instream(input_file);
  int n_tot = 0;
  float magdiff;
  if (!instream.is_open()) {
    cerr <<  "ERROR: Could not open " << input_file << "!\n";
    exit (1);
  }
  while (instream >> instmag_tmp >> instmag_err_tmp >> stdmag_tmp >> color_tmp >> airmass_tmp) {
    n_tot++;
  }

  photsource *data = new photsource[n_tot];

  // clear the error states (EOF) in the ifstream before reset
  instream.clear();
  instream.seekg(0);

  i = 0;
  while (instream >> instmag_tmp >> instmag_err_tmp >> stdmag_tmp >> color_tmp >> airmass_tmp) {
    magdiff = stdmag_tmp-instmag_tmp;
    data[i].magdiff = magdiff;
    data[i].color   = color_tmp;
    data[i].airmass = airmass_tmp;
    data[i].weight  = 1./(instmag_err_tmp*instmag_err_tmp);
    // Set the flag
    if (magdiff <= zpmin || magdiff >= zpmax) data[i].flag = false;
    else data[i].flag = true;
    data[i].flag_old = true;
    i++;
  }

  instream.close();

  cout << "#" << endl;

  // Iterate the fit until no more data points are added or removed
  // Loop over 3, 2 and 1 parameter fits
  int fitcase;

  for (fitcase=0; fitcase<=3; fitcase++) {

    if (fitcase == 0) cout << "# 3-parameter fit: ZP + EXTCOEFF + COLCOEFF" << endl;
    if (fitcase == 1) cout << "# 2-parameter fit: ZP + EXTCOEFF (COLCOEFF fixed)" << endl;
    if (fitcase == 2) cout << "# 2-parameter fit: ZP + COLCOEFF (EXTCOEFF fixed)" << endl;
    if (fitcase == 3) cout << "# 1-parameter fit: ZP only (EXTCOEFF and COLCOEFF fixed)" << endl;

    // Do an interative fit;
    // Break conditions: (1) no more changes, (2) convergence
    // (3) more than 5 iterations, (4) more than 60% of initially filtered objects rejected
    bool change_status = true;
    int n_iter = 0;
    int status = 0;
    int n_fit = 0;
    float delta_ZP  = 1.;
    float delta_EXT = 1.;
    float delta_COL = 1.;
    float old_ZP  = 0.;
    float old_EXT = 0.;
    float old_COL = 0.;
    float rmsval  = 0.;
    string message = "";
    string textstatus = "";

    while (status == 0) {

      // record old data to check for convergence
      if (n_iter > 0) {
	old_ZP = ZP;
	old_EXT = extcoeff;
	old_COL = colcoeff;
      }

      // do the fit
      do_fit(data, n_tot, &ZP, &extcoeff, &colcoeff, EXTFIX, COLFIX, fitcase);
      n_fit = get_ngood(data, n_tot);

      // check convergence
      if (n_iter > 0) {
	delta_ZP  = fabs(ZP - old_ZP);
	delta_EXT = fabs(extcoeff - old_EXT);
	delta_COL = fabs(colcoeff - old_COL);
      }

      // Reject outliers
      change_status = reject_outliers(data, n_tot, ZP, extcoeff, colcoeff, kappa, nonphot_cutoff, &rmsval);

      n_iter++;

      ostringstream ss;
      ss << n_iter;
      string head ("# ITER "+ss.str()+": ");

      // Evaluate break conditions

      // Fit converged, or no more data points taken away or added
      if ((delta_ZP <= convergence && delta_EXT <= convergence && delta_COL <= convergence) || !change_status) {
	message = head+"SUCCESS: Fit converged";
	textstatus = "GOOD";
	status = 1;
      }

      // Not converging
      if (n_iter >= max_iter) {
	ostringstream ss;
	ss << max_iter;
	message = head+"WARNING: Maximum number of iterations reached ("+ss.str()+")";
	textstatus = "BAD";
	status = 1;
      }

      // Too many data points removed
      if ( (float) get_ngood(data, n_tot) / (float) n_tot < 0.30) {
	message = head+"WARNING: > 70% of the data rejected in next fit; probably bad fit or not photometric";
	textstatus = "BAD";
	status = 1;
      }

      // report the fit results
      if (status > 0) {
	cout << message << endl;
	cout << "#   ZP    EXT   COL     RMS  N_used/N_tot" << endl;
	head = "";
      }
      cout << setprecision(5) << head << setw(6) << left << setfill('0') << ZP << " ";
      cout << setprecision(3) << setw(6) << left << extcoeff << " ";
      cout << setprecision(3) << setw(6) << left << colcoeff << " ";
      cout << setprecision(3) << setw(5) << rmsval << " " << setw(4) << right << setfill(' ') << n_fit << "/" << n_tot << endl;
      if (status > 0) {
	cout << "#" << endl;
	// write the result to a file which we use for plotting
	write_result(data, path, n_tot, fitcase, ZP, extcoeff, colcoeff, rmsval, textstatus);
      }
    }

    // Reset the flags
    for (i=0; i<n_tot; i++) {
      if (data[i].magdiff <= zpmin || data[i].magdiff >= zpmax) data[i].flag = false;
      else data[i].flag = true;
      data[i].flag_old = true;
    }
  }

  delete [] data;

}

//******************************************************************
void do_fit(photsource *data, int n_tot, float *ZP_ptr, float *extcoeff_ptr, float *colcoeff_ptr, 
	    const float EXTFIX, const float COLFIX, const int fitcase)
{
  int P;
  // fitcase = 0: 3 parameter fit
  // fitcase = 1: 2 parameter fit for ZP and EXT, COL fixed
  // fitcase = 2: 2 parameter fit for ZP and COL, EXT fixed
  // fitcase = 3: 1 parameter fit for ZP, EXT and COL fixed
  
  // Count how many (N) GOOD data points we have
  int N = get_ngood(data, n_tot);

  // The number of free parameters
  if (fitcase == 0)      P = 3;
  else if (fitcase == 1) P = 2;
  else if (fitcase == 2) P = 2;
  else if (fitcase == 3) P = 1;
  else {
    cerr << "ERROR: Invalid number of parameters!" << endl;
    exit (1);
  }

  gsl_vector *y; // observed data (magdiff)
  gsl_vector *w; // data weights
  gsl_matrix *X; // data used to predict: airmass, color term
  gsl_vector *c; // the coefficients: ZP, extcoeff, colcoeff
  gsl_matrix *cov;

  // allocate space for the matrices and vectors
  X = gsl_matrix_alloc(N, P);   // this is an input
  y = gsl_vector_alloc(N);      // this is an input
  w = gsl_vector_alloc(N);      // this is an input
  
  c = gsl_vector_alloc(P);      // this is an output
  cov = gsl_matrix_alloc(P, P); // this is an output
  
  // Put the data into the X matrix, row by row
  int ii = 0;
  for (int i=0; i<n_tot; i++) {
    if (data[i].flag) {
      // we always fit for the ZP (which is a constant)
      gsl_matrix_set(X, ii, 0, static_cast<double>(1));

      if (fitcase == 0) {
	gsl_matrix_set(X, ii, 1, static_cast<double>(data[i].airmass));
	gsl_matrix_set(X, ii, 2, static_cast<double>(data[i].color));
      }

      if (fitcase == 1) {
	gsl_matrix_set(X, ii, 1, static_cast<double>(data[i].airmass));
      }

      if (fitcase == 2) {
	gsl_matrix_set(X, ii, 1, static_cast<double>(data[i].color));
      }

      ii++;
    }
  }
  
  // fill vector of observed data
  ii = 0;
  for (int i=0; i<n_tot; i++) {
    if (data[i].flag) {
      gsl_vector_set(w, ii, data[i].weight);
      if (fitcase == 0) gsl_vector_set(y, ii, data[i].magdiff);
      if (fitcase == 1) {
	gsl_vector_set(y, ii, data[i].magdiff - COLFIX * data[i].color);
	//	cout << setprecision(6) << data[i].magdiff - COLFIX * data[i].color << " " << data[i].airmass << endl;
      }
      if (fitcase == 2) gsl_vector_set(y, ii, data[i].magdiff - EXTFIX * data[i].airmass);
      if (fitcase == 3) gsl_vector_set(y, ii, data[i].magdiff - EXTFIX * data[i].airmass - COLFIX * data[i].color);
      ii++;
    }
  }

  double chisq;
  
  // allocate temporary work space for gsl 
  gsl_multifit_linear_workspace *work;
  work = gsl_multifit_linear_alloc(N, P);
  
  // now do the fit

  gsl_multifit_wlinear(X, w, y, c, cov, &chisq, work);
    
  *ZP_ptr = gsl_vector_get(c, 0);

  if (fitcase == 0) {
    *extcoeff_ptr = gsl_vector_get(c, 1);
    *colcoeff_ptr = gsl_vector_get(c, 2);
  }
  if (fitcase == 1) {
    *extcoeff_ptr = gsl_vector_get(c, 1);
    *colcoeff_ptr = COLFIX;
  }
  if (fitcase == 2) {
    *extcoeff_ptr = EXTFIX;
    *colcoeff_ptr = gsl_vector_get(c, 1);
  }
  if (fitcase == 3) {
    *extcoeff_ptr = EXTFIX;
    *colcoeff_ptr = COLFIX;
  }

  //  cout << "# Errors:" << endl;
  // cout << setprecision(6) << sqrt(gsl_matrix_get(cov,0,0)) << " ";
  // cout << setprecision(6) << sqrt(gsl_matrix_get(cov,1,1)) << " ";
  // cout << setprecision(6) << sqrt(gsl_matrix_get(cov,2,2)) << endl;

  // Print measured against expected data values
  /*
  cout << endl;
  cout << "Measured <=> predicted" << endl;
  for (int i=0; i<N; ++i) {
    double r = gsl_vector_get(c, 0);
    r += data[i].airmass * gsl_vector_get(c, 1); 
    r += data[i].color * gsl_vector_get(c, 2);
    cout << data[i].magdiff << " <=> " << setprecision(6) << r << endl;
  }
  */

  // deallocate  
  gsl_matrix_free(X);
  gsl_matrix_free(cov);
  gsl_vector_free(y);
  gsl_vector_free(c);
  gsl_vector_free(w);
}


//******************************************************************
bool reject_outliers(photsource *data, int n_tot, float ZP, float extcoeff, 
		     float colcoeff, float kappa, float nonphot_cutoff, float *rms_ptr)
{ 
  int i = 0;
  int n_good = get_ngood(data, n_tot);
  vector<float> rms_data, residuals;
  float fit, threshold;

  bool change_status = false; // indicating something has changed

  // Calculate fit residuals (all data points, independent of the flag)
  for (i=0; i<n_tot; i++) {
    fit = ZP + extcoeff*data[i].airmass + colcoeff*data[i].color;
    // residuals of all data points
    residuals.push_back(data[i].magdiff - fit);
    // global rms of the fit for the good data points, only
    if (data[i].flag) rms_data.push_back(data[i].magdiff - fit);
  }

  *rms_ptr = rms(rms_data, n_good);

  threshold = kappa * (*rms_ptr);
  
  // Sigma reject; optionally, adds data points back in
  for (i=0; i<n_tot; i++) {
    // Copy the old flag
    data[i].flag_old = data[i].flag;

    if (fabs(residuals[i]) <= threshold) data[i].flag = true;
    else data[i].flag = false;

    // Reject obviously non-photometric data points (at least 'nonphot_cutoff' below fit)
    if (residuals[i] <= -1. * nonphot_cutoff) data[i].flag = false;

    // Check if the flag has changed
    if (data[i].flag_old != data[i].flag) change_status = true;
  }

  // return the changing status
  return change_status;
}


//******************************************************************
int get_ngood(photsource *data, int n_tot)
{
  int i;
  int n_good = 0;

  for (i=0; i<n_tot; i++) {
    if (data[i].flag) n_good++;
  }

  // Exit if no sources going into fit
  if (n_good == 0) {
    cerr << "ERROR: No sources entering the fit. Check your data / and/or constraints. e.g. ZPmin and ZPmax!" << endl;
    exit (1);
  }

  return n_good;
}


//******************************************************************
void write_result(const photsource *data, const string path, const int n_tot, const int fitcase, 
		  const float ZP, const float extcoeff, const float colcoeff, const float rmsval, 
		  const string textstatus)
{
  ostringstream ss;
  ss << fitcase;
  string outfilename (path+"/photcal_"+ss.str()+".dat");

  // open the output file
  ofstream outfile(outfilename);
  if (!outfile.is_open()) {
    cerr << "ERROR: Could not open file " << outfilename << "!\n";
    exit (1);
  }

  outfile << ZP << " " << extcoeff << " " << colcoeff << " " << rmsval << " " << textstatus << endl;
  int i;
  for (i=0; i<n_tot; i++) {
    outfile << data[i].magdiff << " " << data[i].airmass << " " << data[i].color << " " << data[i].flag << endl;
  }
  outfile.close();
}
