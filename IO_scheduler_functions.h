#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFFER_SIZE 512

#pragma region Global Variables and Structs
//variable to keep track of the number of records
int recordCount = 0;

//define the recordRequest struct
typedef struct recordRequest
{
    //arrival time of request
    float arrivalTime;
    //track requested
    float trackRequest;
    //sector requested
    float sectorRequest;
    //total response time for this job
    float responseTime;
} recordRequest;

//list for storing the recordRequest structs
recordRequest recordRequestList[50];

#pragma endregion Global Variables and Structs

#pragma region Utilities
void SortByClosestTrack(recordRequest list[])
{
    //sort the recordRequestList by trackRequest
    //use bubble sort
    int i, j;
    recordRequest temp;

    for (i = 0; i < recordCount - 1; i++)
    {
        for (j = 0; j < recordCount - i - 1; j++)
        {
            if (recordRequestList[j].trackRequest > recordRequestList[j + 1].trackRequest)
            {
                temp = recordRequestList[j];
                recordRequestList[j] = recordRequestList[j + 1];
                recordRequestList[j + 1] = temp;
            }
        }
    }
};

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

    printf("\nAverage: %f \nVariance: %f\nStd. Deviation: %f\nSum: %f\n", meanValue, varianceValue, stdDevValue, sum);
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

    //put data into respective record request
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
#pragma endregion Utilities

#pragma region Algorithms 

