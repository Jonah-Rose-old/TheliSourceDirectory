#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -p pattern (available: RGGB, GRBG, GBRG, BGGR)\n";
    cerr << "           -q 0    for bilinear Interpolation\n";
    cerr << "           -q 1    for gradient Interpolation\n";
    cerr << "           -q 2    for PPG Interpolation\n";
    cerr << "           -c 2.21 1.00 1.01 1.51 for bayerfilter RGBG colorbalance\n";
    cerr << "           -o 256  for Offset correction before RGBG colorbalance\n";
    cerr << "  PURPOSE: Demosaics a bayer matrix fits image into\n";
    cerr << "  three monochrome fits images for RGB.\n\n";

    exit(1);
  }
}


float hue_transit(float l1,float l2,float l3,float v1,float v3)
{
//printf("hue_transit: l1 %5.1f l2 %5.1f l3 %5.1f v1 %5.1f v3 %5.1f\n",l1,l2,l3,v1,v3);
  if((l1<l2 && l2<l3) || (l1> l2 && l2 > l3))
    return(v1+(v3-v1) * (l2-l1)/(l3-l1));
  else
    return((v1+v3)/2.0+(l2*2.0-l1-l3)/4.0);
}

int direction(float N, float E, float W, float S)
{
  if (N < E && W < S)
    if ( N < W)
      return(1);
    else
      return(3);
  else
    if (E < S)
      return (2);
    else
      return (4);
}

