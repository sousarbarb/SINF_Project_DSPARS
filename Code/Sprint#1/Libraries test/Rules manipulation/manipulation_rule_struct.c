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
	int counter, counter_aux;
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
			if(0 < system_rules[counter]->num_actuator_conditions){
				for(counter_aux = 0; counter_aux < system_rules[counter]->num_actuator_conditions; counter_aux++){
					free(system_rules[counter]->actuator_future_state[counter_aux]);
					system_rules[counter]->actuator_future_state[counter_aux] = NULL;
				}
			}
			free(system_rules[counter]->actuator_future_state);
			system_rules[counter]->actuator_future_state = NULL;
		}
		free(system_rules[counter]);
		system_rules[counter] = NULL;
	}
	free(system_rules);
	system_rules = NULL;
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
			printf("    Condition 1: %s %c %d\n", system_rules[counter]->sensor_condition_1, system_rules[counter]->operator_condition_1, system_rules[counter]->value_condition_1);
		if('_' != system_rules[counter]->logic_operator_condition_1_2[0])
			printf("    Logic operator between condition 1 and 2: %s\n", system_rules[counter]->logic_operator_condition_1_2);
		if(comparison_operators[0] != system_rules[counter]->operator_condition_2)
			printf("    Condition 2: %s %c %d\n", system_rules[counter]->sensor_condition_2, system_rules[counter]->operator_condition_2, system_rules[counter]->value_condition_2);
		if(NULL != system_rules[counter]->actuator_future_state && 0 < system_rules[counter]->num_actuator_conditions)
			print_actuator_future_state_memory_vector(system_rules[counter]->actuator_future_state, system_rules[counter]->num_actuator_conditions,NULL);
	}
	printf("\n");
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
}

