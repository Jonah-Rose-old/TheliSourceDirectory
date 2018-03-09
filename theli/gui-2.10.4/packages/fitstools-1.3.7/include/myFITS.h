#ifndef __MYFITS_H
#define __MYFITS_H

using namespace std;
using namespace CCfits;


//************************************************************************
// myFITS class to keep all the odd CCfits stuff outside my source code.
// Just in case CCfits changes or gets replaced by something else ...
//************************************************************************

class myFITS {

 private:
  bool fileexists;
  long n, m;
  string name;
  valarray<float> data_va;
  //PHDU &image;

 public:
  int bitpix;
  vector<float> data;
  long naxis1, naxis2;
  long nrows, ncols; // for FITS tables
  auto_ptr<FITS> image_ptr;
  
  // constructor
  myFITS (string filename, bool readdata=true, string mode="Read") 
    : fileexists(true) // we need that in oder to be able to set a pointer,
    //even if the image does not exist (see e.g. fitsmask.cpp)
    , n(1)
    , m(1)
    , name(filename)
    , bitpix(-32)
    , nrows(0)
    , ncols(0) 
    , image_ptr(0)
    {
      if (mode.compare("Read") == 0) {
	try {
	  image_ptr.reset( new FITS(filename, Read) );
	}
	catch (FitsException&) {
	  fileexists = false;
	  if (readdata) {
	    cerr << "ERROR: Could not read " << filename << endl;
	    exit (1);
	  }
	}
      }
      else
	image_ptr.reset( new FITS(filename, Write) );

      if (fileexists) {
	PHDU &image = image_ptr->pHDU();

	// If this is not a MEF file (number of axes != 0) 
	// determine geometry and bitpix, and read the data
	if (image.axes() != 0) {
	  n = image.axis(0);
	  m = image.axis(1);
	  bitpix = image.bitpix();
	  naxis1 = n;
	  naxis2 = m;
	  if (readdata) readData();
	}
      }
    }
  
  // "Empty" constructor
  myFITS () 
    : fileexists(true)
    , n(1)
    , m(1)
    , name("")
    , bitpix(-32)
    , nrows(0)
    , ncols(0)
    , image_ptr(0)
    {}

  // destructor; doesn't do anything at the moment
  ~myFITS() {}
  
  // actively close a FITS file; needed in rare cases to free the memory
  void destroy() {
    image_ptr->destroy();
  }

  // open a FITS file; similar to ctor, but needed when the pointer
  // has been set but nothing was done yet with the image
  // e.g. swarpfilter
  void open(string filename, bool readdata=true, string mode="Read") 
  {
    if (mode.compare("Read") == 0) 
      image_ptr.reset( new FITS(filename, Read) );
    else
      image_ptr.reset( new FITS(filename, Write) );
    PHDU &image = image_ptr->pHDU();
    // If this is not a MEF file (number of axes != 0) 
    if (image.axes() != 0) {
      n = image.axis(0);
      m = image.axis(1);
      bitpix = image.bitpix();
      naxis1 = n;
      naxis2 = m;
      if (readdata) readData();
    }
  }

  // read the data (or a rectangular section thereof)
  void readData();
  void readData(valarray<float>&);
  void readExtData(int);
  void readExtData(vector<float>&, int); 
  void readDataSection(long, long, long, long);
  void readDataSectionEff(long, long, long, long, valarray<float>&);
  void check_BSCALE();

  // add an image to a FITS extension
  //  void addExtData(auto_ptr<FITS> imageout_ptr, vector<float>&, int, int, int m);

  //***********************************************
  // Various read keyword functions
  //***********************************************
  
  // Read any key and return its value as a string
  string readKey(string key);
  double readDoubleKey(string key);

  // Read a FITS key. Requires the user to know the keyword type (string or numeric)
  template <class T>
  void readmyKey(string key, T &value) {
    PHDU &image = image_ptr->pHDU();
    try {
      image.readKey(key, value);
    }
    catch (FitsException&) {
      cerr << "ERROR: Could not find key " << key << " in " << name << endl;
      exit (1);
    }
  }

  // Read a FITS key. If it doesn't exist, read an alternative keyword
  template <class T>
  T readKeyBackup(string key, string keybackup) {
    T value;
    PHDU &image = image_ptr->pHDU();
    try {
      image.readKey(key, value);
      return value;
    }
    catch (FitsException&) {
      try {
	image.readKey(keybackup, value);
	return value;
      }
      catch (FitsException&) {
	// The key does not exist
	cerr << "ERROR: Could not find keys " << key << " and "
	     << keybackup << " in " << name << endl;
	return T();
      }
    }
  }

  // read a FITS key failsafe (return a default value if key not found,
  // and write the key if a comment string is given)
  // IMPORTANT! "initvalue" MUST BE OF THE SAME TYPE AS THE KEYWORD GOING 
  // TO BE READ! IF YOU E.G. READ A FLOAT, INITVALUE MUST ALSO BE A FLOAT!
  template <class T>
  T readKeyFailsafe(string key, T initvalue, string comment = "") {
    T value;
    PHDU &image = image_ptr->pHDU();
    try {
      image.readKey(key, value);
      return value;
    }
    catch (FitsException&) {
      if (!comment.empty()) 
	image.addKey(key, initvalue, comment);
      return initvalue;
    }
  }

  // add a FITS key
  template <class T>
  void addKey(string key, T value, string comment) {
    PHDU &image = image_ptr->pHDU();
    try {
      image.addKey(key, value, comment);
    }
    catch (FitsException&) {
      cerr << "ERROR: Could not add key " << key << " to " << name << endl;
    }
  }

  int eBitpix(int);
  long eNaxis1(int);
  long eNaxis2(int);
  float exptime();
  double mjdobs();

  float readExtKey(string keyname, int extnum, float fallback=-1.E9);
  double readDoubleExtKey(string keyname, int extnum, double fallback=-1.E9);
  long  readLongExtKey(string keyname, int extnum, long fallback=-1E9);
  string readStringExtKey(string key, int extnum);

  // How many extensions exist in the FITS file
  int numExt();

  // TABLE operations
  
  // Read a column from a FITS table into a vector
  template <class T>
  void readTableColumn(string tablename, string columnname, vector<T> &data) {
    // try opening the table
    try {
      ExtHDU &table = image_ptr->extension(tablename);
      // The table dimensions
      nrows = table.rows();
      ncols = table.numCols();
      if (nrows > 0 && ncols > 0) {
	data.resize(nrows,0);
	table.column(columnname).read(data, 1, nrows);
      }
      else {
	cerr << "ERROR: Table " << columnname << " does not contain any data!" << endl;
      }
    }
    catch (FitsException&) {
      cerr << "ERROR: Table " << tablename << " does not exist in this FITS file!";
    }
  
  
  }
};

#endif
