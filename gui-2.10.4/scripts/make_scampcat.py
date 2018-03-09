from astropy.io import fits
import sys

hdu=fits.PrimaryHDU()
hdulist = fits.HDUList([hdu])

for i in range(1, len(sys.argv)-1):
  print sys.argv[i]

  hdulisttable = fits.open(sys.argv[i])
  hdulist.append(hdulisttable[1])
  hdulist.append(hdulisttable[2])

hdulist.writeto(sys.argv[len(sys.argv)-1])
