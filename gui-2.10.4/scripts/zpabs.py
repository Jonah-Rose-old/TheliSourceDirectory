#!/usr/bin/env python

import sys
import matplotlib.pyplot as plt
import matplotlib.lines as lines
from matplotlib import cm
import numpy as np
from numpy import *
import string

def readInput(file):
    f = open(file, "r")

    magdiff = []
    airmass = []
    color   = []
    flag    = []

    i=0
    for line in f.readlines():
        if i==0:
            ZP_tmp, EXT_tmp, COL_tmp, rms_tmp, STATUS = string.split(line)
            ZP  = float(ZP_tmp)
            EXT = float(EXT_tmp)
            COL = float(COL_tmp)
            rms = float(rms_tmp)
        else:
            magdiff_tmp, airmass_tmp, color_tmp, flag_tmp = string.split(line)
            magdiff.append(float(magdiff_tmp))
            airmass.append(float(airmass_tmp))
            color.append(float(color_tmp))
            flag.append(int(flag_tmp))
        i=i+1
    f.close()

    magdiff = np.array(magdiff)
    airmass = np.array(airmass)
    color   = np.array(color)
    flag    = np.array(flag)

    return magdiff, airmass, color, flag, ZP, EXT, COL, rms, STATUS


# main part
path=sys.argv[1]
ytitle=sys.argv[2]
xtitle=sys.argv[3]

fig = plt.figure()
fig.subplots_adjust(wspace=0.3,hspace=0.0)
fig.set_size_inches(8,7)

# Font sizes
fs  = 8
fs2 = 6
ms  = 3

for solution in range(4):

    # Read the data
    magdiff, airmass, color, flag, ZP, EXT, COL, rms, STATUS = readInput(path+"/photcal_"+str(solution)+".dat")
    nobj = len(magdiff)
    index_bad  = []
    index_good = []
    for i in range(nobj):
        if flag[i] == 0: index_bad.append(i)
        if flag[i] == 1: index_good.append(i)

    ydata1 = magdiff - COL*color
    ydata2 = magdiff - EXT*airmass
    
    # Plot limits
    airmass_min   = 0.95
    airmass_max   = max(ravel(airmass)) + 0.05
    color_min     = min(ravel(color)) - 0.05
    color_max     = max(ravel(color)) + 0.05
    ydata1_min    = min(ydata1[index_good]) - 0.2
    ydata1_max    = max(ydata1[index_good]) + 0.1
    ydata2_min    = min(ydata2[index_good]) - 0.2
    ydata2_max    = max(ydata2[index_good]) + 0.1
    airmass_range = [airmass_min, airmass_max]
    color_range   = [color_min, color_max]
    ydata1_range  = [ydata1_min, ydata1_max]
    ydata2_range  = [ydata2_min, ydata2_max]
    
    if STATUS == "GOOD":
        statuscolor = '#00aa00'

    if STATUS == "BAD":
        statuscolor = '#aa0000'

    if solution == 0:
        titlestring1 = "3-parameter fit (#1)"
        titlestring2 = "ZP = %.3f\nEXT = %.3f\nCOL = %.3f\nrms = %.3f" % (ZP, EXT, COL, rms)
    if solution == 1:
        titlestring1 = "2-parameter fit (#2)"
        titlestring2 = "ZP = %.3f\nEXT = %.3f\nCOL = %.3f (fixed)\nrms = %.3f" % (ZP, EXT, COL, rms)
    if solution == 2:
        titlestring1 = "2-parameter fit (#3)"
        titlestring2 = "ZP = %.3f\nEXT = %.3f (fixed)\nCOL = %.3f\nrms = %.3f" % (ZP, EXT, COL, rms)
    if solution == 3:
        titlestring1 = "1-parameter fit (#4)"
        titlestring2 = "ZP = %.3f\nEXT = %.3f (fixed)\nCOL = %.3f (fixed)\nrms = %.3f" % (ZP, EXT, COL, rms)
    
    ax1 = fig.add_subplot(4,2,2*(solution+1)-1)
    ax1.plot(airmass[index_good], ydata1[index_good], '.', color='black', markersize=ms)
    ax1.plot(airmass[index_bad], ydata1[index_bad], 'x', color='red', markersize=ms)
    if solution == 3:
        ax1.set_xlabel('Airmass', fontsize=fs)
    if solution == 1:
        ax1.set_ylabel('(color corrected)', fontsize=fs)
    if solution == 2:
        ax1.set_ylabel(ytitle, fontsize=fs)
    ax1.set_xlim(airmass_range)
    ax1.set_ylim(ydata1_range)
    fit_vertx = airmass_range
    fit_verty = [ZP + EXT*airmass_min, ZP + EXT*airmass_max]
    fit_line  = lines.Line2D(fit_vertx, fit_verty, color='blue', linestyle='-', linewidth=1)
    ax1.add_line(fit_line)
    ax1.tick_params(labelsize=fs)
    ax1.text(airmass_min+0.05, ydata1_max-0.05, titlestring1, fontsize=fs);
    ax1.text(airmass_min+0.05, ydata1_min+0.05, STATUS, fontsize=fs, color=statuscolor, weight='bold');
    if solution != 3:
        ax1.set_xticklabels([])

    ax2 = fig.add_subplot(4,2,2*(solution+1))
    ax2.plot(color[index_good], ydata2[index_good], '.', color='black', markersize=ms)
    ax2.plot(color[index_bad], ydata2[index_bad], 'x', color='red', markersize=ms)
    if solution == 3:
        ax2.set_xlabel(xtitle, fontsize=fs)
    if solution == 1:
        ax2.set_ylabel('(extinction corrected)', fontsize=fs)
    if solution == 2:
        ax2.set_ylabel(ytitle, fontsize=fs)
    ax2.set_xlim(color_range)
    ax2.set_ylim(ydata2_range)
    fit_vertx = color_range
    fit_verty = [ZP + COL*color_min, ZP + COL*color_max]
    fit_line  = lines.Line2D(fit_vertx, fit_verty, color='blue', linestyle='-', linewidth=1)
    ax2.add_line(fit_line)
    ax2.tick_params(labelsize=fs)
    if COL > 0.:
        ax2.text(color_min+0.1, ydata2_max-0.05, titlestring2, color='#008800', fontsize=fs2, va='top');
    else:
        ax2.text(color_min+0.1, ydata2_min+0.05, titlestring2, color='#008800', fontsize=fs2, va='bottom');
    if solution != 3:
        ax2.set_xticklabels([])

plt.savefig(path+'photcal.png', bbox_inches='tight', dpi=150)
