all: talkback talker rdtinterface RDTsend RDTrecv
talkback: talkback.c
	gcc talkback.c -o talkback
talker: talker.c
	gcc talker.c -o talker
rdtinterface: rdtinterface.c
	gcc -c rdtinterface.c
	ar -cvr librdt.a *.o
RDTsend: RDTsend.c rdtinterface
	gcc -o RDTsend RDTsend.c librdt.a
RDTrecv: RDTrecv.c rdtinterface
	gcc -o RDTrecv RDTrecv.c librdt.a
clean:
	$(RM) talkback talker rdtinterface.o librdt.a
