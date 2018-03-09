#include <iomanip>
#include <cstdlib>
#include <cstring> 
#include <cstdio>  
#include <cmath>   
#include <sstream> 
#include <fstream> 
#include <iostream>
#include <vector>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "          -f filter curve\n";
    cerr << "          -r reflectivity\n";
    cerr << "          -n number of reflective surfaces\n";
    cerr << "          -q quantum efficiency\n";
    cerr << "          -s flux-calibrated spectrum\n";
    cerr << "          -w lambda_min lambda_max\n";
    cerr << "         [-e atmospheric extinction at airmass 1.0 (default: Paranal)]\n";
    cerr << "         [-a airmass (default: 1.0)]\n";
    cerr << "         [-o ouput file for convolved flux-calibrated spectrum]\n";
  }

  exit (0);
}


//*******************************************************************
int main(int argc, char *argv[])
{
  const int FILEMAX = 4096;
  char input_filter[FILEMAX];
  char input_reflectance[FILEMAX];
  char input_spectrum[FILEMAX];
  char input_qe[FILEMAX];
  char input_extinction[FILEMAX];
  char output_spectrum[FILEMAX];
  
  int n_ref = 2;
  long i;
  double airmass = 1.0;

  bool flag_r, flag_s, flag_e, flag_q, flag_f;

  double lambda_min = 3300.;
  double lambda_max = 9000.;

  flag_r = flag_s = flag_e = flag_q = flag_f = false;

  // the wavelengths at which we have atmospheric extinction
  double lambda_paranal[80] = {
    3100.0, 3200.0, 3300.0, 3325.0, 3375.0, 3425.0, 3475.0, 3525.0, 3575.0, 3625.0, 3675.0, 3725.0, 
    3775.0, 3825.0, 3875.0, 3925.0, 3975.0, 4025.0, 4075.0, 4125.0, 4175.0, 
    4225.0, 4275.0, 4325.0, 4375.0, 4425.0, 4475.0, 4525.0, 4575.0, 4625.0, 
    4675.0, 4725.0, 4775.0, 4825.0, 4875.0, 4925.0, 4975.0, 5025.0, 5075.0, 
    5125.0, 5175.0, 5225.0, 5275.0, 5325.0, 5375.0, 5425.0, 5475.0, 5525.0, 
    5575.0, 5625.0, 5675.0, 5725.0, 5775.0, 5825.0, 5875.0, 5925.0, 5975.0, 
    6025.0, 6075.0, 6125.0, 6175.0, 6225.0, 6275.0, 6325.0, 6375.0, 6425.0, 
    6475.0, 6525.0, 6575.0, 6625.0, 6675.0, 6725.0, 6775.0, 7060.0, 7450.0, 
    7940.0, 8500.0, 8675.0, 8850.0, 10000.0};
  
  // mean Paranal extinction at airmass 1.0
  double data_paranal[80] = {0.85, 0.80, 0.75,
    0.686, 0.606, 0.581, 0.552, 0.526, 0.504, 0.478, 0.456, 0.430, 0.409, 
    0.386, 0.378, 0.363, 0.345, 0.330, 0.316, 0.298, 0.285, 0.274, 0.265, 
    0.253, 0.241, 0.229, 0.221, 0.212, 0.204, 0.198, 0.190, 0.185, 0.182, 
    0.176, 0.169, 0.162, 0.157, 0.156, 0.153, 0.146, 0.143, 0.141, 0.139, 
    0.139, 0.134, 0.133, 0.131, 0.129, 0.127, 0.128, 0.130, 0.134, 0.132, 
    0.124, 0.122, 0.125, 0.122, 0.117, 0.115, 0.108, 0.104, 0.102, 0.099, 
    0.095, 0.092, 0.085, 0.086, 0.083, 0.081, 0.076, 0.072, 0.068, 0.064, 
    0.064, 0.048, 0.042, 0.032, 0.030, 0.029, 0.022};

  // print usage if no arguments were given

  // print usage if no arguments were given
  if (argc == 1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'f': strcpy(input_filter,argv[++i]);
	flag_f = true;
	break;
      case 'r': strcpy(input_reflectance,argv[++i]);
	flag_r = true;
	break;
      case 'q': strcpy(input_qe,argv[++i]);
	flag_q = true;
	break;
      case 'e': strcpy(input_extinction,argv[++i]);
	flag_e = true;
	break;
      case 's': strcpy(input_spectrum,argv[++i]);
	flag_s = true;
	break;
      case 'o': strcpy(output_spectrum,argv[++i]);
	break;
      case 'a': airmass = atof(argv[++i]);
	break;
      case 'w': lambda_min = atof(argv[++i]);
	lambda_max = atof(argv[++i]);
	break;
      case 'n': n_ref = atoi(argv[++i]);
	break;
      }
    }
  }

  double lambda_tmp, data_tmp;

  vector<double> lambda_f, data_f;
  vector<double> lambda_q, data_q;
  vector<double> lambda_s, data_s;
  vector<double> lambda_r, data_r;
  vector<double> lambda_e, data_e;

  // The filter function
  ifstream instream_f(input_filter);
  if (flag_f) {
    if (!instream_f.is_open()) {
      cerr <<  "ERROR: Could not open " << input_filter << "!\n";
      exit (1);
    }
    while (instream_f >> lambda_tmp >> data_tmp) {
      lambda_f.push_back(lambda_tmp);
      data_f.push_back(data_tmp);
    }
    instream_f.close();
  }
  else {
    cerr << "ERROR: No filter curve was supplied!" << endl;
    exit (1);
  }

  // The quantum efficiency
  ifstream instream_q(input_qe);
  if (flag_q) {
    if (!instream_q.is_open()) {
      cerr <<  "ERROR: Could not open " << input_qe << "!\n";
      exit (1);
    }
    while (instream_q >> lambda_tmp >> data_tmp) {
      lambda_q.push_back(lambda_tmp);
      data_q.push_back(data_tmp);
    }
    instream_q.close();
  }
  else {
    lambda_q.push_back(3000.);
    lambda_q.push_back(10000.);
    data_q.push_back(1.0);
    data_q.push_back(1.0);
  }
  
  // The reflectance
  ifstream instream_r(input_reflectance);
  if (flag_r) {
    if (!instream_r.is_open()) {
      cerr <<  "ERROR: Could not open " << input_reflectance << "!\n";
      exit (1);
    }
    while (instream_r >> lambda_tmp >> data_tmp) {
      lambda_r.push_back(lambda_tmp);
      data_r.push_back(pow(data_tmp,n_ref));
    }
    instream_r.close();
  }
  else {
    lambda_r.push_back(3000.);
    lambda_r.push_back(10000.);
    data_r.push_back(1.0);
    data_r.push_back(1.0);
  }
  
  // The flux-calibrated spectrum
  ifstream instream_s(input_spectrum);
  if (flag_s) {
    if (!instream_s.is_open()) {
      cerr <<  "ERROR: Could not open " << input_spectrum << "!\n";
      exit (1);
    }
    while (instream_s >> lambda_tmp >> data_tmp) {
      lambda_s.push_back(lambda_tmp);
      data_s.push_back(data_tmp);
    }
    instream_s.close();
  }
  else {
    lambda_s.push_back(3000.);
    lambda_s.push_back(10000.);
    data_s.push_back(1.0);
    data_s.push_back(1.0);
  }

  // The extinction (actually, atmospheric throughput)
  ifstream instream_e(input_extinction);
  if (flag_e) {
    if (!instream_e.is_open()) {
      cerr <<  "ERROR: Could not open " << input_extinction << "!\n";
      exit (1);
    }
    while (instream_e >> lambda_tmp >> data_tmp) {
      lambda_e.push_back(lambda_tmp);
      data_e.push_back(exp( log( 1. - data_tmp) * airmass));
    }
    instream_e.close();
  }
  else {
    for (i=0; i<77; i++) {
      lambda_e.push_back(lambda_paranal[i]);
      data_e.push_back(data_paranal[i]);
    }
  }

  double *lambda_e_arr = new double[lambda_e.size()];
  double *lambda_r_arr = new double[lambda_r.size()];
  double *lambda_f_arr = new double[lambda_f.size()];
  double *lambda_q_arr = new double[lambda_q.size()];
  double *data_e_arr = new double[data_e.size()];
  double *data_r_arr = new double[data_r.size()];
  double *data_f_arr = new double[data_f.size()];
  double *data_q_arr = new double[data_q.size()];

  for (size_t i=0; i<lambda_e.size(); i++) {
    lambda_e_arr[i] = lambda_e[i];
    data_e_arr[i]   = data_e[i];
  }
  for (size_t i=0; i<lambda_r.size(); i++) {
    lambda_r_arr[i] = lambda_r[i];
    data_r_arr[i]   = data_r[i];
  }
  for (size_t i=0; i<lambda_f.size(); i++) {
    lambda_f_arr[i] = lambda_f[i];
    data_f_arr[i]   = data_f[i];
  }
  for (size_t i=0; i<lambda_q.size(); i++) {
    lambda_q_arr[i] = lambda_q[i];
    data_q_arr[i]   = data_q[i];
  }


  // create interpolating function for extinction 
  gsl_interp_accel *acc_e = gsl_interp_accel_alloc();
  gsl_spline *spline_e = gsl_spline_alloc(gsl_interp_linear, lambda_e.size());
  gsl_spline_init(spline_e, lambda_e_arr, data_e_arr, lambda_e.size());

  // create interpolating function for reflectance
  gsl_interp_accel *acc_r = gsl_interp_accel_alloc();
  gsl_spline *spline_r = gsl_spline_alloc(gsl_interp_linear, lambda_r.size());
  gsl_spline_init(spline_r, lambda_r_arr, data_r_arr, lambda_r.size());

  // create interpolating function for quantum efficiency
  gsl_interp_accel *acc_q = gsl_interp_accel_alloc();
  gsl_spline *spline_q = gsl_spline_alloc(gsl_interp_linear, lambda_q.size());
  gsl_spline_init(spline_q, lambda_q_arr, data_q_arr, lambda_q.size());

  // create interpolating function for filter
  gsl_interp_accel *acc_f = gsl_interp_accel_alloc();
  gsl_spline *spline_f = gsl_spline_alloc(gsl_interp_linear, lambda_f.size());
  gsl_spline_init(spline_f, lambda_f_arr, data_f_arr, lambda_f.size());

  vector<double> spectrum_conv;
  vector<double> lambda_conv;

  for (size_t i=0; i<lambda_s.size(); i++) {
    if (lambda_s[i] >= lambda_min && lambda_s[i] <= lambda_max) {
      spectrum_conv.push_back(data_s[i] * 
			      gsl_spline_eval(spline_e, lambda_s[i], acc_e) * 
			      gsl_spline_eval(spline_r, lambda_s[i], acc_r) * 
			      gsl_spline_eval(spline_f, lambda_s[i], acc_f) * 
			      gsl_spline_eval(spline_q, lambda_s[i], acc_q));
      lambda_conv.push_back(lambda_s[i]);
    }
  }
  
  // Integrate the flux over the bandpass
  double dlambda, flux = 0.;
  for (size_t i=1; i<lambda_conv.size()-1; i++) {
    // The size of the wavelength bin, centered on the current measurement
    dlambda = fabs(0.5 * (lambda_conv[i+1] - lambda_conv[i-1]));
    // Increment the flux
    flux += dlambda * spectrum_conv[i];
  }
   
  cout << flux << endl;

  // open the output file
  ofstream outfile(output_spectrum);
  if (!outfile.is_open()) {
    cerr << "ERROR: Could not open file " << output_spectrum << "!\n";
    exit (1);
  }

  for (size_t i=0; i<spectrum_conv.size(); i++) {
    outfile << lambda_conv[i] << " " << spectrum_conv[i] << endl;
  }
  outfile.close();

  gsl_spline_free(spline_e);
  gsl_spline_free(spline_r);
  gsl_spline_free(spline_f);
  gsl_spline_free(spline_q);
  gsl_interp_accel_free(acc_e);
  gsl_interp_accel_free(acc_r);
  gsl_interp_accel_free(acc_f);
  gsl_interp_accel_free(acc_q);

  exit (0);
}
