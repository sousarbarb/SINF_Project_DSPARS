/*
 * manipulation_mote_struct.c
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


#include "manipulation_mote_struct.h"

mote ** mote_vector_creation(int number_motes, int * error_func){
	// Argument error checking
	if(1 > number_motes){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] mote_vector_creation: Number of motes invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_1;
		return NULL;
	}
	
	
	// Function's code
	int counter, aux = 0;
	mote **system_motes = NULL;
	system_motes = (mote **) malloc(sizeof(mote *) * number_motes);
	if(NULL == system_motes){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] mote_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_MOTE_STRUCT_2);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_2;
		return NULL;
	}
	for(counter = 0; counter < number_motes; counter++){
		system_motes[counter] = (mote *) malloc(sizeof(mote));
		if(NULL == system_motes[counter]){
			printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] mote_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_MOTE_STRUCT_2);
			if(0 < counter)
				free_mote_memory(system_motes, counter, NULL);
			else{
				free(system_motes);
				system_motes = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_2;
			return NULL;
		}
		do{
			printf("[%d] Insert the mote id (must be higher than 0): ", counter);
			scanf(" %d", &aux);
			getchar();
			if(0 < counter) 
				if(0 <= search_mote(system_motes, counter, aux, NULL))
					aux = 0;
		}while(1 > aux);
		system_motes[counter]->id = aux;
		system_motes[counter]->luminosity = 0;
		system_motes[counter]->temperature = 0;
		system_motes[counter]->humidity = 0;
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_NONE;
	return system_motes;
}

void free_mote_memory(mote** system_motes, int number_motes, int * error_func){
	// Argument error checking
	if(NULL == system_motes){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] free_mote_memory: Pointer to the mote's structure invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_3;
		return;
	}
	else if(0 > number_motes){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] free_mote_memory: Number of motes invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_1;
		return;
	}
	
	
	// Function's code
	int counter;
	for(counter = 0; counter < number_motes; counter++){
		free(system_motes[counter]);
		system_motes[counter] = NULL;
	}
	free(system_motes);
	system_motes = NULL;
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_NONE;
}

int search_mote(mote** system_motes, int number_motes, int mote_id, int * error_func){
	// Argument error checking
	if(NULL == system_motes){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] search_mote: Pointer to the mote's structure invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_3;
		return -2;
	}
	else if(1 > number_motes){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] search_mote: Number of motes invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_1;
		return -2;
	}
	else if(1 > mote_id){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] search_mote: Mote_id invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_4);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_4;
		return -2;
	}
	
	
	// Function's code
	int counter;
	for(counter = 0; (counter < number_motes); counter++)
		if(mote_id == system_motes[counter]->id)
			break;
			
	if(number_motes == counter){
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_NONE;
		return -1;
	}
	else{
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_NONE;
		return counter;
	}
}

void print_motes_vector(mote** system_motes, int number_motes, int * error_func){
	// Argument error checking
	if(NULL == system_motes){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] print_motes_vector: Pointer to the mote's structure invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_3;
		return;
	}
	else if(0 > number_motes){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] print_motes_vector: Number of motes invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_1;
		return;
	}
	
	
	// Function's code
	int counter;
	printf("\n+++++ MOTE'S VECTOR +++++\n");
	for(counter = 0; number_motes > counter; counter++)
		printf("[%d][MOTE ID: %d]\n    Luminosity  = %.2f lux\n    Temperature = %.2f ºC\n    Humidity    = %.2f %%\n", counter, system_motes[counter]->id, system_motes[counter]->luminosity, system_motes[counter]->temperature, system_motes[counter]->humidity);
	printf("\n");
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_NONE;
}

void print_single_mote(mote** system_motes, int number_motes, int mote_id, int * error_func){
	// Argument error checking
	if(NULL == system_motes){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] print_single_mote: Pointer to the mote's structure invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_3;
		return;
	}
	else if(1 > number_motes){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] print_single_mote: Number of motes invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_1;
		return;
	}
	else if(1 > mote_id){
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] print_single_mote: Mote_id invalid.\n", ERROR_LIB_MAN_MOTE_STRUCT_4);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_4;
		return;
	}
	
	
	// Function's code
	int m_position;
	m_position = search_mote(system_motes, number_motes, mote_id, NULL);
	if(0 <= m_position)
		printf("[%d][MOTE ID: %d]\n    Luminosity  = %.2f lux\n    Temperature = %.2f ºC\n    Humidity    = %.2f %%\n", m_position, system_motes[m_position]->id, system_motes[m_position]->luminosity, system_motes[m_position]->temperature, system_motes[m_position]->humidity);
	else{
		printf("[ERROR_LIB_MAN_MOTE_STRUCT %d] print_single_mote: Mote_id doesn't belong to the system motes.\n", ERROR_LIB_MAN_MOTE_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_5;
		return;
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_MOTE_STRUCT_NONE;
}
