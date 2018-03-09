#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <CCfits>
#include <fitsio.h>
#include "myFITS.h"
#include "fitstools.h"
#include "statistics.h"

using namespace std;



//****************************************************************
// extract the path name
//****************************************************************
void dirname(const string &str, string &dir) {
  unsigned found = str.find_last_of("/");
  dir = str.substr(0,found);
}


//****************************************************************
// split a sentence into a vector of words
//****************************************************************
vector<string> getword(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}



//****************************************************************
// extract the file name
//****************************************************************
void filename(const string &str, string &file) {
  unsigned found = str.find_last_of("/");
  file = str.substr(found+1);
}


//****************************************************************
// extract the base name
//****************************************************************
void basename(const string &str, const string &suffix, string &base) {
  unsigned found = str.find_last_of("/");
  string filename = str.substr(found+1);

  size_t suffixpos = filename.find(suffix, filename.length()-suffix.length());
  filename.erase(suffixpos,suffix.length());
  base = filename;
}


//****************************************************************
// remove all whitespace
//****************************************************************
void removeallwhitespace(string &str)
{
  size_t i = 0;

  // Trim white space
  while (i<str.length()) {
    if (isspace(str[i])) str.erase(i,1);
    else i++;
  }
}


//****************************************************************
// remove leading and trailing whitespace
//****************************************************************
void trim(string &str, const string& whitespace)
{
  const size_t strBegin = str.find_first_not_of(whitespace);
  if (strBegin != string::npos) {
    const size_t strEnd = str.find_last_not_of(whitespace);
    const size_t strRange = strEnd - strBegin + 1;
    
    str = str.substr(strBegin, strRange);
  }
}


//****************************************************************
// replace multiple white spaces by single white spaces
//****************************************************************
void reducewhitespace(string &str, const string &fill,
		      const string &whitespace)
{
  // trim first
  string result = str;
  trim(result, whitespace);

  // replace sub ranges
  size_t beginSpace = result.find_first_of(whitespace);
  while (beginSpace != string::npos) {
    const size_t endSpace = result.find_first_not_of(whitespace, beginSpace);
    const size_t range = endSpace - beginSpace;
    
    result.replace(beginSpace, range, fill);
    
    const size_t newStart = beginSpace + fill.length();
    beginSpace = result.find_first_of(whitespace, newStart);
  }
  
  str = result;
}


//****************************************************************
// Remove all characters in string 'key' from string 'src'
//****************************************************************
string RemoveChars(string src, string key)
{
  string dest(src.length(), ' ');
  bool found;
  size_t i, j, k;
  
  k = 0;
  // step through the source string
  for (i=0; i<src.length(); i++) {
    found = false;
    // step through the string with bad chars and look for matches
    for (j=0; j<key.length(); j++ ) {
      if ( src[i] == key[j] ) found = true;
    }
    // if no match found, append the source char to the destination char
    if (!found) dest[k++] = src[i];
  }
  
  removeallwhitespace(dest);

  return (dest);
}

//****************************************************************
// Split IRAF-style [XMIN:XMAX,YMIN:YMAX] keyword
//****************************************************************
void split_datasection(string src, vector<long> &vertices)
{
  size_t i = 0;

  // erase [ and ] characters (first and last);
  src.erase(0,1);
  src.erase(src.length()-1,1);

  // replace : and , with white space
  while (i<src.length()) {
    if (src[i] == ':' || src[i] == ',') src[i] = ' ';
    else i++;
  }

  string sub; 
  long test;
  istringstream iss(src);
  string::size_type sz;

  while (iss >> sub) {
    test = stol(sub,&sz);
    vertices.push_back(test); // stol() from C++11
  }
}

/*
Polygon tester (pnpoly)

Copyright (c) 1970-2003, Wm. Randolph Franklin

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions: 

(1) Redistributions of source code must retain the above copyright notice, this 
list of conditions and the following disclaimers.
(2) Redistributions in binary form must reproduce the above copyright notice in 
the documentation and/or other materials provided with the distribution.
(3) The name of W. Randolph Franklin may not be used to endorse or promote 
products derived from this Software without specific prior written permission. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE. 
*/


