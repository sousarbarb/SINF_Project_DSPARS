/*
 * testprogram_v3_13032018.c
 * 
 * Copyright 2018	
 *     Daniel Queirós da Silva		<up201503071@fe.up.pt>
 *     Pedro de Castro Albergaria	<up201504635@fe.up.pt>
 *     Ricardo Barbosa Sousa		<up201503004@fe.up.pt>
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
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Error listing
#define ERROR1	1
#define ERROR2	2
#define ERROR3	3

// Limit of number of channels of the system and the argument number of the information relative to the channel's directory
#define ARG_NUMBER_CHANNEL	1
#define CHANNELLIM_INF	0
#define CHANNELLIM_SUP	4	// The maximum value that can be put here is 9 (otherwise, the program will not perform correctly)

// General constants
#define BUFFERSIZE	100

// Global variables
static const char CHANNEL_DEF[]="/dev/pts/";
FILE *sensor_data_channel = NULL;

int main(int argc, char **argv)
{
	/****************
	 * INIT ROUTINE
	 ****************/
	// Verifying if the arguments put on the program execution were correct
	char directory_channel[strlen(argv[ARG_NUMBER_CHANNEL])-1];
	int counter1;
	for(counter1 = 0; counter1 < strlen(argv[ARG_NUMBER_CHANNEL]) - 1; counter1++)
		directory_channel[counter1] = argv[ARG_NUMBER_CHANNEL][counter1];
	directory_channel[counter1] = '\0';
	
	// ERROR 1 - The channel inserted isn't valid
	if(2 !=argc){
		printf("[ERROR %d] The execution of the program implies the definition of the channel that is currently receiving data from the sensors.\n", ERROR1);
		printf("Please execute the program by the following steps:\n\t./testprogram_v3_13032018 \"/dev/pts/x\" \n");
		printf("\tOBS - the character x is the number of the channel wich will receive the simulation data\n");
		exit(-1);
	}
	
	// ERROR 2 - The channel's number selected isn't valid
	else if(0 != strcmp(CHANNEL_DEF, directory_channel) || CHANNELLIM_INF > (argv[ARG_NUMBER_CHANNEL][9]-'0') || CHANNELLIM_SUP < (argv[ARG_NUMBER_CHANNEL][9]-'0')){
		printf("[ERROR %d] The channel directory isn't correctly characterized.\n", ERROR2);
		printf("Please execute the program by the following steps:\n\t./testprogram_v3_13032018 \"/dev/pts/x\" \n");
		printf("\tOBS - the character x is the number of the channel wich will receive the simulation data\n");
		exit(-1);
	}
	
	// Open the information channel
	sensor_data_channel = fopen(argv[ARG_NUMBER_CHANNEL], "r");
	if(NULL == sensor_data_channel){
		printf("[ERROR %d] It's not possible to access the selected channel\n", ERROR3);
		exit(-1);
	}
	
	
	/****************
	 * MAIN PROGRAM
	 ****************/
	int counter_main_1 = 0;
	char buffer[BUFFERSIZE] = {0};
	while(1){
		if(NULL != fgets(buffer, BUFFERSIZE, sensor_data_channel)){
			buffer[strlen(buffer) - 1] = '\0';
			printf("[%d] %s\n", ++counter_main_1, buffer);
		}
	}
	
	
	/***********************
	 * TERMINATION ROUTINE
	 ***********************/
	fclose(sensor_data_channel);
	sensor_data_channel = NULL;
	return 0;
}

