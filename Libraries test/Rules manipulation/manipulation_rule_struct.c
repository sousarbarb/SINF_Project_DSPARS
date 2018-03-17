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
			if(0 < counter){
				free(vector_act);
				vector_act = NULL;
			}
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
	printf("\n    +++++ ACTUATOR'S FUTURE STATE VECTOR +++++\n");
	for(counter = 0; counter < number_actfutstat; counter++)
		printf("    [%d] %s\n", counter, vector_act[counter]);
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

rule **rules_system_vector_creation(int number_rules, int * error_func){
	// Argument error checking
	if(1 > number_rules){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_system_vector_creation: Number of rules invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_5;
		return NULL;
	}
	
	
	// Function's code
	int counter, error_return = 0;
	rule **system_rules = NULL;
	system_rules = (rule **) malloc(sizeof(rule *) * number_rules);
	if(NULL == system_rules){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_system_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
		return NULL;
	}
	for(counter = 0; counter < number_rules; counter++){
		system_rules[counter] = (rule *) malloc(sizeof(rule));
		if(NULL == system_rules[counter]){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_system_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
			if(0 < counter)
				free_rules_system_memory(system_rules, counter, &error_return);
			else{
				free(system_rules);
				system_rules = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
			return NULL;
		}
		
		system_rules[counter]->division_name = string_creation(SIZE_LIB_MAN_RULE_LABEL_DIVISION_NAME, &error_return);
		if((NULL == system_rules[counter]->division_name) || (0 < error_return)){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_system_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
			if(0 < counter)
				free_rules_system_memory(system_rules, counter, &error_return);
			else{
				free(system_rules[counter]);
				system_rules[counter] = NULL;
				free(system_rules);
				system_rules = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
			return NULL;
		}
		system_rules[counter]->division_name[0] = '\0';
		
		error_return = 0;
		
		system_rules[counter]->sensor_condition_1 = string_creation(SIZE_LIB_MAN_RULE_LABEL_SENSOR_CONDITION, &error_return);
		if((NULL == system_rules[counter]->sensor_condition_1) || (0 < error_return)){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_system_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
			if(0 < counter)
				free_rules_system_memory(system_rules, counter, &error_return);
			else{
				free(system_rules[counter]);
				system_rules[counter] = NULL;
				free(system_rules[counter]->division_name);
				system_rules[counter]->division_name = NULL;
				free(system_rules);
				system_rules = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
			return NULL;
		}
		system_rules[counter]->sensor_condition_1[0] = '\0';
		
		error_return = 0;
		
		system_rules[counter]->sensor_condition_2 = string_creation(SIZE_LIB_MAN_RULE_LABEL_SENSOR_CONDITION, &error_return);
		if((NULL == system_rules[counter]->sensor_condition_2) || (0 < error_return)){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_system_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
			if(0 < counter)
				free_rules_system_memory(system_rules, counter, &error_return);
			else{
				free(system_rules[counter]);
				system_rules[counter] = NULL;
				free(system_rules[counter]->division_name);
				system_rules[counter]->division_name = NULL;
				free(system_rules[counter]->sensor_condition_1);
				system_rules[counter]->sensor_condition_1 = NULL;
				free(system_rules);
				system_rules = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
			return NULL;
		}
		system_rules[counter]->sensor_condition_2[0] = '\0';
		
		error_return = 0;
		
		strcpy(system_rules[counter]->logic_operator_condition_1_2, lib_man_rule_logic_operators[0]);
		system_rules[counter]->actuator_future_state = NULL;
		system_rules[counter]->operator_condition_1  = comparison_operators[0];
		system_rules[counter]->operator_condition_2  = comparison_operators[0];
		system_rules[counter]->num_actuator_conditions = 0;
		system_rules[counter]->value_condition_1 = 0;
		system_rules[counter]->value_condition_2 = 0;
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
	return system_rules;
}

void free_rules_system_memory(rule **system_rules, int number_rules, int * error_func){
	// Argument error checking
	if(NULL == system_rules){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] free_rules_system_memory: Pointer to rule's system vector invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_6);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_6;
		return;
	}
	else if(1 > number_rules){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] vector_actuator_future_state_creation: Number of future actuator state condition invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_1;
		return;
	}
	
	
	// Function's code
	int counter;
	for(counter = 0; counter < number_rules; counter++){
		if(NULL != system_rules[counter]->division_name){
			free(system_rules[counter]->division_name);
			system_rules[counter]->division_name = NULL;
		}
		if(NULL != system_rules[counter]->sensor_condition_1){
			free(system_rules[counter]->sensor_condition_1);
			system_rules[counter]->sensor_condition_1 = NULL;
		}
		if(NULL != system_rules[counter]->sensor_condition_2){
			free(system_rules[counter]->sensor_condition_2);
			system_rules[counter]->sensor_condition_1 = NULL;
		}
		if(NULL != system_rules[counter]->actuator_future_state){
			free(system_rules[counter]->actuator_future_state);
			system_rules[counter]->actuator_future_state = NULL;
		}
		free(system_rules[counter]);
		system_rules[counter] = NULL;
	}
	printf("XXX\n");
	free(system_rules);
	system_rules = NULL;
	printf("XXX\n");
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
}

void print_rules_system_vector(rule **system_rules, int number_rules, int * error_func){
	// Argument error checking
	if(NULL == system_rules){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] print_rules_system_vector: Pointer to rule's system vector invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_6);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_6;
		return;
	}
	else if(1 > number_rules){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] print_rules_system_vector: Number of future actuator state condition invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_1;
		return;
	}
	
	
	// Function's code
	int counter;
	printf("\n+++++ DIVISION RULES +++++\n");
	for(counter = 0; counter < number_rules; counter++){
		printf("[----- DIVISION %s -----]\n", system_rules[counter]->division_name);
		if(comparison_operators[0] != system_rules[counter]->operator_condition_1)
			printf("    Condition 1: %s %c %.2f\n", system_rules[counter]->sensor_condition_1, system_rules[counter]->operator_condition_1, system_rules[counter]->value_condition_1);
		if(comparison_operators[0] != system_rules[counter]->operator_condition_2)
			printf("    Condition 2: %s %c %.2f\n", system_rules[counter]->sensor_condition_2, system_rules[counter]->operator_condition_2, system_rules[counter]->value_condition_2);
		if(NULL != system_rules[counter]->actuator_future_state && 0 < system_rules[counter]->num_actuator_conditions)
			print_actuator_future_state_memory_vector(system_rules[counter]->actuator_future_state, system_rules[counter]->num_actuator_conditions,NULL);
	}
	printf("\n");
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
}

