/*
 * matrix_test.c
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
 
#define TYPE_SENS_TEMP  1
#define TYPE_SENS_HUM   2
#define TYPE_SENS_LIGHT 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project_matrix.h"

int search_sensor_mote(char * sensor, /*int number_motes, int * mote_id_sensor,*/ int * type_sensor){
	int integer_aux_1=0/*, integer_aux_2, numeration_sensor*/;
	(*type_sensor)=0;
	switch(sensor[0]){
		case 'L': integer_aux_1=5; (*type_sensor)=TYPE_SENS_LIGHT; break;
		case 'H': integer_aux_1=3; (*type_sensor)=TYPE_SENS_HUM; break;
		case 'T': integer_aux_1=4; (*type_sensor)=TYPE_SENS_TEMP; break;
		default: return -1;
	}
	if('\0' == sensor[integer_aux_1])
		return 1;
/*	numeration_sensor = conversion_of_a_piece_of_a_string_into_integer(sensor, integer_aux_1, &integer_aux_2, strlen(sensor));
	if(numeration_sensor <= number_motes){
		(*mote_id_sensor) = numeration_sensor;
		return 1;
	}*/
	else
		return 0;
}

int main(int argc, char **argv)
{
	char *str_aux = NULL, state[10] = {0};
	int	side,n_divisions, n_elements, index_div_act, index_div_sens, index_act, index_sens, value_sens, type_sens;
	FILE *channel;
	
	printf("Número de divisões:");
	scanf("%d",&n_divisions);
	getchar();
	printf("Número máximo de elementos por divisão:");
	scanf("%d",&n_elements);
	getchar();
	
	write_size_matrix("RGBMatrixConf.txt",n_divisions,n_elements);
	
	side = determination_of_maximum(n_divisions,n_elements);
	str_aux = alocation_memory_matrix(side);
	
	while(1){
		printf("Índice da divisão = ");
		scanf("%d",&index_div_act);
		getchar();
		printf("Índice do atuador = ");
		scanf("%d",&index_act);
		getchar();
		printf("Estado do atuador: ");
		scanf("%s",state);
		getchar();
		str_aux = configuration_matrix_actuators(str_aux,side,index_div_act,index_act,state);
		printf("\nString resultante: %s\n",str_aux);
		
		channel = fopen("/dev/pts/2", "w");
		fprintf(channel, "%s\n", str_aux);
		fclose(channel);
		
		printf("Índice da divisão = ");
		scanf("%d",&index_div_sens);
		getchar();
		printf("Índice do sensor = ");
		scanf("%d",&index_sens);
		getchar();
		printf("valor do sensor: ");
		scanf("%d",&value_sens);
		getchar();
		
		search_sensor_mote("HUM",&type_sens);
		str_aux = configuration_matrix_sensors(str_aux,side,index_div_sens,index_sens,value_sens,type_sens);
		printf("\nString resultante: %s\n",str_aux);
		
		channel = fopen("/dev/pts/2", "w");
		fprintf(channel, "%s\n", str_aux);
		fclose(channel);
		write_config_matrix("matrix.txt",str_aux);
	}
	free(str_aux);
	return 0;
}

