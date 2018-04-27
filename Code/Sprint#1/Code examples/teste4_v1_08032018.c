/*
 * teste4_v1_08032018.c
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

#define BUFFERSIZE 100

typedef struct userInfoStructure{
	char *userID;
	char *username;
	char *birthday;
} userType;

void printUserInfoStructure(userType** vector, int nUsers){
	int i;
	if(NULL == vector || 0 >= nUsers){
		printf("You're so stupid that you send wrong arguments to the function \"printUserInfoStructure\"\nCATCHAO\n");
		return;
	}
	printf("\n----- VOLATILE DATABASE OF USERS -----\n");
	for(i=0; i<nUsers; i++)
		printf("[User %d]\n  UserID:   %s\n  Username: %s\n  Birthday: %s\n\n", i+1, vector[i]->userID, vector[i]->username, vector[i]->birthday);
}

void freeAllTheFuckin_Memory(userType** vector, int nUsers){
	int j;
	for(j=0; j<nUsers; j++){
		free(vector[j]->userID);
		vector[j]->userID = NULL;
		
		free(vector[j]->username);
		vector[j]->username = NULL;
		
		free(vector[j]->birthday);
		vector[j]->birthday = NULL;
		
		free(vector[j]);
		vector[j] = NULL;
	}
	free(vector);
	vector = NULL;
}

int main(int argc, char **argv)
{
	int numberUsers, i, j;
	char buffer[BUFFERSIZE];
	userType **vector_users;
	printf("How many users do you want to store in memory? ");
	scanf("%d", &numberUsers);
	getchar();
	if(0 < numberUsers){
		vector_users = (userType **) malloc(numberUsers * sizeof(userType*));
		if(NULL == vector_users){
			printf("It isn't possible allocate memory right now. Please try agaian later.\n");
			exit(0);
		}
		for(i=0; i<numberUsers; i++){
			vector_users[i] = (userType *) malloc(sizeof(userType));
			if(NULL == vector_users[i]){
				printf("It isn't possible allocate memory right now. Please try agaian later.\n");
				for(j=0; j<i; j++){
					free(vector_users[j]->userID);
					vector_users[j]->userID = NULL;
					
					free(vector_users[j]->username);
					vector_users[j]->username = NULL;
					
					free(vector_users[j]->birthday);
					vector_users[j]->birthday = NULL;
					
					free(vector_users[j]);
					vector_users[j] = NULL;
				}
				free(vector_users);
				vector_users = NULL;
				exit(0);
			}
			
			printf("[User %d] Insert userID:   ", i+1);
			fgets(buffer, BUFFERSIZE, stdin);
			buffer[strlen(buffer)-1] = '\0';
			
			vector_users[i]->userID = (char *) malloc(strlen(buffer));
			if(NULL == vector_users[i]->userID){
				printf("It isn't possible allocate memory right now. Please try agaian later.\n");
				for(j=0; j<=i; j++){
					free(vector_users[j]->userID);
					vector_users[j]->userID = NULL;
					
					free(vector_users[j]->username);
					vector_users[j]->username = NULL;
					
					free(vector_users[j]->birthday);
					vector_users[j]->birthday = NULL;
					
					free(vector_users[j]);
					vector_users[j] = NULL;
				}
				free(vector_users);
				vector_users = NULL;
				exit(0);
			}
			
			strcpy(vector_users[i]->userID, buffer);
			
			printf("[User %d] Insert username: ", i+1);
			fgets(buffer, BUFFERSIZE, stdin);
			buffer[strlen(buffer)-1] = '\0';
			
			vector_users[i]->username = (char *) malloc(strlen(buffer));
			if(NULL == vector_users[i]->username){
				printf("It isn't possible allocate memory right now. Please try agaian later.\n");
				for(j=0; j<=i; j++){
					free(vector_users[j]->userID);
					vector_users[j]->userID = NULL;
					
					free(vector_users[j]->username);
					vector_users[j]->username = NULL;
					
					free(vector_users[j]->birthday);
					vector_users[j]->birthday = NULL;
					
					free(vector_users[j]);
					vector_users[j] = NULL;
				}
				free(vector_users);
				vector_users = NULL;
				exit(0);
			}
			
			strcpy(vector_users[i]->username, buffer);
			
			printf("[User %d] Insert birthday: ", i+1);
			fgets(buffer, BUFFERSIZE, stdin);
			buffer[strlen(buffer)-1] = '\0';
			
			vector_users[i]->birthday = (char *) malloc(strlen(buffer));
			if(NULL == vector_users[i]->birthday){
				printf("It isn't possible allocate memory right now. Please try agaian later.\n");
				for(j=0; j<=i; j++){
					free(vector_users[j]->userID);
					vector_users[j]->userID = NULL;
					
					free(vector_users[j]->username);
					vector_users[j]->username = NULL;
					
					free(vector_users[j]->birthday);
					vector_users[j]->birthday = NULL;
					
					free(vector_users[j]);
					vector_users[j] = NULL;
				}
				free(vector_users);
				vector_users = NULL;
				exit(0);
			}
			
			strcpy(vector_users[i]->birthday, buffer);
		}
	}
	
	printUserInfoStructure(vector_users, numberUsers);
	
	freeAllTheFuckin_Memory(vector_users, numberUsers);
	
	printf("\nPROGRAM ENDED\n");
	return 0;
}

/********************************************************
 * POSSIBLE OUTCOME OF THE PROGRAM:
 * 
 * How many users do you want to store in memory? 3
 * [User 1] Insert userID:   201503004
 * [User 1] Insert username: Ricardo Barbosa Sousa
 * [User 1] Insert birthday: 21-09-1997
 * [User 2] Insert userID:   201503071
 * [User 2] Insert username: Daniel Queirós da Silva
 * [User 2] Insert birthday: 19-05-1997
 * [User 3] Insert userID:   201504635
 * [User 3] Insert username: Pedro de Castro Albergaria
 * [User 3] Insert birthday: 22-08-1997
 *
 * ----- VOLATILE DATABASE OF USERS -----
 * [User 1]
 *   UserID:   201503004
 *   Username: Ricardo Barbosa Sousa
 *   Birthday: 21-09-1997
 * [User 2]
 *   UserID:   201503071
 *   Username: Daniel Queirós da Silva!
 *   Birthday: 19-05-1997
 * [User 3]
 *   UserID:   201504635
 *   Username: Pedro de Castro Albergaria
 *   Birthday: 22-08-1997
 *
 * PROGRAM ENDED
 *
 ********************************************************/ 
