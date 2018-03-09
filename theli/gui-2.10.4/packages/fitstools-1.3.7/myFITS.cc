#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <CCfits>
#include <fitsio.h>
#include "myFITS.h"
#include "fitstools.h"

using namespace std;


//****************************************************************
// Get exposure time from FITS image
//****************************************************************
float myFITS::exptime()
{
  float exptime;
  PHDU &image = image_ptr->pHDU();
  try {
    image.readKey("EXPTIME", exptime);
    return exptime;
  }
  catch (FitsException&) {
    try {
      image.readKey("EXPOSURE", exptime);
      return exptime;
    }
    catch (FitsException&) {
      cerr << "ERROR: Could not find EXPTIME nor EXPOSURE keywords in " << name << "!\n";
      exit (1);
    }
  }
}


double myFITS::mjdobs()
{
  double mjdobs;
  PHDU &image = image_ptr->pHDU();
  try {
    image.readKey("MJD-OBS", mjdobs);
    return mjdobs;
  }
  catch (FitsException&) {
    try {
      image.readKey("RJD-OBS", mjdobs);
      return mjdobs - 0.5;
    }
    catch (FitsException&) {
      cerr << "ERROR: Could not find MJD-OBS nor RJD-OBS keyword in " << name << "!\n";
      exit (1);
    }
  }
}


//****************************************************************
// Read the data section from a myFITS object
//****************************************************************
void myFITS::readData() {
  long i;
  
  PHDU &image = image_ptr->pHDU();
  data.resize(n*m,0);

  // I don't like valarrays ...
  image.read(data_va);
  for (i=0; i<n*m; i++) {
    data[i] = data_va[i];
  }

  // empty the valarray (mostly, can't have zero length)...
  data_va.resize(1,0);
}


//****************************************************************
// Read the data section from a myFITS object
//****************************************************************
void myFITS::readData(valarray<float>&va) {
  PHDU &image = image_ptr->pHDU();
  image.read(va);
}


//****************************************************************
// Read a rectangular subset of the data section from a myFITS object
//****************************************************************
void myFITS::readDataSection(long xmin, long xmax, long ymin, long ymax) {

  long i, nsub, msub;
  
  vector<long> vertex1(2);
  vector<long> vertex2(2);
  vector<long> stride(2);
  vertex1[0] = xmin;
  vertex1[1] = ymin;
  vertex2[0] = xmax;
  vertex2[1] = ymax;
  stride[0]  = 1;
  stride[1]  = 1;

  nsub = xmax - xmin + 1;
  msub = ymax - ymin + 1;

  data.resize(nsub*msub,0);

  PHDU &image = image_ptr->pHDU();

  image.read(data_va, vertex1, vertex2, stride);
  for (i=0; i<nsub*msub; i++) {
    data[i] = data_va[i];
  }
}


//****************************************************************
// Read a rectangular subset of the data section from a myFITS object.
// More memory efficient
//****************************************************************
void myFITS::readDataSectionEff(long xmin, long xmax, long ymin, long ymax, 
				valarray<float> &data_out) {

  long nsub, msub;
  
  vector<long> vertex1(2);
  vector<long> vertex2(2);
  vector<long> stride(2);
  vertex1[0] = xmin;
  vertex1[1] = ymin;
  vertex2[0] = xmax;
  vertex2[1] = ymax;
  stride[0]  = 1;
  stride[1]  = 1;

  nsub = xmax - xmin + 1;
  msub = ymax - ymin + 1;

  PHDU &image = image_ptr->pHDU();

  data_out.resize(nsub*msub);

  image.read(data_out, vertex1, vertex2, stride);
}


//****************************************************************
// Read the data section from a FITS extension
//****************************************************************
void myFITS::readExtData(vector<float> &dataext, int extnum) {

  long i;
  valarray<float>tmparray;

  ExtHDU &image = image_ptr->extension(extnum);
  naxis1 = image.axis(0);
  naxis2 = image.axis(1);
  
  dataext.resize(naxis1*naxis2,0);
  image.read(tmparray);
  for (i=0; i<naxis1*naxis2; i++) {
    dataext[i] = tmparray[i];
  }
}


//****************************************************************
// Read the data section from a FITS extension
//****************************************************************
void myFITS::readExtData(int extnum) {

  long i;
  valarray<float>tmparray;

  ExtHDU &image = image_ptr->extension(extnum);
  naxis1 = image.axis(0);
  naxis2 = image.axis(1);
  
  data.resize(naxis1*naxis2,0);
  image.read(tmparray);
  for (i=0; i<naxis1*naxis2; i++) {
    data[i] = tmparray[i];
  }
}


//****************************************************************
// Add data to a FITS extension
//****************************************************************
//void myFITS::addExtData(auto_ptr<FITS> imageout_ptr, vector<float>&data, int extnum, int n, int m) {
// ZZZ does not even operate on a myFITS!!! REWRITE
/*
void myFITS::addExtData(FITS *imageout_ptr, vector<float>&data, int extnum, int n, int m) {

  long j;
  //  ExtHDU &image = imageout_ptr->extension(extnum);

  std::vector<long> extAx(2,0);
  extAx[0] = n;
  extAx[1] = m;
  string newName ("CHIP"+NumberToString(extnum));

  ExtHDU *imageExt = imageout_ptr->addImage(newName, FLOAT_IMG, extAx);

  // create some data for the image extension.
  long extElements = data.size();
  valarray<float> outdata(extElements);
  for (j=0; j<extElements; j++) {
    outdata[j] = data[j];
  }

  long fpixel(1);
  
  // write the image extension data: also demonstrates switching between HDUs.
  imageExt->write(fpixel,extElements,outdata);
}
*/

