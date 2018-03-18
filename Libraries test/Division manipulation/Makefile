all: manipulation_division_struct.o actuators_lib_struct.o main.c
	gcc main.c manipulation_division_struct.o actuators_lib_struct.o -o main
manipulation_division_struct.o: manipulation_division_struct.c manipulation_division_struct.h
	gcc -c manipulation_division_struct.c
actuators_lib_struct.o: actuators_lib_struct.c actuators_lib_struct.h
	gcc -c actuators_lib_struct.c
clean:
	rm *.o main *~
