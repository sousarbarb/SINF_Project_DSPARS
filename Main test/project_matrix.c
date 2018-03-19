/*
 * define_size_matrix.c
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
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "project_matrix.h"

int determination_of_maximum(int divisions, int elements){
	if(divisions > elements)
		return divisions;
	else if(divisions < elements)
		return elements;
	else
		return divisions;
}

int number_of_pixels(int constant){
	return MAX_PIXELS/constant;
}

void write_size_matrix(char *file_name, int num_divisions, int num_elements){

	FILE *f_write=NULL;
	char side_c[10] = {0}, pixels_c[10] = {0};
	int side_i = 0, pixels_i = 0;
	
	f_write = fopen(file_name,"w");

	side_i = determination_of_maximum(num_divisions,num_elements);
	pixels_i = number_of_pixels(side_i);
	
	sprintf(side_c,"%d",side_i);
	sprintf(pixels_c,"%d",pixels_i);
	printf("-a %s -b %s\n",side_c,pixels_c);
	fprintf(f_write,"-a %s -b %s",side_c,pixels_c);
	
	fclose(f_write);
	f_write = NULL;
}

char* alocation_memory_matrix(int side){
	int size_of_alocation = 0, area_matrix = 0, counter = 0;
	char *buffer_matrix;
	
	area_matrix = side*side;
	
	size_of_alocation = SIZE_COLOR * area_matrix + (area_matrix - 1) + 2;
	
	buffer_matrix = (char*) malloc(sizeof(char*) * size_of_alocation);
	
	if(NULL == buffer_matrix)
		printf("[ERROR_PROJECT_MATRIX %d] alocation_memory_matrix: Memory allocated failed.\n",ERROR_PROJECT_MATRIX_1);
	buffer_matrix[0] = '[';
	if(buffer_matrix[1] != '\0'){	//caso haja corrompimento de dados
		buffer_matrix[1] = '\0';
	}
	for(counter = 0; counter < area_matrix; counter++){
		if(counter == (area_matrix - 1)){
			strcat(buffer_matrix,"[127,127,127]]");
		}
		else 
			strcat(buffer_matrix,"[127,127,127],");
	}
	return buffer_matrix;
}

char* configuration_matrix(char *string_matrix_config, int matrix_side, int index_division, int index_actuator, char* actuator_state){	//cada coluna é uma divisão
	
	int step = 0, column_actuator_cell = 0, curr_pos = 0, column_top_cell = 0, index_state = 0;
	
	if((matrix_side < index_division) || (index_division <= 0)){
		printf("[ERROR_PROJECT_MATRIX %d] configuration_matrix: Division's index incorrect.\n",ERROR_PROJECT_MATRIX_2);
		return string_matrix_config;
	}
	if((index_actuator > matrix_side) || (index_actuator <= 0)){
		printf("[ERROR_PROJECT_MATRIX %d] configuration_matrix: Actuator's index incorrect.\n",ERROR_PROJECT_MATRIX_3);
		return string_matrix_config;		
	}
	if(string_matrix_config == NULL){
		printf("[ERROR_PROJECT_MATRIX %d] configuration_matrix: String invalid.\n",ERROR_PROJECT_MATRIX_4);
		return string_matrix_config;
	}
	
	column_top_cell = matrix_side*(index_division - 1) + 1; //supondo que o index_division não pode ser zero
	column_actuator_cell = column_top_cell + index_actuator - 1; //supondo que o index_actuator não pode ser zero
	
	curr_pos = 1 + (SIZE_COLOR + 1)*(column_actuator_cell - 1);
	index_state = 0;
	if(strcmp(actuator_state,"ON") == 0){
		for(step = curr_pos; step <= (curr_pos + SIZE_COLOR - 1); step++){
			string_matrix_config[step] = on_state[index_state];
			index_state++;
		}
	}
	else if(strcmp(actuator_state,"OFF") == 0){
		for(step = curr_pos; step <= (curr_pos + SIZE_COLOR - 1); step++){
			string_matrix_config[step] = off_state[index_state];
			index_state++;
		}	
	}
	else{
		printf("[ERROR_PROJECT_MATRIX %d] configuration_matrix: Actuator's state invalid.\n",ERROR_PROJECT_MATRIX_5);
		return string_matrix_config;		
	}
	return string_matrix_config;
}

void write_config_matrix(char *config_name_file, char *string_matrix_config){
	FILE *f_write;
	
	f_write = fopen(config_name_file,"w");
	
	fputs(string_matrix_config,f_write);
	
	fclose(f_write);	
}
