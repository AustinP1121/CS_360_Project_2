#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

//define type for the recordRequest struct
typedef struct recordRequest recordRequest;

//variable to keep track of the number of records
int recordCount = 0;

//list for storing the recordRequest structs
recordRequest recordRequestList[50];

//define the recordRequest struct
struct recordRequest
{
    float arrivalTime;
    float trackTime;
    float sectorRequest;
    float responseTime;
};

void SortLineData(char * buf)
{
    /*sort the data from the line using the space character as a delimiter*/

    int *recordCountPtr = &recordCount;

    int i = 0;
    char value;

    recordRequest record;
    char *token = strtok(buf, " ");

    record.arrivalTime = atof(token);

    while (token != NULL)
    {
        token = strtok(NULL, " ");

        if (i == 0)
        {
            record.trackTime = atof(token);
        }
        else if (i == 1)
        {
            record.sectorRequest = atof(token);
        }    

        i++;   
    }

    //put data into respective record
    recordRequestList[recordCount].arrivalTime = record.arrivalTime;
    recordRequestList[recordCount].trackTime = record.trackTime;
    recordRequestList[recordCount].sectorRequest = record.sectorRequest;
    //increment the recordCount
    (*recordCountPtr)++; 

}

void ReadFromFile(char *filename)
{
    //char buffer for storing the line data
    char lineBuffer[BUFFER_SIZE];

    //using for each character read from the file
    char ch;
    //file pointer
    FILE *f; 

    //open the file in read mode
    f = fopen(filename, "r");
    
    //check if the file exists
    if (f == NULL)
    {
        printf("\nThe file path is incorrect or does not exist...\n");
    }  
    else
    {
        //read the file line by line
        while(fgets(lineBuffer, BUFFER_SIZE, f))
        
        {
            SortLineData(lineBuffer);
        }
    }

    for (int i = 0; i < 10; i++)
    {
        printf("\nArrival: %f Track: %f Sector: %f", recordRequestList[i].arrivalTime, recordRequestList[i].trackTime, recordRequestList[i].sectorRequest);
    }
        
    //close file
    fclose(f);
}

void FCFSAlgorithm()
{
    for (int i = 0; i < recordCount; i++)
    {
        
    }
    
}