//***************************************************************
// Test if a point is inside a polygon
//***************************************************************
int pnpoly(long nvert, vector<float> &vertx, vector<float> &verty, float testx, float testy)
{
  long i, j, c = 0;
  for (i=0, j=nvert-1; i<nvert; j=i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
      c = !c;
  }
  return c;
}


//***************************************************************
// Apply polygon to a mask image
//***************************************************************
void apply_poly(vector<int> &data, long nvert, vector<float> &vertx, vector<float> &verty, 
		long n, long m, int maskvalue)
{
  int polytest;
  long i, j;
  float x, y;

  // apply the polygon mask
  for (j=0; j<m; j++) {
    // we have to add +1 to compensate for arrays in C starting with 0
    y = (float) j + 1;
    for (i=0; i<n; i++) {
      x = (float) i + 1;
      // test if the pixel is inside or outside the polygon
      polytest = pnpoly(nvert, vertx, verty, x, y);

      // mark the pixels inside the polygon
      if (polytest == 1) data[i+n*j] += maskvalue;
    }
  }
}


//***************************************************************
// Apply "polygon" circle to a mask image
//***************************************************************
void apply_polycircle(vector<int> &data, float x, float y, float r, 
		      long n, long m, int maskvalue)
{
  long i, j;
  float ii, jj, d;

  // apply the polygon mask
  for (j=0; j<m; j++) {
    jj = (float) j;
    for (i=0; i<n; i++) {
      ii = (float) i;
      // mark the pixels if inside the circle
      d = (ii-x) * (ii-x) + (jj-y) * (jj-y);
      if (d <= r*r) data[i+n*j] += maskvalue;
    }
  }
}


//***************************************************************
// Number of vertices in a polygon
//***************************************************************
long polygon_nvert(string polystring)
{
  size_t i;
  long nvert;

  // how many vertices do we have; simply count the number of commas
  nvert = 0;
  for (i=0; i<polystring.length(); i++) {
    if (polystring[i] == ',') nvert++;
  }
  
  return (nvert + 1) / 2;
}


//****************************************************************
// split a ds9 region POLYGON string into x- and y vertice arrays
//****************************************************************
void polygon2vertices(string polystring, vector<float> &vertx, vector<float> &verty)
{
  size_t pos;
  long j, k;

  // remove all whitespace
  removeallwhitespace(polystring);

  // remove the 'polygon(' string and the closing ')'
  polystring.erase(0,8);
  polystring.erase(polystring.length()-1,1);

  // split the polygon string into its vertices
  j = 0;
  k = 0;
  while (polystring.find(",") != string::npos) {
    pos = polystring.find(",");
    if (k%2 == 0) 
      vertx[j] = atof(polystring.substr(0,pos).c_str());
    else {
      verty[j] = atof(polystring.substr(0,pos).c_str());
      j++;
    }
    k++;
    polystring.erase(0,pos+1);
  }
  // The last element has to be extracted separately, as no string is left:
  verty[j] = atof(polystring.c_str());
}


//****************************************************************
// Extract midpoint and radius from a ds9 circle region
//****************************************************************
void polygon2circle(string polystring, float *x_ptr, float *y_ptr, float *r_ptr)
{
  size_t pos;

  // remove whitespace
  removeallwhitespace(polystring);

  // remove the 'circle(' string and the closing ')'
  polystring.erase(0,7);
  polystring.erase(polystring.length()-1,1);

  // extract the midpoint and the radius
  pos = polystring.find(",");
  *x_ptr = atof(polystring.substr(0,pos).c_str());
  polystring.erase(0,pos+1);

  pos = polystring.find(",");
  *y_ptr = atof(polystring.substr(0,pos).c_str());
  polystring.erase(0,pos+1);

  *r_ptr = atof(polystring.substr(0,pos).c_str());
}