int rules_association_to_structures(rule ***system_rules, int *number_rules, int number_motes, int * error_func){
	// Function's code
	int counter1, counter2, counter3, counter4, error_1, integer_aux1;
	char charac1, string_aux1[SIZE_STRING_BUFFER_1], string_aux2[SIZE_STRING_BUFFER_2];
	
	// - TEMPLATE
	printf("\n++++++++++ TEMPLATE FOR THE FILE SensorRules.txt ++++++++++\n\n");
	printf("Type of comparasion operators that are available to choose: '<' or '>'\n");
	printf("Type of logic operators that are available to choose: \"AND\" or \"OR\"\n");
	printf("Possibles actuator's state: \"ON\" or \"OFF\"");
	printf("Maximum ratings:\n");
	printf("    Size of division's name:            %d characters\n", SIZE_LIB_MAN_RULE_LABEL_DIVISION_NAME);
	printf("    Size of sensor's name:              %d characters\n", SIZE_LIB_MAN_RULE_LABEL_SENSOR_CONDITION);
	printf("    Size of actuator's pretended state: %d characters\n", SIZE_LIB_MAN_RULE_LABEL_ACTUAT_FUT_STATE);
	printf("    Size of rule's characterization:    %d characters\n", SIZE_STRING_BUFFER_1);
	printf("Examples of rules definition in the file:\n\n");
	printf("    (rule 1)    <room_name>: <sensor_name_1><condition_1><value_1> [AND / OR <sensor_name_2><condition_2><value_2>] <actuator>:<state>[,<actuator>:<state>]\n");
	printf("    (rule 2)    <room_name>: <sensor_name_1><condition_1><value_1> [AND / OR <sensor_name_2><condition_2><value_2>] <actuator>:<state>[,<actuator>:<state>]\n");
	printf("...\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
	
	// - SensorRules.txt existence verification by the user
	printf("The file that contains the rules of the system is already created and the rules especified by the template showned before (Y - yes / N - no, and press Enter)? ");
	charac1 = getchar();
	getchar();
	if(('N' == charac1) || ('n' == charac1)){
		printf("Then, if you want to supervise the system please create the file.\n");
		
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
		return -1;
	}
	
	// - SensorRules.txt existence verification by the program
	sensor_rules_file = fopen("SensorRules.txt", "r");
	if(NULL == sensor_rules_file){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: File SensorRule.txt doesn't exist.\n", ERROR_LIB_MAN_RULE_STRUCT_7);
		printf("Please, create the file according to the following template:\n");
		printf("\n++++++++++ TEMPLATE FOR THE FILE SensorRules.txt ++++++++++\n\n");
		printf("Type of comparasion operators that are available to choose: '<' or '>'\n");
		printf("Type of logic operators that are available to choose: \"AND\" or \"OR\"\n");
		printf("Possibles actuator's state: \"ON\" or \"OFF\"");
		printf("Maximum ratings:\n");
		printf("    Size of division's name:            %d characters\n", SIZE_LIB_MAN_RULE_LABEL_DIVISION_NAME);
		printf("    Size of sensor's name:              %d characters\n", SIZE_LIB_MAN_RULE_LABEL_SENSOR_CONDITION);
		printf("    Size of actuator's pretended state: %d characters\n", SIZE_LIB_MAN_RULE_LABEL_ACTUAT_FUT_STATE);
		printf("    Size of rule's characterization:    %d characters\n", SIZE_STRING_BUFFER_1);
		printf("Examples of rules definition in the file:\n\n");
		printf("    (rule 1)    <room_name>: <sensor_name_1><condition_1><value_1> [AND / OR <sensor_name_2><condition_2><value_2>] <actuator>:<state>[,<actuator>:<state>]\n");
		printf("    (rule 2)    <room_name>: <sensor_name_1><condition_1><value_1> [AND / OR <sensor_name_2><condition_2><value_2>] <actuator>:<state>[,<actuator>:<state>]\n");
		printf("...\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
		
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_7;
		return -1;
	}
	
	// - Insertion of rule's number to the program know how many it needs to process
	printf("\n");
	do{
		printf("OBS - the number of rules specified must correspond to the number of rules in the file SensorRule.txt\n");
		printf("Please insert the number of rules that the system needs to atend (number of rules must be grater than 0): ");
		scanf(" %d", number_rules);
		getchar();
		
		if(1 > (*number_rules))
			printf("Number of rules INVALID.\n");
	} while(1 > (*number_rules));
	
	// - Creation of the rule's vector of structures that will contain the information relative to the rules aplied to the system
	(*system_rules) = rules_system_vector_creation((*number_rules), &error_1);
	if(NULL == system_rules || 0 < error_1){
		printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Memory allocated failed. It wans't possible to create the vector with the use of the function rules_system_vector_creation.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
		return -1;
	}
	
	for(counter1 = 0; counter1 < (*number_rules); counter1++){
		// - Reading the rule from the file
		if(NULL == fgets(string_aux1, SIZE_STRING_BUFFER_1, sensor_rules_file)){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: It wasn't possible to associate the number of rules specified by the user with the number of lines in the text file.\n", ERROR_LIB_MAN_RULE_STRUCT_8);
			printf("    Number of rules specified by the user on the program: %d\n    Number of rules read by the program: %d\n", (*number_rules), counter1);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_8;
			return -1;
		}
		integer_aux1 = strlen(string_aux1);
		if('\n' == string_aux1[integer_aux1 - 1])
			string_aux1[integer_aux1 - 1] = '\0';
		if(0 == (integer_aux1 - 1)){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Rule's description invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_9);
			printf("    The problem was found in the RULE %d\n", counter1 + 1);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_9;
			return -1;
		}
		integer_aux1--;
			
		// - Rule's printing for debbuging
		printf("[RULE %d] %s\n", counter1 + 1, string_aux1);
		
		// - Processing division's name
		for(counter2 = 0; ':' != string_aux1[counter2] && counter2 < integer_aux1 && counter2 < SIZE_STRING_BUFFER_2; counter2++)
			string_aux2[counter2] = string_aux1[counter2];
		string_aux2[counter2] = '\0';
		if(integer_aux1 == counter2){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Rule's description invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_9);
			printf("    The problem was found in the RULE %d\n", counter1 + 1);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_9;
			return -1;
		}
		counter2++;
		strcpy((*system_rules)[counter1]->division_name, string_aux2);
		
		// - Processing condition 1
		while(' ' == string_aux1[counter2] && counter2 < integer_aux1)
			counter2++;
		if(integer_aux1 == counter2){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Rule's description invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_9);
			printf("    The problem was found in the RULE %d\n", counter1 + 1);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_9;
			return -1;
		}
		else if(counter2 < integer_aux1 && (':' == string_aux1[counter2] || comparison_operators[1] == string_aux1[counter2] || comparison_operators[2] == string_aux1[counter2])){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Rule's description invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_9);
			printf("    The problem was found in the RULE %d\n", counter1 + 1);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_9;
			return -1;
		}
		
		for(counter3 = 0; (comparison_operators[1] != string_aux1[counter2] && comparison_operators[2] != string_aux1[counter2]) && counter2 < integer_aux1 && counter3 < SIZE_STRING_BUFFER_2; counter3++){
			string_aux2[counter3] = string_aux1[counter2];
			counter2++;
		}
		string_aux2[counter3] = '\0';
		if(integer_aux1 == counter2){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Rule's description invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_9);
			printf("    The problem was found in the RULE %d\n", counter1 + 1);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_9;
			return -1;
		}
		
		if(1 != validation_of_sensor(string_aux2, number_motes)){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: The sensor especified in the condition doesn't exist.\n", ERROR_LIB_MAN_RULE_STRUCT_11);
			printf("    The problem was found in the RULE %d\n", counter1 + 1);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_11;
			return -1;
		}
		
		strcpy((*system_rules)[counter1]->sensor_condition_1, string_aux2);
		(*system_rules)[counter1]->operator_condition_1 = string_aux1[counter2];
		counter2++;
		(*system_rules)[counter1]->value_condition_1 = conversion_of_a_piece_of_a_string_into_integer(string_aux1, counter2, &counter2, integer_aux1);
		
		// - Processing condition 2
		while(' ' == string_aux1[counter2] && counter2 < integer_aux1)
			counter2++;
		if(integer_aux1 == counter2){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Rule's description invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_9);
			printf("    The problem was found in the RULE %d\n", counter1 + 1);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_9;
			return -1;
		}
		
		if((lib_man_rule_logic_operators[1][0] == string_aux1[counter2] && lib_man_rule_logic_operators[1][1] == string_aux1[counter2+1] && lib_man_rule_logic_operators[1][2] == string_aux1[counter2+2] && ' ' == string_aux1[counter2+3])  ||  (lib_man_rule_logic_operators[2][0] == string_aux1[counter2] && lib_man_rule_logic_operators[2][1] == string_aux1[counter2+1] && ' ' == string_aux1[counter2+2])){
			if(lib_man_rule_logic_operators[1][0] == string_aux1[counter2] && lib_man_rule_logic_operators[1][1] == string_aux1[counter2+1] && lib_man_rule_logic_operators[1][2] == string_aux1[counter2+2] && ' ' == string_aux1[counter2+3]){
				// LOGIC OPERATOR: AND
				strcpy((*system_rules)[counter1]->logic_operator_condition_1_2, lib_man_rule_logic_operators[1]);
				counter2 = counter2 + 3;
			}
			else if(lib_man_rule_logic_operators[2][0] == string_aux1[counter2] && lib_man_rule_logic_operators[2][1] == string_aux1[counter2+1] && ' ' == string_aux1[counter2+2]){
				// LOGIC OPERATOR: OR
				strcpy((*system_rules)[counter1]->logic_operator_condition_1_2, lib_man_rule_logic_operators[2]);
				counter2 = counter2 + 2;
			}
			while(' ' == string_aux1[counter2] && counter2 < integer_aux1)
				counter2++;
			if(integer_aux1 == counter2){
				printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Rule's description invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_9);
				printf("    The problem was found in the RULE %d\n", counter1 + 1);
				if(NULL != error_func)
					(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_9;
				return -1;
			}
			for(counter3 = 0; (comparison_operators[1] != string_aux1[counter2] && comparison_operators[2] != string_aux1[counter2]) && counter2 < integer_aux1 && counter3 < SIZE_STRING_BUFFER_2; counter3++){
				string_aux2[counter3] = string_aux1[counter2];
				counter2++;
			}
			string_aux2[counter3] = '\0';
			if(integer_aux1 == counter2){
				printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Rule's description invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_9);
				printf("    The problem was found in the RULE %d\n", counter1 + 1);
				if(NULL != error_func)
					(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_9;
				return -1;
			}
			
			if(1 != validation_of_sensor(string_aux2, number_motes)){
				printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: The sensor especified in the condition doesn't exist.\n", ERROR_LIB_MAN_RULE_STRUCT_11);
				printf("    The problem was found in the RULE %d\n", counter1 + 1);
				if(NULL != error_func)
					(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_11;
				return -1;
			}
			
			strcpy((*system_rules)[counter1]->sensor_condition_2, string_aux2);
			(*system_rules)[counter1]->operator_condition_2 = string_aux1[counter2];
			counter2++;
			(*system_rules)[counter1]->value_condition_2 = conversion_of_a_piece_of_a_string_into_integer(string_aux1, counter2, &counter2, integer_aux1);
		}
		else{
			strcpy((*system_rules)[counter1]->logic_operator_condition_1_2, lib_man_rule_logic_operators[0]);
			(*system_rules)[counter1]->sensor_condition_2[0] = '\0';
			(*system_rules)[counter1]->operator_condition_2 = comparison_operators[0];
			(*system_rules)[counter1]->value_condition_2 = 0;
		}
		
		// - Processing actuators
		while(' ' == string_aux1[counter2] && counter2 < integer_aux1)
			counter2++;
		if(integer_aux1 == counter2){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Rule's description invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_9);
			printf("    The problem was found in the RULE %d\n", counter1 + 1);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_9;
			return -1;
		}
		counter4 = 0;
		for(counter3 = counter2; counter3 < integer_aux1 && '\0' != string_aux1[counter3]; counter3++)
			if(':' == string_aux1[counter3])
				counter4++;
		(*system_rules)[counter1]->actuator_future_state = actuator_future_state_vector_creation(counter4, SIZE_LIB_MAN_RULE_LABEL_ACTUAT_FUT_STATE, &error_1);
		if(NULL == (*system_rules)[counter1]->actuator_future_state || 0 < error_1){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Memory allocated failed. It wans't possible to create the vector with the use of the function rules_system_vector_creation.\n", ERROR_LIB_MAN_RULE_STRUCT_3);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_3;
			return -1;
		}
		(*system_rules)[counter1]->num_actuator_conditions = counter4;
		
		for(counter3 = 0; (counter3 < (*system_rules)[counter1]->num_actuator_conditions) && ('\0' != string_aux1[counter2]); counter3++){
			counter4 = 0;
			while(' ' != string_aux1[counter2] && '\0' != string_aux1[counter2]){
				(*system_rules)[counter1]->actuator_future_state[counter3][counter4] = string_aux1[counter2];
				counter2++;
				counter4++;
			}
			(*system_rules)[counter1]->actuator_future_state[counter3][counter4] = '\0';
			while(' ' == string_aux1[counter2] && '\0' != string_aux1[counter2] && counter2 < integer_aux1)
				counter2++;
		}
		if(counter3 < (*system_rules)[counter1]->num_actuator_conditions){
			printf("[ERROR_LIB_MAN_RULE_STRUCT %d] rules_association_to_structures: Rule's description invalid.\n", ERROR_LIB_MAN_RULE_STRUCT_9);
			printf("    The problem was found in the RULE %d\n", counter1 + 1);
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_9;
			return -1;
		}
	}
	
	fclose(sensor_rules_file);
	sensor_rules_file = NULL;
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_RULE_STRUCT_NONE;
	return 0;
}

int conversion_of_a_piece_of_a_string_into_integer(char * string, int start_index, int * end_index, int string_size){
	int integer_value = 0;
	while(0 <= ((int)(string[start_index]-'0')) && 10 > ((int)(string[start_index]-'0')) && start_index < string_size){
		integer_value = integer_value*10 + ((int)(string[start_index]-'0'));
		start_index++;
	}
	(*end_index) = start_index;
	return integer_value;
}

int validation_of_sensor(char * sensor, int number_motes){
	int integer_aux_1=0, integer_aux_2, numeration_sensor;
	switch(sensor[0]){
		case 'L': integer_aux_1=5; break;
		case 'H': integer_aux_1=3; break;
		case 'T': integer_aux_1=4; break;
		default: return -1;
	}
	if('\0' == sensor[integer_aux_1])
		return 1;
	numeration_sensor = conversion_of_a_piece_of_a_string_into_integer(sensor, integer_aux_1, &integer_aux_2, strlen(sensor));
	if(numeration_sensor <= number_motes)
		return 1;
	else
		return 0;
}

