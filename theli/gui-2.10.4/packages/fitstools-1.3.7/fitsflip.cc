#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -o output_image \n";
    cerr << "           -f (x or y, for flipping) \n";
    cerr << "           -r angle (90, -90, or 180) \n";
    cerr << "  Note: If both a flip and rotation are given,\n";
    cerr << "        then the flipping is done first.\n";
    cerr << "        If neither -f nor -r is specified, then\n";
    cerr << "        nothing is done.\n";
    cerr << "        Positive angles mean counterclockwise.\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  bool flipped;
  int n, m, angle;
  long i, j, k;
  float crpix1old, crpix2old, crpix1new, crpix2new;
  float cd11old, cd12old, cd21old, cd22old;
  float cd11new, cd12new, cd21new, cd22new;
  string input_image, output_image, flip;

  cd11new = cd12new = cd21new = cd22new = 0.0;
  crpix1new = crpix2new = 0.;

  flipped = false;
  angle = 0;
  
  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'f': flip = argv[++i];
	    flipped = true;
            break;
        case 'r': angle = atoi(argv[++i]);
            break;
      }
    }
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  vector<float> data_out(n*m,0);
  vector<float> data_tmp(n*m,0);

  // check if the CRPIX keywords are present
  crpix1old = image.readKeyFailsafe("CRPIX1", float(n/2), "Reference pixel of first axis");
  crpix2old = image.readKeyFailsafe("CRPIX2", float(m/2), "Reference pixel of second axis");
  cd11old = image.readKeyFailsafe("CD1_1", -0.001, "Linear projection matrix");
  cd12old = image.readKeyFailsafe("CD1_2",  0.000, "Linear projection matrix");
  cd21old = image.readKeyFailsafe("CD2_1",  0.000, "Linear projection matrix");
  cd22old = image.readKeyFailsafe("CD2_2",  0.001, "Linear projection matrix");

  k = 0;

  //*************************
  // flip in x direction
  //*************************
  if (flip.compare("x") == 0) {
    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	data_out[k++] = image.data[n-1-i + n*j];
      }
    }
    
    // update the reference pixel
    crpix1new = n - crpix1old;
    crpix2new = crpix2old;
    // CD matrix
    cd11new = -cd11old;
    cd12new =  cd12old;
    cd21new = -cd21old;
    cd22new =  cd22old;
    // "update" the old values in case we do a rotation as well
    crpix1old = crpix1new;
    crpix2old = crpix2new;
    cd11old = cd11new;
    cd12old = cd12new;
    cd21old = cd21new;
    cd22old = cd22new;
  }

  //*************************
  // flip in y direction
  //*************************
  else if (flip.compare("y") == 0) {
    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	data_out[k++] = image.data[i + n*(m-1-j)];
      }
    }
    
    // update the reference pixel
    crpix1new = crpix1old;
    crpix2new = m - crpix2old;
    // CD matrix
    cd11new =  cd11old;
    cd12new = -cd12old;
    cd21new =  cd21old;
    cd22new = -cd22old;
    // "update" the old values in case we do a rotation as well
    crpix1old = crpix1new;
    crpix2old = crpix2new;
    cd11old = cd11new;
    cd12old = cd12new;
    cd21old = cd21new;
    cd22old = cd22new;
  }
  
  //*************************
  // rotate
  //*************************
  k = 0;
  if (angle != 0) {
    // Copy the flipped image if a flip was done; we have "data_tmp = data_in" already
    if (flipped) {
      for (i=0; i<n*m; i++) data_tmp[i] = data_out[i];
    }
    else {
      for (i=0; i<n*m; i++) data_tmp[i] = image.data[i];
    }

    if (angle == 90) {
      for (j=0; j<n; j++) {
	for (i=0; i<m; i++) {
	  data_out[k++] = data_tmp[j+n*(m-i-1)];
	}
      }
      // calculate the new reference pixel
      crpix1new = m - crpix2old;
      crpix2new = crpix1old;
      // CD matrix
      cd11new = -cd12old;
      cd12new = cd11old;
      cd21new = -cd22old;
      cd22new = cd21old;
    }
    
    if (angle == -90) {
      for (j=0; j<n; j++) {
	for (i=0; i<m; i++) {
	  data_out[k++] = data_tmp[n-j-1 + n*i];
	}
      }
      // calculate the new reference pixel
      crpix1new = crpix2old;
      crpix2new = n - crpix1old;
      // CD matrix
      cd11new = cd12old;
      cd12new = -cd11old;
      cd21new = cd22old;
      cd22new = -cd21old;
    }
    
    if (angle == 180) {
      for (j=0; j<m; j++) {
	for (i=0; i<n; i++) {
	  data_out[k++] = data_tmp[n-i-1+n*(m-j-1)];
	}
      }
      // calculate the new reference pixel
      // crpix1new = 0.5 * (n + m) - crpix1old;
      // crpix2new = 0.5 * (n + m) - crpix2old;
      crpix1new = n - crpix1old;
      crpix2new = m - crpix2old;
      // CD matrix
      cd11new = -cd11old;
      cd12new = -cd12old;
      cd21new = -cd21old;
      cd22new = -cd22old;
    }
  }

  // write the transformed image
  if (abs(angle) == 90) 
    writeImage(output_image, input_image, data_out, m, n, call);
  else
    writeImage(output_image, input_image, data_out, call);

  // update the header
  myFITS imageout(output_image, false, "Write");
  imageout.addKey("CRPIX1", crpix1new, "Reference pixel of first axis");
  imageout.addKey("CRPIX2", crpix2new, "Reference pixel of second axis");
  imageout.addKey("CD1_1", cd11new, "Linear projection matrix");
  imageout.addKey("CD1_2", cd12new, "Linear projection matrix");
  imageout.addKey("CD2_1", cd21new, "Linear projection matrix");
  imageout.addKey("CD2_2", cd22new, "Linear projection matrix");

  return 0;
}