//****************************************************************
// mask a ds9 polygon region
//****************************************************************
void mask_ds9_polygon(vector<int> &mask, string polystring, long n, long m)
{
  // how many vertices do we have
  long nvert = polygon_nvert(polystring);

  vector<float> vertx(nvert,0);
  vector<float> verty(nvert,0);

  // Extract vertices
  polygon2vertices(polystring, vertx, verty);

  // Mask polygon'd pixels in the mask image
  apply_poly(mask, nvert, vertx, verty, n, m, 1);
}


//****************************************************************
// mask a ds9 circle region
//****************************************************************
void mask_ds9_circle(vector<int> &mask, string polystring, long n, long m)
{
  float x, y, r;
  
  x = y = r = 0.;

  // extract the circle midpoint and radius
  polygon2circle(polystring, &x, &y, &r);

  // apply the mask
  apply_polycircle(mask, x, y, r, n, m, 1);
}


//****************************************************************
// Translate a ds9 polygon mask into a mask image;
// The mask image is set to 1 whereever a pixel is found inside 
// ds9-type polygon or circle; unless there is a header line:
//      # Sense: out
// In this case the mask will be inverted
//****************************************************************
void make_ds9_maskimage(vector<int> &mask, char *ds9reg, long n, long m, 
			string area_arg, string combine_arg)
{
  ifstream file;
  long i;
  string area, combine, line;

  // if the mask orientation is determined from the ds9 region file:
  // just for initialisation purposes
  if (area_arg.empty()) area = "in";
  else area = area_arg;

  // if the combine type is determined from the ds9 region file:
  // just for initialisation purposes
  if (combine_arg.empty()) combine = "or";
  else combine = combine_arg;

  // read the polygon
  file.open(ds9reg);
  if (!file.is_open()) {
    cerr << "ERROR: Could not open ds9 region " << ds9reg << endl; 
    exit (1);
  }
  else {
    while (file.good()) {
      getline(file,line);
      // remove leading and trailing whitespace
      trim(line," ");
      // 'Sense': do we keep pixels inside or outside the polygon
      if (line.compare(0, 9, "# Sense: ") == 0 && area_arg.empty()) {
	area = line;
	area.erase(0,9);
      }

      // 'Combine': do we keep pixels in all regions or only in their overlap
      if (line.compare(0, 11, "# Combine: ") == 0 && combine_arg.empty()) {
        combine = line;
	combine.erase(0,11);
      }

      // mask a ds9 polygon
      if (line.compare(0, 8, "polygon(") == 0 || 
	  line.compare(0, 8, "POLYGON(") == 0) {
	mask_ds9_polygon(mask, line, n, m);
      }

      // mask a ds9 circle
      if (line.compare(0, 7, "circle(") == 0 || 
	  line.compare(0, 7, "CIRCLE(") == 0) {
	mask_ds9_circle(mask, line, n, m);
      }
    }
    file.close();
  }

  // if requested, keep only pixels common to all ds9 region elements
  if (combine.compare("and") == 0) {
    float maskmax = max(mask, n*m);
    for (i=0; i<n*m; i++) {
      if (mask[i] < maskmax) mask[i] = 0;
      else mask[i] = 1;
    }
  }

  // invert the mask if area = "out"
  if (area.compare("out") == 0) {
    for (i=0; i<n*m; i++) {
      if (mask[i] > 0) mask[i] = 0;
      else mask[i] = 1;
    }
  }
}


