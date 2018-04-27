/*
 * test_actuators.c
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
#include "actuators_lib_struct.h"

#define NUM_ACTUATORS		5

int main(int argc, char **argv)
{
	actuator **action = actuators_vector_creation(NUM_ACTUATORS,NULL);
	
	strcpy(action[0]->id,"HEATER_KITCHEN");
	strcpy(action[0]->actuation,"ON");
	strcpy(action[1]->id,"WINDOW_ROOM1");
	strcpy(action[1]->actuation,"OFF");
	strcpy(action[2]->id,"HEATER_LIVING_ROOM");
	strcpy(action[2]->actuation,"ON");
	strcpy(action[3]->id,"WINDOW_ROOM2");
	strcpy(action[3]->actuation,"OFF");
	strcpy(action[4]->id,"HEATER_ROOM4");
	strcpy(action[4]->actuation,"ON");
	
	if(0 > search_actuator(action,NUM_ACTUATORS,"WINDOW_ROOM1",NULL)){	//verifica se o atuador existe
			printf("The actuator doesn't exist!\n");
	}
	else
		printf("\nActuator's position in the struscture: %d \n", search_actuator(action,NUM_ACTUATORS,"WINDOW_ROOM1",NULL));	//escreve a posição do id do atuador
	
	print_actuation_vector(action,NUM_ACTUATORS,NULL);
	
	print_actuator_state(action,NUM_ACTUATORS,"WINDOW_ROOM1",NULL);
	return 0;
}

