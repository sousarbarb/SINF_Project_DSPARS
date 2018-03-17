/*
 * actuatores_lib_struct.h
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


#ifndef _ACTUATORES_LIB_STRUCT_H_
#define _ACTUATORES_LIB_STRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_LIB_ACTUATORES_STRUCT_NONE	0
#define ERROR_LIB_ACTUATORES_STRUCT_1		1
#define ERROR_LIB_ACTUATORES_STRUCT_2	  	2
#define ERROR_LIB_ACTUATORES_STRUCT_3	   	3
#define ERROR_LIB_ACTUATORES_STRUCT_4	   	4
#define ERROR_LIB_ACTUATORES_STRUCT_5	   	5

#define	SIZE_OF_ACTUATION					20

typedef struct ACTUATOR_TEMP{
	int id;
	char actuation[SIZE_OF_ACTUATION];
} actuator;

actuator ** mote_vector_creation(int number_actuatores, int * error_func);

void free_actuation_memory(actuator** system_actuatores, int number_actuatores, int * error_func);

int search_actuator(actuator** system_actuatores, int number_actuatores, int actuator_id, int * error_func);

void print_actuation_vector(actuator** system_actuatores, int number_actuatores, int * error_func);

void print_single_actuator(actuator** system_actuatores, int number_actuatores, int actuator_id, int * error_func);

#endif
