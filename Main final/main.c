/*
 * main.c
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


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include "actuators_lib_struct.h"
#include "manipulation_mote_struct.h"
#include "manipulation_rule_struct.h"
#include "manipulation_division_struct.h"
#include "project_matrix.h"

// Errors
#define MAIN_ERROR_1  1
#define MAIN_ERROR_2  1
#define MAIN_ERROR_3  1
#define MAIN_ERROR_4  1
#define MAIN_ERROR_5  1
#define MAIN_ERROR_6  1
#define MAIN_ERROR_7  1
#define MAIN_ERROR_8  1
#define MAIN_ERROR_9  1
#define MAIN_ERROR_10 1

// Errors - thread_data_processing 
#define	ERROR1	1
#define ERROR2	2
#define ERROR3	3
#define	ERROR4	4
#define	ERROR5	5
#define ERROR6	6

// Constants relative to the communication channels
#define MAIN_CHANNEL_SENSOR  1
#define MAIN_CHANNEL_RGBMAT  2
#define MAIN_CHANNEL_LIM_INF 0
#define MAIN_CHANNEL_LIM_SUP 8

// ASCII codes
#define	ASCII_res_number_0	0
#define ASCII_res_number_9	9
#define	ASCII_res_char_A	17
#define ASCII_res_char_F	22
#define ASCII_res_space		-16

// Parameter's position in the string
#define	INIT_BOTTOM		0
#define	INIT_TOP		4
#define	MOTE_ID_BOTTOM	15
#define	MOTE_ID_TOP		19
#define	TEMP_BOTTOM		48
#define	TEMP_TOP		52
#define	HUMID_BOTTOM	54
#define	HUMID_TOP		58
#define	LIGHT_BOTTOM	36
#define	LIGHT_TOP		40
#define	END_BOTTOM		66
#define	END_TOP			67

// Other constants - thread_data_processing
#define LINE	70
#define WORD	6
#define BASE	16
#define SPACE   2

// Types of sensors - function search_sensor_mote
#define TYPE_SENS_TEMP  1
#define TYPE_SENS_HUM   2
#define TYPE_SENS_LIGHT 3

// Global variables
FILE *sensor_data_channel = NULL, *rgb_matrix_channel = NULL;
static const char CHANNEL_DEF[]="/dev/pts/";
int number_motes, number_rules, number_divisions, number_maximum_actuators, matrix_side_size;
int flag_interface = 1;
char **pointer_rgb_channel = NULL;
char *buffer_rgb_channel = NULL;
mote **system_motes = NULL;
rule **system_rules = NULL;
division **system_divisions = NULL;

// Global flags

/***************************************************
 * FUNCTIONS USED BY THREAD DATA PROCESSING (PTH1)
 ***************************************************/
int multiplication_by_10(int times){
	
	if(0 == times)
		return 1000;
	else if(1 == times)
		return 100;
	else if(2 == times)
		return 10;
	else return 1;
}

int power(int base, int exponent){
	int step = 0, result = 1;
	
	if((base < 0) || (exponent < 0)){
		return 0;
	}
	else{
		for(step= 0; step < exponent; step++){
			result = result * base;
		}
		return result;
	}
}

float calculate_temperature(int number_Temp){
	return -39.6 + 0.01*number_Temp;
}

float calculate_relative_humidity(int number_Humid){
	return -2.0468 + 0.0367*number_Humid - 1.5955*0.000001*power(number_Humid,2);
}

float calculate_humidity_compensated_by_temperature(int number_Humid, float humid_relative, float Temp){
	return (Temp - 25)*(0.01 + 0.00008*number_Humid) + humid_relative;
}

float calculate_visible_light (int number_Visible_Light){
	return 0.625*power(10,6)*(number_Visible_Light/4096.0)*(1.5/power(10,5))*1000;
}


/****************************************
 * THREAD DATA PROCESSING (PTH1) - CODE
 ****************************************/
