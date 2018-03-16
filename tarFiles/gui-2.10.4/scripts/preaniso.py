#!/usr/bin/env python

import sys
import numpy as np
import matplotlib.pyplot as plt
from numpy import *
from scipy.optimize import leastsq

# command line args
catalog=sys.argv[1]
step=float(sys.argv[2])

# read scattered data
data = []
file = open(catalog)
for line in file.readlines():
    members = line.split()
    data.append(np.array([float(i) for i in members[0:]]))
data = np.array(data)

# split into separate arrays
rh  = data[:,0]
mag = data[:,1]

# number of data points
nobj = len(rh)

rh_bin = arange(0,10,step)
nbins = len(rh_bin)
rh_histogram = zeros(nbins)
for i in range(0,nobj):
    binindex = 0
    for j in arange(0,10,step):
        if (rh[i] >= j and rh[i]<j+step):
            rh_histogram[binindex] = rh_histogram[binindex] + 1
        binindex = binindex + 1

print rh_bin, rh_histogram

# fitfunc
def gaussfunc(p,x):
        return p[0]*exp(-(x-p[1])*(x-p[1])/(2.*p[2]))

# residuals
def residuals(p, z, x):
    return (z - gaussfunc(p,x))*sqrt(z)

# least squares fitting
guess_amplitude = max(rh_histogram)
guess_mean = 1.
for i in range(0,nbins):
    if rh_histogram[i] == guess_amplitude:
        guess_mean = rh_bin[i]
        break
guess_var = 0.5*np.var(rh)
param_start = [guess_amplitude, guess_mean, guess_var]
param_final = leastsq(residuals, param_start, args=(rh_histogram,rh_bin))

rh_mean = (param_final[0])[1]
rh_sigma = sqrt((param_final[0])[2])

rh_min = rh_mean - 2.5*rh_sigma
rh_max = rh_mean + 2.5*rh_sigma

magfilt = []

for i in range(0,nobj):
    if (rh[i] >= rh_min and rh[i] <= rh_max):
        magfilt.append(mag[i])

if len(magfilt) > 0:
    print 'rg', rh_min, rh_max, 'MAG_AUTO', min(magfilt)+0.3, max(magfilt)
else:
    print 'rg', min(rh), max(rh), 'MAG_AUTO', min(mag), max(mag)
    
# magnitude filtering


#print (param_final[0])[1]
#print sqrt((param_final[0])[2])

#fig = plt.figure()
#ax1 = fig.add_subplot(1,1,1)
#ax1.plot(rh_bin, rh_histogram,'o-')
#x= arange(0,2,0.01)
#ax1.plot(x,gaussfunc(param_final[0],x),'-')
#ax1.set_xlim([0.3,1.5])
#plt.show()
