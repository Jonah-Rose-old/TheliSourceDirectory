#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int main() {
  long nprocs = 1;

  nprocs = sysconf(_SC_NPROCESSORS_ONLN);
  
  if (nprocs < 1) nprocs = 1;
  
  printf("%ld\n", nprocs);

  return (0);
}