//****************************************************************
// Tests whether a x|y coordinate is inside or outside a ds9 polygon
//****************************************************************
int make_ds9_maskcat(float x, float y, char *ds9reg, string area_arg, string combine_arg)
{
  ifstream file;
  string area, combine, line;
  int returnval = 0;

  // if the mask orientation is determined from the ds9 region file:
  // just for initialisation purposes
  if (area_arg.empty()) area = "in";
  else area = area_arg;

  // if the combine type is determined from the ds9 region file:
  // just for initialisation purposes
  if (combine_arg.empty()) combine = "or";
  else combine = combine_arg;

  // read the polygon
  file.open(ds9reg);
  if (!file.is_open()) {
    cerr << "ERROR: Could not open ds9 region " << ds9reg << endl; 
    exit (1);
  }
  else {
    while (file.good()) {
      getline(file,line);
      // remove leading and trailing whitespace
      trim(line," ");
      // 'Sense': do we keep pixels inside or outside the polygon
      if (line.compare(0, 9, "# Sense: ") == 0 && area_arg.empty()) {
	area = line;
	area.erase(0,9);
      }

      // 'Combine': do we keep pixels in all regions or only in their overlap
      if (line.compare(0, 11, "# Combine: ") == 0 && combine_arg.empty()) {
        combine = line;
	combine.erase(0,11);
      }

      // Test if point is within polygon
      if (line.compare(0, 8, "polygon(") == 0 || 
	  line.compare(0, 8, "POLYGON(") == 0) {
	long nvert = polygon_nvert(line);
	vector<float> vertx(nvert,0);
	vector<float> verty(nvert,0);
	// Extract vertices
	polygon2vertices(line, vertx, verty);
	int polytest = pnpoly(nvert, vertx, verty, x, y);
	if (area.compare("in") == 0) {
	  if (polytest == 1) returnval = 1;
	}
	else {
	  if (polytest == 0) returnval = 1;
	}
      }
    }
    file.close();
  }

  return returnval;
}


//****************************************************************
// Determine the position angle from the CD matrix
//****************************************************************
double get_posangle(double cd11, double cd12, double cd21, double cd22)
{
  double pscale1, pscale2;
  double pa, nfac1, nfac2;

  // the pixel scale
  pscale1 = sqrt(cd11 * cd11 + cd21 * cd21);
  pscale2 = sqrt(cd12 * cd12 + cd22 * cd22);

  // take out the pixel scale
  cd11 /= pscale1;
  cd12 /= pscale2;
  cd21 /= pscale1;
  cd22 /= pscale2;

  // sqrt(CD elements) is very close to one, but not perfectly
  // as coordinate system is not necessarily orthogonal (shear + contraction)
  nfac1 = sqrt(cd11 * cd11 + cd21 * cd21);
  nfac2 = sqrt(cd12 * cd12 + cd22 * cd22);

  // make sure sqrt(CD elements) = 1, 
  // so that we can do the inverse trig functions
  cd11 /= nfac1;
  cd21 /= nfac1;
  cd12 /= nfac2;
  cd22 /= nfac2;

  // due to flipping the rotation angle is ambiguous.

  // possibly, the following could be simplified with det(CD), 
  // but at the moment i don't see how to identify the additional 2*PI easily

  // normal
  if      (cd11 <  0 && cd12 <= 0 && cd21 <= 0 && cd22 >  0) pa = acos(-cd11);       //   0 <= phi <  90
  else if (cd11 >= 0 && cd12 <  0 && cd21 <  0 && cd22 <= 0) pa = acos(-cd11);       //  90 <= phi < 180
  else if (cd11 >  0 && cd12 >= 0 && cd21 >= 0 && cd22 <  0) pa = 2.*PI-acos(-cd11); // 180 <= phi < 270
  else if (cd11 <= 0 && cd12 >  0 && cd21 >  0 && cd22 >= 0) pa = 2.*PI-acos(-cd11); // 270 <= phi < 360

  // flipped
  else if (cd11 >= 0 && cd12 >= 0 && cd21 <= 0 && cd22 >= 0) pa = acos(cd11);       //   0 <= phi <  90
  else if (cd11 <  0 && cd12 >  0 && cd21 <  0 && cd22 <  0) pa = acos(cd11);       //  90 <= phi < 180
  else if (cd11 <  0 && cd12 <= 0 && cd21 >= 0 && cd22 <  0) pa = 2.*PI-acos(cd11); // 180 <= phi < 270
  else if (cd11 >= 0 && cd12 <  0 && cd21 >  0 && cd22 >= 0) pa = 2.*PI-acos(cd11); // 270 <= phi < 360

  else {
    // we are very likely close to 0, 90, 180 or 270 degrees, and the CD matrix is slightly non-orthogonal.
    // In this case, lock onto 0, 90, 180 or 270 degrees. Otherwise, exit with an error.
    double cd11cd12 = fabs(cd11/cd12);
    double cd22cd21 = fabs(cd22/cd21);

    if (cd11cd12 > 20. && cd22cd21 > 20.) {
      if (cd11 > 0. && cd22 > 0.) pa = 0.*PI/2.;
      if (cd11 < 0. && cd22 > 0.) pa = 0.*PI/2.;
      if (cd11 > 0. && cd22 < 0.) pa = 2.*PI/2.;
      if (cd11 < 0. && cd22 < 0.) pa = 2.*PI/2.;
    }    

    else if (cd11cd12 < 0.05 && cd22cd21 < 0.05) {
      if (cd12 > 0. && cd21 < 0.) pa = 1.*PI/2.;
      if (cd12 < 0. && cd21 < 0.) pa = 1.*PI/2.;
      if (cd12 > 0. && cd21 > 0.) pa = 3.*PI/2.;
      if (cd12 < 0. && cd21 > 0.) pa = 3.*PI/2.;
    }

    else {
      cerr << "ERROR: Could not determine position angle from CD matrix!\n";
      exit (1);
    }
  }

  return (pa);
}


