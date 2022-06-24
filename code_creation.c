#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* args[])
{
    FILE* fd;
    fd = fopen(args[1], "w+");
    for(int i = 0; i <= 255; i++)
    {
        fprintf(fd, "    /*this is the line %x */NULL,\n", i);
    }
    fclose(fd);
    return 0;
}