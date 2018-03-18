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
#define ERROR_LIB_MAN_RULE_STRUCT_5	   5
#define ERROR_LIB_MAN_RULE_STRUCT_6	   6
#define ERROR_LIB_MAN_RULE_STRUCT_7	   7
#define ERROR_LIB_MAN_RULE_STRUCT_8	   8
#define ERROR_LIB_MAN_RULE_STRUCT_9	   9
#define ERROR_LIB_MAN_RULE_STRUCT_10   10
#define ERROR_LIB_MAN_RULE_STRUCT_11   11

#define SIZE_LIB_MAN_RULE_LABEL_DIVISION_NAME    20
#define SIZE_LIB_MAN_RULE_LABEL_SENSOR_CONDITION 20
#define SIZE_LIB_MAN_RULE_LABEL_ACTUAT_FUT_STATE 20
#define SIZE_LIB_MAN_RULE_LABEL_LOG_OPERATORS    4
#define SIZE_LIB_MAN_RULE_LABEL_ACTUAT_STATES    4
#define SIZE_STRING_BUFFER_1                     100
#define SIZE_STRING_BUFFER_2                     20

#define NUMBER_LIB_MAN_RULE_COMP_OPERATOR 3
#define NUMBER_LIB_MAN_RULE_LOG_OPERATORS 3
#define NUMBER_LIB_MAN_RULE_ACTUAT_STATES 2

typedef struct RULE_TEMP{
	char  *division_name;
	char  *sensor_condition_1;
	char  operator_condition_1;
	int   value_condition_1;
	char  logic_operator_condition_1_2[SIZE_LIB_MAN_RULE_LABEL_LOG_OPERATORS];
	char  *sensor_condition_2;
	char  operator_condition_2;
	int   value_condition_2;
	int   num_actuator_conditions;
	char  **actuator_future_state;
} rule;

FILE *sensor_rules_file;

static const char comparison_operators[] = {'_', '<','>'};
static const char lib_man_rule_logic_operators[NUMBER_LIB_MAN_RULE_LOG_OPERATORS][SIZE_LIB_MAN_RULE_LABEL_LOG_OPERATORS]  = {"___", "AND", "OR"};
static const char lib_man_rule_actuators_state[NUMBER_LIB_MAN_RULE_ACTUAT_STATES][SIZE_LIB_MAN_RULE_LABEL_ACTUAT_STATES]  = {"ON", "OFF"};

char **actuator_future_state_vector_creation(int number_actfutstat, int max_charact_string, int * error_func);

void print_actuator_future_state_memory_vector(char **vector_act, int number_actfutstat, int * error_func);

char *string_creation(int max_charact_string, int * error_func);

rule **rules_system_vector_creation(int number_rules, int * error_func);

void free_rules_system_memory(rule **system_rules, int number_rules, int * error_func);

void print_rules_system_vector(rule **system_rules, int number_rules, int * error_func);

int rules_association_to_structures(rule ***system_rules, int *number_rules, int number_motes, int * error_func);

int conversion_of_a_piece_of_a_string_into_integer(char * string, int start_index, int * end_index, int string_size);

int validation_of_sensor(char * sensor, int number_motes);

#endif
