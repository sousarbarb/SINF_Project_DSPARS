/*
 * main.c
 * 
 * Copyright 2018 Pedro Albergaria <pedro@PedroAlbergaria>
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
#include "manipulation_division_struct.h"

int main(int argc, char **argv)
{
	division **system_divisions;
	int number_divisions = 0;
	
	
	printf("Insira o número de divisões: ");
	scanf(" %d", &number_divisions);
	
	system_divisions = division_vector_creation(number_divisions, NULL);
	printf("AERG\n");
	insert_info_division_struct(system_divisions, number_divisions, NULL);
	return 0;
}

