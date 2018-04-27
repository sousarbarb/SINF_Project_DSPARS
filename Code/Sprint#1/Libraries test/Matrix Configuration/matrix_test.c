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
	char *str_aux = NULL, state[4] = {0};
	int	side, index_division, index_actuator;
	
	printf("Nº de células por lado da matrix = ");
	scanf("%d",&side);
	getchar();
	str_aux = alocation_memory_matrix(side);
	
	while(1){
		printf("Índice da divisão = ");
		scanf("%d",&index_division);
		getchar();
		printf("Índice do atuador = ");
		scanf("%d",&index_actuator);
		getchar();
		printf("Estado do atuador: ");
		scanf("%s",state);
		getchar();
		
		str_aux = configuration_matrix(str_aux,side,index_division,index_actuator,state);
		printf("\nString resultante: %s\n",str_aux);
	}
	
	return 0;
}

