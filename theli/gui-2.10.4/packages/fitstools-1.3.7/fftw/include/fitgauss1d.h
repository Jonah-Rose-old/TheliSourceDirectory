#ifndef __FITGAUSS1D_H
#define __FITGAUSS1D_H

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

using namespace std;

int gauss1d_f(const gsl_vector *x, void *params, gsl_vector *f);
int gauss1d_df (const gsl_vector *x, void *params, gsl_matrix *J);
int gauss1d_fdf (const gsl_vector *x, void *params,
		 gsl_vector *f, gsl_matrix *J);
void do_gauss1d_fit(const vector<float> &, const vector<float> &,
		    const float, const float, const float, 
		    vector<float> &, const bool);

#endif