//****************************************************************
// Check whether file is a FITS file
//****************************************************************
void checkfile(string filename)
{
  try {
    auto_ptr<FITS> pInfile(new FITS(filename, Read, true));
  }
  catch (FitsException&) {
    cerr << "ERROR: File " << filename << " does not exist, cannot be read, or is no FITS file!\n";
    exit (1);
  }
}


//****************************************************************
// Write a FITS image, preserve BITPIX 
//****************************************************************
void writeImage(string outfilename, string headfilename, valarray<float> &dataout, 
		long n, long m, string history)
{
  long naxis = 2;
  long fpixel = 1;
  long naxes[2] = {n, m};

  // overwrite existing file if the file already exists.
  outfilename = "!" + outfilename;
  
  try {
    // get reference data from header
    auto_ptr<FITS> imagehead_ptr( new FITS(headfilename, Read));
    PHDU &imagehead = imagehead_ptr->pHDU();
    int bpix = imagehead.bitpix();
    imagehead.readAllKeys();

    // create the output FITS file
    auto_ptr<FITS> imageout_ptr( new FITS(outfilename, bpix, naxis, naxes));
    PHDU &imageout = imageout_ptr->pHDU();
    imageout.copyAllKeys(&imagehead);
    if (history.compare("nohistory") != 0) 
      imageout.writeHistory(history);
    imageout.write(fpixel,n*m,dataout);
  }
  catch (FitsException&) {
    cerr << "ERROR in writeImage(): Could not read (or write) from (to) " 
	 << headfilename << " (" << outfilename << ")!" << endl;
  }
}


//****************************************************************
// Overloaded: Write a FITS image, assume geometry; preserve BITPIX 
//****************************************************************
void writeImage(string outfilename, string headfilename, valarray<float> &dataout, 
		string history)
{
  long n, m;

  // get image geometry from the header image
  try {
    auto_ptr<FITS> imagehead_ptr( new FITS(headfilename, Read));
    PHDU &imagehead = imagehead_ptr->pHDU();
    n = imagehead.axis(0);
    m = imagehead.axis(1);
    imagehead_ptr->destroy(); // not sure we actually need this
  }
  catch (FitsException&) {
    cerr << "ERROR in writeImage(): Could not read from " << headfilename << endl;
    exit(1);
  }
  
  writeImage(outfilename, headfilename, dataout, n, m, history);
}


//****************************************************************
// Overloaded: Write a FITS image with minimum header
//****************************************************************
void writeImage(string outfilename, valarray<float> &dataout, long n, long m, int bpix)
{
  long naxis = 2;
  long fpixel = 1;
  long naxes[2] = {n, m};

  // overwrite existing file if the file already exists.
  outfilename = "!" + outfilename;

  // create the output FITS file
  try {
    auto_ptr<FITS> imageout_ptr( new FITS(outfilename, bpix, naxis, naxes));
    PHDU &imageout = imageout_ptr->pHDU();
    imageout.write(fpixel,n*m,dataout);
  }
  catch (FitsException&) {
    cerr << "ERROR in writeImage(): Could not write " << outfilename << ")!" << endl;
  }
}


