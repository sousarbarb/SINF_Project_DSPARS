/*
 * manipulation_rule_struct.h
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


#ifndef _MANIPULATION_RULE_STRUCT_H_
#define _MANIPULATION_RULE_STRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_LIB_MAN_RULE_STRUCT_NONE 0
#define ERROR_LIB_MAN_RULE_STRUCT_1	   1
#define ERROR_LIB_MAN_RULE_STRUCT_2	   2
#define ERROR_LIB_MAN_RULE_STRUCT_3	   3
#define ERROR_LIB_MAN_RULE_STRUCT_4	   4

typedef struct RULE_TEMP{
	char *division_name;
	char *sensor_condition_1;
	char operator_condition_1;
	float value_condition_1;
	char *sensor_condition_2;
	char operator_condition_2;
	float value_condition_2;
	int num_actuator_conditions;
	char **actuator_future_state;
} rule;

char **actuator_future_state_vector_creation(int number_actfutstat, int max_charact_string, int * error_func);

void free_actuator_future_state_memory(char **vector_act, int number_actfutstat, int * error_func);

void print_actuator_future_state_memory_vector(char **vector_act, int number_actfutstat, int * error_func);

#endif
