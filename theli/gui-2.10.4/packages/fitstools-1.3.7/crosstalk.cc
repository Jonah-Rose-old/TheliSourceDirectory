#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "    This programme corrects normal crosstalk or row/col crosstalk. Either each quadrant\n";
    cerr << "    is scaled and subtracted (normal xtalk), or the average lines or columns of each\n";
    cerr << "    quadrant are subtracted from all other quadrants (row/col xtalk).\n\n";
    cerr << "    For many parallel readout stripes (MULTI XTALK) a median can be subtracted.\n\n";
    cerr << "    The input image is overwritten if no output image is given.\n\n";
    cerr << "    A negative scaling factor means that the crosstalk leaves a negative\n";
    cerr << "    imprint of one quadrant in another quadrant.\n\n";
    cerr << "  USAGE:  " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "          [-o output_image] \n";
    cerr << "          [-v scaling factor (for modes 1-9, see below)\n";
    cerr << "          [-h direction (x|y) and number of readout stripes (two arguments, for -m 10)\n";
    cerr << "           -m correction scheme, an integer from 1-9: \n";
    cerr << "              1: NORMAL XTALK, 4 readout quadrants, 2 in x- and 2 in y-direction\n";
    cerr << "              2: NORMAL XTALK, 2 readout quadrants, 1 in x- and 2 in y-direction\n";
    cerr << "              3: NORMAL XTALK, 2 readout quadrants, 2 in x- and 1 in y-direction\n";
    cerr << "              4: ROW XTALK, 4 readout quadrants, 2 in x- and 2 in y-direction\n";
    cerr << "              5: ROW XTALK, 2 readout quadrants, 1 in x- and 2 in y-direction\n";
    cerr << "              6: ROW XTALK, 2 readout quadrants, 2 in x- and 1 in y-direction\n";
    cerr << "              7: COLUMN XTALK, 4 readout quadrants, 2 in x- and 2 in y-direction\n";
    cerr << "              8: COLUMN XTALK, 2 readout quadrants, 1 in x- and 2 in y-direction\n";
    cerr << "              9: COLUMN XTALK, 2 readout quadrants, 2 in x- and 1 in y-direction\n";
    cerr << "             10: MULTI XTALK (requires option -h)\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int xlow1, xlow2, xhigh1, xhigh2;
  int ylow1, ylow2, yhigh1, yhigh2;
  int flag_o, method, nsection, flag_h, width;
  long n, m, i, j, ih, jh, il, jl, t, k, l;
  float scale, medianval;
  string input_image, output_image, direction;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  flag_o = 0;
  method = 1;
  scale  = 1.0;
  nsection = 0;
  flag_h = 0;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
	    flag_o = 1;
            break;
        case 'v': scale = atof(argv[++i]);
            break;
        case 'm': method = atoi(argv[++i]);
            break;
        case 'h': direction = argv[++i];
	    nsection = atoi(argv[++i]);
	    flag_h = 1;
            break;
      }
    }
  }

  if (method > 10 || method < 1) {
    cerr << "Error: crosstalk correction scheme (option -m) must be an integer between 1 and 10\n";
    exit (1);
  }

  if (flag_h == 0 && method == 10) {
    cerr << "ERROR: For crosstalk mode 10 you must specify the readout direction and the\n";
    cerr << "       number of readout sections with the -h argument, e.g. \"-h x 32\"\n";
    exit (1);
  }

  if (method == 10) {
    if (nsection < 2) {
      cerr << "ERROR: The number of readout sections must be larger than 1.\n";
      exit (1);
    }
    if (direction.compare("x") != 0 && direction.compare("y") != 0) {
      cerr << "ERROR: The readout direction must be either x or y.\n";
      exit (1);
    }
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // overwrite the input image if no output image is given
  if (output_image.empty()) output_image = input_image;

  // read the FITS image
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  vector<float> data(n*m,0);

  // the crosstalk images
  // we subtract from each quadrant the other quadrants, 
  // scaled with some parameter;
  // we assume that the crosstalk amplitude is the same for all quadrants

  if (method == 1) {
    xlow1  = 0;       // 1
    xlow2  = n/2-1;   // 512
    xhigh1 = n/2;     // 513
    xhigh2 = n-1;     // 1024
    ylow1  = 0;       // 1
    ylow2  = m/2-1;   // 512
    yhigh1 = m/2;     // 513
    yhigh2 = m-1;     // 1024
    
    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	ih = i + xhigh1;
	jh = j + yhigh1;
	il = i - xhigh1;
	jl = j - yhigh1;
	
	// ll = lr + ul + ur
	if (i>=xlow1 && i<=xlow2 && j>=ylow1 && j<=ylow2)
	  data[i+n*j] = image.data[i+n*j] - scale * (image.data[ih+n*j] + image.data[i+n*jh] + image.data[ih+n*jh]);
	// lr = ll + ul + ur
	if (i>=xhigh1 && i<=xhigh2 && j>=ylow1 && j<=ylow2)
	  data[i+n*j] = image.data[i+n*j] - scale * (image.data[il+n*j] + image.data[il+n*jh] + image.data[i+n*jh]);
	// ul = lr + ll + ur
	if (i>=xlow1 && i<=xlow2 && j>=yhigh1 && j<=yhigh2)
	  data[i+n*j] = image.data[i+n*j] - scale * (image.data[ih+n*jl] + image.data[i+n*jl] + image.data[ih+n*j]);
	// ur = lr + ul + ll
	if (i>=xhigh1 && i<=xhigh2 && j>=yhigh1 && j<=yhigh2)
	  data[i+n*j] = image.data[i+n*j] - scale * (image.data[i+n*jl] + image.data[il+n*j] + image.data[il+n*jl]);
      }
    }
  }

  if (method == 2) {
    xlow1  = 0;       // 1
    xhigh1 = n-1;     // 1024
    ylow1  = 0;       // 1
    ylow2  = m/2;     // 513
    yhigh1 = m/2-1;   // 512
    yhigh2 = m-1;     // 1024
    
    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	jh = j + ylow2;
	jl = j - ylow2;
	// fix lower quadrant
	if (i>=xlow1 && i<=xhigh1 && j>=ylow1 && j<=yhigh1)
	  data[i+n*j] = image.data[i+n*j] - scale * image.data[i+n*jh];
	// fix upper quadrant
	if (i>=xlow1 && i<=xhigh1 && j>=ylow2 && j<=yhigh2)
	  data[i+n*j] = image.data[i+n*j] - scale * image.data[i+n*jl];
      }
    }
  }

  if (method == 3) {
    xlow1  = 0;       // 1
    xlow2  = n/2;     // 513
    xhigh1 = n/2-1;   // 512
    xhigh2 = n-1;     // 1024
    ylow1  = 0;       // 1
    yhigh1 = m-1;     // 1024
    
    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	ih = i + xlow2;
	il = i - xlow2;
	// fix left quadrant
	if (i>=xlow1 && i<=xhigh1 && j>=ylow1 && j<=yhigh1)
	  data[i+n*j] = image.data[i+n*j] - scale * image.data[ih+n*j];
	// fix right quadrant
	if (i>=xlow2 && i<=xhigh2 && j>=ylow1 && j<=yhigh1)
	  data[i+n*j] = image.data[i+n*j] - scale * image.data[il+n*j];
      }
    }
  }

  if (method == 4) {
    xlow2  = n/2;     // 513
    xhigh1 = n/2-1;   // 512
    xhigh2 = n-1;     // 1024
    ylow2  = m/2;     // 513
    yhigh1 = m/2-1;   // 512
    yhigh2 = m-1;     // 1024
    
    vector<float> ll_input(n/2*m/2,0);
    vector<float> lr_input(n/2*m/2,0);
    vector<float> ul_input(n/2*m/2,0);
    vector<float> ur_input(n/2*m/2,0);
    vector<float> ll_mean(n/2*m/2,0);
    vector<float> lr_mean(n/2*m/2,0);
    vector<float> ul_mean(n/2*m/2,0);
    vector<float> ur_mean(n/2*m/2,0);
    
    t = 0;
    for (j=0; j<m/2; j++) {
      for (i=0; i<n/2; i++) {
	ll_input[t++] = image.data[i+n*j];
      }
    }
    t = 0;
    for (j=0; j<m/2; j++) {
      for (i=n/2; i<n; i++) {
	lr_input[t++] = image.data[i+n*j];
      }
    }
    t = 0;
    for (j=m/2; j<m; j++) {
      for (i=0; i<n/2; i++) {
	ul_input[t++] = image.data[i+n*j];
      }
    }
    t = 0;
    for (j=m/2; j<m; j++) {
      for (i=n/2; i<n; i++) {
	ur_input[t++] = image.data[i+n*j];
      }
    }
    
    collapse(ll_input, ll_mean, "x", n/2, m/2, "mean");
    collapse(lr_input, lr_mean, "x", n/2, m/2, "mean");
    collapse(ul_input, ul_mean, "x", n/2, m/2, "mean");
    collapse(ur_input, ur_mean, "x", n/2, m/2, "mean");
    
    // ll = ll - scale * mean(ll+lr+ul+ur)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n/2; i++) {
	data[i+n*j] = image.data[i+n*j] -
	  scale * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
      }
    }

    // lr = lr - scale * mean(lr+ll+ul+ur)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n/2; i++) {
	ih = i + xlow2;
	data[ih+n*j] = image.data[ih+n*j] -
	  scale * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
      }
    }
    
    // ul = ul - scale * mean(ul+lr+ll+ur)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n/2; i++) {
	jh = j + ylow2;
	data[i+n*jh] = image.data[i+n*jh] -
	  scale * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
      }
    }
    
    // ur = ur - scale * mean(ur+lr+ul+ll)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n/2; i++) {
	ih = i + xlow2;
	jh = j + ylow2;
	data[ih+n*jh] = image.data[ih+n*jh] -
	  scale * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
      }
    }
  }

  if (method == 5) {
    xhigh1 = n-1;     // 1024
    ylow2  = m/2;     // 513
    yhigh1 = m/2-1;   // 512
    yhigh2 = m-1;     // 1024
    
    vector<float> bo_input(n*m/2,0);
    vector<float> up_input(n*m/2,0);
    vector<float> bo_mean(n*m/2,0);
    vector<float> up_mean(n*m/2,0);
    
    t = 0;
    for (j=0; j<m/2; j++) {
      for (i=0; i<n; i++) {
	bo_input[t++] = image.data[i+n*j];
      }
    }
    t = 0;
    for (j=m/2; j<m; j++) {
      for (i=0; i<n; i++) {
	up_input[t++] = image.data[i+n*j];
      }
    }
    
    collapse(bo_input, bo_mean, "x", n, m/2, "mean");
    collapse(up_input, up_mean, "x", n, m/2, "mean");
    
    // bo = bo - scale * mean(bo+up)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n; i++) {
	data[i+n*j] = image.data[i+n*j] - scale * (bo_mean[i+n*j] + up_mean[i+n*j]);
      }
    }
    
    // up = up - scale * mean(bo+up)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n; i++) {
	jh = j + ylow2;
	data[i+n*jh] = image.data[i+n*jh] - scale * (bo_mean[i+n*j] + up_mean[i+n*j]);
      }
    }
  }

  if (method == 6) {
    xlow2  = n/2;     // 513
    xhigh1 = n/2-1;   // 512
    xhigh2 = n-1;     // 1024
    yhigh1 = m-1;     // 1024
    
    vector<float> le_input(n/2*m,0);
    vector<float> ri_input(n/2*m,0);
    vector<float> le_mean(n/2*m,0);
    vector<float> ri_mean(n/2*m,0);
    
    t = 0;
    for (j=0; j<m; j++) {
      for (i=0; i<n/2; i++) {
	le_input[t++] = image.data[i+n*j];
      }
    }
    t = 0;
    for (j=0; j<m; j++) {
      for (i=n/2; i<n; i++) {
	ri_input[t++] = image.data[i+n*j];
      }
    }
    
    collapse(le_input, le_mean, "x", n/2, m, "mean");
    collapse(ri_input, ri_mean, "x", n/2, m, "mean");
    
    // le = le - scale * mean(le+ri)
    for (j=0; j<m; j++) {
      for (i=0; i<n/2; i++) {
	data[i+n*j] = image.data[i+n*j] - scale * (le_mean[i+n/2*j] + ri_mean[i+n/2*j]);
      }
    }
    
    // ri = ri - scale * mean(le+ri)
    for (j=0; j<m; j++) {
      for (i=0; i<n/2; i++) {
	ih = i + xlow2;
	data[ih+n*j] = image.data[ih+n*j] - scale * (le_mean[i+n/2*j] + ri_mean[i+n/2*j]);
      }
    }
  }
  

  if (method == 7) {
    xlow2  = n/2;     // 513
    xhigh1 = n/2-1;   // 512
    xhigh2 = n-1;     // 1024
    ylow2  = m/2;     // 513
    yhigh1 = m/2-1;   // 512
    yhigh2 = m-1;     // 1024
    
    vector<float> ll_input(n/2*m/2,0);
    vector<float> lr_input(n/2*m/2,0);
    vector<float> ul_input(n/2*m/2,0);
    vector<float> ur_input(n/2*m/2,0);
    vector<float> ll_mean(n/2*m/2,0);
    vector<float> lr_mean(n/2*m/2,0);
    vector<float> ul_mean(n/2*m/2,0);
    vector<float> ur_mean(n/2*m/2,0);
    
    t = 0;
    for (j=0; j<m/2; j++) {
      for (i=0; i<n/2; i++) {
	ll_input[t++] = image.data[i+n*j];
      }
    }
    t = 0;
    for (j=0; j<m/2; j++) {
      for (i=n/2; i<n; i++) {
	lr_input[t++] = image.data[i+n*j];
      }
    }
    t = 0;
    for (j=m/2; j<m; j++) {
      for (i=0; i<n/2; i++) {
	ul_input[t++] = image.data[i+n*j];
      }
    }
    t = 0;
    for (j=m/2; j<m; j++) {
      for (i=n/2; i<n; i++) {
	ur_input[t++] = image.data[i+n*j];
      }
    }
    
    collapse(ll_input, ll_mean, "y", n/2, m/2, "mean");
    collapse(lr_input, lr_mean, "y", n/2, m/2, "mean");
    collapse(ul_input, ul_mean, "y", n/2, m/2, "mean");
    collapse(ur_input, ur_mean, "y", n/2, m/2, "mean");
    
    // ll = ll - scale * mean(ll+lr+ul+ur)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n/2; i++) {
	data[i+n*j] = image.data[i+n*j] -
	  scale * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
      }
    }
    
    // lr = lr - scale * mean(lr+ll+ul+ur)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n/2; i++) {
	ih = i + xlow2;
	data[ih+n*j] = image.data[ih+n*j] -
	  scale * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
      }
    }
    
    // ul = ul - scale * mean(ul+lr+ll+ur)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n/2; i++) {
	jh = j + ylow2;
	data[i+n*jh] = image.data[i+n*jh] -
	  scale * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
      }
    }
    
    // ur = ur - scale * mean(ur+lr+ul+ll)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n/2; i++) {
	ih = i + xlow2;
	jh = j + ylow2;
	data[ih+n*jh] = image.data[ih+n*jh] -
	  scale * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
      }
    }
  }

  if (method == 8) {
    xhigh1 = n-1;     // 1024
    ylow2  = m/2;     // 513
    yhigh1 = m/2-1;   // 512
    yhigh2 = m-1;     // 1024
    
    vector<float> bo_input(n*m/2,0);
    vector<float> up_input(n*m/2,0);
    vector<float> bo_mean (n*m/2,0);
    vector<float> up_mean (n*m/2,0);
    
    // bo = bo - scale * mean(bo+up)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n; i++) {
	data[i+n*j] = image.data[i+n*j] - scale * (bo_mean[i+n*j] + up_mean[i+n*j]);
      }
    }
    
    t = 0;
    for (j=0; j<m/2; j++) {
      for (i=0; i<n; i++) {
	bo_input[t++] = image.data[i+n*j];
      }
    }
    t = 0;
    for (j=m/2; j<m; j++) {
      for (i=0; i<n; i++) {
	up_input[t++] = image.data[i+n*j];
      }
    }
    
    collapse(bo_input, bo_mean, "y", n, m/2, "mean");
    collapse(up_input, up_mean, "y", n, m/2, "mean");
    
    // bo = bo - scale * mean(bo+up)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n; i++) {
	data[i+n*j] = image.data[i+n*j] - scale * (bo_mean[i+n*j] + up_mean[i+n*j]);
      }
    }
    
    // up = up - scale * mean(bo+up)
    for (j=0; j<m/2; j++) {
      for (i=0; i<n; i++) {
	jh = j + ylow2;
	data[i+n*jh] = image.data[i+n*jh] - scale * (bo_mean[i+n*j] + up_mean[i+n*j]);
      }
    }
  }

  if (method == 9) {
    xlow2  = n/2;     // 513
    xhigh1 = n/2-1;   // 512
    xhigh2 = n-1;     // 1024
    yhigh1 = m-1;     // 1024
    
    vector<float> le_input(n/2*m,0);
    vector<float> ri_input(n/2*m,0);
    vector<float> le_mean(n/2*m,0);
    vector<float> ri_mean(n/2*m,0);
    
    t = 0;
    for (j=0; j<m; j++) {
      for (i=0; i<n/2; i++) {
	le_input[t++] = image.data[i+n*j];
      }
    }
    t = 0;
    for (j=0; j<m; j++) {
      for (i=n/2; i<n; i++) {
	ri_input[t++] = image.data[i+n*j];
      }
    }
    
    collapse(le_input, le_mean, "y", n/2, m, "mean");
    collapse(ri_input, ri_mean, "y", n/2, m, "mean");
    
    // le = le - scale * mean(le+ri)
    for (j=0; j<m; j++) {
      for (i=0; i<n/2; i++) {
	data[i+n*j] = image.data[i+n*j] - scale * (le_mean[i+n/2*j] + ri_mean[i+n/2*j]);
      }
    }
    
    // ri = ri - scale * mean(le+ri)
    for (j=0; j<m; j++) {
      for (i=0; i<n/2; i++) {
	ih = i + xlow2;
	data[ih+n*j] = image.data[ih+n*j] - scale * (le_mean[i+n/2*j] + ri_mean[i+n/2*j]);
      }
    }
  }
  
  // this is for the typical readout modes of hawaii2 arrays
  if (method == 10) {
    // allocate some memory
    vector<float> h2_median(n*m,0);
    vector<float> h2_sample(nsection,0);
    
    if (direction.compare("x") == 0) {
      width = m/nsection;
      for (j=0; j<width; j++) {
	for (i=0; i<n; i++) {
	  l = 0;
	  for (k=0; k<nsection; k++) 
	    h2_sample[l++] = image.data[i+n*(j+width*k)];
	  medianval = median(h2_sample, nsection);
	  for (k=0; k<nsection; k++) 
	    h2_median[i+n*(j+width*k)] = medianval;
	}
      }
    }

    if (direction.compare("y") == 0) {
      width = n/nsection;
      for (j=0; j<m; j++) {
	for (i=0; i<width; i++) {
	  l = 0;
	  for (k=0; k<nsection; k++)
	    h2_sample[l++] = image.data[i+width*k+n*j];
	  medianval = median(h2_sample, nsection);
	  for (k=0; k<nsection; k++)
	    h2_median[i+width*k+n*j] = medianval;
	}
      }
    }

    for (k=0; k<n*m; k++) {
      data[k] = image.data[k] - h2_median[k];
    }
  }

  // write the result
  if (flag_o == 1) writeImage(output_image, input_image, data, call);
  if (flag_o == 0) writeImage(input_image, input_image, data, call);

  return 0;
}
