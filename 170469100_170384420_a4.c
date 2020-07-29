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
int resource_request(int process_num, int request[]);
int resource_release(int process_num,int release[]);
void read_file(FILE* file);
void get_resource_process_value(FILE* file);
void user_commands();

void print_available();
void print_maximum();
void print_allocation();
void print_need();


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

	//Dynamically Allocate the Need, Allocation, Available, Safe Sequence arrays
    available = (int *)malloc(m_resources * sizeof(*available));
    safeSeq = (int *)malloc(n_processes * sizeof(*safeSeq));


    // Allocating the availabel values to the available aarray 
    int j = 2;
    for (int i=0;i<m_resources;i++){
        available[i] = atoi(argv[j]);
        j++;
    }

    allocation = (int **)malloc(n_processes * sizeof(*allocation));
    for(int i=0; i<n_processes; i++){
        allocation[i] = (int *)malloc(m_resources * sizeof(**allocation));
    }
    
    // Initializing the allocation array to 0s
    for (int i=0;i<n_processes;i++){
        for (int j=0;j<m_resources;j++){
            allocation[i][j] = 0;
        }
    }

    maximum = (int **)malloc(n_processes * sizeof(*maximum));
    for(int i=0; i<n_processes; i++){
        maximum[i] = (int *)malloc(m_resources * sizeof(**maximum));
    }

   need = (int **)malloc(n_processes * sizeof(*need));
    for(int i=0; i<n_processes; i++){
        need[i] = (int *)malloc(m_resources * sizeof(**need));
    }
    
    // Moving the cursor back to the starting of the file
    fseek(file,0,SEEK_SET);
	// reads the contents of the file
	read_file(file);

	// initializing need matrix
    for (int p=0;p<n_processes;p++){
        for (int r=0;r<m_resources;r++){
            need[p][r] = maximum[p][r]-allocation[p][r];
        }
    }

	printf("Maximum resources from file: \n");
	// Function for all possible user commands:
    // Exit == end program
    user_commands();

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

void print_available(){
    for (int i = 0; i<m_resources; i++){
        printf("%d ", available[i]);
    }
    printf("\n");
}

void print_maximum(){
    for (int i = 0; i< n_processes; i++){
        for (int j = 0; j < m_resources; j++){
            printf("%d ", maximum[i][j]);
        }
        printf("\n");
    }
}

void print_allocation(){
    for (int i = 0; i< n_processes; i++){
        for (int j = 0; j < m_resources; j++){
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }
}

void print_need(){
    for (int i = 0; i< n_processes; i++){
        for (int j = 0; j < m_resources; j++){
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
}

void user_commands(){
    int counter = 0;
    int end = 0;
    while (end!=1) {
        printf("Enter Command ('Exit' to end): ");
        char str[20]; 
        scanf("%[^\n]%*c", str);
        
        char *ptr = strtok(str, " ");
        int count = 0;
        char command[5];
        int process_num;
        int temp_resources[m_resources];
        int i = 0;
        while(ptr != NULL)
        {
            if (count == 0){
                strcpy(command, ptr);
            } else if (count == 1){
                process_num = atoi(ptr);
            } else {
                temp_resources[i] = atoi(ptr);
                i++;
            }
            ptr = strtok(NULL, " ");
            count++;
        }

        if (strcmp(command, "RQ") == 0){
            if (resource_request(process_num, temp_resources) == 0){
                printf("Request is satisfied. \n\n");
                continue;
            } else{
                printf("Request is not satisfied. \n\n");
                continue;
            }
            
        } else if (strcmp(command, "RL") == 0){
            int i = resource_release(process_num, temp_resources);
            if (i==0){
                printf("Release Request is satisfied. \n Resources have been released\n");
            } else{
                printf("Release Request is denied. No enough allocated resources\n");
            }

        }else if (strcmp(command, "*") == 0){
           
            printf("\n* Current state of Available, Maximum, Allocation, Need arrays\n");
            printf("Available: \n");
            print_available();
            printf("Maximum: \n");
            print_maximum();
            printf("Allocation: \n");
            print_allocation();
            printf("Need: \n");
            print_need();

        } else if (strcmp(command, "Run")==0){ 
            
        } else if (strcmp(command, "Exit")==0){
            break;
        }
    }
}



int resource_release(int process_num,int release[])
{
    // Update the arrays
    for(int i=0;i<m_resources;i++){
        //releasing resources means adding them back to available array
        available[i]+=release[i];
        //subtracting from allocated arrays since it will be loosing resources
        allocation[process_num][i]-=release[i];
        //adding to the need array since we will be needing more resources now
        need[process_num][i]+=release[i];
        if (available[i]>need[process_num][i]){
            available[i]-=need[process_num][i]+maximum[process_num][i];
            need[process_num][i] =0;
        }

        // if allocation doesnt have enough resources to release then it will deny the request
        if (allocation[process_num][i]<0){
            return -1;

            // go back to previous stage of array values since the request is denied now

            for(int x=0;x<i;x++){
                available[i] -= release[i];
                allocation[process_num][i] += release[i];
                need[process_num][i] -= release[i];
            }
            return 0;

        }
    }
}


int resource_request(int process_num, int request[]){
    
    int safe_state = TRUE;
    int wait_state = FALSE;

    for (int x=0;x<m_resources;x++)
    {
        if (request[x]>need[process_num][x])
        {            
            safe_state=FALSE; // you can't request/allocate resources more than its needed and hence it enters unsafe sequence
            return -1;
        }   
        else continue;
    }
    for (int x=0;x<m_resources;x++)
    {
        if (request[x]>available[x])
        {
            wait_state = TRUE;
            break;
        }
        else continue;

    }
    if (wait_state==TRUE){
        return -1;
    }
    for (int i=0;i<m_resources;i++){
        available[i] = available[i] - request[i];
        allocation[process_num][i] = allocation[process_num][i]+request[i];
        need[process_num][i] = need[process_num][i] - request[i];
    }
    return 0;
}