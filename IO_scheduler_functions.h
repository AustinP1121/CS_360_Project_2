#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

void ReadFromFile(char *filename);

void ReadFromFile(char *filename)
{
    char buffer[BUFFER_SIZE+1];

    char ch;
    FILE *f; 

    f = fopen(filename, "r");
    
    if (f == NULL)
    {
        printf("\nThe file path is incorrect or does not exist...\n");
    }
    else
    {
        do
        {
            ch = fgetc(f);
            strncat(buffer, &ch, 1);
            printf("%c", ch);
        } while (ch != EOF);

        printf("\n%s\n", buffer);
    }
    
    fclose(f);
}