void FCFSAlgorithm()
{
    /***Variable definitions***/
    float currentTrack = 0;
    float currentSector = 0;
    float seekTime = 0; 
    float transferTime = 0;
    float sectorTransferTime = 1.2;

    /*the time it takes to rotate around the disk over the number of sectors on 
    the disk: 5/10 = 0.5*/
    float diskRotateTime = 0.5;

    //sort the list according to arrival time
    SortRecordRequestList();

    for (int i = 0; i < recordCount; i++)
    {
        //print the requested track and sector
        printf("\nTrack: %f\nSector: %f\n", recordRequestList[i].trackRequest, recordRequestList[i].sectorRequest);
        printf("Current Track: %f\nCurrent Sector: %f\n", currentTrack, currentSector);

        //the track request is the same as the current track position
        if (recordRequestList[i].trackRequest == currentTrack)
        {
            //the sector request is greater than the current sector position
            if (recordRequestList[i].sectorRequest > currentSector)
            {
                //calculate the transfer time
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((10 - currentSector) - recordRequestList[i].sectorRequest) * diskRotateTime;

                //update the current sector position
                currentSector = recordRequestList[i].sectorRequest + 1;
                //update the response time
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            //the sector request is behind the current sector
            else if (recordRequestList[i].sectorRequest < currentSector)
            {
                //find the number of sectors traveled to get to the requested sector
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((10 - currentSector) + recordRequestList[i].sectorRequest) * diskRotateTime;

                currentSector = recordRequestList[i].sectorRequest + 1;
                currentTrack = recordRequestList[i].trackRequest;

                //update the current sector position
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            else
            {
                currentSector = recordRequestList[i].sectorRequest + 1;
                currentTrack = recordRequestList[i].trackRequest;
                
                //update the response time
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
        }
        //the track request is in another track
        else if(recordRequestList[i].trackRequest > currentTrack)
        {
            //the requested sector is ahead of the current sector
            if (recordRequestList[i].sectorRequest > currentSector)
            {   
                //print current track and sector
                //calculate the seek time
                seekTime = (12 + 0.1 * (recordRequestList[i].trackRequest - currentTrack));

                //calculate the transfer time
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((recordRequestList[i].sectorRequest - currentSector) * diskRotateTime);

                currentTrack = recordRequestList[i].trackRequest;                
                //update the current sector position
                currentSector = recordRequestList[i].sectorRequest + 1;

                //update the current track position
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            //the requested sector is behind the current sector
            else if (recordRequestList[i].sectorRequest < currentSector)
            {
                //calculate the seek time
                seekTime += (12 + 0.1 * (recordRequestList[i].trackRequest - currentTrack)); 

                //calculate the transfer time
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((10 - currentSector) + recordRequestList[i].sectorRequest) * diskRotateTime;
                
                //update the current sector position
                currentSector = recordRequestList[i].sectorRequest + 1;
                //update the current track position
                currentTrack = recordRequestList[i].trackRequest;
                //update the response time
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            else
            {   
                //calculate the seek time
                seekTime += (12 + 0.1 * (recordRequestList[i].trackRequest - currentTrack));

                //update the current track position
                currentTrack = recordRequestList[i].trackRequest;
                //update the response time
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
        }
        //the requested track is less than the current track
        else if(recordRequestList[i].trackRequest < currentTrack)
        {
            //the requested sector is ahead of the current sector
            if (recordRequestList[i].sectorRequest > currentSector)
            {
                //calculate the seek time
                seekTime += (12 + 0.1 * (currentTrack - recordRequestList[i].trackRequest));
                //calculate the transfer time
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((recordRequestList[i].sectorRequest - currentSector) * diskRotateTime);

                //update the current sector position
                currentSector = recordRequestList[i].sectorRequest + 1;
                currentTrack = recordRequestList[i].trackRequest;

                //update the current track position
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            //the requested sector is behind the current sector
            else if (recordRequestList[i].sectorRequest < currentSector)
            {
                //calculate the seek time
                seekTime += (12 + 0.1 * (currentTrack - recordRequestList[i].trackRequest));
                //calculate the transfer time
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((10 - currentSector) + recordRequestList[i].sectorRequest) * diskRotateTime;

                //update the current sector position
                currentSector = recordRequestList[i].sectorRequest + 1;
                //update the current track position
                currentTrack = recordRequestList[i].trackRequest;

                //update the response time
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }  
            else
            {
                seekTime += (12 + 0.1 * (currentTrack - recordRequestList[i].trackRequest));

                transferTime += sectorTransferTime;

                currentSector = recordRequestList[i].sectorRequest + 1;
                currentTrack = recordRequestList[i].trackRequest;

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
        }
        printf("\nSeek time: %f\nTransfer time: %f \nTime: %f\n", seekTime, transferTime, recordRequestList[i].responseTime);
        transferTime = 0, seekTime = 0;
    }

    MathComputations(recordRequestList);
}

void ShortestSeekTimeFirstAlgorithm()
{
    /***Variable definitions***/
    float currentTrack = 0;
    float currentSector = 0;
    float seekTime = 0; 
    float transferTime = 0;
    float sectorTransferTime = 1.2;

    /*the time it takes to rotate around the disk over the number of sectors on 
    the disk: 5/10 = 0.5*/
    float diskRotateTime = 0.5;    

    //sort the list of requests by track
    SortByClosestTrack(recordRequestList);
    
    for (int i = 0; i < recordCount; i++)
    {
        //print the requested track and sector
        printf("\nTrack: %f\nSector: %f\n", recordRequestList[i].trackRequest, recordRequestList[i].sectorRequest);
        printf("Current Track: %f\nCurrent Sector: %f\n", currentTrack, currentSector);

        //the track request is the same as the current track position
        if (recordRequestList[i].trackRequest == currentTrack)
        {
            //the sector request is greater than the current sector position
            if (recordRequestList[i].sectorRequest > currentSector)
            {
                //calculate the transfer time
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((10 - currentSector) - recordRequestList[i].sectorRequest) * diskRotateTime;

                //update the current sector position
                currentSector = recordRequestList[i].sectorRequest + 1;
                //update the response time
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            //the sector request is behind the current sector
            else if (recordRequestList[i].sectorRequest < currentSector)
            {
                //find the number of sectors traveled to get to the requested sector
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((10 - currentSector) + recordRequestList[i].sectorRequest) * diskRotateTime;

                currentSector = recordRequestList[i].sectorRequest + 1;
                currentTrack = recordRequestList[i].trackRequest;

                //update the current sector position
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            else
            {
                currentSector = recordRequestList[i].sectorRequest + 1;
                currentTrack = recordRequestList[i].trackRequest;
                
                //update the response time
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
        }
        //the track request is in another track
        else if(recordRequestList[i].trackRequest > currentTrack)
        {
            //the requested sector is ahead of the current sector
            if (recordRequestList[i].sectorRequest > currentSector)
            {   
                //print current track and sector
                //calculate the seek time
                seekTime = (12 + 0.1 * (recordRequestList[i].trackRequest - currentTrack));

                //calculate the transfer time
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((recordRequestList[i].sectorRequest - currentSector) * diskRotateTime);

                currentTrack = recordRequestList[i].trackRequest;                
                //update the current sector position
                currentSector = recordRequestList[i].sectorRequest + 1;

                //update the current track position
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            //the requested sector is behind the current sector
            else if (recordRequestList[i].sectorRequest < currentSector)
            {
                //calculate the seek time
                seekTime += (12 + 0.1 * (recordRequestList[i].trackRequest - currentTrack)); 

                //calculate the transfer time
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((10 - currentSector) + recordRequestList[i].sectorRequest) * diskRotateTime;
                
                //update the current sector position
                currentSector = recordRequestList[i].sectorRequest + 1;
                //update the current track position
                currentTrack = recordRequestList[i].trackRequest;
                //update the response time
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            else
            {   
                //calculate the seek time
                seekTime += (12 + 0.1 * (recordRequestList[i].trackRequest - currentTrack));

                //update the current track position
                currentTrack = recordRequestList[i].trackRequest;
                //update the response time
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
        }
        //the requested track is less than the current track
        else if(recordRequestList[i].trackRequest < currentTrack)
        {
            //the requested sector is ahead of the current sector
            if (recordRequestList[i].sectorRequest > currentSector)
            {
                //calculate the seek time
                seekTime += (12 + 0.1 * (currentTrack - recordRequestList[i].trackRequest));
                //calculate the transfer time
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((recordRequestList[i].sectorRequest - currentSector) * diskRotateTime);

                //update the current sector position
                currentSector = recordRequestList[i].sectorRequest + 1;
                currentTrack = recordRequestList[i].trackRequest;

                //update the current track position
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
            //the requested sector is behind the current sector
            else if (recordRequestList[i].sectorRequest < currentSector)
            {
                //calculate the seek time
                seekTime += (12 + 0.1 * (currentTrack - recordRequestList[i].trackRequest));
                //calculate the transfer time
                transferTime += sectorTransferTime;
                //calculate the disk rotation time and add to total transfer time
                transferTime += ((10 - currentSector) + recordRequestList[i].sectorRequest) * diskRotateTime;

                //update the current sector position
                currentSector = recordRequestList[i].sectorRequest + 1;
                //update the current track position
                currentTrack = recordRequestList[i].trackRequest;

                //update the response time
                recordRequestList[i].responseTime += (seekTime + transferTime);
            }  
            else
            {
                seekTime += (12 + 0.1 * (currentTrack - recordRequestList[i].trackRequest));

                transferTime += sectorTransferTime;

                currentSector = recordRequestList[i].sectorRequest + 1;
                currentTrack = recordRequestList[i].trackRequest;

                recordRequestList[i].responseTime += (seekTime + transferTime);
            }
        }
        printf("\nSeek time: %f\nTransfer time: %f \nTime: %f\n", seekTime, transferTime, recordRequestList[i].responseTime);
        transferTime = 0, seekTime = 0;
    }

    MathComputations(recordRequestList);
}

#pragma endregion Algorithms