//void myFITS::addExtData(auto_ptr<FITS> imageout_ptr, vector<float>&data, int extnum, int n, int m) {
// ZZZ does not even operate on a myFITS!!! REWRITE


//****************************************************************
// Count the number of extensions in a FITS file
//****************************************************************
int myFITS::numExt()
{
  int i=1;
  bool breakcondition = false;
  while (!breakcondition) {
    try {
      image_ptr->read(i++);
    }
    catch (FitsException&) {
      breakcondition = true;
    }
  }
  return i-2; // i was incremented even if no more extensions were found 
}


//****************************************************************
// Count the number of extensions in a FITS file
//****************************************************************
int myFITS::eBitpix(int extnum)
{
  try {
    ExtHDU &image_ext = image_ptr->extension(extnum);
    bitpix = image_ext.bitpix();
  }
  catch (FitsException&) {
    cerr << "ERROR: Could not read extension " << extnum << " in " << name << endl;
  }
  return bitpix;
}


//****************************************************************
// Image geometry (NAXIS) of extension
//****************************************************************
long myFITS::eNaxis1(int extnum)
{
  try {
    ExtHDU &image_ext = image_ptr->extension(extnum);
    naxis1 = image_ext.axis(0);
  }
  catch (FitsException&) {
    cerr << "ERROR: Could not read extension " << extnum << " in " << name << endl;
  }
  return naxis1;
}


//****************************************************************
// Image geometry (NAXIS) of extension
//****************************************************************
long myFITS::eNaxis2(int extnum)
{
  try {
    ExtHDU &image_ext = image_ptr->extension(extnum);
    naxis2 = image_ext.axis(1);
  }
  catch (FitsException&) {
    cerr << "ERROR: Could not read extension " << extnum << " in " << name << endl;
  }
  return naxis2;
}


//****************************************************************
// Count the number of extensions in a FITS file
//****************************************************************
/*
void myFITS::readExtKey(string keyname, int extension)
{
  float keyvalue;
  try {
    ExtHDU &image_ext = image_ptr->extension(extension);
    image_ext.readKey(keyname, keyvalue);
  }
  catch (FitsException&) {
    cerr << "ERROR: Could not read extension " << extension << " in " << name << endl;
  }

  cout << keyvalue << endl;
}
*/


//****************************************************************
// Probes a FITS extension for a float keyword
// For some reson I cannot "template" this in myFITS
//****************************************************************
float myFITS::readExtKey(string keyname, int extnum, float fallback)
{
  float keyvalue;
  try {
    ExtHDU &image_ext = image_ptr->extension(extnum);
    image_ext.readKey(keyname, keyvalue);
  }
  catch (FitsException&) {
    if (fallback == -1.E9) {
      cerr << "ERROR: Could not read extension " << extnum << " in " << name << endl;
    }
    else 
      keyvalue = fallback;
  }
  return keyvalue;
}


//****************************************************************
// Probes a FITS extension for a double keyword
// For some reson I cannot "template" this in myFITS
//****************************************************************
double myFITS::readDoubleExtKey(string keyname, int extnum, double fallback)
{
  double keyvalue;
  try {
    ExtHDU &image_ext = image_ptr->extension(extnum);
    image_ext.readKey(keyname, keyvalue);
  }
  catch (FitsException&) {
    if (fallback == -1.E9) {
      cerr << "ERROR: Could not read extension " << extnum << " in " << name << endl;
    }
    else 
      keyvalue = fallback;
  }
  return keyvalue;
}


//****************************************************************
// Probes a FITS extension for a float keyword
// For some reson I cannot "template" this in myFITS
//****************************************************************
long myFITS::readLongExtKey(string keyname, int extnum, long fallback)
{
  float keyvalue;
  try {
    ExtHDU &image_ext = image_ptr->extension(extnum);
    image_ext.readKey(keyname, keyvalue);
  }
  catch (FitsException&) {
    if (fallback == -1E9) {
      cerr << "ERROR: Could not read keyword " << keyname << " in extension " << extnum << " in " << name << endl;
    }
    else 
      keyvalue = fallback;
  }
  return keyvalue;
}


//****************************************************************
// read any FITS key and return it as a string
//****************************************************************
string myFITS::readKey(string key)
{
  string keyvalue;
  PHDU &image = image_ptr->pHDU();
  try {
    string tmpval;
    image.readKey(key, tmpval);
    keyvalue = tmpval;
  }
  catch (Keyword::WrongKeywordValueType&) {
    float tmpval;
    image.readKey(key, tmpval);
    keyvalue = NumberToString(tmpval);
  }
  catch (HDU::NoSuchKeyword&) {
    keyvalue = "";
  }
  return keyvalue;
}


//****************************************************************
// read any FITS key in an extension and return it as a string
//****************************************************************
string myFITS::readStringExtKey(string key, int extnum)
{
  string keyvalue;
  try {
    string tmpval;
    ExtHDU &image_ext = image_ptr->extension(extnum);
    image_ext.readKey(key, tmpval);
    keyvalue = tmpval;
  }
  catch (HDU::NoSuchKeyword&) {
    keyvalue = "";
  }

  return keyvalue;
}
