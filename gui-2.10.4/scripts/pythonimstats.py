#!/usr/bin/env python

# $1: path
# $2: pixelscale

import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab

# read the data
flt=[]; #str = []
file = open(sys.argv[1]+"/statistics.dat")
for line in file.readlines():
    members = line.split()
    if members[5] == "NA" : members[5] = "-1"
    if members[6] == "NA" : members[6] = "-1"
    if members[7] == "NA" : members[7] = "-1"
    if members[8] == "NA" : members[8] = "-1"
    if members[9] == "NA" : members[9] = "-1"
    if members[10] == "NA" : members[10] = "1.0"
    # skip the first column with the file names
    flt.append(np.array([float(i) for i in members[1:]]))
flt = np.array(flt)

n=len(flt)

xrange=[0,n+1]

image = np.arange(1,n+1)

pixscale = float(sys.argv[2])

fig = plt.figure()
fig.subplots_adjust(wspace=0.5,hspace=0.4)

ax1 = fig.add_subplot(2,2,1)
# the mode is stored in the second column
ax1.plot(image, flt[:,1], 'o-')
ax1.set_xlabel('Image')
ax1.set_ylabel('Mode [ADU]')
ax1.set_title('Sky background', color='#bb0000')
#locs, labels = plt.xticks()
#plt.xticks(locs+1)
ax1.set_xlim(xrange)

ax2 = fig.add_subplot(2,2,2)
ax2.plot(image, flt[:,4], 'o-')
ax2.set_xlim(xrange)
ax2.set_xlabel('Image')
ax2.set_ylabel('FWHM [pixel]')
ax2.set_title('Seeing', color='#bb0000')
yrange3 = plt.ylim()
ax3 = ax2.twinx()
ax3.set_ylim([yrange3[0]*pixscale, yrange3[1]*pixscale])
ax3.set_ylabel('FWHM ["]')
#locs, labels = plt.xticks()
#plt.xticks(locs+1)
ax3.set_xlim(xrange)

ax4 = fig.add_subplot(2,2,3)
ax4.plot(image, flt[:,8], 'o-')
ax4.set_xlabel('Image')
ax4.set_ylabel('Relative ZP [mag]')
ax4.set_title('Transparency', color='#bb0000')
#locs, labels = plt.xticks()
#plt.xticks(locs+1)
ax4.plot(xrange, [0.0,0.0])
ax4.set_xlim(xrange)

ax5 = fig.add_subplot(2,2,4)
ax5.plot(image, flt[:,9], 'o-')
ax5.set_xlabel('Image')
ax5.set_ylabel('Airmass')
ax5.set_title('Airmass', color='#bb0000')
ax5.set_xlim(xrange)
ax5.set_ylim(ax5.get_ylim()[::-1])
yrange6 = plt.ylim()
ax6 = ax5.twinx()
ax6.set_ylim([90.-180./3.14159*np.arccos(1./yrange6[0]), 90.-180./3.14159*np.arccos(1./yrange6[1])])
ax6.set_ylabel('Elevation [deg]')

plt.savefig('statistics.png', bbox_inches='tight')
