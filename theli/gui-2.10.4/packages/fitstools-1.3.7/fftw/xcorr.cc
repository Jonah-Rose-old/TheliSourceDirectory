#include <pthread.h>
#include <CCfits>
#include <fitsio.h>
#include <fftw3.h>
#include <unistd.h>
#include "fitstools.h"
#include "convolve.h"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>

#include "gaussian.cc"

using namespace std;

void find_peak(vector<float> &data, long ninput, long minput);

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -r ref_image \n";
    cerr << "           -c com_image \n";
    cerr << "           -o output_image\n";
    cerr << "          [-n number of CPUs (determined automatically if not specified)]\n\n";
    cerr << "  PURPOSE: Cross-correlates two images and returns the position of the correlation peak.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int n, m, ncpu, nthreads, nthreads_user;
  long i, j, n_new, m_new;
  string com_image, ref_image, output_image;
  
  nthreads_user = 0;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'r': ref_image = argv[++i];
	break;
      case 'c': com_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'n': nthreads_user = atoi(argv[++i]);
        break;
      }
    } 
  }

  // how many threads
  ncpu = sysconf(_SC_NPROCESSORS_ONLN);
  nthreads = ncpu;
  if (nthreads_user > 0) {
    if (nthreads_user > ncpu) nthreads = ncpu;
    else nthreads = nthreads_user;
  }

  // read the FITS header and the data block
  checkfile(ref_image);
  checkfile(com_image);
  myFITS image_ref(ref_image);
  myFITS image_com(com_image);
  
  n = image_ref.naxis1;
  m = image_ref.naxis2;
  
  // Zeropad data, to accomodate very large offsets 
  // Max offset is the full field of view, thus we double the image size 
  n_new = 2*n-1;
  m_new = 2*m-1;

  int left, right, bottom, top;

  vector<float> refpad(n_new*m_new,0);
  vector<float> compad(n_new*m_new,0);
  vector<float> data_out(n_new*m_new,0);

  left = n/2;
  right = n/2;
  bottom = m/2;
  top = m/2;

  for (j=0; j<m_new; j++) {
    for (i=0; i<n_new; i++) {
      if (i>=left && i<n_new-right && j>=bottom && j<m_new-top) {
	refpad[i+n_new*j] = image_ref.data[i-left + n*(j-bottom)];
	compad[i+n_new*j] = image_com.data[i-left + n*(j-bottom)];
      }
    }
  }

  // cross-correlate the two images
  xcorr(refpad, compad, data_out, n_new, m_new, nthreads);

  writeImage(output_image, ref_image, data_out, n_new, m_new);

  find_peak(data_out, n_new, m_new);

  return 0;
}


//***************************************************************
void find_peak(vector<float> &data, long ninput, long minput)
{
  long i, j, ipos = 0, jpos = 0;
  double max = 0., xpos, ypos;
  
  // ignore the boundary (FFT may put spurious pixels there)
  // we choose a 3 pixel wide boundary, so there won't be any boundary issues when assigning the 7x7 subarray
  for (j=3; j<minput-3; j++) {
    for (i=3; i<ninput-3; i++) {
      if (data[i+ninput*j] >= max) {
	ipos = i;
	jpos = j;
	max = (double) data[i+ninput*j];
      }
    }
  }

  xpos = (double) ipos;
  ypos = (double) jpos;

  // we fit a 7x7 pixel area centred on the maximum pixel
  const size_t nx = 7, ny = 7, n = 7*7;
  const gsl_multifit_fdfsolver_type *T;
  gsl_multifit_fdfsolver *s;
  int status;
  unsigned int k, iter = 0;
  const size_t p = 4;

  gsl_matrix *covar = gsl_matrix_alloc (p, p);
  double z[n], sigma[n];
  struct data d = { nx, ny, z, sigma };

  gsl_multifit_function_fdf f;
  const gsl_rng_type *type;
  gsl_rng *r;

  gsl_rng_env_setup();

  type = gsl_rng_default;
  r = gsl_rng_alloc (type);
  
  f.f = &gauss_f;
  f.df = &gauss_df;
  f.fdf = &gauss_fdf;
  f.n = n;
  f.p = p;
  f.params = &d;

  // The data in which we search the maximum
  k = 0;
  for (j=-3; j<=3; j++) {
    for (i=-3; i<=3; i++) {
      z[k] = data[ipos+i+ninput*(jpos+j)] / max;
      sigma[k++] = 1.;  // same weight for all pixels
    }
  }

  // the initial guess has to be as close as possible (totally sucks...)  
  double x_init[4] = { 1.0, 3.0, 3.0, 2.0 };

  gsl_vector_view x = gsl_vector_view_array (x_init, p);

  T = gsl_multifit_fdfsolver_lmsder;
  s = gsl_multifit_fdfsolver_alloc (T, n, p);
  gsl_multifit_fdfsolver_set (s, &f, &x.vector);
  
  do {
    iter++;
    status = gsl_multifit_fdfsolver_iterate (s);
    
    if (status) break;

    status = gsl_multifit_test_delta (s->dx, s->x, 1e-4, 1e-4);
  }
  while (status == GSL_CONTINUE && iter < 50000);
  
  //  gsl_multifit_covar (s->J, 0.0, covar);
  
  #define FIT(i) gsl_vector_get(s->x, i)
  // #define ERR(i) sqrt(gsl_matrix_get(covar,i,i))
  
  printf("%.1f %.1f\n", FIT(1)+xpos-3.0+1, FIT(2)+ypos-3.0+1);

  gsl_multifit_fdfsolver_free (s);
  gsl_matrix_free (covar);
  gsl_rng_free (r);
}
