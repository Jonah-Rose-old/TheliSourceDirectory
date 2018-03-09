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
    cerr << "           -o output_image \n";
    cerr << "           -c curve (available: gamma , asinhsqrt, canon , ddp) \n";
    cerr << "           -p [parameter for curve; middle value:default] \n";
    cerr << "           ( p : gamma     0.5 .. 2.2 .. 8.0            )\n";
    cerr << "           ( p : asinhsqrt 0.25 .. 0.8 .. 2.0           )\n";
    cerr << "           ( p : canon     80 .. 43 .. 30  reverse slope)\n";
    cerr << "           ( p : ddp       80 .. 50 .. 30  reverse slope)\n";
    cerr << "  PURPOSE: Gamma-scaling-curve on fits image \n";
    cerr << "  that is single or RGB-average grey-fits with values 0-255.\n";
    cerr << "  Clipping: Negative inputs will be set to NULL\n\n";

    exit(1);
  }
}

void GAMMA(vector<float> &data, double e)
{
  ulong k, dim = data.size();
  for (k=0; k<dim; k++) {
    if (data[k] >= 0) data[k] = 255.0 * pow(data[k] / 255.0, 1.0/e);
    else data[k] = 0.;
  }
}

void ASINHSQRT(vector<float> &data, double e)
{
  ulong k, dim = data.size();
  for (k=0; k<dim; k++) {
    if (data[k] >= 0) data[k] = 255.0 * (pow( asinh( sqrt(data[k] / 255.0*1.38)), 1.0/e));
    else data[k] = 0.;
  }
}

void CANON(vector<float> &data, double e)
{
  ulong k, dim = data.size();
  for (k=0; k<dim; k++) {
    if (data[k] >= 0) data[k] = 255.0 * (1.0 - exp(-data[k] / e));
    else data[k] = 0.;
  }
}

void DDP(vector<float> &data, double e)
{
  ulong k, dim = data.size();
  for (k=0; k<dim; k++) {
    if (data[k] >= 0) data[k] = 305.0 * (data[k] / (data[k]+e));
    else data[k] = 0.;
  }
}

int main(int argc, char *argv[])
{
  bool parmgiven = false;
  double parm;
  long i;
  string input_image, output_image, curve;
  
  parm = 2.2;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'c': curve = argv[++i];
	break;
      case 'p': parm = atof(argv[++i]);	
	parmgiven = true;
	break;
      }
    }
  }
  
  if (! (curve.compare("gamma") == 0 || curve.compare("asinhsqrt") == 0 || 
         curve.compare("canon") == 0 || curve.compare("ddp") == 0 )) {
    cerr << "Curve not recognised.\n";
    return 0;
  }

  // default values
  if (!parmgiven) {
    if (curve.compare("gamma") == 0)     parm = 2.2;
    if (curve.compare("asinhsqrt") == 0) parm = 0.8;
    if (curve.compare("canon") == 0)     parm = 43.;
    if (curve.compare("ddp") == 0)       parm = 50.;
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image(input_image);
  
  if (curve.compare("gamma") == 0)     GAMMA(image.data, parm);
  if (curve.compare("asinhsqrt") == 0) ASINHSQRT(image.data, parm);
  if (curve.compare("canon") == 0)     CANON(image.data, parm);
  if (curve.compare("ddp") == 0)       DDP(image.data, parm);

  // write the new image to memory
  writeImage(output_image, input_image, image.data, call);
  
  return 0;
}
