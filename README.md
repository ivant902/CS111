# You Spin Me Round Robin

This is a linked-list implementation of Round Robin scheduling given a quantum length and will return the average response and wait times. 

## Building

```shell
make
```

## Running

Ensure that there is a processes.txt file in the same directory. Processes.txt should be in the format:

4 

1, 0, 7

2, 2, 4

3, 4, 1

4, 5, 4

The first line is the total number of processes, with each following entry being a new process. The three numbers in each entry are the PID, arrival time, and burst time, respectively. Ensure that the numbers are comma separated and remember to end the file with a newline. (there should not be newlines in between each entry or between the first line and the first entry)
```shell
./rr processes.txt quantum_length
```
Quantum_length should be a number greater than 0 and represents the number of cycles each process will run before giving up the CPU for the next process in the queue. 


## Cleaning up
To clean up, run 'make clean' to remove binary and object files. 
```shell
make clean
```
