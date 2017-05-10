all: talkback talker rdtinterface
talkback: talkback.c
	gcc talkback.c -o talkback
talker: talker.c
	gcc talker.c -o talker
rdtinterface: rdtinterface.c
	gcc -c rdtinterface.c
	ar -cvr librdt.a *.o
clean:
	$(RM) talkback talker rdtinterface.o librdt.a
