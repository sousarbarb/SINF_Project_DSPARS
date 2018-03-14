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
#include <time.h>
#include <unistd.h>

#define STRING_LENGHT 100
#define SLEEP_TIME 0.5

void print_time(time_t tt) {
	char buffer[80] = {0};
	struct tm* st = localtime(&tt);
	strftime(buffer, 80, "%c", st);
	printf("%s\n", buffer);
}

int main(int argc, char **argv)
{
	FILE *data_strings = NULL, *channel = NULL;
	char str_test[STRING_LENGHT]={0};
	int counter1 = 0;
	time_t t = time(NULL);

	/* opening file for reading */
	data_strings = fopen("msg.txt" , "r");
	channel = fopen("/dev/pts/1", "w");
	
	if(data_strings == NULL) {
		perror("Error opening file");
		return(-1);

   }
   
   print_time(t);
   
   while(1) {
		while(fgets(str_test, STRING_LENGHT, data_strings) != NULL && counter1<60) {
			if((str_test[0] != '\n')) {
				printf("%s\n", str_test);
				counter1++;
			}
			fprintf(channel, "%s\n", str_test);
		}
		fseek(data_strings, 0, SEEK_SET);
	}
   
   fclose(data_strings);
   fclose(channel);
   
	return 0;
}
