# Project Title: Banker's Algorithm

### Motivation
CP 386 - Assignment 4

### Features
This program is a multithreaded implementation of the banker's algorithm. This algorithm is used as a resource allocation and deadlock avoidance algorithm. It tests for safety by simulating the allocation for predetermined maximum amount of resources. It checks the 'safe state' of each process before deciding to satisfy the resource request of the user or not.

Our code specifically dynamically allocates the array for number of resources and processes after reading the user's input and sample file containing the maximum number of resources. This means it is not restricted to 4 resources x 5 processes (as shown in our example).

### Installation
```bash
# compile
gcc -Werror -Wall -g -std=gnu99 -o 170469100_170384420_a04 *.c -lrt -lpthread

# execute
./170469100_170384420_a04
```

### Code Example
```
./170469100_170384420_a04 sample4_in.txt 10 7 7 8
```
#### Parameters
```
Executable - ./170469100_170384420_a04
Sample File (maximum resources available) - sample4_in.txt
Available Resources - 10 7 7 8
```

### Input
```
./170469100_170384420_a04 sample4_in.txt 10 7 7 8
Maximum resources from file: 
6 4 7 3 
4 2 3 2 
2 5 3 3 
6 3 3 2 
5 6 7 5 
Enter Command ('Exit' to end): RQ 0 3 1 2 1
Request is satisfied. 

Enter Command ('Exit' to end): RQ 1 1 1 1 1
Request is satisfied. 

Enter Command ('Exit' to end): RQ 2 5 5 5 5
Request is not satisfied. 

Enter Command ('Exit' to end): RQ 2 0 3 0 1
Request is satisfied. 

Enter Command ('Exit' to end): RQ 3 0 0 0 2
Request is satisfied. 

Enter Command ('Exit' to end): RQ 4 3 0 0 0
Request is satisfied. 
```

### Output
```
Enter Command ('Exit' to end): *

* Current state of Available, Maximum, Allocation, Need arrays
Available: 
3 2 4 3 
Maximum: 
6 4 7 3 
4 2 3 2 
2 5 3 3 
6 3 3 2 
5 6 7 5 
Allocation: 
3 1 2 1 
1 1 1 1 
0 3 0 1 
0 0 0 2 
3 0 0 0 
Need: 
3 3 5 2 
3 1 2 1 
2 2 3 2 
6 3 3 0 
2 6 7 5 
```
```
Enter Command ('Exit' to end): Run

 Safe Sequence < 1 2 0 3 4 >
--> Process 1
        Allocated :   1  1  1  1
        Needed    :   3  1  2  1
        Available :   3  2  4  3
        Thread has started...
        Thread has finished...
        Thread is Releasing Resources...

        Now Available :   4  3  5  4


--> Process 2
        Allocated :   0  3  0  1
        Needed    :   2  2  3  2
        Available :   4  3  5  4
        Thread has started...
        Thread has finished...
        Thread is Releasing Resources...

        Now Available :   4  6  5  5


--> Process 0
        Allocated :   3  1  2  1
        Needed    :   3  3  5  2
        Available :   4  6  5  5
        Thread has started...
        Thread has finished...
        Thread is Releasing Resources...

        Now Available :   7  7  7  6


--> Process 3
        Allocated :   0  0  0  2
        Needed    :   6  3  3  0
        Available :   7  7  7  6
        Thread has started...
        Thread has finished...
        Thread is Releasing Resources...

        Now Available :   7  7  7  8


--> Process 4
        Allocated :   3  0  0  0
        Needed    :   2  6  7  5
        Available :   7  7  7  8
        Thread has started...
        Thread has finished...
        Thread is Releasing Resources...

        Now Available :  10  7  7  8
```

### Authors
```
Marvi Shaikh - 170384420
Ravneet Kaur - 170469100
```

### Individual Contribution
#### Marvi Shaikh
```
get_resource_process_value
print_available
print_maximum
print_allocation
print_need
resource_request 
get_safeSeq
```

#### Ravneet Kaur
```
resource_release
read_file
void user_commands();
thread_process
```

### Credits
Textbook: Operating System Concepts, Tenth Edition

### License




