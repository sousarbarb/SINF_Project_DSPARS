all: actuators_lib_struct.o test_actuators.c
	gcc test_actuators.c actuators_lib_struct.o -o test_actuators
actuators_lib_struct.o: actuators_lib_struct.c actuators_lib_struct.h
	gcc -c actuators_lib_struct.c
clean:
	rm *.o test_actuators *~
