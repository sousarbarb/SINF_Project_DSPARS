/*
 * file_strings_to_channels.c
 * 
 * Copyright 2018 
 * 	   Daniel Queirós da Silva		<up201503071@fe.up.pt>
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

#define STRING_LENGHT 70


int main(int argc, char **argv)
{
	FILE *data_strings = NULL, *channel = NULL;
	char str_test[STRING_LENGHT]={0};

	/* opening file for reading */
	data_strings = fopen("msg.txt" , "r");
	channel = fopen("/dev/pts/4", "w"); //compilar com - ./file_strings_to_channels "/dev/pts/4"
	
	if(data_strings == NULL) {
		perror("Error opening file");
		return(-1);

   }

   while(1) {
		while(fgets(str_test, STRING_LENGHT, data_strings) != NULL) {
			if((str_test[0] != '\n')) {
				printf("%s\n", str_test);
			}
			fprintf(channel, "%s\n", str_test);
		}
		fseek(data_strings, 0, SEEK_SET);
	}
   
   fclose(data_strings);
   fclose(channel);
   
	return 0;
}
