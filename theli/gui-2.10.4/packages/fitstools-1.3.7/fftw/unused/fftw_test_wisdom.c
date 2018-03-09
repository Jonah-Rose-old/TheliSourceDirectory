#include <stdio.h>
#include <fftw3.h>

int main()
{
  FILE *file;
  int wisdom;

  file = fopen("/home/mischa/.fftw/wisdom3.txt", "r");
  wisdom = fftw_import_wisdom_from_file(file);
  printf("Reading from file: %d\n", wisdom);
  fclose(file);

  /* reading from absolute path */
  wisdom = fftw_import_wisdom_from_filename("/home/mischa/.fftw/wisdom3.txt");
  printf("Reading from abs path: %d\n", wisdom);

  /* reading from whereever the program is launched (I assume) */
  wisdom = fftw_import_wisdom_from_filename("wisdom3.txt");
  printf("Reading from rel path: %d\n", wisdom);
  
  return 0;
}
