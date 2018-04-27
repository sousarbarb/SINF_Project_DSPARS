/*
 * read_channels_strings.c
 * 
 * Copyright 2018	Daniel Queirós da Silva		<up201503071@fe.up.pt>
 * 					Pedro de Castro Albergaria	<up201504635@fe.up.pt>
 * 					Ricardo Barbosa Sousa		<up201503004@fe.up.pt>
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

#define BUFFER	100

FILE *f = NULL;

int main(int argc, char **argv)
{
	int  nmessages = 0;
	char buff[BUFFER]={0};
	f = fopen("/dev/pts/1", "r");
	while(1){
		if(NULL != fgets(buff, BUFFER, f)){
			printf("[%d] %s\n", nmessages, buff);
			nmessages++;
		}
	}
	return 0;
}

