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

actuator ** actuatores_vector_creation(int number_actuatores, int * error_func){
	// Argument error checking
	if(1 > number_actuatores){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] actuatores_vector_creation: Number of actuatores invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_1;
		return NULL;
	}
	
	
	// Function's code
	int counter, aux = 0;
	actuator **system_actuatores = NULL;
	system_actuatores = (actuator **) malloc(sizeof(actuator *) * number_actuatores);
	if(NULL == system_actuatores){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] actuatores_vector_creation: Memory allocated failed.\n", ERROR_LIB_ACTUATORES_STRUCT_2);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_2;
		return NULL;
	}
	for(counter = 0; counter < number_actuatores; counter++){
		system_actuatores[counter] = (actuator *) malloc(sizeof(actuator));
		if(NULL == system_actuatores[counter]){
			printf("[ERROR_LIB_ACTUATORES_STRUCT %d] actuatores_vector_creation: Memory allocated failed.\n", ERROR_LIB_ACTUATORES_STRUCT_2);
			if(0 < counter)
				free_actuation_memory(system_actuatores, counter, NULL);
			else{
				free(system_actuatores);
				system_actuatores = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_2;
			return NULL;
		}
							/*VER ABAIXO !!!*/
		do{
			printf("[%d] Insert the mote id (must be higher than 0): ", counter);
			scanf(" %d", &aux);
			getchar();
			if(0 < counter) 
				if(0 <= search_actuator(system_actuatores, counter, aux, NULL))
					aux = 0;
		}while(1 > aux);
		system_actuatores[counter]->id = aux;
		system_actuatores[counter]->actuation[0] = '\0';
							/*ATÉ AQUI !!!*/
	}			
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_NONE;
	return system_actuatores;
}

void free_actuation_memory(actuator** system_actuatores, int number_actuatores, int * error_func){
	// Argument error checking
	if(NULL == system_actuatores){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] free_mote_memory: Pointer to the actuatores's structure invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_3;
		return;
	}
	else if(0 > number_actuatores){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] free_mote_memory: Number of actuatores invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_1;
		return;
	}
	
	
	// Function's code
	int counter;
	for(counter = 0; counter < number_actuatores; counter++){
		free(system_actuatores[counter]);
		system_actuatores[counter] = NULL;
	}
	free(system_actuatores);
	system_actuatores = NULL;
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_NONE;
}

int search_actuator(actuator** system_actuatores, int number_actuatores, int actuator_id, int * error_func){
	// Argument error checking
	if(NULL == system_actuatores){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] search_actuator: Pointer to the actuatores's structure invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_3;
		return -2;
	}
	else if(1 > number_actuatores){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] search_actuator: Number of actuatores invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_1;
		return -2;
	}
	else if(1 > actuator_id){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] search_mote: Actuator_id invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_4);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_4;
		return -2;
	}
	
	
	// Function's code
	int counter;
	for(counter = 0; (counter < number_actuatores); counter++)
		if(actuator_id == system_actuatores[counter]->id)
			break;
			
	if(number_actuatores == counter){
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_NONE;
		return -1;
	}
	else{
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_NONE;
		return counter;
	}
}

void print_actuatores_vector(actuator** system_actuatores, int number_actuatores, int * error_func){
	// Argument error checking
	if(NULL == system_actuatores){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] print_actuatores_vector: Pointer to the actuatores's structure invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_3;
		return;
	}
	else if(0 > number_actuatores){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] print_actuatores_vector: Number of actuatores invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_1;
		return;
	}
	
	
	// Function's code
	int counter;
	printf("\n+++++ ACTUATORES'S VECTOR +++++\n");
	for(counter = 0; number_actuatores > counter; counter++)
		printf("[%d][ACTUATOR ID: %d]\n    State  : %s\n", counter, system_actuatores[counter]->id, system_actuatores[counter]->actuation);
	printf("\n");
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_NONE;
		
	return;
}

void print_single_actuator(actuator** system_actuatores, int number_actuatores, int actuator_id, int * error_func){
	// Argument error checking
	if(NULL == system_actuatores){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] print_single_actuator: Pointer to the actuator's structure invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_3;
		return;
	}
	else if(1 > number_actuatores){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] print_single_actuator: Number of actuatores invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_1;
		return;
	}
	else if(1 > actuator_id){
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] print_single_actuator: Actuator_id invalid.\n", ERROR_LIB_ACTUATORES_STRUCT_4);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_4;
		return;
	}
	
	
	// Function's code
	int act_position;
	act_position = search_actuator(system_actuatores, number_actuatores, actuator_id, NULL);
	if(0 <= act_position)
		printf("[%d][ACTUATOR ID: %d]\n    State  : %s\n", act_position, system_actuatores[act_position]->id, system_actuatores[act_position]->actuation);
	else{
		printf("[ERROR_LIB_ACTUATORES_STRUCT %d] print_single_actuator: Actuator_id doesn't belong to the system actuatores.\n", ERROR_LIB_ACTUATORES_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_5;
		return;
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_ACTUATORES_STRUCT_NONE;
}