void *thread_data_processing(void *arg){
	// Variable's declaration
	int step_line = 0, step_word = 0, fill_up = 0, times_10 = 0, times_16 = 0;
	int	decimal_id = 0, convert = 0, flag_invalid = 0, decimal_temp = 0, decimal_humid = 0, decimal_visible_light = 0;
	float temp = 0, relative_humidity = 0, humidity_compensated_by_temperature = 0, visible_light = 0;
	char line[LINE], word[WORD];
	
	// Code
	while(1 == flag_interface){
		fgets(line, LINE, sensor_data_channel);
		printf("DEBUG: %s", line);
		for(step_line = 0; step_line < strlen(line); step_line++){
			// Beginning of line
			if(INIT_BOTTOM == step_line){
				fill_up = 0;
				for(step_word = INIT_BOTTOM; step_word <= INIT_TOP; step_word++){
					if(step_word != (INIT_BOTTOM + SPACE)){
						word[fill_up] = line[step_word];
						fill_up++;
					}
				}
				word[fill_up] = '\0';
				
				if(0 == strcmp(word,"7E45")){
					printf("Valid init!\n");
				}
				else {
					printf("ERROR[%d] - The message start bytes are incorrect!\n",ERROR1);
					//break;
				}
			}
			// MOTE ID
			else if(MOTE_ID_BOTTOM == step_line){
				flag_invalid = 0;
				times_10 = 0;
				decimal_id = 0;
				for(step_word = MOTE_ID_BOTTOM; step_word <= MOTE_ID_TOP; step_word++){
					convert = line[step_word] - '0';
					if((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9) && (step_word != (MOTE_ID_BOTTOM + SPACE))){
						decimal_id = decimal_id + convert*multiplication_by_10(times_10);
						times_10++;
					}
					else if(!((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9)) && (step_word != (MOTE_ID_BOTTOM + SPACE))){
						flag_invalid = 1;
						break;
					}
					else{
						decimal_id += 0;
					}
				}
				if((decimal_id > 9999) || (flag_invalid)){
					printf("ERROR[%d] - Mote ID invalid!\n",ERROR2);
				}
				else{
					printf("Mote ID: %d\n",decimal_id);
				}
			}
			// Temperature parameter
			else if(TEMP_BOTTOM == step_line){
				flag_invalid = 0;
				times_16 = 0;
				decimal_temp = 0;
				for(step_word = TEMP_TOP; step_word >= TEMP_BOTTOM; step_word--){
					convert = line[step_word] - '0';
					if((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F) && (step_word != (TEMP_BOTTOM + SPACE))){
						convert = line[step_word] - 'A' + 10;
						decimal_temp = decimal_temp + convert*power(BASE,times_16);
						times_16++;
					}
					else if((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9) && (step_word != (TEMP_BOTTOM + SPACE))){
						decimal_temp = decimal_temp + convert*power(BASE,times_16);
						times_16++;
					}
					else if(!((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F)) && !((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9)) && (step_word != (TEMP_BOTTOM + SPACE))){ 
						flag_invalid = 1;
						break;
					}
					else{
						decimal_temp += 0;
					}
				}
				if(flag_invalid){
					printf("ERROR[%d] - Temperature parameter invalid!\n",ERROR3);
				}
				else{
					//printf("Temperatura decimal = %d\n", decimal_temp);
					temp = calculate_temperature(decimal_temp);
					system_motes[search_mote(system_motes,number_motes,decimal_id,NULL)]->temperature = temp;
					
					printf("Temperature = %.2f ºC\n",temp);
				}
			}
			// Humidity parameter
			else if(HUMID_BOTTOM == step_line){
				flag_invalid = 0;
				times_16 = 0;
				decimal_humid = 0;
				for(step_word = HUMID_TOP; step_word >= HUMID_BOTTOM; step_word--){
					convert = line[step_word] - '0';
					if((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F) && (step_word != (HUMID_BOTTOM + SPACE))){
						convert = line[step_word] - 'A' + 10;
						decimal_humid = decimal_humid + convert*power(BASE,times_16);
						times_16++;
					}
					else if((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9) && (step_word != (HUMID_BOTTOM + SPACE))){
						decimal_humid = decimal_humid + convert*power(BASE,times_16);
						times_16++;
					}
					else if(!((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F)) && !((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9)) && (step_word != (HUMID_BOTTOM + SPACE))){
						flag_invalid = 1;
						break;
					}
					else{
						decimal_humid += 0;
					}
				}
				if(flag_invalid){
					printf("ERROR[%d] - Humidity parameter invalid!\n",ERROR4);
				}
				else{
					//printf("Humidade decimal = %d\n",decimal_humid);
					relative_humidity = calculate_relative_humidity(decimal_humid);
					printf("Relative humidity = %.2f %%\n",relative_humidity);
					
					humidity_compensated_by_temperature = calculate_humidity_compensated_by_temperature(decimal_humid,relative_humidity,temp);
					system_motes[search_mote(system_motes,number_motes,decimal_id,NULL)]->humidity = humidity_compensated_by_temperature;
					
					printf("Humidity compensated by temperature = %.2f %%\n",humidity_compensated_by_temperature);
				}
			}
			// Visible light parameter
			else if(LIGHT_BOTTOM == step_line){
				flag_invalid = 0;
				times_16 = 0;
				decimal_visible_light = 0;
				for(step_word = LIGHT_TOP; step_word >= LIGHT_BOTTOM; step_word--){
					convert = line[step_word] - '0';
					if((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F) && (step_word != (LIGHT_BOTTOM + SPACE))){
						convert = line[step_word] - 'A' + 10;
						decimal_visible_light = decimal_visible_light + convert*power(BASE,times_16);
						times_16++;
					}
					else if((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9) && (step_word != (LIGHT_BOTTOM + SPACE))){
						decimal_visible_light = decimal_visible_light + convert*power(BASE,times_16);
						times_16++;
					}
					else if(!((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F)) && !((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9)) && (step_word != (LIGHT_BOTTOM + SPACE))){
						flag_invalid = 1;
						break;
					}
					else{
						decimal_visible_light += 0;
					}
				}
				if(flag_invalid){
					printf("ERROR[%d] - Visible light parameter invalid!\n",ERROR5);
				}
				else{
					//printf("Luz visível decimal = %d\n",decimal_visible_light);
					visible_light = calculate_visible_light(decimal_visible_light);
					system_motes[search_mote(system_motes,number_motes,decimal_id,NULL)]->luminosity = visible_light;
					
					printf("Visible light = %.2f lux\n", visible_light);
				}
			}
			// End of line
			else if (END_BOTTOM == step_line){
				fill_up = 0;
				for(step_word = END_BOTTOM; step_word <= END_TOP; step_word++){
					word[fill_up] = line[step_word];
					fill_up++;
				}
				word[fill_up] = '\0';
				if(0 == strcmp(word,"7E")){
					printf("Valid end!\n");
					break;
				}
				else {
					printf("ERROR[%d] - Invalid end!\n",ERROR6);
					break;
				}
			}
		}
	}
	pthread_exit(NULL);
}


