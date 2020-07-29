#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>

#define FALSE 0
#define TRUE 1

int processes_ran = 0; // process counter for thread processing
int *available; // integer array of available amount for each resource
int **allocation; // pointer array of size n_processes*m_resources that defines the currently allocated resources for each process
int **maximum;// pointer of size n_processes*m_resources that defines the maximum demand of resources for each process
int **need; // pointer of size n_processes*m_resources that defines the remaining need of resources for each process
int *safeSeq;// array of size n_processes to store the safe sequence

int main(int argc, char** argv) {

    char *fileName = argv[1];
    FILE* file = fopen(fileName, "r");
    if(!file)
	{
		printf("Error in opening input file...exiting with error code -1\n");
		return -1;
	}

}