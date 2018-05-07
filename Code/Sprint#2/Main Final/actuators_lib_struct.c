/*
 * actuators_lib_struct.c
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


#include "actuators_lib_struct.h"

actuator ** actuators_vector_creation(int number_actuators, int * error_func){
	// Argument error checking
	if(1 > number_actuators){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] actuators_vector_creation: Number of actuators invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_1;
		return NULL;
	}
	
	
	// Function's code
	int counter;
	actuator **system_actuators = NULL;
	system_actuators = (actuator **) malloc(sizeof(actuator *) * number_actuators);
	if(NULL == system_actuators){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] actuators_vector_creation: Memory allocated failed.\n", ERROR_LIB_ACTUATORS_STRUCT_2);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_2;
		return NULL;
	}
	for(counter = 0; counter < number_actuators; counter++){
		system_actuators[counter] = (actuator *) malloc(sizeof(actuator));
		if(NULL == system_actuators[counter]){
			printf("[ERROR_LIB_ACTUATORS_STRUCT %d] actuators_vector_creation: Memory allocated failed.\n", ERROR_LIB_ACTUATORS_STRUCT_2);
			if(0 < counter)
				free_actuation_memory(system_actuators, counter, NULL);
			else{
				free(system_actuators);
				system_actuators = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_2;
			return NULL;
		}
		system_actuators[counter]->id[0] = '\0';
		system_actuators[counter]->actuation[0] = '\0';
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_NONE;
	return system_actuators;
}

void free_actuation_memory(actuator** system_actuators, int number_actuators, int * error_func){
	// Argument error checking
	if(NULL == system_actuators){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] free_actuation_memory: Pointer to the actuators's structure invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_3;
		return;
	}
	else if(0 > number_actuators){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] free_actuation_memory: Number of actuators invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_1;
		return;
	}
	
	
	// Function's code
	int counter;
	for(counter = 0; counter < number_actuators; counter++){
		free(system_actuators[counter]);
		system_actuators[counter] = NULL;
	}
	free(system_actuators);
	system_actuators = NULL;
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_NONE;
}

int search_actuator(actuator** system_actuators, int number_actuators, char *actuator_id, int * error_func){
	// Argument error checking
	if(NULL == system_actuators){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] search_actuator: Pointer to the actuators's structure invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_3;
		return -2;
	}
	else if(1 > number_actuators){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] search_actuator: Number of actuators invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_1;
		return -2;
	}
	else if(strcmp("\0",actuator_id) == 0){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] search_actuator: Actuator_id invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_4);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_4;
		return -2;
	}
	
	
	// Function's code
	int counter;
	for(counter = 0; (counter < number_actuators); counter++)
		if(strcmp(actuator_id,system_actuators[counter]->id) == 0)
			break;
			
	if(number_actuators == counter){
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_NONE;
		return -1;
	}
	else{
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_NONE;
		return counter;
	}
}

void print_actuation_vector(actuator** system_actuators, int number_actuators, int * error_func){
	// Argument error checking
	if(NULL == system_actuators){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] print_actuators_vector: Pointer to the actuators's structure invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_3;
		return;
	}
	else if(0 > number_actuators){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] print_actuators_vector: Number of actuators invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_1;
		return;
	}
	
	
	// Function's code
	int counter;
	printf("\n+++++ ACTUATORS'S VECTOR +++++\n");
	for(counter = 0; number_actuators > counter; counter++)
		printf("[%d][ACTUATOR ID: %s]\n    State  : %s\n", counter, system_actuators[counter]->id, system_actuators[counter]->actuation);
	printf("++++++++++++++++++++++++++++++\n");
	printf("\n");
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_NONE;
		
	return;
}

void print_actuator_state(actuator** system_actuators, int number_actuators, char *actuator_id, int * error_func){
	// Argument error checking
	if(NULL == system_actuators){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] print_single_actuator: Pointer to the actuator's structure invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_3;
		return;
	}
	else if(1 > number_actuators){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] print_single_actuator: Number of actuators invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_1;
		return;
	}
	else if(strcmp("\0",actuator_id) == 0){
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] print_single_actuator: Actuator_id invalid.\n", ERROR_LIB_ACTUATORS_STRUCT_4);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_4;
		return;
	}
	
	
	// Function's code
	int act_position;
	act_position = search_actuator(system_actuators, number_actuators, actuator_id, NULL);
	if(0 <= act_position)
		printf("[%d][ACTUATOR ID: %s]\n    State  : %s\n", act_position, system_actuators[act_position]->id, system_actuators[act_position]->actuation);
	else{
		printf("[ERROR_LIB_ACTUATORS_STRUCT %d] print_single_actuator: Actuator_id doesn't belong to the system actuators.\n", ERROR_LIB_ACTUATORS_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_5;
		return;
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_ACTUATORS_STRUCT_NONE;
}
