/*
 * teste_08032018.c
 * 
 * Copyright 2018 Ricardo Barbosa Sousa <sousa@debian-rbsousa>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int i;
	if(3 != argc){
		printf("Please run the program on the correctly form:\n\t./teste_v1_08032018 Username Country\n");
		printf("Number of arguments: %d\n", argc);
		exit(0);
		/****************************************************************************************
		 * The value that is put on the function exit is the return value to is parent process.
		 * If you want to see more about the function exit, please go to the link below:
		 * 		https://www.tutorialspoint.com/c_standard_library/c_function_exit.htm
		 ****************************************************************************************/ 
	}
	printf("Hello world, by %s from %s.\n", argv[1], argv[2]);
	
	// Content of the arguments
	for(i=0; i<argc; i++)
		printf("Argument %d: \"%s\"\n", i+1, argv[i]);
	return 0;
}

