#include <fstream>
#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

void rotate_cdmatrix(double&, double&, double&, double&, double);

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "          -p new position angle for the output CD matrix (counting North over East)\n";
    cerr << "         [-i global scamp-style .ahead file (CRPIX, CD-matrix per chip, separated by END keyword)]\n";
    cerr << "         [-o output name for the new .ahead file]\n";
    cerr << "         [-c cd11 cd12 cd21 cd22]\n";
    cerr << "         [-l (print cd11 cd12 cd21 cd22 in a single line; only with option -c)]\n";
    cerr << "         [-f FITS file with CD matrix in the header]\n\n";
    cerr << "  PURPOSE: Rotates a CD matrix to a new position angle.\n";
    cerr << "      Either an input file, or a list of CD matrix elements, or an image must be given.\n\n";
  }

  exit (0);
}

//**************************************************************

int main(int argc, char *argv[])
{
  bool fileflag, cdflag, fitsflag, lineflag;
  long i, numlines, numheads, globflag = 0;
  double phi_new;
  double crpix1, crpix2, cd11, cd12, cd21, cd22;
  char file[FILEMAX], fileout[FILEMAX];
  string input_image;

  fileflag = fitsflag = cdflag = lineflag = false;
  phi_new = crpix1 = crpix2 = 0.;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  //  Read command line arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'i': strcpy(file,argv[++i]);
	fileflag = true;
	globflag++;
	break;
      case 'o': strcpy(fileout,argv[++i]);
	break;
      case 'l': lineflag = true;
	break;
      case 'c': 
	cd11 = atof(argv[++i]);
	cd12 = atof(argv[++i]);
	cd21 = atof(argv[++i]);
	cd22 = atof(argv[++i]);
	cdflag = true;
	globflag++;
	break;
      case 'p': phi_new = atof(argv[++i])*RAD;
	break;
      case 'f': input_image = argv[++i];
	fitsflag = true;
	globflag++;
	break;
      }
    }
  }
  
  if (globflag > 1) {
    cerr <<  "ERROR: Either a header file, or a CD matrix, or a FITS image must be given.\n";
    exit (1);
  }

  if (!fileflag && !cdflag && !fitsflag) {
    cerr <<  "ERROR: No input specified.\n";
    exit (1);
  }
  
  // *************************************************************
  // a CD matrix was given
  // *************************************************************
  
  if (cdflag && !lineflag) {
    rotate_cdmatrix(cd11, cd12, cd21, cd22, phi_new);
    printf("CD1_1   = %3.10g\n", cd11);
    printf("CD1_2   = %3.10g\n", cd12);
    printf("CD2_1   = %3.10g\n", cd21);
    printf("CD2_2   = %3.10g\n", cd22);
    return 0;
  }

  if (cdflag && lineflag) {
    rotate_cdmatrix(cd11, cd12, cd21, cd22, phi_new);
    printf("%3.10g %3.10g %3.10g %3.10g\n", cd11, cd12, cd21, cd22);
    return 0;
  }

  // *************************************************************
  // a FITS file was given
  // *************************************************************

  if (fitsflag) {
    checkfile(input_image);
    myFITS image(input_image, false);
    image.readmyKey("CD1_1", cd11);
    image.readmyKey("CD1_2", cd12);
    image.readmyKey("CD2_1", cd21);
    image.readmyKey("CD2_2", cd22);
    rotate_cdmatrix(cd11, cd12, cd21, cd22, phi_new);
    printf("CD1_1   = %3.10g\n", cd11);
    printf("CD1_2   = %3.10g\n", cd12);
    printf("CD2_1   = %3.10g\n", cd21);
    printf("CD2_2   = %3.10g\n", cd22);
    return 0;
  }

  // *************************************************************
  // a scamp-style global_header.ahead file was specified as input
  // *************************************************************

  ifstream infile(file);
  if (!infile.is_open()) {
    cerr << "ERROR: Could not open file " << file << "!\n";
    exit (1);
  }
  
  // how many images are in the file
  numlines = 0;
  numheads = 0;
 
  i = 1;
  string lines;
  while (infile) {
    getline(infile, lines);
    reducewhitespace(lines);
    if (lines.compare("END") == 0) 
      numheads++;
    numlines++;
  }

  // we have to subtract 1 from the number of lines
  numlines--;
  
  infile.clear(); // clear the error states (EOF) in the ifstream before we can reset it;
  infile.seekg(0);
  
  vector<string> line(numlines); // Vector holding header lines

  i = 0;
  while (infile && i <numlines) {
    getline(infile, lines);
    // replace the "=" sign with a blank, as it might become part of the keyword if the latter is 8 chars long
    replace( lines.begin(), lines.end(), '=', ' ' );
    reducewhitespace(lines);
    line[i++] = lines;
  }

  infile.close();

  // open the output file
  ofstream outfile(fileout);
  if (!outfile.is_open()) {
    cerr << "ERROR: Could not open file " << fileout << "!\n";
    exit (1);
  }

  string substr, keyval;
  string keyword, keyvalue;
  istringstream iss;

  for (i=0; i<numlines; i++) {
    if (line[i].compare("END") != 0) {
      iss.str(line[i]);
      iss >> keyword;
      iss >> keyvalue;
      if (keyword.compare("CRPIX1") == 0) crpix1 = atof(keyvalue.c_str());
      else if (keyword.compare("CRPIX2") == 0) crpix2 = atof(keyvalue.c_str());
      else if (keyword.compare("CD1_1") == 0) cd11 = atof(keyvalue.c_str());
      else if (keyword.compare("CD1_2") == 0) cd12 = atof(keyvalue.c_str());
      else if (keyword.compare("CD2_1") == 0) cd21 = atof(keyvalue.c_str());
      else if (keyword.compare("CD2_2") == 0) cd22 = atof(keyvalue.c_str());
      else outfile << line[i] << endl;
    }
    else {
      // one chip has been read in; now transform it:
      rotate_cdmatrix(cd11, cd12, cd21, cd22, phi_new);
      // write the result
      outfile << "CRPIX1  = " << crpix1 << endl;
      outfile << "CRPIX2  = " << crpix2 << endl;
      outfile << "CD1_1   = " << cd11 << endl;
      outfile << "CD1_2   = " << cd12 << endl;
      outfile << "CD2_1   = " << cd21 << endl;
      outfile << "CD2_2   = " << cd22 << endl;
      outfile << "END\n";
    }
  }

  outfile.close();

  return 0;
}


//************************************************
void rotate_cdmatrix(double &cd11, double &cd12, double &cd21, double &cd22, double pn)
{
  double po, det;
  double f11, f12, f21, f22;

  // the current position angle of the CD matrix
  po = get_posangle(cd11, cd12, cd21, cd22);

  // is the matrix flipped (det(CD) > 0 if flipped)
  det = cd11*cd22 - cd12*cd21;
  if (det < 0) f11 = 1.;
  else f11 = -1.;

  f12 = 0.;
  f21 = 0.;
  f22 = 1.;

  // unflip the matrix
  matrix_mult(f11, f12, f21, f22, cd11, cd12, cd21, cd22);
  
  // rotate the matrix to the new position angle
  matrix_mult(cos(pn-po), -sin(pn-po), sin(pn-po), cos(pn-po),
	      cd11, cd12, cd21, cd22);

  // flip the matrix
  matrix_mult(f11, f12, f21, f22, cd11, cd12, cd21, cd22);
}
