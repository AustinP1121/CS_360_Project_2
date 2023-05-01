#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFFER_SIZE 512

//define type for the recordRequest struct
typedef struct recordRequest recordRequest;

//variable to keep track of the number of records
int recordCount = 0;

//define the recordRequest struct
struct recordRequest
{
    //arrival time of request
    float arrivalTime;
    //track requested
    float trackRequest;
    //sector requested
    float sectorRequest;
    //total response time for this job
    float responseTime;
};

//list for storing the recordRequest structs
recordRequest recordRequestList[50];

void MathComputations(recordRequest list[])
{
    float meanValue = 0;
    float varianceValue = 0;
    float sum = 0;
    float stdDevValue= 0;

    for (int recordIdx = 0; recordIdx < recordCount; recordIdx++)
    {
        sum += list[recordIdx].responseTime;
        varianceValue += pow(list[recordIdx].responseTime, 2); 
    }
    
    meanValue = sum / recordCount;
    varianceValue = (varianceValue / recordCount) - pow(meanValue, 2);
    stdDevValue = sqrtf(varianceValue);

    printf("\nAverage: %f \nVariance: %f\nStd. Deviation: %f\n", meanValue, varianceValue, stdDevValue);
}

void SortRecordRequestList()
{
    //sort the recordRequestList by arrivalTime
    //use bubble sort
    int i, j;
    recordRequest temp;

    for (i = 0; i < recordCount - 1; i++)
    {
        for (j = 0; j < recordCount - i - 1; j++)
        {
            if (recordRequestList[j].arrivalTime > recordRequestList[j + 1].arrivalTime)
            {
                temp = recordRequestList[j];
                recordRequestList[j] = recordRequestList[j + 1];
                recordRequestList[j + 1] = temp;
            }
        }
    }
}

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
            record.trackRequest = atof(token);
        }
        else if (i == 1)
        {
            record.sectorRequest = atof(token);
        }    

        i++;   
    }

    //put data into respective record
    recordRequestList[recordCount].arrivalTime = record.arrivalTime;
    recordRequestList[recordCount].trackRequest = record.trackRequest;
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
        
    //close file
    fclose(f);
}

void FCFSAlgorithm()
{
    int currentTrack = 0;
    int currentSector = 0;
    float seekTime = 0; 
    float transferTime = 0;
    float sectorTransferTime = 0.5;  

    SortRecordRequestList();

    for (int i = 0; i < recordCount; i++)
    {
        transferTime = 0, seekTime = 0;
        if (recordRequestList[i].trackRequest == currentTrack)
        {
            if (recordRequestList[i].sectorRequest > currentSector)
            {
                //include if the track is the same?
                seekTime += 12;

                transferTime += ((recordRequestList[i].sectorRequest - currentSector) * sectorTransferTime);

                currentSector = recordRequestList[i].sectorRequest;

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            else if (recordRequestList[i].sectorRequest < currentSector)
            {
                //include if the track is the same? 
                seekTime += 12; 

                transferTime += ((currentSector - recordRequestList[i].sectorRequest) * sectorTransferTime);

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            else
            {
                seekTime += 12;

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
        }
        else if(recordRequestList[i].trackRequest > currentTrack)
        {
            if (recordRequestList[i].sectorRequest > currentSector)
            {
                seekTime += (12 + 0.1 * (recordRequestList[i].trackRequest - currentTrack));

                transferTime += ((recordRequestList[i].sectorRequest - currentSector) * sectorTransferTime);

                currentSector = recordRequestList[i].sectorRequest;

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            else if (recordRequestList[i].sectorRequest < currentSector)
            {
                seekTime += (12 + 0.1 * (recordRequestList[i].trackRequest - currentTrack)); 

                transferTime += ((currentSector - recordRequestList[i].sectorRequest) * sectorTransferTime);

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            else
            {
                seekTime += (12 + 0.1 * (recordRequestList[i].trackRequest - currentTrack));

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
        }
        else if(recordRequestList[i].trackRequest < currentTrack)
        {
            if (recordRequestList[i].sectorRequest > currentSector)
            {
                seekTime += (12 + 0.1 * (currentTrack - recordRequestList[i].trackRequest));

                transferTime += ((recordRequestList[i].sectorRequest - currentSector) * sectorTransferTime);

                currentSector = recordRequestList[i].sectorRequest;

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            else if (recordRequestList[i].sectorRequest < currentSector)
            {
                seekTime += (12 + 0.1 * (currentTrack - recordRequestList[i].trackRequest));

                transferTime += ((currentSector - recordRequestList[i].sectorRequest) * sectorTransferTime);

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }  
            else
            {
                seekTime += (12 + 0.1 * (currentTrack - recordRequestList[i].trackRequest));

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
        }
    }

    printf("\nSeek time total: %f,\nTransfer time total: %f", seekTime, transferTime);

    MathComputations(recordRequestList);
}

