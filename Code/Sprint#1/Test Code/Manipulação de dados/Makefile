all: manipulation_mote_struct.o manipulate_data.c
	gcc manipulate_data.c manipulation_mote_struct.o -o manipulate_data
manipulation_mote_struct.o: manipulation_mote_struct.c manipulation_mote_struct.h
	gcc -c manipulation_mote_struct.c
clean:
	rm *.o manipulate_data *~
