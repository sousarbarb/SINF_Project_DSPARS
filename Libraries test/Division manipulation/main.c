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
#include "actuators_lib_struct.h"

int main(int argc, char **argv)
{
	division **system_divisions;
	int number_divisions = 0, counter1 = 0, counter2 = 0;
	
	printf("Insira o número de divisões: ");
	scanf(" %d", &number_divisions);
	
	system_divisions = insert_info_division_struct(&number_divisions, NULL);

	// Insert test
	for(counter1 = 0; counter1 < number_divisions; counter1++) {
		printf("\n\n------DIVISION Nº%d------\n\n", counter1+1);
		printf("[DIVISION NAME] %s\n", system_divisions[counter1]->division_name);
		for(counter2 = 0; counter2 < system_divisions[counter1]->num_sensors; counter2++)
			printf("[SENSOR] %s\n", system_divisions[counter1]->sensors[counter2]);
		for(counter2 = 0; counter2 < system_divisions[counter1]->num_actuator; counter2++)
			printf("[ACTUATOR] %s\n", system_divisions[counter1]->division_actuators[counter2]->id);
	}
	// Search test
	return 0;

}

