/*
 * manipulation_rule_struct.c
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

/***********************************************************************************************************************************
 * RULES TEMPLATE:
 * 
 * <room_name>: <sensor_name><condition><value> [AND / OR <sensor_name><condition><value>] <actuator>:<state>[,<actuator>:<state>]
 ***********************************************************************************************************************************/

#include "manipulation_rule_struct.h"


char **actuator_future_state_vector_creation(int number_actfutstat, int max_charact_string, int * error_func){
	// Argument error checking
	if(1 > number_actfutstat){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] vector_actuator_future_state_creation: Number of future actuator state condition invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_1);
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
	char **vector_act=NULL;
	vector_act = (char **) malloc(sizeof(char) * number_actfutstat);
	if(NULL == vector_act){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] vector_actuator_future_state_creation: Memory allocated failed.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
		return NULL;
	}
	for(counter = 0; counter < number_actfutstat; counter++){
		vector_act[counter] = (char *) malloc(max_charact_string);
		if(NULL == vector_act[counter]){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] vector_actuator_future_state_creation: Memory allocated failed.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
			if(0 < counter)
				free_actuator_future_state_memory(vector_act, counter, NULL);
			else{
				free(vector_act);
				vector_act = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
			return NULL;
		}
		vector_act[counter][0] = '\0';
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
	return vector_act;
}

void free_actuator_future_state_memory(char **vector_act, int number_actfutstat, int * error_func){
	// Argument error checking
	if(NULL == vector_act){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] free_actuator_future_state_memory: Pointer to the string's vector invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_4);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_4;
		return;
	}
	else if(1 > number_actfutstat){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] free_actuator_future_state_memory: Number of future actuator state condition invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_1;
		return;
	}
	
	
	// Function's code
	int counter;
	for(counter = 0; counter < number_actfutstat; counter++){
		free(vector_act[counter]);
		vector_act[counter] = NULL;
	}
	free(vector_act);
	vector_act = NULL;
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
}

void print_actuator_future_state_memory_vector(char **vector_act, int number_actfutstat, int * error_func){
	// Argument error checking
	if(NULL == vector_act){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] print_actuator_future_state_memory_vector: Pointer to the string's vector invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_4);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_4;
		return;
	}
	else if(1 > number_actfutstat){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] print_actuator_future_state_memory_vector: Number of future actuator state condition invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_1;
		return;
	}
	
	
	// Function's code
	int counter;
	printf("\n+++++ ACTUATOR'S FUTURE STATE VECTOR +++++\n");
	for(counter = 0; counter < number_actfutstat; counter++)
		printf("[%d] %s\n", counter, vector_act[counter]);
	printf("\n");
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
}

char *string_creation(int max_charact_string, int * error_func){
	// Argument error checking
	if(2 > max_charact_string){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] string_creation: Maximum string lenght invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_2);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_2;
		return NULL;
	}
	
	
	// Function's code
	char *string = NULL;
	string = (char *) malloc(max_charact_string);
	if(NULL == string){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] string_creation: Memory allocated failed.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
		return NULL;
	}
	string[0] = '\0';
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
	return string;
}



