#ifndef STUFF_H
#define STUFF_H
#define TRUE 1;
#define FALSE 0;

int FILEMAX = 4096;
double const PI = 3.1415926535;


//************************************************************
float mean_of_array(float *data, long dim)
{
  long i, n;
  float avg;
  
  avg = 0.;
  n = 0;
  
  for (i = 0; i < dim; i++) {
    avg += data[i];
    n++;
  }
  
  if (n != 0.) {
    return (avg / (float) n);
  }
  else return (0.);
}


//************************************************************
float rms_of_array(float *data, long dim)
{
  long i, n;
  float avg, sdev;
  
  n = 0;
  avg = mean_of_array(data, dim);
  
  sdev = 0.;
  for (i = 0; i < dim; i++)  {
    sdev += (avg - data[i]) * (avg - data[i]);
    n++;
  }
  
  if (n > 1) {
    return sqrt(sdev / (float) (n - 1));
  }
  else return (0.);
}


//********************************************************
void matrix_mult(double a11, double a12, double a21, double a22, 
		 double *b11_ptr, double *b12_ptr, double *b21_ptr, 
		 double *b22_ptr)
{
  double c11, c12, c21, c22;

  c11 = a11 * *b11_ptr + a12 * *b21_ptr;
  c12 = a11 * *b12_ptr + a12 * *b22_ptr;
  c21 = a21 * *b11_ptr + a22 * *b21_ptr;
  c22 = a21 * *b12_ptr + a22 * *b22_ptr;

  *b11_ptr = c11;
  *b12_ptr = c12;
  *b21_ptr = c21;
  *b22_ptr = c22;
}


//************************************************
// remove leading and trailing whitespace
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}


//*******************************************************
char *RemoveChars( char *src, char *key )
{
  char *dest;
  size_t len_src;
  size_t len_key;
  int found;
  int i;
  int j;
  int k;
  
  i = 0;
  j = 0;
  k = 0;
  len_src = 0;
  len_key = 0;
  dest = NULL;
  
  len_src = strlen( src );
  len_key = strlen( key );
  
  dest = (char*) malloc(sizeof(char) * len_src + 1 );
  
  if ( NULL == dest ) {
    printf("Unable to allocate memory\n");
    exit (1);
  }
  
  memset( dest, 0x00, sizeof( char ) * len_src + 1 );
  
  for ( i = 0; i < len_src; i++ ) {
    found = FALSE;
    for ( j = 0; j < len_key; j++ ) {
      if ( src[i] == key[j] ) found = TRUE;
    }
    
    if (found == 0) {
      dest[k] = src[i];
      k++;
    }
  }
  
  return (dest);
}

//*****************************************************************

float min_array(float *data, long dim)
{
  long i;
  float min;
  
  min = data[0];
  
  for (i = 0; i < dim; i++)  {
    if (data[i] < min) min = data[i];
  }
  
  return (min);
}

//*****************************************************************

double min_array_dbl(double *data, long dim)
{
  long i;
  double min;
  
  min = data[0];
  
  for (i = 0; i < dim; i++)  {
    if (data[i] < min) min = data[i];
  }
  
  return (min);
}

//*****************************************************************

float max_array(float *data, long dim)
{
  long i;
  float max;
  
  max = data[0];
  
  for (i = 0; i < dim; i++)  {
    if (data[i] > max) max = data[i];
  }
  
  return (max);
}

#endif
