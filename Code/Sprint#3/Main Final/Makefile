all: actuators_lib_struct.o manipulation_mote_struct.o manipulation_rule_struct.o manipulation_division_struct.o project_matrix.o main.c
	gcc main.c -pthread -lpq actuators_lib_struct.o manipulation_mote_struct.o manipulation_rule_struct.o manipulation_division_struct.o project_matrix.o -o main
actuators_lib_struct.o: actuators_lib_struct.c actuators_lib_struct.h
	gcc -c actuators_lib_struct.c
manipulation_mote_struct.o: manipulation_mote_struct.c manipulation_mote_struct.h
	gcc -c manipulation_mote_struct.c
manipulation_rule_struct.o: manipulation_rule_struct.c manipulation_rule_struct.h
	gcc -c manipulation_rule_struct.c
manipulation_division_struct.o: manipulation_division_struct.c manipulation_division_struct.h
	gcc -c manipulation_division_struct.c
project_matrix.o: project_matrix.c project_matrix.h
	gcc -c project_matrix.c
clean:
	rm -f *.o*~
