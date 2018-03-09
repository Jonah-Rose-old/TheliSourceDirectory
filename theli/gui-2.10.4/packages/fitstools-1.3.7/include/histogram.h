#ifndef __HISTOGRAM_H
#define __HISTOGRAM_H

using namespace std;


//************************************************************************
// histogram class to perform basic histogram tasks
//************************************************************************

class histogram {

 public:
  // constructor
  histogram(vector<float> data, float binsize=0.0, float fraction=1.0) {
    make_histogram(data, binsize, fraction);
  }
  
  vector<float> counts;  // actual counts per bin
  vector<float> bins;    // center bin positions
  
  float minval = 0.;     // records the minimum of the data
  float maxval = 0.;     // records the maximum of the data
  long numbins;          // the number of bins
  void normalize();      // normalize the histogram
  float peakpos();       // return the bin value for which the most counts
                         // are recorded
  float peakval();       // the maximum value of the histogram
  float fwhmguess();     // a guess of the FWHM

  void make_histogram(vector<float> &, float, float);

  // destructor
  ~histogram() {}
  
 private:
  long numdatapoints = 0;

};

#endif
