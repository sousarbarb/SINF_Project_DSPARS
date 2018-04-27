all: project_matrix.o matrix_test.c
	gcc matrix_test.c project_matrix.o -o matrix_test
manipulation_mote_struct.o: project_matrix.c project_matrix.h
	gcc -c project_matrix.c
clean:
	rm *.o matrix_test *~
