/*
 * testprogram_v2_11032018.c
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
 * Instructions to compile the program in the terminal:
 * 		gcc testprogram_v2_11032018.c -pthread -o testprogram_v2_11032018
 * Then executed the program.
 * 
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define BUFFERSIZE	75

// Global variables

FILE *channel = NULL;
char str_test[BUFFERSIZE]={0}, channel_buffer[BUFFERSIZE]={0};
uint8_t flag_thread_reading_data = 0;
int counter=0;

void *read_sensors_data(void *arg){
	printf("THREAD READ_SENSORS_DATA ON\n");
	while(flag_thread_reading_data){
		if(NULL != fgets(channel_buffer, BUFFERSIZE, channel)){
			strcpy(str_test, channel_buffer);
			++counter;
			str_test[ strlen(str_test)-1 ] = '\0';
			printf("TH_RSD[%d]: %s\n", counter, str_test);
		}
	}
	printf("Thread of reading sensor data is ended.\n");
	pthread_exit(NULL);
}

void *print_data(void *arg){
	char c;
	printf("THREAD PRINT_DATA ON\n");
	while(1){
		printf("Press the number of the instruction (and then press ENTER) to select an option:\n");
		printf("    -1-Print the buffered data\n    -2-Exit the program\n");
		c = getchar();
		if('1'==c)
			printf("[%d] %s\n", counter, str_test);
		else if ('2'==c){
			flag_thread_reading_data = 0;
			break;
		}
		else
			printf("Select a valid option\n");
	}
	printf("Thread that prints data is ended.\n");
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	// Verifying if the arguments put on the program execution were correct
	if(2 !=argc){
		printf("Please execute the program by the following steps:\n    ./testprogram_v1_09032018 \"/dev/pts/x\" \n");
		printf("    OBS - the character x is the number of the channel wich will receive the simulation data\n");
		exit(-1);
	}
	
	// Local variables
	pthread_t pth1, pth2;
	
	// Open the channel to read the sensors information
	channel = fopen(argv[1], "r");
	if(NULL == channel){
		printf("The channel wich was inserted isn't valid or the channel is occupied\nPlease execute the program again and select a valid channel\n");
		exit(-1);
	}
	
	// Creation of the thread that is responsible for reading the data from the sensors
	flag_thread_reading_data = 1;
	pthread_create(&pth1, NULL, read_sensors_data, NULL);
	pthread_create(&pth2, NULL, print_data, NULL);
	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);
	
	// Program is finished
	printf("\nPROGRAM ENDED\n");
	fclose(channel);
	return 0;
}

