#include <iostream>
#include <fftw3.h>
//#include "fftw++.h"

using namespace std;

int main()
{

  cout << fftw_import_wisdom_from_filename("/home/mischa/.fftw/wisdom3.txt") << endl;
  
  return 0;
}
