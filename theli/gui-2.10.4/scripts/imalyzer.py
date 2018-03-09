#!/usr/bin/env python

import sys
import numpy as np
import matplotlib.pyplot as plt
from numpy import *
from matplotlib import cm
from matplotlib.colors import LinearSegmentedColormap
from scipy.interpolate import Rbf

# command line args
catalog=sys.argv[1]
imagename=sys.argv[2]
pixscale=float(sys.argv[3])
xmax=float(sys.argv[4])
ymax=float(sys.argv[5])
colorrange=float(sys.argv[6])/100.
units=sys.argv[7]
scalemin=sys.argv[8]
scalemax=sys.argv[9]
scaling=sys.argv[10]
nstars=sys.argv[11]
smoothing=sys.argv[12]
contour=sys.argv[13]

# read scattered data
data = []
file = open(catalog)
for line in file.readlines():
    members = line.split()
    data.append(np.array([float(i) for i in members[0:]]))
data = np.array(data)

# split into separate arrays
xpos  = data[:,0]
ypos  = data[:,1]
fwhm  = data[:,2]
epsx  = data[:,3]
epsy  = data[:,4]

# number of data points
nobj = len(xpos)

fwhm_orig = fwhm

# define a grid across the image area
step = 20
xi = np.arange(0,xmax,step)
yi = np.arange(0,ymax,step)
xgrid, ygrid = np.meshgrid(xi, yi)

# 2% of the image width shall correspond to 10% anisotropy
stickscale = 20. * xmax / 100.

if smoothing == "unsmoothed":
    rbf = Rbf(xpos, ypos, fwhm, function='linear')
if smoothing == "smoothed":
    rbf = Rbf(xpos, ypos, fwhm, smooth=0.01)
fwhm_fit = rbf(xgrid, ygrid)

if units == "arcsec":
    fwhm_fit = fwhm_fit * pixscale
if units == "pixel":
    fwhm_fit = fwhm_fit
if units == "minFWHM":
    fwhm_fit = fwhm_fit / min(ravel(fwhm_fit))

# the colorrange of the plot
if scaling == "fullrange":
    colmin = min(ravel(fwhm_fit))
    colmax = max(ravel(fwhm_fit))
if scaling == "max":
    colmin = min(ravel(fwhm_fit))
    colmax = colmin + colorrange*colmin
if scaling == "absolute":
    colmin = float(scalemin)
    colmax = float(scalemax)


######################################################
# make the plot
######################################################

# user-defined colormap
cdict = {
    'red':
    ((0.00, 0.00, 0.00),
     (0.05, 0.00, 0.00),
     (0.20, 0.00, 0.00),
     (0.40, 0.00, 0.00),
     (0.55, 1.00, 1.00),
     (0.85, 1.00, 1.00),
     (1.00, 1.00, 1.00)),
    'green':
    ((0.00, 0.00, 0.00),
     (0.05, 0.00, 0.00),
     (0.20, 1.00, 1.00),
     (0.40, 1.00, 1.00),
     (0.55, 1.00, 1.00),
     (0.85, 0.00, 0.00),
     (1.00, 0.00, 0.00)),
    'blue':
    ((0.00, 0.50, 0.50),
     (0.05, 1.00, 1.00),
     (0.20, 1.00, 1.00),
     (0.40, 0.00, 0.00),
     (0.55, 0.00, 0.00),
     (0.85, 0.00, 0.00),
     (1.00, 1.00, 1.00))}

# the plotting windows
left   = 0.06
bottom = 0.15
width  = 0.70
height = 0.80
rect1  = left, bottom, width, height
rect2  = left+width+0.05, bottom, 0.02, height

# the FWHM image
fig = plt.figure()
ax1 = fig.add_axes(rect1, aspect='equal')
my_cmap = LinearSegmentedColormap('my_colormap',cdict,256)
image = ax1.pcolor(xgrid, ygrid, fwhm_fit, cmap=my_cmap, vmin=colmin, vmax=colmax)

# the ellipticity sticks
xp1 = zeros(nobj)
xp2 = zeros(nobj)
yp1 = zeros(nobj)
yp2 = zeros(nobj)

for i in range(0,nobj):
    xp1[i] = xpos[i] - 0.5 * epsx[i] * stickscale
    xp2[i] = xpos[i] + 0.5 * epsx[i] * stickscale
    yp1[i] = ypos[i] - 0.5 * epsy[i] * stickscale
    yp2[i] = ypos[i] + 0.5 * epsy[i] * stickscale

ax1.plot((xp1,xp2),(yp1,yp2),'k')

i_l = 0.03*xmax
i_r = 0.03*xmax + 0.1*stickscale
i_b = 0.07*xmax
ax1.plot([i_l,i_r],[i_b,i_b],'w',lw=2)
ax1.text(i_l,0.03*xmax,'10%',color='w', weight='bold')
if contour == "yes":
    ax1.contour(xgrid, ygrid, fwhm_fit, [median(ravel(fwhm_fit))], colors='w', linewidths=[2])
ax1.set_xlim(0,xmax)
ax1.set_ylim(0,ymax)

ax2=fig.add_axes(rect2)
cbar=fig.colorbar(image, cax=ax2)
if units == "arcsec":
    ax2.set_ylabel("FWHM [arcsec]")
if units == "pixel":
    ax2.set_ylabel("FWHM [pixel]")
if units == "minFWHM":
    ax2.set_ylabel("FWHM / FWHM_min")

# figure annotations
fwhmstr_pix = str(median(fwhm_orig))
fwhmstr_arc = str(median(fwhm_orig)*pixscale)
fwhmstr_pix1 = fwhmstr_pix[0:5]
fwhmstr_arc1 = fwhmstr_arc[0:5]
#fig.suptitle(imagename)
fig.text(0.1,bottom+height+0.01,imagename+".fits")
fig.text(0.1,0.03,"Median FWHM: "+fwhmstr_pix1+" pixel")
fig.text(0.1,0.07,"Median FWHM: "+fwhmstr_arc1+" arcsec")
fig.text(0.5,0.05,"("+str(nstars)+" stars used)")

plt.savefig(imagename+".png")
