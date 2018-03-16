#include <stdio.h>

int main( int argc, char* argv[] ) {
   FILE *fd;
   int c;
   int i;

   if( argc != 2 ) {
      printf( "usage: hexdump > filename\n" );
      return( 1 );
   }

   fd = fopen( argv[1], "rb" );
   if( fd == NULL ) {
      printf( "could not open %s\n", argv[1] );
      return( 1 );
   }

   printf( "const unsigned char image[] = {\n" );

   i = 0;

   c = fgetc( fd );
   while( !feof(fd) ) {
      printf( "0x%02X", (unsigned char)c );
      i++;
      c = fgetc( fd );

      if( !feof(fd) ) {
         printf( "," );
      }

      if( i%15 == 0 ) {
         printf("\n");
      }
   }

   printf( " };\n" );
   printf( "const unsigned int len = %d;\n", i );
   fclose( fd );
   return( 0 );
}

