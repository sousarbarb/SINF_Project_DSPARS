/*
 * manipulation_division_struct.c
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
/*******************************************************************
 * SENSOR CONFIGURATION TEMPLATE
 * 
 * <room_name>:<sensor_name>[,<sensor_name>];<actuator>[,actuator]
 * *****************************************************************/

#include "manipulation_division_struct.h"
#include "actuators_lib_struct.h"

division ** division_vector_creation(int number_divisions, int * error_func) {
	// Argument error checking
	if(1 > number_divisions){
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] division_vector_creation: Number of divisions invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_1;
		return NULL;
	}
	
	// Function's code
	int counter;
	division **system_divisions = NULL;
	system_divisions = (division **) malloc(sizeof(division *) * number_divisions);
	if(NULL == system_divisions) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] division_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
		free(system_divisions);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_3;
		return NULL;
	}
	for(counter = 0; counter < number_divisions; counter++){
		system_divisions[counter] = (division *) malloc(sizeof(division));
		if(NULL == system_divisions[counter]){
			printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] division_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
				free(system_divisions[counter]);
				system_divisions = NULL;
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_3;
			return NULL;
		}
		system_divisions[counter]->division_name = string_division_creation(NULL);
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	printf("Número de divisões: %d\n",number_divisions);
	return system_divisions;
}

char *string_division_creation(int * error_func) {
	// Function's code
	char *string = NULL;
	string = (char *) malloc(SIZE_LIB_MAN_DIVISION_LABEL_DIVISION_NAME);
	if(NULL == string){
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] string_division_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_3;
		return NULL;
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	return string;
}

char **string_vector_sensors_creation(int number_sensors, int * error_func) {
	// Argument error checking
	if(1 > number_sensors){
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] string_vector_sensors_crestion: Number of sensors invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_1;
		return NULL;
	}

	// Function's code
	int counter;
	char **vector_sensors = NULL;
	vector_sensors = (char **) malloc(sizeof(char *) * number_sensors);
	if(NULL == vector_sensors){
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] string_vector_sensors_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
		free(vector_sensors);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_3;
		return NULL;
	}
	for(counter = 0; counter < number_sensors; counter++){
		vector_sensors[counter] = string_division_creation(NULL);
		if(NULL == vector_sensors[counter]){
			printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] vector_actuator_future_state_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
			if(0 < counter)
				free_vector_string_memory(vector_sensors, number_sensors, NULL);
			else{
				free(vector_sensors);
				vector_sensors = NULL;
			}
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_3;
			return NULL;
		}
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	return vector_sensors;
}
	
void free_vector_string_memory(char **vector_string, int number_sensors, int * error_func){
	// Argument error checking
	if(NULL == vector_string) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] free_vector_string_memory: Pointer to the string's vector invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_5;
		return;
	}
	if(1 > number_sensors) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] free_vector_string_memory:  Number of sensores invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_2);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_2;
		return;
	}
		
	// Function's code
	int counter;
	for(counter = 0; counter < number_sensors; counter++) {
		free(vector_string[counter]);
		vector_string[counter] = NULL;
	}
	free(vector_string);
	vector_string = NULL;
	if(NULL != error_func) {
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	}
}

void free_memory_all(division **system_divisions, int number_divisions, int * error_func) {
	// Argument error checking
	if(NULL == system_divisions) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] free_memory_all: Pointer to the division struct invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_5;
		return;
	}
	if(1 > number_divisions)  {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] free_memory_all: Number of divisions invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_1;
		return;
	}

	// Function´s code
	int counter;
	for(counter = 0; counter < number_divisions; counter++) {
		free(system_divisions[counter]->division_name);
		free_vector_string_memory(system_divisions[counter]->sensors, system_divisions[counter]->num_sensors, NULL);
		free(system_divisions[counter]);
		free_actuation_memory(system_divisions[counter]->division_actuators, system_divisions[counter]->num_actuator, NULL);	
	}
	free(system_divisions);
	system_divisions = NULL;
	if(NULL != error_func) {
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	}
	return;
}

