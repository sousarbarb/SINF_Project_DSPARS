/*
 * testprogram_v1_09032018.c
 * 
 * Copyright 2018	
 * 					Daniel Queirós da Silva		<up201503071@fe.up.pt>
 *					Pedro de Castro Albergaria	<up201504635@fe.up.pt>
 *					Ricardo Barbosa Sousa		<up201503004@fe.up.pt>
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
#include <string.h>

#define  BUFFERSIZE	70

FILE *channel = NULL;

int main(int argc, char **argv)
{
	int counter=0;
	char str_test[BUFFERSIZE];
	
	// Verifying if the arguments put on the program execution were correct
	if(2 !=argc){
		printf("Please execute the program by the following steps:\n    ./testprogram_v1_09032018 \"/dev/pts/x\" \n");
		printf("    OBS - the character x is the number of the channel wich will receive the simulation data\n");
		exit(-1);
	}
	
	// Open the channel to read the sensors information
	channel = fopen(argv[1], "r");
	if(NULL == channel){
		printf("The channel wich was inserted isn't valid or the channel is occupied\nPlease execute the program again and select a valid channel\n");
		exit(-1);
	}
	
	// Read the information
	while(1){
		if(NULL != fgets(str_test, BUFFERSIZE, channel)){
			++counter;
			str_test[ strlen(str_test)-1 ] = '\0';
			printf("[Line data %d] %s\n", counter, str_test);
		}
	}
	
	fclose(channel);
	return 0;
}

