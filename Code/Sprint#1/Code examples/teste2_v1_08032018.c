/*
 * teste2_v1_08032018.c
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

#define STRING_MAX 50

struct UserID {
	char username[STRING_MAX];
	char country[STRING_MAX];
} user1, user2;

int main(int argc, char **argv)
{
	int NumbUsers;
	if(3 != argc){
		printf("Please run the program correctly: ./teste2_v1_08032018 \"Username\" \"Country\"\n");
		exit(0);
	}
	strcpy(user1.username, argv[1]);
	strcpy(user1.country, argv[2]);
	NumbUsers = 1;
	printf("Do you want to specify another user? (1 - Yes / 0 - No, and then press Enter) ");
	if('1' == getchar()){
		NumbUsers = 2;
		getchar();		// To ignore the character '\n'
		printf("\nInsert username for user2 (please press Enter after that): ");
		fgets(user2.username, STRING_MAX, stdin);
		user2.username[strlen(user2.username)-1]='\0';	// To eliminate the character '\n' from the string user2.username
		printf("Insert the country which user2 is from (please press Enter after that): ");
		fgets(user2.country, STRING_MAX, stdin);
		user2.country[strlen(user2.country)-1]='\0';	// To eliminate the character '\n' from the string user2.country
	
		/****************************************************************************************************
		 * NOTE:
		 * We cannot use the scanf function because it only read until a space is found. To read a complete
		 * string until the character '\n' is founded, we need to use fgets
		 ****************************************************************************************************/ 
	}
	printf("\n----- INFO USER 1 -----\n  Username: %s\n  Country: %s\n", user1.username, user1.country);
	if(2 == NumbUsers)
		printf("----- INFO USER 2 -----\n  Username: %s\n  Country: %s\n\n", user2.username, user2.country);
	return 0;
}

