#include "IO_scheduler_functions.h"

int main(int argc, char *argv[])
{
    ReadFromFile("./recordRequests.txt");
    FCFSAlgorithm();
    
}