int main(int argc, char *argv[])
{
  int n, m, flag_q;
  float R=1.0, G=1.0, g=1.0, B=1.0, o=0.0;  // Colorbalance of 4 Colorfilters
  int xoffset,yoffset; // for color determing
  long i, j, k1 = 0;
  float H,V;  // for Gradient horizontal, vertical
  float DN,DE,DW,DS,dne,dnw;   // for PPG
  string input_image, pattern, tmp;
  string out1, out2, out3;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  // default: Linear interpolation
  flag_q = 0;
    
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'p': pattern = argv[++i];
	break;
      case 'q': flag_q = atoi(argv[++i]);
	break;
      case 'c': R = atof(argv[++i]);
	G = atof(argv[++i]);
	B = atof(argv[++i]);
	g = atof(argv[++i]);
	break;
      case 'o': o = atof(argv[++i]);
	break;
      }
    }
  }

  if (! (pattern.compare("RGGB") == 0 || pattern.compare("GRBG") == 0 || 
         pattern.compare("GBRG") == 0 || pattern.compare("BGGR") == 0)) {
    cout << "\nBayer pattern not recognised. Nothing will be done.\n\n";
    return 0;
  }

  // read the FITS header and the data block
  checkfile(input_image);
  myFITS image_in(input_image);
  n = image_in.naxis1;
  m = image_in.naxis2;
  
  // chop the last row / column of pixels if the image dimensions are uneven
  if ( n % 2 != 0) n = n - 1;
  if ( m % 2 != 0) m = m - 1;
  
  // the file name prefix
  tmp = input_image;
  tmp.resize(input_image.length() - 5); // hardcoded file suffix ".fits"
  out1 = tmp;
  out2 = tmp;
  out3 = tmp;

  // cut all pattern to RGGB
  if (pattern.compare("RGGB") == 0) {
    xoffset=0;
    yoffset=0;
  }
  if (pattern.compare("GRBG") == 0) {
    xoffset=1;
    yoffset=0;
    
  }
  if (pattern.compare("GBRG") == 0) {
    xoffset=0;
    yoffset=1;
  }
  if (pattern.compare("BGGR") == 0) {
    xoffset=1;
    yoffset=1;
  }
  /*
    RGRGRGRGR
    gBgBgBgBg
    RGRGRGRGR
    gBgBgBgBg
    
  */

  out1 = out1 + ".R.fits";
  out2 = out2 + ".G.fits";
  out3 = out3 + ".B.fits";
  
  vector<float> channel1(n*m,0);
  vector<float> channel2(n*m,0);
  vector<float> channel3(n*m,0);
  
  // correct Offset & correct colors
  if (!(R==1.0 && G==1.0 && B==1.0 && o==0.0)) {
    printf("subtracts offset:%4.2f\ncolorbalance with sensormultipliers of R:G:B:G = %4.2f:%4.2f:%4.2f:%4.2f\n",o,R,G,B,g);
    
    for (j=0; j<m; j++) { // rows
      for (i=0; i<n; i++) { // columns
	
	if( j==0 || j>=m-1 || i==0 || i>=n-1) {  // upper lower left right edge
	  // don't care here
	  k1++;
	}
	else{
	  if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // Rotes Feld
	    image_in.data[(i+n*j)] = R*(image_in.data[(i+n*j)]-o);
	    k1++;
	  }
	  if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	    image_in.data[(i+n*j)]= G*(image_in.data[(i+n*j)]-o);
	    k1++;
	  }
	  if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	    image_in.data[(i+n*j)]= g*(image_in.data[(i+n*j)]-o);
	    k1++;;
	  }
	  if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	    image_in.data[(i+n*j)]  = B*(image_in.data[(i+n*j)]-o);
	    k1++;
	  }
	}
      }
    }
  }
  
  k1 = 0;  // red
  if (flag_q == 0){
    for (j=0; j<m; j++) { //Zeilen
      for (i=0; i<n; i++) { // Spalten
	
	if(j==0||j>=m-1||i==0|| i>=n-1) {  // Rand oben unten links rechts
	  /*channel1[k1]=image_in.data[(i+n*j)];
	    channel2[k1]=image_in.data[(i+n*j)];
	    channel3[k1]=image_in.data[(i+n*j)];*/
	  if(!xoffset && !yoffset){  // RGGB
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i+1+n*j)];
	      channel3[k1] = image_in.data[(i+1+n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i-n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i-1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i-1-n+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  if(xoffset && !yoffset){ //GRBG
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i-1+n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i-n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i+1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i+1-n+n*j)];
	      channel2[k1] = image_in.data[(i+1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  if(!xoffset && yoffset){ //GBRG
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i-n+n*j)];
	      channel3[k1] = image_in.data[(i+1-n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i+n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i-1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i-1+n+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  if(xoffset && yoffset){ //BGGR
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i-1-n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i+n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i+1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i+1+n+n*j)];
	      channel2[k1] = image_in.data[(i+1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  k1++;
	}
	else{
	  if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	    channel1[k1] = image_in.data[(i+n*j)];
	    channel2[k1] = (image_in.data[(i-1+n*j)] + image_in.data[(i+1+n*j)]
			  + image_in.data[(i-n+n*j)] + image_in.data[(i+n+n*j)]) / 4.0;
	    channel3[k1] = (image_in.data[(i-n-1+n*j)] + image_in.data[(i-n+1+n*j)] +
			    image_in.data[(i+n-1+n*j)] + image_in.data[(i+n+1+n*j)]) / 4.0;
	    k1++;
	  }
	  if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	    channel1[k1] = (image_in.data[(i-n+n*j)] + image_in.data[(i+n+n*j)]) / 2.0;
	    channel2[k1] = image_in.data[(i+n*j)];
	    channel3[k1] = (image_in.data[(i-1+n*j)] + image_in.data[(i+1+n*j)]) / 2.0;
	    k1++;
	  }
          if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	    channel1[k1] = (image_in.data[(i-1+n*j)] + image_in.data[(i+1+n*j)]) / 2.0;
	    channel2[k1] = image_in.data[(i+n*j)];
	    channel3[k1] = (image_in.data[(i-n+n*j)] + image_in.data[(i+n+n*j)]) / 2.0;
	    k1++;;
          }
	  if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	    channel1[k1] = (image_in.data[(i-n-1+n*j)] + image_in.data[(i-n+1+n*j)] +
			    image_in.data[(i+n-1+n*j)] + image_in.data[(i+n+1+n*j)]) / 4.0;
	    channel2[k1] = (image_in.data[(i-1+n*j)] + image_in.data[(i+1+n*j)]
			  + image_in.data[(i-n+n*j)] + image_in.data[(i+n+n*j)]) / 4.0;
	    channel3[k1] = image_in.data[(i+n*j)];
	    k1++;
	  }
	}
      }
    }
  }
  
  // Gradienten Mode;  refers to: grafics.cs.msa.ru/en/publications/text/gc2004lk.pdf (A. Lukin
  // and D. Kubasov)
  if (flag_q == 1){
    for (j=0; j<m; j++) { //Zeilen
      for (i=0; i<n; i++) { // Spalten
	
	if(j==0||j>=m-1||i==0|| i>=n-1) {  // Rand oben unten links rechts
	  /*channel1[k1]=image_in.data[(i+n*j)];
	    channel2[k1]=image_in.data[(i+n*j)];
	    channel3[k1]=image_in.data[(i+n*j)];*/
	  if(!xoffset && !yoffset){  // RGGB
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i+1+n*j)];
	      channel3[k1] = image_in.data[(i+1+n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i-n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i-1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i-1-n+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  if(xoffset && !yoffset){ //GRBG
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i-1+n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i-n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i+1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i+1-n+n*j)];
	      channel2[k1] = image_in.data[(i+1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  if(!xoffset && yoffset){ //GBRG
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i-n+n*j)];
	      channel3[k1] = image_in.data[(i+1-n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i+n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i-1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i-1+n+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  if(xoffset && yoffset){ //BGGR
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i-1-n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i+n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i+1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i+1+n+n*j)];
	      channel2[k1] = image_in.data[(i+1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  k1++;
	}
	else{
	  if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld 
	    channel1[k1] = image_in.data[(i+n*j)];
	    // Gradients determing for green
	    H=abs(image_in.data[(i-1+n*j)] - image_in.data[(i+1+n*j)]);
	    V=abs(image_in.data[(i-n+n*j)] - image_in.data[(i+n+n*j)]);
	    if ( H>=V )
	      channel2[k1] = (image_in.data[(i-n+n*j)] + image_in.data[(i+n+n*j)]) / 2.0;
	    else
	      channel2[k1] = (image_in.data[(i-1+n*j)] + image_in.data[(i+1+n*j)]) / 2.0;
	    // Gradients determing for blue H=L (H=high to Right down)
	    H=abs(image_in.data[(i-n-1+n*j)] - image_in.data[(i+1+n*j)]);
	    V=abs(image_in.data[(i+n-1+n*j)] - image_in.data[(i-n+1+n*j)]);
	    if (H>=V)
              channel3[k1] = (image_in.data[(i+n-1+n*j)] + image_in.data[(i-n+1+n*j)]) / 2.0;
	    else
              channel3[k1] = (image_in.data[(i-n-1+n*j)] + image_in.data[(i+n+1+n*j)]) / 2.0;
	    k1++;
	  }
	  if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	    channel1[k1] = (image_in.data[(i-n+n*j)] + image_in.data[(i+n+n*j)]) / 2.0;
	    channel2[k1] = image_in.data[(i+n*j)];
	    channel3[k1] = (image_in.data[(i-1+n*j)] + image_in.data[(i+1+n*j)]) / 2.0;
	    k1++;
	  }
          if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	    channel1[k1] = (image_in.data[(i-1+n*j)] + image_in.data[(i+1+n*j)]) / 2.0;
	    channel2[k1] = image_in.data[(i+n*j)];
	    channel3[k1] = (image_in.data[(i-n+n*j)] + image_in.data[(i+n+n*j)]) / 2.0;
	    k1++;;
	  }
	  if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	    // Gradients determing for Red H=L (H=high to Right down)
	    H=abs(image_in.data[(i-n-1+n*j)] - image_in.data[(i+1+n*j)]);
	    V=abs(image_in.data[(i+n-1+n*j)] - image_in.data[(i-n+1+n*j)]);
	    if (H>=V)
              channel1[k1] = (image_in.data[(i+n-1+n*j)] + image_in.data[(i-n+1+n*j)]) / 2.0;
	    else
              channel1[k1] = (image_in.data[(i-n-1+n*j)] + image_in.data[(i+n+1+n*j)]) / 2.0;
	    // Gradients determing for green
	    H=abs(image_in.data[(i-1+n*j)] - image_in.data[(i+1+n*j)]);
	    V=abs(image_in.data[(i-n+n*j)] - image_in.data[(i+n+n*j)]);
	    if ( H>=V )
	      channel2[k1] = (image_in.data[(i-n+n*j)] + image_in.data[(i+n+n*j)]) / 2.0;
	    else
	      channel2[k1] = (image_in.data[(i-1+n*j)] + image_in.data[(i+1+n*j)]) / 2.0;
	    channel3[k1] = image_in.data[(i+n*j)];
	    k1++;
	  }
	}
      }
    }
  }
  
  // PPG, web.cecs.pdx.edu/~cklin/demosaic/  (Chuan-Kai Lin)
  if (flag_q == 2){
    // Calculation the green values at red and blue pixels
    for (j=0; j<m; j++) { //Zeilen
      for (i=0; i<n; i++) { // Spalten
	
	//if(j<=2||j>=m-2||i<=2|| i>=n-2) {         // 3 Reihen Rand oben unten links rechts
	if(j<=2||j>=m-3||i<=2|| i>=n-3) {
	  channel1[k1]=image_in.data[(i+n*j)];  // alle gleich gemacht mit aktueller Farbe
	  channel2[k1]=image_in.data[(i+n*j)];
	  channel3[k1]=image_in.data[(i+n*j)]; 
	  k1++;
	}
	else{
	  //Gradients calculation  for green values at red or blue pixels
	  DN=abs(image_in.data[(i-2*n+n*j)]-image_in.data[(i+n*j)])*2.0+abs(image_in.data[(i-n+n*j)]-image_in.data[(i+n+n*j)]);
	  DE=abs(image_in.data[(i+n*j)]-image_in.data[(i+2+n*j)])*2.0+abs(image_in.data[(i-1+n*j)]-image_in.data[(i+1+n*j)]);
	  DW=abs(image_in.data[(i-2+n*j)]-image_in.data[(i+n*j)])*2.0+abs(image_in.data[(i-1+n*j)]-image_in.data[(i+1+n*j)]);
	  DS=abs(image_in.data[(i+n*j)]-image_in.data[(i+2*n+n*j)])*2.0+abs(image_in.data[(i-n+n*j)]-image_in.data[(i+n+n*j)]);
	  
	  if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld 
	    channel1[k1] = image_in.data[(i+n*j)];
	    switch (direction(DN,DE,DW,DS)){
	    case 1: channel2[k1] = (image_in.data[(i-n+n*j)]*3.0+ image_in.data[(i+n*j)]
				  + image_in.data[(i+n+n*j)] - image_in.data[(i-2*n+n*j)]) / 4.0;break;
	    case 2: channel2[k1] = (image_in.data[(i+1+n*j)]*3.0+ image_in.data[(i+n*j)]
				  + image_in.data[(i-1+n*j)] - image_in.data[(i+2+n*j)]) / 4.0;break;
	    case 3: channel2[k1] = (image_in.data[(i-1+n*j)]*3.0+ image_in.data[(i+n*j)]
				  + image_in.data[(i+1+n*j)] - image_in.data[(i-2+n*j)]) / 4.0;break;
	    case 4: channel2[k1] = (image_in.data[(i+n+n*j)]*3.0+ image_in.data[(i+n*j)]
				  + image_in.data[(i-n+n*j)] - image_in.data[(i+2*n+n*j)]) / 4.0;break;
	    }
	    //channel3[k1] = (image_in.data[(i-n-1+n*j)] + image_in.data[(i-n+1+n*j)] +
	    //image_in.data[(i+n-1+n*j)] + image_in.data[(i+n+1+n*j)]) / 4.0;
	    k1++;
	  }
	  if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	    //channel1[k1] = (image_in.data[(i-n-1+n*j)] + image_in.data[(i-n+1+n*j)] +
	    //image_in.data[(i+n-1+n*j)] + image_in.data[(i+n+1+n*j)]) / 4.0;
	    switch (direction(DN,DE,DW,DS)){
	    case 1: channel2[k1] = (image_in.data[(i-n+n*j)]*3.0+ image_in.data[(i+n*j)]
				  + image_in.data[(i+n+n*j)] - image_in.data[(i-2*n+n*j)]) / 4.0;break;
	    case 2: channel2[k1] = (image_in.data[(i+1+n*j)]*3.0+ image_in.data[(i+n*j)]
				  + image_in.data[(i-1+n*j)] - image_in.data[(i+2+n*j)]) / 4.0;break;
	    case 3: channel2[k1] = (image_in.data[(i-1+n*j)]*3.0+ image_in.data[(i+n*j)]
				  + image_in.data[(i+1+n*j)] - image_in.data[(i-2+n*j)]) / 4.0;break;
	    case 4: channel2[k1] = (image_in.data[(i+n+n*j)]*3.0+ image_in.data[(i+n*j)]
				  + image_in.data[(i-n+n*j)] - image_in.data[(i+2*n+n*j)]) / 4.0;break;
	    }
	    channel3[k1] = image_in.data[(i+n*j)];
	    k1++;
	  }
	  if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	    //channel1[k1] = (image_in.data[(i-n+n*j)] + image_in.data[(i+n+n*j)])/2.0;
	    channel2[k1] = image_in.data[(i+n*j)];
	    //channel3[k1] = (image_in.data[(i-1+n*j)] + image_in.data[(i+1+n*j)])/2.0;
	    k1++;
	  }
          if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	    //channel1[k1] = (image_in.data[(i-1+n*j)] + image_in.data[(i+1+n*j)])/2.0;
	    channel2[k1] = image_in.data[(i+n*j)];
	    //channel3[k1] = (image_in.data[(i-n+n*j)] + image_in.data[(i+n+n*j)])/2.0;
	    k1++;;
	  }
	}
      }
    }
    
    k1 = 0; 
    // Calculating blue and red at green pixels
    // Calculating blue or red at red or blue pixels
    for (j=0; j<m; j++) { //Zeilen
      for (i=0; i<n; i++) { // Spalten
	
	if(j<=2||j>=m-3||i<=2|| i>=n-3) {  // drei Reihen Rand oben unten links rechts
	  /*channel1[k1]=image_in.data[(i+n*j)];  // alle gleich gemacht mit aktueller Farbe
	    channel2[k1]=image_in.data[(i+n*j)];
	    channel3[k1]=image_in.data[(i+n*j)];*/
	  if(!xoffset && !yoffset){  // RGGB
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i+1+n*j)];
	      channel3[k1] = image_in.data[(i+1+n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i-n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i-1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i-1-n+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  if(xoffset && !yoffset){ //GRBG
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i-1+n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i-n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i+1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i+1-n+n*j)];
	      channel2[k1] = image_in.data[(i+1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  if(!xoffset && yoffset){ //GBRG
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i-n+n*j)];
	      channel3[k1] = image_in.data[(i+1-n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i+n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i+1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i-1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i-1+n+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  if(xoffset && yoffset){ //BGGR
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld
	      channel1[k1] = image_in.data[(i+n*j)];
	      channel2[k1] = image_in.data[(i-1+n*j)];
	      channel3[k1] = image_in.data[(i-1-n+n*j)]; 
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	      channel1[k1] = image_in.data[(i+n+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-1+n*j)];
	    }
	    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	      channel1[k1] = image_in.data[(i+1+n*j)];
	      channel2[k1] = image_in.data[(i+n*j)];
	      channel3[k1] = image_in.data[(i-n+n*j)];
	    }
	    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	      channel1[k1] = image_in.data[(i+1+n+n*j)];
	      channel2[k1] = image_in.data[(i+1+n*j)];
	      channel3[k1] = image_in.data[(i+n*j)];
	    }
	  }
	  k1++;
	}
	else{
	  // diagonal Gradients
	  dne=abs(image_in.data[(i-n+1+n*j)]-image_in.data[(i+n-1+n*j)])+abs(image_in.data[(i-2*n+2+n*j)]-image_in.data[(i+n*j)])+abs(image_in.data[(i+n*j)]-image_in.data[(i+2*n-2+n*j)])+abs(channel2[(i-n+1+n*j)]-channel2[(i+n*j)])+abs(channel2[(i+n*j)]-channel2[(i+n-1+n*j)]);
	  dnw=abs(image_in.data[(i-n-1+n*j)]-image_in.data[(i+n+1+n*j)])+abs(image_in.data[(i-2-2*n+n*j)]-image_in.data[(i+n*j)])+abs(image_in.data[(i+n*j)]-image_in.data[(i+2+2*n+n*j)])+abs(channel2[(i-n-1+n*j)]-channel2[(i+n*j)])+abs(channel2[(i+n*j)]-channel2[(i+n+1+n*j)]);
	  if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // rotes Feld 
	    if (dne<=dnw)
	      //printf("hue_transit Value: %5.1f\n",hue_transit(channel2[i-n+1+n*j],channel2[i+n*j],channel2[i+n-1+n*j], image_in.data[(i-n+1+n*j)], image_in.data[(i+n-1+n*j)]));
              channel3[k1]=hue_transit(channel2[i-n+1+n*j],channel2[i+n*j],channel2[i+n-1+n*j], image_in.data[(i-n+1+n*j)], image_in.data[(i+n-1+n*j)]);
	    else
	      channel3[k1]=hue_transit(channel2[i-n-1+n*j],channel2[i+n*j],channel2[i+n+1+n*j], image_in.data[(i-n-1+n*j)], image_in.data[(i+n+1+n*j)]);
	    k1++;
	  }
	  if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // gruenes Feld1, rot oben
	    channel1[k1] = hue_transit(channel2[(i-n+n*j)],image_in.data[(i+n*j)],channel2[(i+n+n*j)],image_in.data[(i-n+n*j)],image_in.data[(i+n+n*j)]);
	    channel3[k1] = hue_transit(channel2[(i-1+n*j)],image_in.data[(i+n*j)],channel2[(i+1+n*j)],image_in.data[(i-1+n*j)],image_in.data[(i+1+n*j)]);
	    k1++;
	  }
          if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // gruenes Feld 2 blau oben
	    channel1[k1] = hue_transit(channel2[(i-1+n*j)],image_in.data[(i+n*j)],channel2[(i+1+n*j)],image_in.data[(i-1+n*j)],image_in.data[(i+1+n*j)]);
	    channel3[k1] = hue_transit(channel2[(i-n+n*j)],image_in.data[(i+n*j)],channel2[(i+n+n*j)],image_in.data[(i-n+n*j)],image_in.data[(i+n+n*j)]);
	    k1++;;
	  }
	  if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blaues Feld
	    if (dne<=dnw)
              channel1[k1]=hue_transit(channel2[(i-n+1+n*j)],channel2[(i+n*j)],channel2[(i+n-1+n*j)], image_in.data[(i-n+1+n*j)], image_in.data[(i+n-1+n*j)]);
	    else
	      channel1[k1]=hue_transit(channel2[(i-n-1+n*j)],channel2[(i+n*j)],channel2[(i+n+1+n*j)], image_in.data[(i-n-1+n*j)], image_in.data[(i+n+1+n*j)]);
	    k1++;
	  }
	}
      } 
    } 
  }
  
  // Testoutputs
  /*
    printf("Check\n");
    printf("FlagQ=%i\n",flag_q);
    printf("n=%i\n",n);
    printf("m=%i\n",m);
    printf("xoffset=%i\n",xoffset);
    printf("yoffset=%i\n",yoffset);
    printf("k=%i i+n*j=%i\n",k1, i+n*j);
  */

  // write the demosaiced FITS images  
  writeImage(out1, input_image, channel1);
  writeImage(out2, input_image, channel2);
  writeImage(out3, input_image, channel3);
  
  return 0;
}
