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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project_matrix.h"

int main(int argc, char **argv)
{
	char *str_aux = NULL, state[10] = {0};
	int	side,n_divisions, n_elements, index_div, index_act;
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
		scanf("%d",&index_div);
		getchar();
		printf("Índice do atuador = ");
		scanf("%d",&index_act);
		getchar();
		printf("Estado do atuador: ");
		scanf("%s",state);
		getchar();
		
		str_aux = configuration_matrix(str_aux,side,index_div,index_act,state);
		printf("\nString resultante: %s\n",str_aux);
		channel = fopen("/dev/pts/3", "w");
		fprintf(channel, "%s\n", str_aux);
		fclose(channel);
		write_config_matrix("matrix.txt",str_aux);
	}
	return 0;
}

