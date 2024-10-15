## UID: 123456789

## Pipe Up

One sentence description

## Building

Enter the command "make" while in the directory including the .c and Makefile.

## Running

Running
>./pipe ls cat
will output the same as 
>ls | cat 
```
Output: 
Makefile
pipe
pipe.c
pipe.o
__pycache__
README.md
test_lab1.py
```
Running 
>./pipe ls cat wc
will output the same as 
>ls | cat | wc 
```
Output:
7 7 63 
```
(outputs will vary depending on local/host system)
## Cleaning up
To clean up the program, run
>make clean
