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

int n_processes = 0; 
int m_resources = 0;

//Functions
void get_resource_process_value(FILE* file); // Function to get the number of processes and resources from the test file
void read_file(FILE* file);


int main(int argc, char** argv) {

    char *fileName = argv[1];
    FILE* file = fopen(fileName, "r");
    if(!file)
	{
		printf("Error in opening input file...exiting with error code -1\n");
		return -1;
	}

	// Retrieving the value for processes and resources dynamically
    get_resource_process_value(file);

	// reads the contents of the file
	read_file(file);

}

void get_resource_process_value(FILE* file){

    char line[100]; // each line from the sample file
    char *each_line=NULL; // to store each line
    char *line_token=NULL; // to store elements of each line

    while( fgets(line,sizeof(line),file)){
        each_line = strtok(line, "\r\n" ); // splitting the line by  new character
        m_resources=0;
        line_token = strtok(each_line, "," ); // splitting the contents of each line by comma
        
        while (line_token!=NULL){ 
            line_token = strtok(NULL, "," );
            m_resources++;
        }
        each_line = strtok(NULL, "\r\n" );
        n_processes++;
    }
    
}

void read_file(FILE* file){
    char line[100]; // each line from the sample file
    char *each_line=NULL; // to store each line
    char *line_token=NULL; // to store elements of each line
    int process = 0; // counter for number of processes
    int resource = 0; // counter for number of resource in each process
    while( fgets(line,sizeof(line),file)){
        each_line = strtok(line, "\r\n" ); // splitting the line by  new character
        resource=0;
        line_token = strtok(each_line, "," ); // splitting the contents of each line by comma
        
        while (line_token!=NULL){
            // initialize maximum
            maximum[process][resource] = atoi(line_token);  // converting the content char to integer to store in the maximum integer array
            line_token = strtok(NULL, "," );
            resource++;
            
        }
        each_line = strtok(NULL, "\r\n" );
        process++;
    }

}