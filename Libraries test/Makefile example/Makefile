all: manipulation_mote_struct.o main.c
	gcc main.c manipulation_mote_struct.o -o main
manipulation_mote_struct.o: manipulation_mote_struct.c manipulation_mote_struct.h
	gcc -c manipulation_mote_struct.c
clean:
	rm *.o main *~
