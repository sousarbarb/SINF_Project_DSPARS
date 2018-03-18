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
//#include "actuators_lib_struct.h"

division ** division_vector_creation(int ndivisions, int * error_func) {
	// Argument error checking
	if(1 > ndivisions){
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] division_vector_creation: Number of divisions invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_1;
		return NULL;
	}
	
	// Function's code
	int counter;
	division **system_divisions = NULL;
	system_divisions = (division **) malloc(sizeof(division *) * ndivisions);
	if(NULL == system_divisions) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] division_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_3;
		return NULL;
	}
	for(counter = 0; counter < ndivisions; counter++){
		system_divisions[counter] = (division *) malloc(sizeof(division));
		if(NULL == system_divisions[counter]){
			printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] division_vector_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
				free(system_divisions[counter]);
				system_divisions = NULL;
			if(NULL != error_func)
				(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_3;
			return NULL;
		}
		system_divisions[counter]->division_name = string_creation(NULL);
	}
	if(NULL != error_func)
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	return system_divisions;
}

char *string_creation(int * error_func){
	// Function's code
	char *string = NULL;
	string = (char *) malloc(SIZE_LIB_MAN_DIVISION_LABEL_DIVISION_NAME);
	if(NULL == string){
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] string_creation: Memory allocated failed.\n", ERROR_LIB_MAN_DIVISION_STRUCT_3);
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
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] vector_actuator_future_state_creation: Number of future actuator state condition invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
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
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_3;
		return NULL;
	}
	for(counter = 0; counter < number_sensors; counter++){
		vector_sensors[counter] = string_creation(NULL);
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
		
		//FALTA PARA OS ATUADORES	
	}
	free(system_divisions);
	system_divisions = NULL;
	if(NULL != error_func) {
		(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_NONE;
	}
	return;
}

void insert_info_division_struct(division **system_divisions, int * number_divisions, int *error_func) {
	//Argument's error checking
	if(NULL == system_divisions) {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] free_memory_all: Pointer to the division struct invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_5);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_5;
		return;
	}
	if(1 > (*number_divisions))  {
		printf("[ERROR_LIB_MAN_DIVISION_STRUCT %d] free_memory_all: Number of divisions invalid.\n", ERROR_LIB_MAN_DIVISION_STRUCT_1);
		if(NULL != error_func)
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_1;
		return;
	}
	
	//Function's code 
	FILE *sensor_configuration = NULL;
	int counter1, counter2, aux_virgula = 0, counter3, counter4 = 0, counter6, counter7 = 0;
	char str_file[SIZE_STRING_BUFFER_1];
	char str_aux1[SIZE_STRING_BUFFER_1], str_aux2[SIZE_STRING_BUFFER_1];
	
	system_divisions = division_vector_creation((*number_divisions), NULL);
	
	sensor_configuration = fopen("SensorConfiguration.txt", "r");
	
	if(NULL == sensor_configuration) {
		printf("Error opening the file\n");	
		if(NULL != error_func) {
			(*error_func) = ERROR_LIB_MAN_DIVISION_STRUCT_5;
			return;
		}
	}
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
	
	//while(fgets(str_file, SIZE_STRING_BUFFER_1, sensor_configuration) != NULL) {
		for(counter6 = 0; fgets(str_file, SIZE_STRING_BUFFER_1, sensor_configuration) != NULL; counter6++) {
			if('\n' == str_file[strlen(str_file) - 1])
				str_file[strlen(str_file) - 1] = '\0';
				
			printf("[DIVISION CONFIG %d] %s\n", counter6+1, str_file);
			
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
			str_aux2[counter2] = '\0';
			counter2++;
			system_divisions[counter6]->num_sensors = aux_virgula+1;
		
			system_divisions[counter6]->sensors = string_vector_sensors_creation(aux_virgula+1, NULL);
			
			for(counter3 = 0; '\0' != str_aux2[counter3]; counter3++) {
				system_divisions[counter6]->sensors[counter4][counter7] = str_aux2[counter3];
				counter7++;
				if(',' == str_aux2[counter3] || '\0' == str_aux2[counter3]) {
					system_divisions[counter6]->sensors[counter4][counter7-1] = '\0';
					counter4++;
					counter7 = 0;
				}
			}
		}
	//}
	fclose(sensor_configuration);
}	
