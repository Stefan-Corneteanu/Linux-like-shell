Linux like shell

A college project aimed at creating a Linux like shell. This project's primary goals are C programming, process forking, pipeing, and concurrent programming

How to run (in Linux only):
1) In the Linux terminal, go to the folder where the myshell.c file is
2) Run the following command: gcc myshell.c -lreadline. It will create a new file, a.out
3) By typing ./a.out, you will enter the shell

Brief description of the functions:
1) exit: exits the current shell, returns to the standard Linux shell
2) cd: change directory, a function taken from Linux, it changes the current working directory
3) help: lists available functions, inspired from the actual Linux help function
4) hello: greets current user
5) ls: lists the files/folders in the current working directory, a function taken from Linux
6) pwd: a function taken from Linux used to print the current working directory
7) czr: given an input text the shell encrypts or decrypts it using Caesar's algorithm
8) randfunc: calls a random function out of the other ones
9) print: an attempt at implementing Linux's echo function, outputs a text at the terminal, takes into account escaped characters, single and double quotes
10) countdown: takes as parameter a number and counts down to 0