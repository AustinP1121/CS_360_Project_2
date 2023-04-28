#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

struct recordRequest
{
    float arrivalTime;
    float trackTime;
    float sectorRequest;
    float responseTime;
};


void SortLineData(char * buf)
{
    
}

void ReadFromFile(char *filename)
{
    char lineBuffer[BUFFER_SIZE];

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
            strncat(lineBuffer, &ch, 1);
            printf("%c", ch);
        } while (ch != EOF);

        printf("\n\nBuffer contents:\n%s", lineBuffer);
    }
    
    fclose(f);
}

