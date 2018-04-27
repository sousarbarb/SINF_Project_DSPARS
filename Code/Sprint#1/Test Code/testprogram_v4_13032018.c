/*
 * testprogram_v4_13032018.c
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
#include <errno.h>

// Error listing
#define ERROR1	1
#define ERROR2	2
#define ERROR3	3

#define ERROR_PROCESS_SENS_CONF_ROOM1	1
#define ERROR_PROCESS_SENS_CONF_ROOM2	2
#define ERROR_PROCESS_SENS_CONF_ROOM3	3

#define ERROR_PROCESS_RULE1	1
#define ERROR_PROCESS_RULE2	2
#define ERROR_PROCESS_RULE3	3

// Limit of number of channels of the system and the argument number of the information relative to the channel's directory
#define ARG_NUMBER_CHANNEL	1
#define CHANNELLIM_INF	0
#define CHANNELLIM_SUP	4	// The maximum value that can be put here is 9 (otherwise, the program will not perform correctly)

// General constants
#define RULE_BUFFER_SIZE			200
#define SENS_CONF_BUFFER_SIZE		200
#define ROOM_TYPE_SIZE				12
#define SENSOR_TYPE_SIZE			6
#define ACTUATOR_TYPE_SIZE			6

#define NUMB_TYPE_ROOMS				4
#define NUMB_TYPE_SENSORS			3
#define NUMB_TYPE_ACTUATORS			2

#define NUMB_SIZE					4
#define BASECONVERT_PROCESS_RULE	10
#define PROCESS_RULE_BUFFER_SIZE	20

// Global variables
FILE *sensor_data_channel = NULL;
static const char CHANNEL_DEF[]="/dev/pts/";

static const char ROOM_TYPE[NUMB_TYPE_ROOMS][ROOM_TYPE_SIZE] = {"ROOM", "KITCHEN", "LIVING_ROOM", "OTHER"};
static const char SENSOR_TYPE[3][SENSOR_TYPE_SIZE]           = {"TEMP", "HUM", "LIGHT"};
static const char ACTUATOR_TYPE[2][ACTUATOR_TYPE_SIZE]       = {"HEAT", "LIGHT"};

static const char LOGIC_AND[] = "AND";
static const char LOGIC_OR[]  = "OR";


/*********************
 * FUNCTION'S HEADER
 *********************/
int process_sensor_configuration_room(const char *sensor_configuration_room, int size_string_sensconf, char** sensors, int* numb_sensors_room, char** actuators, int* numb_actuators_room);
int process_rule(const char *rule, int size_string_rule);


/********
 * MAIN
 ********/
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
	int counter_main_1 = 0, counter_main_aux_1, rule_string_size, sens_conf_string_size;
	int numb_sens, numb_act;
	char sens_conf[RULE_BUFFER_SIZE]   = {0};
	char **sensors_conf = NULL, **actuators_conf = NULL;
	char rule_buffer[RULE_BUFFER_SIZE] = {0};
	while(1){
		// Insertion of the sensor's configuration
		printf("[%d] Insert sensor's configuration of a room according to the template: ", ++counter_main_1);
		fgets(sens_conf, SENS_CONF_BUFFER_SIZE, stdin);
		sens_conf_string_size = strlen(sens_conf)-1;
		sens_conf[sens_conf_string_size] = '\0';
		process_sensor_configuration_room(sens_conf, sens_conf_string_size, sensors_conf, &numb_sens, actuators_conf, &numb_act);
		
		
		// Free the memory of the sensor's configuration
		if(NULL != sensors_conf && 0 < numb_sens){
			for(counter_main_aux_1 = 0; counter_main_aux_1 < numb_sens; counter_main_aux_1++){
				free(sensors_conf[counter_main_aux_1]);
				sensors_conf[counter_main_aux_1] = NULL;
			}
			free(sensors_conf);
			sensors_conf = NULL;
		}
		
		
		// Insertion of the rule
		printf("[%d] Insert rule according to the template: ", counter_main_1);
		fgets(rule_buffer, RULE_BUFFER_SIZE, stdin);
		rule_string_size = strlen(rule_buffer)-1;
		rule_buffer[rule_string_size] = '\0';
		process_rule(rule_buffer, rule_string_size);
		
		
		// Free the memory of the actuator's configuration
		if(NULL != actuators_conf && 0 < numb_act){
			for(counter_main_aux_1 = 0; counter_main_aux_1 < numb_act; counter_main_aux_1++){
				free(actuators_conf[counter_main_aux_1]);
				actuators_conf[counter_main_aux_1] = NULL;
			}
			free(actuators_conf);
			actuators_conf = NULL;
		}
	}
	
	
	/***********************
	 * TERMINATION ROUTINE
	 ***********************/
	fclose(sensor_data_channel);
	sensor_data_channel = NULL;
	return 0;
}