/*******************************************************
 * FUNCTIONS USED BY THREAD RULE IMPLEMENTATION (PTH2)
 *******************************************************/
int search_sensor_mote(char * sensor, int number_motes, int * mote_id_sensor, int * type_sensor){
	int integer_aux_1=0, integer_aux_2, numeration_sensor;
	(*type_sensor)=0;
	switch(sensor[0]){
		case 'L': integer_aux_1=5; (*type_sensor)=TYPE_SENS_LIGHT; break;
		case 'H': integer_aux_1=3; (*type_sensor)=TYPE_SENS_HUM; break;
		case 'T': integer_aux_1=4; (*type_sensor)=TYPE_SENS_TEMP; break;
		default: return -1;
	}
	if('\0' == sensor[integer_aux_1])
		return 1;
	numeration_sensor = conversion_of_a_piece_of_a_string_into_integer(sensor, integer_aux_1, &integer_aux_2, strlen(sensor));
	if(numeration_sensor <= number_motes){
		(*mote_id_sensor) = numeration_sensor;
		return 1;
	}
	else
		return 0;
}


/********************************************
 * THREAD RULE IMPLEMENTATION (PTH2) - CODE
 ********************************************/
void *thread_rule_implementation(void *arg){
	char actuator_string[SIZE_LIB_MAN_RULE_LABEL_ACTUAT_FUT_STATE];
	int rule_index, mote_id_sensor_1, type_sensor_1, mote_id_sensor_2, type_sensor_2, logic_value_condition_1 = 0, logic_value_condition_2 = 0;
	int index_division, index_actuators, counter1, actuator_fut_state;
	float value_sensor_1, value_sensor_2;
	while(1 == flag_interface){
		for(rule_index = 0; rule_index < number_rules; rule_index++){
			switch(system_rules[rule_index]->logic_operator_condition_1_2[0]){
				case '_':
					search_sensor_mote(system_rules[rule_index]->sensor_condition_1, number_motes, &mote_id_sensor_1, &type_sensor_1);
					
					// Sensor_1's type (temperature, humidity, luminosity) search
					switch(type_sensor_1){
						case TYPE_SENS_TEMP: 
							value_sensor_1 = system_motes[mote_id_sensor_1 - 1]->temperature;
							break;
						case TYPE_SENS_HUM: 
							value_sensor_1 = system_motes[mote_id_sensor_1 - 1]->humidity;
							break;
						case TYPE_SENS_LIGHT: 
							value_sensor_1 = system_motes[mote_id_sensor_1 - 1]->luminosity;
							break;
						default:
							printf("[MAIN_ERROR %d] It wansn't possible to know what's the logic operator between the first and second condition.\n", MAIN_ERROR_8);
							pthread_exit(NULL);
					}
					
					// Discovering the compare operator in condition 1
					switch(system_rules[rule_index]->operator_condition_1){
						case '>':
							if(((int)value_sensor_1) > system_rules[rule_index]->value_condition_1){
								for(counter1 = 0; counter1 < system_rules[rule_index]->num_actuator_conditions; counter1++){
									actuator_fut_state = find_actuator_future_state(system_rules[rule_index]->actuator_future_state[counter1], actuator_string);
									search_division_actuator(system_divisions, number_divisions, system_rules[rule_index]->division_name, actuator_string, &index_division, &index_actuators, NULL);
									
									if(1 == actuator_fut_state){
										buffer_rgb_channel = configuration_matrix(buffer_rgb_channel, matrix_side_size, index_division + 1, index_actuators + 1,"ON");
										rgb_matrix_channel = fopen((*pointer_rgb_channel), "w");
										fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
										fclose(rgb_matrix_channel);
										rgb_matrix_channel = NULL;
									}
									else{
										buffer_rgb_channel = configuration_matrix(buffer_rgb_channel, matrix_side_size, index_division + 1, index_actuators + 1,"OFF");
										rgb_matrix_channel = fopen((*pointer_rgb_channel), "w");
										fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
										fclose(rgb_matrix_channel);
										rgb_matrix_channel = NULL;
									}
								}
							}
							break;
						case '<':
							if(((int)value_sensor_1) < system_rules[rule_index]->value_condition_1){
								for(counter1 = 0; counter1 < system_rules[rule_index]->num_actuator_conditions; counter1++){
									actuator_fut_state = find_actuator_future_state(system_rules[rule_index]->actuator_future_state[counter1], actuator_string);
									search_division_actuator(system_divisions, number_divisions, system_rules[rule_index]->division_name, actuator_string, &index_division, &index_actuators, NULL);
									
									if(1 == actuator_fut_state){
										buffer_rgb_channel = configuration_matrix(buffer_rgb_channel, matrix_side_size, index_division + 1, index_actuators + 1,"ON");
										rgb_matrix_channel = fopen((*pointer_rgb_channel), "w");
										fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
										fclose(rgb_matrix_channel);
										rgb_matrix_channel = NULL;
									}
									else{
										buffer_rgb_channel = configuration_matrix(buffer_rgb_channel, matrix_side_size, index_division + 1, index_actuators + 1,"OFF");
										rgb_matrix_channel = fopen((*pointer_rgb_channel), "w");
										fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
										fclose(rgb_matrix_channel);
										rgb_matrix_channel = NULL;
									}
								}
							}
							break;
						default:
							printf("[MAIN_ERROR %d] Operator in condition 1 invalid.\n", MAIN_ERROR_10);
							pthread_exit(NULL);
					}
					
					break;
				case 'A': 
					search_sensor_mote(system_rules[rule_index]->sensor_condition_1, number_motes, &mote_id_sensor_1, &type_sensor_1);
					search_sensor_mote(system_rules[rule_index]->sensor_condition_2, number_motes, &mote_id_sensor_2, &type_sensor_2);
					
					// Sensor_1's type (temperature, humidity, luminosity) search
					switch(type_sensor_1){
						case TYPE_SENS_TEMP: 
							value_sensor_1 = system_motes[mote_id_sensor_1 - 1]->temperature;
							break;
						case TYPE_SENS_HUM: 
							value_sensor_1 = system_motes[mote_id_sensor_1 - 1]->humidity;
							break;
						case TYPE_SENS_LIGHT: 
							value_sensor_1 = system_motes[mote_id_sensor_1 - 1]->luminosity;
							break;
						default:
							printf("[MAIN_ERROR %d] It wansn't possible to know what's the sensor's type.\n", MAIN_ERROR_9);
							pthread_exit(NULL);
					}
					
					// Sensor_2's type (temperature, humidity, luminosity) search
					switch(type_sensor_2){
						case TYPE_SENS_TEMP: 
							value_sensor_2 = system_motes[mote_id_sensor_2 - 1]->temperature;
							break;
						case TYPE_SENS_HUM: 
							value_sensor_2 = system_motes[mote_id_sensor_2 - 1]->humidity;
							break;
						case TYPE_SENS_LIGHT: 
							value_sensor_2 = system_motes[mote_id_sensor_2 - 1]->luminosity;
							break;
						default:
							printf("[MAIN_ERROR %d] It wansn't possible to know what's the sensor's type.\n", MAIN_ERROR_9);
							pthread_exit(NULL);
					}
					
					// Discovering the compare operator in condition 1
					switch(system_rules[rule_index]->operator_condition_1){
						case '>':
							if(((int)value_sensor_1) > system_rules[rule_index]->value_condition_1)
								logic_value_condition_1 = 1;
							else
								logic_value_condition_1 = 0;
							break;
						case '<':
							if(((int)value_sensor_1) < system_rules[rule_index]->value_condition_1)
								logic_value_condition_1 = 1;
							else
								logic_value_condition_1 = 0;
							break;
						default:
							logic_value_condition_1 = 0;
							printf("[MAIN_ERROR %d] Operator in condition 1 invalid.\n", MAIN_ERROR_10);
							pthread_exit(NULL);
					}
					
					// Discovering the compare operator in condition 2
					switch(system_rules[rule_index]->operator_condition_2){
						case '>':
							if(((int)value_sensor_2) > system_rules[rule_index]->value_condition_2)
								logic_value_condition_2 = 1;
							else
								logic_value_condition_2 = 0;
							break;
						case '<':
							if(((int)value_sensor_2) < system_rules[rule_index]->value_condition_2)
								logic_value_condition_2 = 1;
							else
								logic_value_condition_2 = 0;
							break;
						default:
							logic_value_condition_2 = 0;
							printf("[MAIN_ERROR %d] Operator in condition 2 invalid.\n", MAIN_ERROR_10);
							pthread_exit(NULL);
					}
					
					if(1==logic_value_condition_1 && 1==logic_value_condition_2){
						for(counter1 = 0; counter1 < system_rules[rule_index]->num_actuator_conditions; counter1++){
							actuator_fut_state = find_actuator_future_state(system_rules[rule_index]->actuator_future_state[counter1], actuator_string);
							search_division_actuator(system_divisions, number_divisions, system_rules[rule_index]->division_name, actuator_string, &index_division, &index_actuators, NULL);
							
							if(1 == actuator_fut_state){
								buffer_rgb_channel = configuration_matrix(buffer_rgb_channel, matrix_side_size, index_division + 1, index_actuators + 1,"ON");
								rgb_matrix_channel = fopen((*pointer_rgb_channel), "w");
								fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
								fclose(rgb_matrix_channel);
								rgb_matrix_channel = NULL;
							}
							else{
								buffer_rgb_channel = configuration_matrix(buffer_rgb_channel, matrix_side_size, index_division + 1, index_actuators + 1,"OFF");
								rgb_matrix_channel = fopen((*pointer_rgb_channel), "w");
								fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
								fclose(rgb_matrix_channel);
								rgb_matrix_channel = NULL;
							}
						}
					}
					
					break;
				case 'O': 
					search_sensor_mote(system_rules[rule_index]->sensor_condition_1, number_motes, &mote_id_sensor_1, &type_sensor_1);
					search_sensor_mote(system_rules[rule_index]->sensor_condition_2, number_motes, &mote_id_sensor_2, &type_sensor_2);
					
					// Sensor_1's type (temperature, humidity, luminosity) search
					switch(type_sensor_1){
						case TYPE_SENS_TEMP: 
							value_sensor_1 = system_motes[mote_id_sensor_1 - 1]->temperature;
							break;
						case TYPE_SENS_HUM: 
							value_sensor_1 = system_motes[mote_id_sensor_1 - 1]->humidity;
							break;
						case TYPE_SENS_LIGHT: 
							value_sensor_1 = system_motes[mote_id_sensor_1 - 1]->luminosity;
							break;
						default:
							printf("[MAIN_ERROR %d] It wansn't possible to know what's the sensor's type.\n", MAIN_ERROR_9);
							pthread_exit(NULL);
					}
					
					// Sensor_2's type (temperature, humidity, luminosity) search
					switch(type_sensor_2){
						case TYPE_SENS_TEMP: 
							value_sensor_2 = system_motes[mote_id_sensor_2 - 1]->temperature;
							break;
						case TYPE_SENS_HUM: 
							value_sensor_2 = system_motes[mote_id_sensor_2 - 1]->humidity;
							break;
						case TYPE_SENS_LIGHT: 
							value_sensor_2 = system_motes[mote_id_sensor_2 - 1]->luminosity;
							break;
						default:
							printf("[MAIN_ERROR %d] It wansn't possible to know what's the sensor's type.\n", MAIN_ERROR_9);
							pthread_exit(NULL);
					}
					
					// Discovering the compare operator in condition 1
					switch(system_rules[rule_index]->operator_condition_1){
						case '>':
							if(((int)value_sensor_1) > system_rules[rule_index]->value_condition_1)
								logic_value_condition_1 = 1;
							else
								logic_value_condition_1 = 0;
							break;
						case '<':
							if(((int)value_sensor_1) < system_rules[rule_index]->value_condition_1)
								logic_value_condition_1 = 1;
							else
								logic_value_condition_1 = 0;
							break;
						default:
							logic_value_condition_1 = 0;
							printf("[MAIN_ERROR %d] Operator in condition 1 invalid.\n", MAIN_ERROR_10);
							pthread_exit(NULL);
					}
					
					// Discovering the compare operator in condition 2
					switch(system_rules[rule_index]->operator_condition_2){
						case '>':
							if(((int)value_sensor_2) > system_rules[rule_index]->value_condition_2)
								logic_value_condition_2 = 1;
							else
								logic_value_condition_2 = 0;
							break;
						case '<':
							if(((int)value_sensor_2) < system_rules[rule_index]->value_condition_2)
								logic_value_condition_2 = 1;
							else
								logic_value_condition_2 = 0;
							break;
						default:
							logic_value_condition_2 = 0;
							printf("[MAIN_ERROR %d] Operator in condition 2 invalid.\n", MAIN_ERROR_10);
							pthread_exit(NULL);
					}
					
					if(1==logic_value_condition_1 || 1==logic_value_condition_2){
						for(counter1 = 0; counter1 < system_rules[rule_index]->num_actuator_conditions; counter1++){
							actuator_fut_state = find_actuator_future_state(system_rules[rule_index]->actuator_future_state[counter1], actuator_string);
							search_division_actuator(system_divisions, number_divisions, system_rules[rule_index]->division_name, actuator_string, &index_division, &index_actuators, NULL);
							
							if(1 == actuator_fut_state){
								buffer_rgb_channel = configuration_matrix(buffer_rgb_channel, matrix_side_size, index_division + 1, index_actuators + 1,"ON");
								rgb_matrix_channel = fopen((*pointer_rgb_channel), "w");
								fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
								fclose(rgb_matrix_channel);
								rgb_matrix_channel = NULL;
							}
							else{
								buffer_rgb_channel = configuration_matrix(buffer_rgb_channel, matrix_side_size, index_division + 1, index_actuators + 1,"OFF");
								rgb_matrix_channel = fopen((*pointer_rgb_channel), "w");
								fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
								fclose(rgb_matrix_channel);
								rgb_matrix_channel = NULL;
							}
						}
					}
					
					break;
				default: 
					printf("[MAIN_ERROR %d] It wansn't possible to know what's the logic operator between the first and second condition.\n", MAIN_ERROR_8);
					pthread_exit(NULL);
			}
		}
	}
	pthread_exit(NULL);
}


