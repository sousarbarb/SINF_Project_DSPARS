/*
 * manipulation_division_struct.h
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

#ifndef _MANIPULATION_DIVISION_STRUCT_H_
#define _MANIPULATION_DIVISION_STRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actuators_lib_struct.h"

#define ERROR_LIB_MAN_DIVISION_STRUCT_NONE 0
#define ERROR_LIB_MAN_DIVISION_STRUCT_1	   1
#define ERROR_LIB_MAN_DIVISION_STRUCT_2	   2
#define ERROR_LIB_MAN_DIVISION_STRUCT_3	   3
#define ERROR_LIB_MAN_DIVISION_STRUCT_4	   4
#define ERROR_LIB_MAN_DIVISION_STRUCT_5	   5
#define ERROR_LIB_MAN_DIVISION_STRUCT_6	   6
#define ERROR_LIB_MAN_DIVISION_STRUCT_7	   7
#define ERROR_LIB_MAN_DIVISION_STRUCT_8	   8

#define MAX_CHARACT_STRING	20
#define SIZE_LIB_MAN_DIVISION_LABEL_DIVISION_NAME   	20
#define SIZE_LIB_MAN_DIVISION_LABEL_DIVISION_SENSOR 	20
#define SIZE_LIB_MAN_DIVISION_LABEL_DIVISION_ACTUATOR	20
#define SIZE_STRING_BUFFER_1                     		100

typedef struct DIVISION_TEMP{
	char *division_name;
	int num_sensors;
	char **sensors;
	int num_actuator;
	actuator **division_actuators;
} division;

division ** division_vector_creation(int number_ivisions, int * error_func);

char *string_creation(int * error_func);

char **string_vector_sensors_creation(int number_sensors, int * error_func);

void free_vector_string_memory(char **vector_string, int number_sensors, int * error_func);

void free_memory_all(division **system_divisions, int number_divisions, int * error_func);

division ** insert_info_division_struct(int * number_divisions, int *error_func);

void search_division_actuator(division ** system_divisions, int number_divisions, char *division_name, char *division_actuator_id, int * index_division, int * index_actuator, int * error_func);

#endif