/*******************
 * FUNCTION'S CODE
 *******************/
int process_sensor_configuration_room(const char *sensor_configuration_room, int size_string_sensconf, char** sensors, int* numb_sensors_room, char** actuators, int* numb_actuators_room){
	char number_manipulation[PROCESS_RULE_BUFFER_SIZE];
	char type_of_room[PROCESS_RULE_BUFFER_SIZE];
	char type_of_sensor[PROCESS_RULE_BUFFER_SIZE];
	char type_of_actuator[PROCESS_RULE_BUFFER_SIZE];
	int numb_type_of_room, numb_room;
	int numb_sensors = 0, numb_actuators = 0;
	int counter, counter_aux_1, present_position, temporary_position;
	
	
	// Process the type of the room
	present_position = 0;
	for(counter = 0; (9 < (sensor_configuration_room[present_position]-'0') || 0 > (sensor_configuration_room[present_position]-'0')) && (':' != sensor_configuration_room[present_position]) && ('\0' != sensor_configuration_room[present_position]); counter++){
		type_of_room[counter] = sensor_configuration_room[present_position];
		present_position++;
	}
	type_of_room[counter] = '\0';
	if('\0' == sensor_configuration_room[present_position]){
		printf("\n[ERROR_PROCESS_SENS_CONF_ROOM %d] The present room's configuration isn't correctly defined.\n\n", ERROR_PROCESS_SENS_CONF_ROOM1);
		return 0;
	}
	for(numb_type_of_room = 0; (0 != strcmp(type_of_room, ROOM_TYPE[numb_type_of_room])) && (numb_type_of_room < NUMB_TYPE_ROOMS); numb_type_of_room++);
	if(NUMB_TYPE_ROOMS != numb_type_of_room)
		printf("    Type of the room: %s\n", ROOM_TYPE[numb_type_of_room]);
	else{
		printf("\n[ERROR_PROCESS_SENS_CONF_ROOM %d] Invalid room inserted in the present sensor's configuration.\n\n", ERROR_PROCESS_SENS_CONF_ROOM2);
		return 0;
	}
	
	
	// Process the number of the room
	for(counter = 0; (9 > (sensor_configuration_room[present_position]-'0')) && (0 < (sensor_configuration_room[present_position]-'0')); counter++){
		number_manipulation[counter] = sensor_configuration_room[present_position];
		present_position++;
	}
	if(0 == counter){
		number_manipulation[counter] = '1';
		counter++;
	}
	number_manipulation[counter] = '\0';
	numb_room = (int) strtol(number_manipulation, NULL, BASECONVERT_PROCESS_RULE);
	printf("    Number of the room: %d\n", numb_room);
	
	
	// Increase the present_position until the sensor's configuration of a certain room is founded
	while((':' != sensor_configuration_room[present_position]) && ('\0' != sensor_configuration_room[present_position]))
		present_position++;
	while((' ' == sensor_configuration_room[present_position]) && ('\0' != sensor_configuration_room[present_position]))
		present_position++;
	if('\0' == sensor_configuration_room[present_position] || '\0' == sensor_configuration_room[present_position + 1]){
		printf("\n[ERROR_PROCESS_SENS_CONF_ROOM %d] The present room's configuration isn't correctly defined.\n\n", ERROR_PROCESS_SENS_CONF_ROOM1);
		return 0;
	}
	
	
	// Sensors processing
	if((':' == sensor_configuration_room[present_position]) || (' ' == sensor_configuration_room[present_position]))
		present_position++;
	printf("    Room's sensors:\n");
	if('\0' == sensor_configuration_room[present_position]){
		printf("\n[ERROR_PROCESS_SENS_CONF_ROOM %d] The present room's configuration isn't correctly defined.\n\n", ERROR_PROCESS_SENS_CONF_ROOM1);
		return 0;
	}
	else if(':' == sensor_configuration_room[present_position]){
		printf("        (None)\n");
		present_position++;
	}
	else{
		numb_sensors++;
		temporary_position = present_position;
		while((':' != sensor_configuration_room[temporary_position]) && (size_string_sensconf > temporary_position)){
			if(',' == sensor_configuration_room[temporary_position])
				numb_sensors++;
			temporary_position++;
		}
		sensors = (char **) malloc(sizeof(char*) * numb_sensors);
		if(NULL == sensors){
			printf("\n[ERROR_PROCESS_SENS_CONF_ROOM %d] Memory allocation isn't possible.\n\n", ERROR_PROCESS_SENS_CONF_ROOM3);
			return 0;
		}
		for(counter = 0; counter < numb_sensors; counter++){
			sensors[counter]=(char *) malloc( (SENSOR_TYPE_SIZE + NUMB_SIZE) );
			if(NULL == sensors[counter]){
				printf("\n[ERROR_PROCESS_SENS_CONF_ROOM %d] Memory allocation isn't possible.\n\n", ERROR_PROCESS_SENS_CONF_ROOM3);
				return 0;
			}
			for(counter_aux_1 = 0; ',' != sensor_configuration_room[present_position] && ':' != sensor_configuration_room[present_position]; counter_aux_1++){
				if('\0' == sensor_configuration_room[present_position]){
					printf("\n[ERROR_PROCESS_SENS_CONF_ROOM %d] The present room's configuration isn't correctly defined.\n\n", ERROR_PROCESS_SENS_CONF_ROOM1);
					return 0;
				}
				sensors[counter][counter_aux_1] = sensor_configuration_room[present_position];
				present_position++;
			}
			sensors[counter][counter_aux_1] = '\0';
			printf("        [Sensor %d] %s\n", counter+1, sensors[counter]);
			if(',' == sensor_configuration_room[present_position])
				present_position++;
		}
	}
	printf("    The room has %d sensors\n", numb_sensors);
	
	// Actuators processing
	if(('\0' == sensor_configuration_room[present_position]) && (':' != sensor_configuration_room[present_position-1])){
		printf("\n[ERROR_PROCESS_SENS_CONF_ROOM %d] The present room's configuration isn't correctly defined.\n\n", ERROR_PROCESS_SENS_CONF_ROOM1);
		return 0;
	}
	else if((':' == sensor_configuration_room[present_position]) || (' ' == sensor_configuration_room[present_position]))
		present_position++;
	
	printf("    Room's actuators:\n");
	if('\0' == sensor_configuration_room[present_position])
		printf("        (None)\n");
	else{
		numb_actuators++;
		temporary_position = present_position;
		while(('\0' != sensor_configuration_room[temporary_position]) && (size_string_sensconf > temporary_position)){
			if(',' == sensor_configuration_room[temporary_position])
				numb_actuators++;
			temporary_position++;
		}
		actuators = (char **) malloc(sizeof(char*) * numb_actuators);
		if(NULL == actuators){
			printf("\n[ERROR_PROCESS_SENS_CONF_ROOM %d] Memory allocation isn't possible.\n\n", ERROR_PROCESS_SENS_CONF_ROOM3);
			return 0;
		}
		for(counter = 0; counter < numb_actuators; counter++){
			actuators[counter]=(char *) malloc( (ACTUATOR_TYPE_SIZE + NUMB_SIZE) );
			if(NULL == actuators[counter]){
				printf("\n[ERROR_PROCESS_SENS_CONF_ROOM %d] Memory allocation isn't possible.\n\n", ERROR_PROCESS_SENS_CONF_ROOM3);
				return 0;
			}
			for(counter_aux_1 = 0; ',' != sensor_configuration_room[present_position] && '\0' != sensor_configuration_room[present_position]; counter_aux_1++){
				actuators[counter][counter_aux_1] = sensor_configuration_room[present_position];
				present_position++;
			}
			actuators[counter][counter_aux_1] = '\0';
			printf("        [Actuator %d] %s\n", counter+1, actuators[counter]);
			present_position++;
		}
	}
	printf("    The room has %d actuators\n", numb_actuators);
	
		
	// Passing values to the main function
	(*numb_sensors_room)   = numb_sensors;
	(*numb_actuators_room) = numb_actuators;
}

 
int process_rule(const char *rule, int size_string_rule){
	char number_manipulation[PROCESS_RULE_BUFFER_SIZE];
	char type_of_room[PROCESS_RULE_BUFFER_SIZE];
	char type_of_sensor[PROCESS_RULE_BUFFER_SIZE];
	char type_of_actuator[PROCESS_RULE_BUFFER_SIZE];
	int numb_type_of_room, numb_room;
	int counter, present_position;
	
	// Process the type of the room
	present_position = 0;
	for(counter = 0; (9 < (rule[present_position]-'0') || 0 > (rule[present_position]-'0')) && (':' != rule[present_position]) && ('\0' != rule[present_position]); counter++){
		type_of_room[counter] = rule[present_position];
		present_position++;
	}
	type_of_room[counter] = '\0';
	if('\0' == rule[present_position]){
		printf("\n[ERROR_PROCESS_RULE %d] The present rule isn't correctly defined.\n\n", ERROR_PROCESS_RULE1);
		return 0;
	}
	for(numb_type_of_room = 0; (0 != strcmp(type_of_room, ROOM_TYPE[numb_type_of_room])) && (numb_type_of_room < NUMB_TYPE_ROOMS); numb_type_of_room++);
	if(NUMB_TYPE_ROOMS != numb_type_of_room)
		printf("    Type of the room: %s\n", ROOM_TYPE[numb_type_of_room]);
	else{
		printf("\n[ERROR_PROCESS_RULE %d] Invalid room inserted in the present rule.\n\n", ERROR_PROCESS_RULE2);
		return 0;
	}
	
	
	// Process the number of the room
	for(counter = 0; (9 > (rule[present_position]-'0')) && (0 < (rule[present_position]-'0')); counter++){
		number_manipulation[counter] = rule[present_position];
		present_position++;
	}
	if(0 == counter){
		number_manipulation[counter] = '1';
		counter++;
	}
	number_manipulation[counter] = '\0';
	numb_room = (int) strtol(number_manipulation, NULL, BASECONVERT_PROCESS_RULE);
	printf("    Number of the room: %d\n", numb_room);
	
	
	// Increase the present_position until a condition is founded
	while((':' != rule[present_position]) && ('\0' != rule[present_position]))
		present_position++;
	if(':' == rule[present_position])
		present_position++;
	while((' ' == rule[present_position]) && ('\0' != rule[present_position]))
		present_position++;
	if('\0' == rule[present_position]){
		printf("\n[ERROR_PROCESS_RULE %d] The present rule isn't correctly defined.\n\n", ERROR_PROCESS_RULE1);
		return 0;
	}
	
	
	// Process the rule's condition
	printf("    Rule aplied to the present room: %s\n", &rule[present_position]);
	
	/**************
	 * INCOMPLETE
	 **************/
}