division ** insert_info_division_struct(int * number_divisions, int *error_func) {
	//Function's code 
	FILE *sensor_configuration = NULL;
	division **system_divisions = NULL;
	int counter1, counter2, aux_virgula = 0, counter3, counter4 = 0, counter6, counter7 = 0, counter8 = 0, counter9 = 0, counter10 = 0, counter11 = 0, aux_actuators = 0;
	char str_file[SIZE_STRING_BUFFER_1];
	char str_aux1[SIZE_STRING_BUFFER_1], str_aux2[SIZE_STRING_BUFFER_1], str_aux3[SIZE_STRING_BUFFER_1];
	
	do{
		printf("Insert the number of divisions to consider in the system (needs to be greater than 0): ");
		scanf(" %d", number_divisions);
		getchar();
	} while(1 > (*number_divisions));
	system_divisions = division_vector_creation((*number_divisions), NULL);	
	sensor_configuration = fopen("SensorConfiguration.txt", "r");
	if(NULL == sensor_configuration) {
		printf("When you write the sensors configuration .txt file, please follow the instructions below\n");
		printf("\n++++++++++ TEMPLATE FOR THE FILE SensorConfiguration.txt ++++++++++\n\n");
		printf("Maximum ratings:\n");
		printf("    Size of division's name:            %d characters\n", SIZE_LIB_MAN_DIVISION_LABEL_DIVISION_NAME);
		printf("    Size of sensor's name:              %d characters\n", SIZE_LIB_MAN_DIVISION_LABEL_DIVISION_SENSOR);
		printf("    Size of actuator's name: %d characters\n", SIZE_LIB_MAN_DIVISION_LABEL_DIVISION_ACTUATOR);
		printf("    Size of configuration's characterization:    %d characters\n", SIZE_STRING_BUFFER_1);
		printf("Examples of sensors configuration in the file:\n\n");
		printf("    (Configuration 1)    <room_name>:<sensor_name>[,<sensor_name>];<actuator>[,actuator]\n");
		printf("    (Configuration 2)    <room_name>:<sensor_name>[,<sensor_name>];<actuator>[,actuator]\n");
		printf("...\n\n+++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
		if(NULL != error_func) {
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_5;
			return NULL;
		}
	}
	for(counter6 = 0; fgets(str_file, SIZE_STRING_BUFFER_1, sensor_configuration) != NULL; counter6++) {
		if('\n' == str_file[strlen(str_file) - 1])
			str_file[strlen(str_file) - 1] = '\0';
		
		for(counter1 = 0; ':' != str_file[counter1]; counter1++) {
			str_aux1[counter1] = str_file[counter1];
		}
		str_aux1[counter1] = '\0';
		strcpy(system_divisions[counter6]->division_name, str_aux1);
		counter1++;
		for(counter2 = counter1; ';' != str_file[counter2]; counter2++) {
			str_aux2[counter2-counter1] = str_file[counter2];
			if(',' == str_file[counter2])
				aux_virgula++;
		}
		str_aux2[counter2-counter1] = '\0';
		counter2++;
		aux_virgula++;
		system_divisions[counter6]->num_sensors = aux_virgula;
		system_divisions[counter6]->sensors = string_vector_sensors_creation(aux_virgula, NULL);
		aux_virgula = 0;
		for(counter3 = 0; '\0' != str_aux2[counter3]; counter3++) {
			system_divisions[counter6]->sensors[counter4][counter7] = str_aux2[counter3];
			counter7++;
			if(',' == str_aux2[counter3]) {
				system_divisions[counter6]->sensors[counter4][counter7-1] = '\0';
				counter4++;
				counter7 = 0;
			}
			else if('\0' == str_aux2[counter3+1]) {
				system_divisions[counter6]->sensors[counter4][counter7] = '\0';
				puts(system_divisions[counter6]->sensors[counter4]);
			}
			puts(system_divisions[counter6]->sensors[counter4]);
		}
		for(counter8 = counter2; '\0' != str_file[counter8]; counter8++) {
			str_aux3[counter8-counter2] = str_file[counter8];
			if(',' == str_file[counter8])
				aux_actuators++;
		}
		str_aux3[counter8-counter2] = '\0';
		aux_actuators++;
		system_divisions[counter6]->num_actuator = aux_actuators;
		system_divisions[counter6]->division_actuators = actuators_vector_creation(aux_actuators, NULL);
		aux_actuators = 0;
		for(counter9 = 0;'\0' != str_aux3[counter9]; counter9++) {
			system_divisions[counter6]->division_actuators[counter10]->id[counter11] = str_aux3[counter9];
			counter11++;
			if(',' == str_aux3[counter9]) {
				system_divisions[counter6]->division_actuators[counter10]->id[counter11-1] = '\0';
				counter10++;
				counter11 = 0;
			}
			if('\0' == str_aux3[counter9+1])
				system_divisions[counter6]->division_actuators[counter10]->id[counter11] = '\0';
		}
		counter10 = 0;
		counter11 = 0;
		counter7 = 0;
		counter4 = 0;
	}
	fclose(sensor_configuration);
	sensor_configuration = NULL;
	
	if(NULL != error_func) {
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	}
	return system_divisions;
}

void search_division_actuator(division ** system_divisions, int number_divisions, char *division_name, char *division_actuator_id, int * index_division, int * index_actuator, int * error_func) {
	//Argument's checking
	if(NULL == system_divisions) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] search_division_actuator: Pointer to the division struct invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_5;
		return;
	}
	if(1 > number_divisions)  {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] search_division_actuator: Number of divisions invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_1;
		return;
	}
	if(NULL == division_name) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] search_division_actuator: POinter to the division invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_8);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_8;
		return;
	}
	if(NULL == division_actuator_id) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] search_division_actuator: Pointer to the division actuator name invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_8);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_8;
		return;
	}
	//Function's code
	int counter1 = 0;
	
	for(counter1 = 0; system_divisions[counter1]; counter1++) {
		if(strcmp(division_name, system_divisions[counter1]->division_name)==0) {
			break;
		}
	}
	(*index_division) = counter1;
	(*index_actuator) = search_actuator(system_divisions[counter1]->division_actuators, system_divisions[counter1]->num_actuator, division_actuator_id, NULL);

	if(NULL != error_func) {
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	}
}

void print_division_struct(division ** system_divisions, int number_divisions, int * error_func) {
	// Argument's error check
	if(NULL == system_divisions) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] print_division_struct: Pointer to the division struct invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_5;
		return;
	}
	if(1 > number_divisions)  {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] print_division_struct: Number of divisions invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_1;
		return;
	}
	
	// Function's code
	int counter1 = 0, counter2 = 0;
	
	for(counter1 = 0; counter1 < number_divisions; counter1++) {
		printf("\n\n------DIVISION Nº%d------\n\n", counter1+1);
		printf("[DIVISION NAME] %s\n", system_divisions[counter1]->division_name);
		for(counter2 = 0; counter2 < system_divisions[counter1]->num_sensors; counter2++)
			printf("[SENSOR] %s\n", system_divisions[counter1]->sensors[counter2]);
		for(counter2 = 0; counter2 < system_divisions[counter1]->num_actuator; counter2++)
			printf("[ACTUATOR] %s\n", system_divisions[counter1]->division_actuators[counter2]->id);
	}

	if(NULL != error_func) {
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	}
}
