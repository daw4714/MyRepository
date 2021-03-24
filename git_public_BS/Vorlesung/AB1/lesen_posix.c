#include <fcntl.h>
#include <stdio.h>

int main()
{
    char buf [512];
    int fd = open( "myfile.txt", O_RDONLY );
    if( fd != -1 ) {
        int n = read( fd, buf, 512 );
        buf[n]=0;
        printf("Inhalt: %s", buf);
        close( fd );
    } else { printf("File not found.\n");}
}
