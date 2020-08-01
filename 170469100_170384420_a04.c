/*
CP386 Assignment 4
Partner 1 - Ravneet Kaur
ID # - 170469100
Github ID - rav-kaur

Partner 2 - Marvi Shaikh
ID # - 170384420
Date - July 29, 2020
Github ID - marvi99

Github URL - 

*/

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

pthread_mutex_t lockResources; // mutex lock
pthread_cond_t condition;

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

int get_safeSeq();
void* thread_process(void* );


int main(int argc, char** argv) {
    
    int commands_given = 0;
    for (int i = 0; i < argc; ++i){
        commands_given++;
    }
    if (commands_given!=6){
        printf("\nError: Missing arguments: \n Required: .c sample4_in.txt available resources\n\n");
    } else{
    


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
        print_maximum();
        
        // Function for all possible user commands:
        // Exit == end program
        user_commands();
    }

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
            if (get_safeSeq()==-1){
                printf("Unsafe Sequence\n");
                exit(-1);
            }
            printf("\n Safe Sequence < ");
            for (int i =0;i<n_processes;i++){
                printf("%d ",safeSeq[i]);
            }
            printf(">");
            end=1;
            sleep(1);
            pthread_t threads[n_processes];
            int threadsNo[n_processes];
            for(int t=0;t<n_processes;t++)
            {
                threadsNo[t] = t;
            }
            for(int i=0;i<n_processes;i++)
            {
                // Creating threads for each process and running the thread_process functiin
                pthread_create(&threads[i],NULL,&thread_process,(void*)(&threadsNo[i]));
            }
            for(int i=0; i<n_processes; i++)
            {
                // Joining threads to run in sequence
                pthread_join(threads[i], NULL);
            }
            // If user enters "Exit" command , the function terminates
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
            return -1; //  Request Denies
        }   
        else continue;
    }
    for (int x=0;x<m_resources;x++)
    {
        if (request[x]>available[x]) //  if request exceeds the number of available resources, the process enters wait state where it waits until available resources increases
        {
            wait_state = TRUE; // Process enters wait state
            break;
        }
        else continue;

    }
    if (wait_state==TRUE){
        return -1; // Request Denied
    }
    for (int i=0;i<m_resources;i++){
        available[i] = available[i] - request[i]; //  decrease available resources
        allocation[process_num][i] = allocation[process_num][i]+request[i]; // increase allocation
        need[process_num][i] = need[process_num][i] - request[i]; // decrease need
    }
    return 0;
}

int get_safeSeq(){
    // get safe sequence
    int temp_resources[m_resources];
    for(int i=0; i<m_resources; i++) {
        temp_resources[i] = available[i];
    }
    int finished[n_processes];
    for(int i=0; i<n_processes; i++) finished[i] = FALSE;
    int process_finished=0;

    // looping while all processes are in the finished array / All processes are visited
    while(process_finished < n_processes) {
        int safe = FALSE;
        for(int i=0; i<n_processes; i++) {
            if(!finished[i]) {
                int possible = TRUE;
                for(int j=0; j<m_resources; j++)
                {
                    if(need[i][j] > temp_resources[j])
                    {
                        possible = FALSE;
                        break;
                    }
                }
                if(possible) 
                {
                    for(int j=0; j<m_resources; j++){
                        temp_resources[j] += allocation[i][j];
                    }
                    safeSeq[process_finished] = i;
                    finished[i] = TRUE;
                    ++process_finished;
                    safe = TRUE;
                }
            }
        }
        if(!safe){
            return -1; // No Safe Sequence found
        }
        
    }
    return 0; // safe sequence found}
}

void* thread_process(void *arg) {

        int p = *((int *) arg);

	// lock resources
        pthread_mutex_lock(&lockResources);

        // condition check
        while(p != safeSeq[processes_ran]){
            pthread_cond_wait(&condition, &lockResources);
        }
                
	// process
        printf("\n--> Process %d", p);
        printf("\n\tAllocated : ");
        for(int i=0; i<m_resources; i++){
            printf("%3d", allocation[p][i]);
        }
                

        printf("\n\tNeeded    : ");
        for(int i=0; i<m_resources; i++){
            printf("%3d", need[p][i]);
        }
        
        printf("\n\tAvailable : ");
        for(int i=0; i<m_resources; i++){
            printf("%3d", available[i]);
        }
    
        printf("\n"); sleep(1);

        printf("\tThread has started...");
        printf("\n"); 
        sleep(2); // process code
        printf("\tThread has finished...");
        printf("\n"); 
        sleep(1);
        printf("\tThread is Releasing Resources...");
        printf("\n"); 
        sleep(1);

        // updating the available array 
        for(int i=0; i<m_resources; i++){
            available[i] += allocation[p][i];
        }

        printf("\n\tNow Available : ");
        for(int i=0; i<m_resources; i++){
            printf("%3d", available[i]);
        }
        printf("\n\n");
        sleep(1);

	// condition broadcast
        processes_ran++;
        pthread_cond_broadcast(&condition);
        pthread_mutex_unlock(&lockResources); // unlocking threads
	    pthread_exit(NULL);
}