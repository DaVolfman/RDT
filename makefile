all: rdtinterface RDTsend RDTrecv
rdtinterface: rdtinterface.c
	gcc -c rdtinterface.c
	ar -cvr librdt.a *.o
RDTsend: RDTsend.c rdtinterface
	gcc -o RDTsend RDTsend.c librdt.a
RDTrecv: RDTrecv.c rdtinterface
	gcc -o RDTrecv RDTrecv.c librdt.a
clean:
	$(RM) RDTsend RDTrecv rdtinterface.o librdt.a
