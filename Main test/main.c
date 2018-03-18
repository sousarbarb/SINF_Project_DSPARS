/*
 * main.c
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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include "actuators_lib_struct.h"
#include "manipulation_mote_struct.h"
#include "manipulation_rule_struct.h"

// Errors
#define MAIN_ERROR_1  1
#define MAIN_ERROR_2  1
#define MAIN_ERROR_3  1
#define MAIN_ERROR_4  1
#define MAIN_ERROR_5  1
#define MAIN_ERROR_6  1
#define MAIN_ERROR_7  1
#define MAIN_ERROR_8  1
#define MAIN_ERROR_9  1
#define MAIN_ERROR_10 1

// Constants relative to the communication channels
#define MAIN_CHANNEL_SENSOR  1
#define MAIN_CHANNEL_RGBMAT  2
#define MAIN_CHANNEL_LIM_INF 0
#define MAIN_CHANNEL_LIM_SUP 5

// Global variables
FILE *sensor_data_channel = NULL, *rgb_matrix_channel = NULL;
static const char CHANNEL_DEF[]="/dev/pts/";
int number_motes, number_rules, number_divisions;
mote **system_motes = NULL;
rule **system_rules = NULL;

// Global flags


/****************************************
 * THREAD DATA PROCESSING (PTH1) - CODE
 ****************************************/
void *thread_data_processing(void *arg){
	pthread_exit(NULL);
}


/********************************************
 * THREAD RULE IMPLEMENTATION (PTH2) - CODE
 ********************************************/
void *thread_rule_implementation(void *arg){
	pthread_exit(NULL);
}


/********************************************
 * THREAD ACTUALIZE ACTUATORS (PTH3) - CODE
 ********************************************/
void *actualize_actuators(void *arg){
	pthread_exit(NULL);
}


/***************************************
 * THREAD USER INTERFACE (PTH4) - CODE
 ***************************************/
void *user_interface(void *arg){
	pthread_exit(NULL);
}


/************************
 * FUNCTION MAIN - CODE
 ************************/
