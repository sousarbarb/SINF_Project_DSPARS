/*
 * define_size_matrix.c
 * 
 * Copyright 2018 Daniel Silva <daniel@debian>
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
 
#ifndef _PROJECT_MATRIX_H_
#define _PROJECT_MATRIX_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define ERROR_PROJECT_MATRIX_1		1
#define ERROR_PROJECT_MATRIX_2		2
#define ERROR_PROJECT_MATRIX_3		3
#define ERROR_PROJECT_MATRIX_4		4
#define ERROR_PROJECT_MATRIX_5		5
#define ERROR_PROJECT_MATRIX_6		6
#define ERROR_PROJECT_MATRIX_7		7
#define ERROR_PROJECT_MATRIX_8		8

#define MAX_PIXELS		700
#define	SIZE_COLOR		13

#define	MAX_LIGHT	350
#define MIN_LIGHT	10
#define	MAX_HUMID	50
#define	MIN_HUMID	5
#define MAX_TEMP	40
#define MIN_TEMP	10

static const char on_state[] = "[  0,255,  0]", off_state[] = "[127,127,127]", under_limit[] = "[145,234,237]", above_limit[] = "[ 18, 24,153]", inside_limit[] = "[ 44,109,222]";

int determination_of_maximum(int divisions, int elements);

int number_of_pixels(int constant);

void write_size_matrix(char *file_name, int num_divisions, int num_elements);

char* alocation_memory_matrix(int side);

char* configuration_matrix_actuators(char *string_matrix_config, int matrix_side, int index_division, int index_actuator, char* actuator_state);

char* configuration_matrix_sensors(char *string_matrix_config, int matrix_side, int index_division, int index_sensor, int sensor_value, int sensor_type);

void write_config_matrix(char *config_name_file, char *string_matrix_config);
#endif
