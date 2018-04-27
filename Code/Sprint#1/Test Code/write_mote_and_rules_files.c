/*
 * write_mote_and_rules_files.c
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

#define BUFFER	200

int nMotes = 0, nDivisions = 0, nRulesDivisions = 0;

typedef struct mote_template{
	float temperature;
	float humidity;
	float luminosity;
} mote;

int main(int argc, char **argv)
{	 
	int counter1, var1; 
	mote **system_motes = NULL; 
	char buffer[BUFFER]; 
	char **sensor_configurations = NULL, **sensor_rules = NULL;
	FILE *sensor_config_txt = NULL, *sensor_rules_txt = NULL;
  
	sensor_config_txt = fopen("SensorConfigurations.txt", "w+");	//Creates an empty file for both reading and writing for sensors configuration
	sensor_rules_txt = fopen("SensorRules.txt", "w+");				//Creates an empty file for both reading and writing for sensors rules
   
	do{ 
		printf("Insert the number of motes of the system (number of Motes > 0; select 0 to exit the program): "); 
		scanf("%d", &nMotes); 
		if(0 == nMotes){ 
			printf("\nPROGRAM ENDED\n"); 
			exit(-1); 
		} 
		getchar(); 
  } while(nMotes < 1); 
   
	// Memory allocation for the system motes 
	system_motes = (mote **) malloc(sizeof(mote *) * nMotes); 
	if(NULL == system_motes){ 
		printf("\n[1] The allocation of memory for the system_motes isn't avaiable.\nPROGRAM ENDED\n"); 
		exit(-1); 
	} 
	for(counter1 = 0; counter1 < nMotes; counter1++){ 
		system_motes[counter1] = (mote *) malloc(sizeof(mote)); 
		if(NULL == system_motes){ 
			printf("\n[2] The allocation of memory for the system_motes[x] isn't avaiable.\nPROGRAM ENDED\n"); 
			exit(-1); 
		} 
		system_motes[counter1]->temperature = 0; 
		system_motes[counter1]->humidity = 0; 
		system_motes[counter1]->luminosity = 0; 
	} 
   
	// Sensor configuration 
	printf("\nInsert number of divisions: "); 
	scanf("%d", &nDivisions); 
	getchar(); 
	sensor_configurations = (char **) malloc(sizeof(char *) * nDivisions); 
	if(NULL == sensor_configurations){ 
		printf("\n[3] The allocation of memory for the sensor_configurations isn't avaiable.\nPROGRAM ENDED\n"); 
		exit(-1); 
	} 
	printf("\nOBS - The format for the sensors configuration must be the following:\n	<room_name>:<sensor_name>[,<sensor_name>]:<actuator>[,actuator]\n\nExample - ROOM1:TEMP1,HUM1,LIGHT1:HEAT_ROOM1,LIGHT_ROOM1\n\n");
	for(counter1 = 0; counter1 < nDivisions; counter1++){ 
		printf("Describe the room [%d] configuration: ", counter1+1); 
		fgets(buffer, BUFFER, stdin); 
		var1 = strlen(buffer)-1; 
		buffer[ var1 ] = '\0'; 
		sensor_configurations[counter1] = (char *) malloc(var1); 
		if(NULL == sensor_configurations[counter1]){ 
			printf("\n[4] The allocation of memory for the sensor_configurations[x] isn't avaiable.\nPROGRAM ENDED\n"); 
			exit(-1); 
		} 
		strcpy(sensor_configurations[counter1], buffer); 
	} 	
   
	// Sensor rules 
	printf("\nInsert number division's rules: "); 
	scanf("%d", &nRulesDivisions); 
	getchar(); 
	sensor_rules = (char **) malloc(sizeof(char *) * nRulesDivisions); 
	if(NULL == sensor_rules){ 
		printf("\n[5] The allocation of memory for the sensor_rules isn't avaiable.\nPROGRAM ENDED\n"); 
		exit(-1); 
	} 
	printf("\nOBS - The format for the sensors rules must be the following:\n	<room_name>: <sensor_name><condition><value> [AND/OR <sensor_name><condition><value>] <actuator>:<state>[,<actuator>:<state>]\n\nExample - ROOM1: TEMP1>25 AND LIGHT1>3000 HEAT_ROOM1:OFF,LIGHT_ROOM1:OFF\n\n");
	for(counter1 = 0; counter1 < nRulesDivisions; counter1++){ 
		printf("Describe the rules applied to the division [%d]: ", counter1+1); 
		fgets(buffer, BUFFER, stdin); 
		var1 = strlen(buffer)-1; 
		buffer[ var1 ] = '\0'; 
		sensor_rules[counter1] = (char *) malloc(var1); 
		if(NULL == sensor_rules[counter1]){ 
			printf("\n[6] The allocation of memory for the sensor_rules[x] isn't avaiable.\nPROGRAM ENDED\n"); 
			exit(-1); 
		} 
		strcpy(sensor_rules[counter1], buffer); 
	} 
   
	// Value of the motes parameters 
	printf("\n----- MOTES PARAMETERS -----\n"); 
	for(counter1 = 0; counter1 < nMotes; counter1++){ 
		printf("[MOTE %d]\n", counter1 + 1); 
		printf("   Temperature = %f\n", system_motes[counter1]->temperature); 
		printf("   Humidity = %f\n", system_motes[counter1]->humidity); 
		printf("   Luminosity = %f\n", system_motes[counter1]->luminosity); 
	} 
   
	// Display the sensors configuration 
	printf("\n----- SENSORS CONFIGURATION -----\n"); 
	for(counter1 = 0; counter1 < nDivisions; counter1++) {
		printf("[Division %d] %s\n", counter1+1, sensor_configurations[counter1]);
		fprintf(sensor_config_txt, "%s\n\n", sensor_configurations[counter1]); 
	}
  
	// Display the sensors rules 
	printf("\n----- SENSORS RULES -----\n"); 
	for(counter1 = 0; counter1 < nRulesDivisions; counter1++) {
		printf("[Division %d Rules] %s\n", counter1+1, sensor_rules[counter1]); 
		fprintf(sensor_rules_txt, "%s\n\n", sensor_rules[counter1]);
	}
  
	fclose(sensor_config_txt);
	fclose(sensor_rules_txt);
   
	// Free Memory  
	for(counter1 = 0; counter1 < nMotes; counter1++){ 
		free(system_motes[counter1]); 
		system_motes[counter1] = NULL; 
	} 
	free(system_motes); 
	system_motes = NULL; 
   
	for(counter1 = 0; counter1 < nDivisions; counter1++){ 
		free(sensor_configurations[counter1]); 
		sensor_configurations[counter1] = NULL; 
	} 
	free(sensor_configurations); 
	sensor_configurations = NULL; 
   
	for(counter1 = 0; counter1 < nDivisions; counter1++){ 
		free(sensor_rules[counter1]); 
		sensor_rules[counter1] = NULL; 
	} 
	free(sensor_rules); 
	sensor_rules = NULL; 
  
	return 0;
}