int main(int argc, char **argv)
{
	/************************************
	 * INIT ROUTINE - ARGUMENT CHECKING
	 ************************************/
	// ERROR 1 - The channels inserted aren't valid
	if(3 !=argc){
		printf("[MAIN_ERROR %d] The execution of the program implies the definition of the channel that is currently receiving data from the sensors and the channel that it'll be used to send the actuators state to the RGBMatrix.\n", MAIN_ERROR_1);
		printf("Please execute the program by the following steps:\n    ./main \"/dev/pts/x\" \"/dev/pts/y\"\n");
		printf("    Channel x -> communication with the system motes (receives data)\n    Channel y -> communication with the RGBMatrix (sends data)\n");
		exit(-1);
	}
	
	int counter1;
	char directory_channel_sensor[strlen(argv[MAIN_CHANNEL_SENSOR])-1], directory_channel_rgbmat[strlen(argv[MAIN_CHANNEL_RGBMAT])-1];
	for(counter1 = 0; counter1 < strlen(argv[MAIN_CHANNEL_SENSOR]) - 1; counter1++)
		directory_channel_sensor[counter1] = argv[MAIN_CHANNEL_SENSOR][counter1];
	directory_channel_sensor[counter1] = '\0';
	for(counter1 = 0; counter1 < strlen(argv[MAIN_CHANNEL_RGBMAT]) - 1; counter1++)
		directory_channel_rgbmat[counter1] = argv[MAIN_CHANNEL_RGBMAT][counter1];
	directory_channel_rgbmat[counter1] = '\0';
	
	// ERROR 2 - The channel's number selected aren't valid
	if(0 != strcmp(CHANNEL_DEF, directory_channel_sensor) || 0 != strcmp(CHANNEL_DEF, directory_channel_rgbmat) || MAIN_CHANNEL_LIM_INF > (argv[MAIN_CHANNEL_SENSOR][9]-'0') || MAIN_CHANNEL_LIM_SUP < (argv[MAIN_CHANNEL_SENSOR][9]-'0') || MAIN_CHANNEL_LIM_INF > (argv[MAIN_CHANNEL_RGBMAT][9]-'0') || MAIN_CHANNEL_LIM_SUP < (argv[MAIN_CHANNEL_RGBMAT][9]-'0')){
		printf("[MAIN_ERROR %d] The channel's directory aren't correctly characterized.\n", MAIN_ERROR_2);
		printf("Please execute the program by the following steps:\n    ./main \"/dev/pts/x\" \"/dev/pts/y\"\n");
		printf("    Channel x -> communication with the system motes (receives data)\n    Channel y -> communication with the RGBMatrix (sends data)\n");
		exit(-1);
	}
	sensor_data_channel = fopen(argv[MAIN_CHANNEL_SENSOR], "r");
	// ERROR 3
	if(NULL == sensor_data_channel){
		printf("[MAIN_ERROR %d] It's not possible to access the selected channel relative with the sensor's readings\n", MAIN_ERROR_3);
		exit(-1);
	}
	rgb_matrix_channel  = fopen(argv[MAIN_CHANNEL_RGBMAT], "w");
	// ERROR 4
	if(NULL == rgb_matrix_channel){
		printf("[MAIN_ERROR %d] It's not possible to access the selected channel relative with the actuator's state display\n", MAIN_ERROR_4);
		fclose(sensor_data_channel);
		sensor_data_channel = NULL;
		exit(-1);
	}
	
	/*************************
	 * VARIABLES DECLARATION
	 *************************/
	int error_check = 0;
	pthread_t pth1, pth2, pth3, pth4;
	
	/**********************************
	 * INIT ROUTINE - WELCOME MESSAGE
	 **********************************/
	printf("\n******************** HAS - HOME AUTOMATIVE SYSTEM ********************\n\n");
	
	/****************************************
	 * INIT ROUTINE - MOTE'S CONFIGURATION
	 ****************************************/
	printf("++++++++++ MOTE'S CONFIGURATION ++++++++++\n");
	do{
		printf("Insert the number of motes that are considered in the system (needs to be greater than 0): ");
		scanf(" %d", &number_motes);
		getchar();
	} while(1 > number_motes);
	system_motes = mote_vector_creation(number_motes, &error_check);
	if(NULL == system_motes || 0 < error_check){
		printf("[MAIN_ERROR %d] The motes weren't created.\n", MAIN_ERROR_5);
		fclose(sensor_data_channel);
		sensor_data_channel = NULL;
		fclose(rgb_matrix_channel);
		rgb_matrix_channel = NULL;
		exit(-1);
	}
	
	/*******************************************
	 * INIT ROUTINE - DIVISION'S CONFIGURATION
	 *******************************************/
	 
	// PEDRO DE CASTRO ALBERGARIA
	
	/****************************************
	 * INIT ROUTINE - RULE'S CONFIGURATION
	 ****************************************/
	if((-1 == rules_association_to_structures(&system_rules, &number_rules, number_motes, &error_check) || 0 < error_check)){
		printf("[MAIN_ERROR %d] The rules weren't created.\n", MAIN_ERROR_6);
		free_mote_memory(system_motes, number_motes, NULL);
		fclose(sensor_data_channel);
		sensor_data_channel = NULL;
		fclose(rgb_matrix_channel);
		rgb_matrix_channel = NULL;
		exit(-1);
	}
	
	/**************
	 * DEBUG ZONE
	 **************/
	print_motes_vector(system_motes, number_motes, &error_check);
	printf("STATUS print_motes_vector: %d\n", error_check);
	print_rules_system_vector(system_rules, number_rules, &error_check);
	printf("STATUS print_rules_system_vector: %d\n", error_check);
	
	/***********************
	 * CREATION OF THREADS
	 ***********************/
	pthread_create(&pth1, NULL, thread_data_processing, NULL);
	pthread_create(&pth2, NULL, thread_rule_implementation, NULL);
	pthread_create(&pth3, NULL, actualize_actuators, NULL);
	pthread_create(&pth4, NULL, user_interface, NULL);
	
	/****************************************
	 * CREATION AND CANCELLATION OF THREADS
	 ****************************************/
	pthread_join(pth4, NULL);
	pthread_cancel(pth1);
	pthread_cancel(pth2);
	pthread_cancel(pth3);
	pthread_cancel(pth4);
	 
	/***********************
	 * TERMINATION ROUTINE
	 ***********************/
	free_mote_memory(system_motes, number_motes, NULL);
	system_motes = NULL;
	free_rules_system_memory(system_rules, number_rules, NULL);
	system_rules = NULL;
	fclose(sensor_data_channel);
	sensor_data_channel = NULL;
	fclose(rgb_matrix_channel);
	rgb_matrix_channel = NULL;
	printf("\n**********************************************************************\n");
	printf("************************* END OF PROGRAM HAS *************************\n");
	printf("**********************************************************************\n\n");
	return 0;
	exit(0);
}

