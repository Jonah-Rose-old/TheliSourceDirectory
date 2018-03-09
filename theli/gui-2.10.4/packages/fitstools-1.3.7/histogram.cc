#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cmath>
#include <cctype>
#include <vector>
#include <algorithm>
#include "statistics.h"
#include "histogram.h"

//************************************************************
// Make a histogram of some data
//************************************************************
void histogram::make_histogram(vector<float> &data_orig, 
			       float binsize, float fraction)
{
  // work with a copy of the data (we might sort it)
  vector<float> data(data_orig);

  unsigned long minind = 0;
  unsigned long maxind = data.size() - 1;

  if (fraction <= 0.) {
    cerr << "ERROR: Request to extract zero objects from array" << endl;
    exit (1);
  }

  // extract the central fraction of the data from which to construct 
  // the histogram (if requested)
  if (fraction < 1.0 && fraction > 0.) {
    sort(data.begin(), data.end());
    minind = 0.5*(1.0-fraction) * maxind;
    maxind = 0.5*(1.0+fraction) * maxind;
    // the min and max data values to consider
    minval = data[minind];
    maxval = data[maxind];
  } else {
    minval = min(data);
    maxval = max(data);
  }

  // The number of bins needed to record the histogram
  numbins = (long) ceil((maxval-minval) / binsize); 

  // check that numbins isn't too large; this is a rather dumb way of doing it,
  // but better than nothing
  long maxbins = 1e6;
  if (numbins > maxbins) {
    cerr << "WARNING: More than 1e6 bins in histogram; re-adjusting bin size" << endl;
    binsize = (maxval-minval) / maxbins;
    cerr << "         New bin size: " << binsize << endl;
    numbins = (long) ceil((maxval-minval) / binsize); 
  }

  // Fix the histogram size
  counts.resize(numbins,0.);
  bins.resize(numbins,0.);

  // Write the bin center values
  for (long i=0; i<numbins; i++) {
    bins[i] = minval+(i+0.5)*binsize;
  }

  // Increment the counts, and the number of total 
  // data points entering the histogram
  for (unsigned long i = minind; i<maxind; i++) {
    long binindex = ceil ((data[i] - minval) / binsize) - 1;
    // catch boundary issues
    if (binindex < 0) binindex = 0;
    if (binindex >= numbins) binindex = numbins - 1;
    counts[binindex]++;
    numdatapoints++;
  }
}

//************************************************************
// Normalize the histogram
//************************************************************
void histogram::normalize() {
  for (auto &it: counts) it /= numdatapoints;
}

//************************************************************
// The bin value for which most counts are recorded
//************************************************************
float histogram::peakpos() {
  return bins[ maxindex( counts, counts.size())];
}

//************************************************************
// The maximum histogram value
//************************************************************
float histogram::peakval() {
  return max(counts);
}

//***********************************************************************
// A guess of the FWHM. This will return meaningful values only
// if the S/N is high and the distribution approximately Gaussian
//***********************************************************************
float histogram::fwhmguess() {
  float fwhmxpos1 = 0.;
  float fwhmxpos2 = 0.;
  float peak = peakval();

  for (unsigned long i=0; i<counts.size(); i++) {
    if (counts[i] >= 0.5*peak) {
      fwhmxpos1 = bins[i];
      break;
    }
  }

  for (long i=counts.size()-1; i>=0; i--) {
    if (counts[i] >= 0.5*peak) {
      fwhmxpos2 = bins[i];
      break;
    }
  }

  return (fwhmxpos2 - fwhmxpos1);
}
