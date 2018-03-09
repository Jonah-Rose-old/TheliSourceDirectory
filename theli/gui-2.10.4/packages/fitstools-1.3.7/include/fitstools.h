#ifndef RAD
#define RAD 3.1415926535/180.
#endif

#ifndef PI
#define PI 3.1415926535
#endif

#ifndef __FITSTOOLS_H
#define __FITSTOOLS_H

#include <sstream>
#include "myFITS.h"

using namespace std;
using namespace CCfits;


//****************************************************************
// extract the path name
//****************************************************************
void dirname(const string& str, string &dir);

//****************************************************************
// split a sentence into a vector of words
//****************************************************************
vector<string> getword(string str, char delimiter);

//****************************************************************
// extract the file name
//****************************************************************
void filename(const string& str, string &file);


//****************************************************************
// extract the base name
//****************************************************************
void basename(const string &str, const string &suffix, string &basename);


//************************************************
// remove all whitespace
//************************************************
void removeallwhitespace(string &str);


//************************************************
// remove leading and trailing whitespace
//************************************************
void trim(string& str, const string& whitespace = " \t");


//****************************************************************
// replace multiple white spaces by single white spaces
//****************************************************************
void reducewhitespace(string &str, const string &fill = " ",
			const string &whitespace = " \t");


//****************************************************************
// Remove all characters in string 'key' from string 'src'
//****************************************************************
string RemoveChars(string src, string key);


//****************************************************************
// Split IRAF-style [XMIN:XMAX,YMIN:YMAX] keyword
//****************************************************************
void split_datasection(string src, vector<long> &vertices);

//***************************************************************
// Polygon functions
//***************************************************************
int pnpoly(long nvert, vector<float> &vertx, vector<float> &verty, float testx, float testy);
void apply_poly(vector<int> &data, long nvert, vector<float> &vertx, vector<float> &verty,  
		long n, long m, int maskvalue);
void apply_polycircle(vector<int> &data, float x, float y, float r, 
		      long n, long m, int maskvalue);
long polygon_nvert(string polystring);
void polygon2vertices(string polystring, vector<float> &vertx, vector<float> &verty);
void polygon2circle(string polystring, float *x_ptr, float *y_ptr, float *r_ptr);
void mask_ds9_polygon(vector<int> &mask, string polystring, long n, long m);
void mask_ds9_circle(vector<int> &mask, string polystring, long n, long m);
void make_ds9_maskimage(vector<int> &mask, char *ds9reg, long n, long m, 
			string area_arg = "", string combine_arg = "");
int make_ds9_maskcat(float x, float y, char *ds9reg, string area_arg = "", 
		     string combine_arg = "");


//****************************************************************
// Determine the position angle from the CD matrix
//****************************************************************
double get_posangle(double cd11, double cd12, double cd21, double cd22);


//****************************************************************
// Get exposure time from FITS image
//****************************************************************
double get_exptime(string filename);


//****************************************************************
// Check for the presence of a FITS table
//****************************************************************
void has_table(char *filename, int extension);


//****************************************************************
// Check whether file is a FITS file
//****************************************************************
void checkfile(string filename);


//****************************************************************
// Convert a number to a string
//****************************************************************
template <class T>
string NumberToString ( T Number )
{
  ostringstream ss;
  ss << Number;
  return ss.str();
}


//****************************************************************
// Write a FITS image (incl. overloaded functions)
//****************************************************************
void writeImage(string outfile, string headfile, valarray<float> &data, long n, long m, 
		string history = "nohistory");
void writeImage(string outfile, string headfile, valarray<float> &data, 
		string history = "nohistory");
void writeImage(string outfile, string headfile, vector<float> &data, long n, long m, 
		string history = "nohistory");
void writeImage(string outfile, string headfile, vector<float> &data, 
		string history = "nohistory");
void writeImage(string outfile, valarray<float> &data, long n, long m, int bpix);
void writeImage(string outfile, vector<float> &data,   long n, long m, int bpix);



//****************************************************************
// Create an empty MEF file
//****************************************************************
void createMEF(string outfilename, string headfilename);


//****************************************************************
// Get a FITS header keyword value. If it doesn't exist, insert 
// it into the header and initialise it to some value. 
// Return the initialised value.
// OBSOLETE?
//****************************************************************
template <class T>
T get_key(PHDU *image, string key, T initvalue, string comment)
{
  T value;
  try {
    image->readKey(key, value);
    return value;
  }
  catch (FitsException&) {
    // The key does not exist
    image->addKey(key, initvalue, comment);
    return initvalue;
  }
}


//****************************************************************
// Return the command line into a string
//****************************************************************
string programcall(char** argv, int argc);

//****************************************************************
// Test whether a string is a float
//****************************************************************
bool isFloat(string someString);


//***************************************************************
// Get the overscan of a single chip (one FITS image) with 4 
// readout quadrants (e.g. Y4KCam@CTIO)
//***************************************************************
float get_overscan_4quadchannels(myFITS &image, int xmin, int xmax, 
				 int ymin, int ymax);


//****************************************************************
// Add an image extension
//****************************************************************
void addImageExtension(auto_ptr<FITS> &imageout_ptr, vector<float>&data, 
		       int extnum, int n, int m);


//************************************************************
// Calculate a Gaussian
//************************************************************
template <class T>
T gauss(T rsq, T ssq)
{
  return 1./sqrt(2.*PI*ssq) * exp(-rsq/(2.*ssq));
}

#endif
