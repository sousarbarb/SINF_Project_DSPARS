/*
 * teste3_v1_08032018.c
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

#define BUFFERSIZE	100

int main(int argc, char **argv)
{
	int numberStrings, i, j;
	char buffer[BUFFERSIZE];
	char **str = NULL;
	printf("How many strings do you want to store? ");
	scanf("%d", &numberStrings);
	getchar();
	
	// Vector allocation
	str = (char **) malloc(sizeof(char *) * numberStrings);
	if(NULL == str){
		printf("For the moment, the allocation of memory isn't avaiable.\n");
		exit(0);
	}
	// Save strings in the strings vector
	for(i = 0; i < numberStrings; i++){
		printf("Insert the string %d: ", i+1);
		fgets(buffer, BUFFERSIZE, stdin);
		buffer[strlen(buffer)-1] = '\0';
		str[i] = (char *) malloc(strlen(buffer));
		
		// Free the memory previously allocated
		if(NULL == str[i]){
			printf("For the moment, the allocation of memory isn't avaiable.\n");
			for(j = 0; j <= i; j++){
				free(str[j]);
				str[j] = NULL;
			}
			free(str);
			str = NULL;
			exit(0);
		}
		strcpy(str[i], buffer);
	}
	
	// Display the strings present in memory
	printf("\n\n----- STRINGS IN MEMORY -----\n");
	for(i = 0; i < numberStrings; i++)
		printf("  String %d: %s\n", i+1, str[i]);
	
	// Memory free
	for(i = 0; i < numberStrings; i++){
		free(str[i]);
		str[i] = NULL;
	}
	free(str);
	str = NULL;
	return 0;
}
