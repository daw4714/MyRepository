#include <stdio.h>
#include <string.h>
int main()
{
   FILE *fp;
   char buffer[20];
   /* Open file for reading */
   fp = fopen("myfile.txt", "r");
   /* Read and display data */
   if( fp != NULL ) {
	fread(buffer, 20, 1, fp);
	printf("%s\n", buffer);
	fclose(fp);
   } else printf("File not found.\n");
   return(0);
}


