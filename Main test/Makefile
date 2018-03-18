all: actuators_lib_struct.o manipulation_mote_struct.o manipulation_rule_struct.o main.c
	gcc main.c -pthread actuators_lib_struct.o manipulation_mote_struct.o manipulation_rule_struct.o -o main
actuators_lib_struct.o: actuators_lib_struct.c actuators_lib_struct.h
	gcc -c actuators_lib_struct.c
manipulation_mote_struct.o: manipulation_mote_struct.c manipulation_mote_struct.h
	gcc -c manipulation_mote_struct.c
manipulation_rule_struct.o: manipulation_rule_struct.c manipulation_rule_struct.h
	gcc -c manipulation_rule_struct.c
clean:
	rm *.o main *~