/***************************************
 * THREAD USER INTERFACE (PTH3) - CODE
 ***************************************/
void *user_interface(void *arg){
	char user_answer;
	while(1 == flag_interface){
		printf("\n");
		printf("++++++++++++++++++++++++++++++++++++++++\n");
		printf("+++++++++++++++ HAS MENU +++++++++++++++\n");
		printf("++++++++++++++++++++++++++++++++++++++++\n");
		printf("\n");
		printf("1. Display division's information       \n");
		printf("2. Display division's rules information \n");
		printf("3. Display sensor's information         \n");
		printf("4. Exit Home Automative System          \n");
		printf("\n");
		printf("Insert the desired option (press Enter after): ");
		user_answer = getchar();
		getchar();
		switch(user_answer){
			case '1':
				print_division_struct(system_divisions, number_divisions, NULL);
				break;
			case '2':
				print_rules_system_vector(system_rules, number_rules, NULL);
				break;
			case '3':
				print_motes_vector(system_motes, number_motes, NULL);
				break;
			case '4':
				flag_interface = 0;
				break;
			default:
				printf("Insert a valid option.\n");
		}
	}
	pthread_exit(NULL);
}


/************************
 * FUNCTION MAIN - CODE
 ************************/
int main(int argc, char **argv)
{
	/************************************
	 * INIT ROUTINE - ARGUMENT CHECKING
	 ************************************/
	// ERROR 1 - The channels inserted aren't valid
	if(3 !=argc){
		printf("[MAIN_ERROR %d] The execution of the program implies the definition of the channel that is currently receiving data from the sensors and the channel that it'll be used to send the actuators state to the RGBMatrix.\n", MAIN_ERROR_1);
		printf("Please execute the program by the following steps:\n    ./main \"/dev/pts/x\" \"/dev/pts/y\"\n");
		printf("    Channel x -> communication with the system motes (receives data)\n    Channel y -> communication with the RGBMatrix (sends data)\n");
		exit(-1);
	}
	
	int counter1;
	char directory_channel_sensor[strlen(argv[MAIN_CHANNEL_SENSOR])-1], directory_channel_rgbmat[strlen(argv[MAIN_CHANNEL_RGBMAT])-1];
	for(counter1 = 0; counter1 < strlen(argv[MAIN_CHANNEL_SENSOR]) - 1; counter1++)
		directory_channel_sensor[counter1] = argv[MAIN_CHANNEL_SENSOR][counter1];
	directory_channel_sensor[counter1] = '\0';
	for(counter1 = 0; counter1 < strlen(argv[MAIN_CHANNEL_RGBMAT]) - 1; counter1++)
		directory_channel_rgbmat[counter1] = argv[MAIN_CHANNEL_RGBMAT][counter1];
	directory_channel_rgbmat[counter1] = '\0';
	
	// ERROR 2 - The channel's number selected aren't valid
	if(0 != strcmp(CHANNEL_DEF, directory_channel_sensor) || 0 != strcmp(CHANNEL_DEF, directory_channel_rgbmat) || MAIN_CHANNEL_LIM_INF > (argv[MAIN_CHANNEL_SENSOR][9]-'0') || MAIN_CHANNEL_LIM_SUP < (argv[MAIN_CHANNEL_SENSOR][9]-'0') || MAIN_CHANNEL_LIM_INF > (argv[MAIN_CHANNEL_RGBMAT][9]-'0') || MAIN_CHANNEL_LIM_SUP < (argv[MAIN_CHANNEL_RGBMAT][9]-'0')){
		printf("[MAIN_ERROR %d] The channel's directory aren't correctly characterized.\n", MAIN_ERROR_2);
		printf("Please execute the program by the following steps:\n    ./main \"/dev/pts/x\" \"/dev/pts/y\"\n");
		printf("    Channel x -> communication with the system motes (receives data)\n    Channel y -> communication with the RGBMatrix (sends data)\n");
		exit(-1);
	}
	sensor_data_channel = fopen(argv[MAIN_CHANNEL_SENSOR], "r");
	// ERROR 3
	if(NULL == sensor_data_channel){
		printf("[MAIN_ERROR %d] It's not possible to access the selected channel relative with the sensor's readings\n", MAIN_ERROR_3);
		exit(-1);
	}
	rgb_matrix_channel  = fopen(argv[MAIN_CHANNEL_RGBMAT], "w");
	// ERROR 4
	if(NULL == rgb_matrix_channel){
		printf("[MAIN_ERROR %d] It's not possible to access the selected channel relative with the actuator's state display\n", MAIN_ERROR_4);
		fclose(sensor_data_channel);
		sensor_data_channel = NULL;
		exit(-1);
	}
	pointer_rgb_channel = &argv[MAIN_CHANNEL_RGBMAT];
	
	/*************************
	 * VARIABLES DECLARATION
	 *************************/
	int error_check = 0, counter_divisions, counter_actuators;
	char user_answer;
	pthread_t pth1, pth2, pth3;
	
	/**********************************
	 * INIT ROUTINE - WELCOME MESSAGE
	 **********************************/
	printf("\n******************** HAS - HOME AUTOMATIVE SYSTEM ********************\n\n");
	
	/****************************************
	 * INIT ROUTINE - MOTE'S CONFIGURATION
	 ****************************************/
	printf("++++++++++ MOTE'S CONFIGURATION ++++++++++\n");
	do{
		printf("Insert the number of motes that are considered in the system (needs to be greater than 0): ");
		scanf(" %d", &number_motes);
		getchar();
	} while(1 > number_motes);
	system_motes = mote_vector_creation(number_motes, &error_check);
	if(NULL == system_motes || 0 < error_check){
		printf("[MAIN_ERROR %d] The motes weren't created.\n", MAIN_ERROR_5);
		fclose(sensor_data_channel);
		sensor_data_channel = NULL;
		fclose(rgb_matrix_channel);
		rgb_matrix_channel = NULL;
		exit(-1);
	}
	
	/*******************************************
	 * INIT ROUTINE - DIVISION'S CONFIGURATION
	 *******************************************/
	error_check = 0;
	system_divisions = insert_info_division_struct(&number_divisions, &error_check);
	if((NULL == system_divisions) || (0 < error_check)){
		printf("[MAIN_ERROR %d] The divisions weren't configurated.\n", MAIN_ERROR_6);
		free_mote_memory(system_motes, number_motes, NULL);
		fclose(sensor_data_channel);
		sensor_data_channel = NULL;
		fclose(rgb_matrix_channel);
		rgb_matrix_channel = NULL;
		exit(-1);
	}
	
	/****************************************
	 * INIT ROUTINE - RULE'S CONFIGURATION
	 ****************************************/
	if((-1 == rules_association_to_structures(&system_rules, &number_rules, number_motes, &error_check) || 0 < error_check)){
		printf("[MAIN_ERROR %d] The rules weren't created.\n", MAIN_ERROR_7);
		free_mote_memory(system_motes, number_motes, NULL);
		system_motes = NULL;
		free_memory_all(system_divisions, number_divisions, NULL);
		system_divisions = NULL;
		fclose(sensor_data_channel);
		sensor_data_channel = NULL;
		fclose(rgb_matrix_channel);
		rgb_matrix_channel = NULL;
		exit(-1);
	}
	
	/*****************************************
	 * INIT ROUTINE - MATRIX'S CONFIGURATION
	 *****************************************/
	number_maximum_actuators = number_max_actuators(system_divisions, number_divisions, &error_check);
	write_size_matrix("RGBMatrixConf.txt", number_divisions, number_maximum_actuators);
	matrix_side_size = determination_of_maximum(number_divisions, number_maximum_actuators);
	buffer_rgb_channel = alocation_memory_matrix(matrix_side_size);
	do{
		printf("The program RGBMatrix is already running (Y - yes / N - no, and press Enter)? ");
		user_answer = getchar();
		getchar();
	} while('Y' != user_answer &&'N' != user_answer &&'y' != user_answer && 'n' != user_answer);
	if('n' == user_answer ||'N' == user_answer){
		printf("Then please execute it with the channel's atribuition to communicate with the program.\n");
		free_mote_memory(system_motes, number_motes, NULL);
		system_motes = NULL;
		free_memory_all(system_divisions, number_divisions, NULL);
		system_divisions = NULL;
		free_rules_system_memory(system_rules, number_rules, NULL);
		system_rules = NULL;
		fclose(sensor_data_channel);
		sensor_data_channel = NULL;
		fclose(rgb_matrix_channel);
		rgb_matrix_channel = NULL;
		exit(-1);
	}
	for(counter_divisions = 0; counter_divisions < number_divisions; counter_divisions++){
		for(counter_actuators= 0; counter_actuators < system_divisions[counter_divisions]->num_actuator; counter_actuators++){
			buffer_rgb_channel = configuration_matrix(buffer_rgb_channel, matrix_side_size, counter_divisions + 1, counter_actuators + 1,"OFF");
			rgb_matrix_channel = fopen(argv[MAIN_CHANNEL_RGBMAT], "w");
			fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
			fclose(rgb_matrix_channel);
			rgb_matrix_channel = NULL;
		}
	}
	
	/**************
	 * DEBUG ZONE
	 **************/
	print_motes_vector(system_motes, number_motes, &error_check);
	print_rules_system_vector(system_rules, number_rules, &error_check);
	print_division_struct(system_divisions, number_divisions, &error_check);
	 
	/***********************
	 * CREATION OF THREADS
	 ***********************/
	pthread_create(&pth1, NULL, thread_data_processing, NULL);
	pthread_create(&pth2, NULL, thread_rule_implementation, NULL);
	pthread_create(&pth3, NULL, user_interface, NULL);
	
	/****************************************
	 * CREATION AND CANCELLATION OF THREADS
	 ****************************************/
	pthread_join(pth3, NULL);
	pthread_cancel(pth1);
	pthread_cancel(pth2);
	pthread_cancel(pth3);
	
	/***********************
	 * TERMINATION ROUTINE
	 ***********************/
	free_mote_memory(system_motes, number_motes, NULL);
	system_motes = NULL;
	free_memory_all(system_divisions, number_divisions, NULL);
	system_divisions = NULL;
	free_rules_system_memory(system_rules, number_rules, NULL);
	system_rules = NULL;
	fclose(sensor_data_channel);
	sensor_data_channel = NULL;
	fclose(rgb_matrix_channel);
	rgb_matrix_channel = NULL;
	printf("\n**********************************************************************\n");
	printf("************************* END OF PROGRAM HAS *************************\n");
	printf("**********************************************************************\n\n");
	return 0;
	exit(0);
}

