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
#include "manipulation_mote_struct.h"

int main(int argc, char **argv)
{
	int nmotes;
	mote **mote_pointer;
	
	printf("Choose number of motes: ");
	scanf("%d", &nmotes);
	
	mote_pointer = mote_vector_creation(nmotes, NULL);
	
	printf("Temperature %f\n", mote_pointer[0]->temperature);
	
	free_mote_memory(mote_pointer, nmotes, NULL);
	
	return 0;
}