//****************************************************************
// Overloaded: Write a FITS image with minimum header
//****************************************************************
void writeImage(string outfilename, vector<float> &dataout, long n, long m, int bpix)
{
  long i, dim = dataout.size();
  valarray<float> data_array(dim);
  for (i=0; i<dim; i++) 
    data_array[i] = dataout[i];

  writeImage(outfilename, data_array, n, m, bpix);
}


//****************************************************************
// Overloaded: Write a FITS image
//****************************************************************
void writeImage(string outfilename, string headfilename, vector<float>&dataout, 
		long n, long m, string history)
{
  long i, dim = dataout.size();
  valarray<float> data_array(dim);
  for (i=0; i<dim; i++) 
    data_array[i] = dataout[i];

  writeImage(outfilename, headfilename, data_array, n, m, history);
}


//****************************************************************
// Overloaded: Write a FITS image, assume geometry; preserve BITPIX 
//****************************************************************
void writeImage(string outfilename, string headfilename, vector<float> &dataout, 
		string history)
{
  long i, dim = dataout.size();
  valarray<float> data_array(dim);
  for (i=0; i<dim; i++) 
    data_array[i] = dataout[i];

  writeImage(outfilename, headfilename, data_array, history);
}


//****************************************************************
// Create a (MEF) FITS image; preserve BITPIX 
//****************************************************************
void createMEF(string outfilename, string headfilename)
{
  try {
    // the reference header file
    FITS imagehead(headfilename, Read);

    // overwrite existing file if the file already exists.
    outfilename = "!" + outfilename;
  
    // create the output FITS file
    //    auto_ptr<FITS> imageout_ptr( new FITS(outfilename, imagehead));
    auto_ptr<FITS> imageout_ptr( new FITS(outfilename, Write));
  }
  catch (FitsException&) {
    cerr << "ERROR: Could not create " << outfilename << endl;
  }
}


//****************************************************************
// Return the calling syntax as a string
//****************************************************************
string programcall(char** argv, int argc)
{
  string call = "";
  long i;

  if (argc > 1) {
    for (i=0; i<argc; i++) {
      call.append(argv[i]);
      call.append(" ");
    }
  }

  return call;
}


//****************************************************************
// Add an image extension
//****************************************************************
void addImageExtension(auto_ptr<FITS> &imageout_ptr, vector<float>&dataext, int extnum, int n, int m) 
{
  long j;

  std::vector<long> extAx(2,0);
  extAx[0] = n;
  extAx[1] = m;
  string newName ("CHIP"+NumberToString(extnum));

  ExtHDU *imageExt = imageout_ptr->addImage(newName, FLOAT_IMG, extAx);

  // create some data for the image extension.
  long extElements = dataext.size();
  valarray<float> outdata(extElements);
  for (j=0; j<extElements; j++) {
    outdata[j] = dataext[j];
  }

  long fpixel(1);
  
  // write the image extension data: also demonstrates switching between HDUs.
  imageExt->write(fpixel,extElements,outdata);
}


//****************************************************************
// Test whether a string is a float
//****************************************************************
bool isFloat(string s) {
  istringstream iss(s);
  float dummy;
  iss >> noskipws >> dummy;
  return iss && iss.eof();     // Result converted to bool
}


//***************************************************************
// Get the overscan of the image
// Y4KCam has 4 readout quadrants, all stored in the same image
//***************************************************************
float get_overscan_4quadchannels(myFITS &image, int xmin, int xmax, int ymin, int ymax)
{
  long i, j, k, n, dim;
  float overscan;

  dim = (xmax - xmin + 1) * (ymax - ymin + 1);

  vector<float> oscan(dim,0);

  n = image.naxis1;

  k = 0;
  for (j=ymin; j<=ymax; j++) {
    for (i=xmin; i<=xmax; i++) {
      oscan[k++] = image.data[i+n*j];
    }
  }

  overscan = median(oscan, dim);
  
  return (overscan);
}  
