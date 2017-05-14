Readme for Project RDT 3.0

Commands:
To build the project:
	make
To delete compiled files:
	make clean
To run the receiving test server:
	./RDTrecv <port number>
To run the sending test client:
	./RDTsend <message file path> <destination IP address> <destination port number> <rate to drop packets in mils> <rate to corrupt packets in mils>

Files:
librdt.a	- static library for RDT the primary product of the project
rdtinterface.h	- Header with function prototypes for librdt
rdtinterface.c	- Source file for librdt
rdtheader.h	- Defines the packet format for librdt
RDTrecv		- Test server endlessly outputs all messages sent to it to stderr.
RDTsend		- Send a file as a message to an RDT receiver.
RDTrecv.c, RDTsend.c - source files for the above
makefile	- self explanatory
short.msg	- contains abcdefghijklmnopqrstuvwxyz
long.msg	- contains the above a large number of times
CS537 RDT writeup.pdf - the writeup for this project
CS537 RDT writeup.tex - source file for the writeup