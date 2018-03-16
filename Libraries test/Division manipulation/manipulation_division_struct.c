/*
 * manipulation_division_struct.c
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
/*******************************************************************
 * SENSOR CONFIGURATION TEMPLATE
 * 
 * <room_name>:<sensor_name>[,<sensor_name>]:<actuator>[,actuator]
 * *****************************************************************/


#include "manipulation_division_struct.h"

division ** division_vector_creation(int number_divisions, int * error_func) {
	// Argument error checking
	if(1 > number_division){
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] division_vector_creation: Number of divisions invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_1;
		return NULL;
	}
	
	// Function's code
	int counter;
	division **system_divisions = NULL;
	system_divisions = (division **) malloc(sizeof((division *) * number_divisions);
	if(NULL == system_divisions) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] division_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_3;
		return NULL;
	}
	
	for(counter = 0; counter < number_divisions; counter++){
		system_divisions[counter] = (division *) malloc(sizeof(divison));
		if(NULL == system_divisions[counter]){
			printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] division_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
			if(0 < counter)
				free_divisions_memory(system_divisions, counter, NULL);
			else{
				free(system_divisons);
				system_divisions = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_3;
			return NULL;
		}
		
		//IMCOMPLETO - CRIAR AS STRINGS E OS VETORES DE STRINGS
		
		system_divisions[counter]->division_name = string_creation(MAX_CHARACT_STRING, NULL);
		system_divisions[counter]->num_luminosity = 0;
		system_divisions[counter]->
		system_divisions[counter]->num_temperature = 0;
		system_divisions[counter]->num_humidity = 0;
		system_divisions[counter]->num_actuator = 0;
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
	return system_divisions;
}

char *string_creation(int max_charact_string, int * error_func){
	// Argument error checking
	if(2 > max_charact_string){
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] string_creation: Maximum string lenght invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_2);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_2;
		return NULL;
	}

	// Function's code
	char *string = NULL;
	string = (char *) malloc(max_charact_string);
	if(NULL == string){
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] string_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
		return NULL;
	}
	string[0] = '\0';
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
	return string;
}

char **string_vector_sensors_creation(int number_sensors, int max_character_string, int * error_func) {
	// Argument error checking
	if(1 > number_sensors){
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] vector_actuator_future_state_creation: Number of future actuator state condition invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_1;
		return NULL;
	}
	else if(2 > max_charact_string){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] vector_actuator_future_state_creation: Maximum string lenght invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_2);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_2;
		return NULL;
	}

	// Function's code
	int counter;
	char **vector_sensors=NULL;
	vector_sensors = (char **) malloc(sizeof(char) * number_sensors);
	if(NULL == vector_sensors){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] string_vector_sensors_creation: Memory allocated failed.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
		return NULL;
	}
	for(counter = 0; counter < number_sensors; counter++){
		vector_sensors[counter] = (char *) malloc(max_charact_string);
		if(NULL == vector_sensors[counter]){
			printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] vector_actuator_future_state_creation: Memory allocated failed.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
			if(0 < counter)
				//free_actuator_future_state_memory(vector_act, counter, NULL);
			else{
				free(vector_sensors);
				vector_sensors = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
			return NULL;
		}
		vector_act[counter][0] = '\0';
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
	return vector_sensors;
}
	
void free_vector_string_memory(char **vector_string, int number_sensors, int * error_func) {
	// Argument error checking
	if(NULL = string) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] free_vector_string_memory: Pointer to the string's vector invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_5;
		return;
	}
	if(1 > number_sensors) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] free_vector_string_memory:  Number of sensores invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_2);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_2;
		return NULL;
	}
		
	// Function's code
	int counter;
	for(counter = 0; counter < number_sensors; conter++) {
		free(vector_string[counter]);
		vector_string[counter] = NULL;
	}
	
	free(vector_string);
	vector_string = NULL;
	if(NULL != error_func) {
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	}
}

void free_memory_all(division **system_divisions, int number_divisions, int number_sensors_luminosity, int number_sensors_temperature, int numer_sensors_humidity, int * error_func) {
	// Argument error checking
	if(NULL == system_divisions) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] free_memory_all: Pointer to the division struct invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_5;
		return;
	}
	if(1 > number_sensors) {
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] free_memory_all: Number of sensors invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_1;
		return;
	}
	if(1 > number_divisions) ) {
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] free_memory_all: Number of divisions invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_1;
		return;
	}
	
	
	// Function´s code
	int counter;
	for(counter = 0; counter < number_divisions; counter++) {
		free(system_divisions[counter]->division_name);
		free_vector_string_memory(system_divisions[counter]->luminosity_sensors, number_sensors_luminosity);
		free_vector_string_memory(system_divisions[counter]->luminosity_sensors, number_sensors_temperature);
		free_vector_string_memory(system_divisions[counter]->luminosity_sensors, number_sensors_humidity);
		
		//FALTA PARA OS ATUADORES	
	}
	free(system_divisions);
	system_divisions = NULL;
		if(NULL != error_func) {
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	}
}
