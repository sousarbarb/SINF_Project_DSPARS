/*
 * main.c
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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <postgresql/libpq-fe.h>
#include "actuators_lib_struct.h"
#include "manipulation_mote_struct.h"
#include "manipulation_rule_struct.h"
#include "manipulation_division_struct.h"
#include "project_matrix.h"

// Errors - MAIN
#define MAIN_ERROR_1  1
#define MAIN_ERROR_2  2
#define MAIN_ERROR_3  3
#define MAIN_ERROR_4  4
#define MAIN_ERROR_5  5

// Errors - Database
#define DB_OK	   0
#define DB_ERROR_1 1

// Errors - thread_data_processing 
#define	ERROR1	1
#define ERROR2	2
#define ERROR3	3
#define	ERROR4	4
#define	ERROR5	5
#define ERROR6	6

// Constants relative to the communication channels
#define MAIN_CHANNEL_SENSOR  1
#define MAIN_CHANNEL_RGBMAT  2
#define MAIN_CHANNEL_LIM_INF 0
#define MAIN_CHANNEL_LIM_SUP 9

// ASCII codes
#define	ASCII_res_number_0	0
#define ASCII_res_number_9	9
#define	ASCII_res_char_A	17
#define ASCII_res_char_F	22
#define ASCII_res_space		-16

// Parameter's position in the string
#define	INIT_BOTTOM		0
#define	INIT_TOP		4
#define	MOTE_ID_BOTTOM	15
#define	MOTE_ID_TOP		19
#define	TEMP_BOTTOM		48
#define	TEMP_TOP		52
#define	HUMID_BOTTOM	54
#define	HUMID_TOP		58
#define	LIGHT_BOTTOM	36
#define	LIGHT_TOP		40
#define	END_BOTTOM		66
#define	END_TOP			67

// Other constants - thread_data_processing
#define LINE	70
#define WORD	6
#define BASE	16
#define SPACE   2

// Types of sensors - function search_sensor_mote
#define TYPE_SENS_TEMP  1
#define TYPE_SENS_HUM   2
#define TYPE_SENS_LIGHT 3

// Global variables
FILE *sensor_data_channel = NULL, *rgb_matrix_channel = NULL;
static const char CHANNEL_DEF[]="/dev/pts/";
int number_motes, number_rules, number_divisions, number_maximum_actuators, number_maximum_sensors, matrix_side_size;
int flag_interface = 1;
int *flag_rules = NULL;
char **pointer_rgb_channel = NULL;
char *buffer_rgb_channel = NULL;
mote **system_motes = NULL;

// DATABASE
const char *conn = "host='db.fe.up.pt' user='sinfa15' password='DSPARS_sinf2018*'";
PGconn *dbconn;


/***************************************************
 * FUNCTIONS USED BY THREAD DATA PROCESSING (PTH1)
 ***************************************************/
int multiplication_by_10(int times);
int power(int base, int exponent);
float calculate_temperature(int number_Temp);
float calculate_relative_humidity(int number_Humid);
float calculate_humidity_compensated_by_temperature(int number_Humid, float humid_relative, float Temp);
float calculate_visible_light (int number_Visible_Light);


/*********************************
 * THREAD DATA PROCESSING (PTH1)
 *********************************/
void *thread_data_processing(void *arg);


/*******************************************************
 * FUNCTIONS USED BY THREAD RULE IMPLEMENTATION (PTH2)
 *******************************************************/
int search_sensor_mote(char * sensor, int number_motes, int * mote_id_sensor, int * type_sensor);


/********************************************
 * THREAD RULE IMPLEMENTATION (PTH2)
 ********************************************/
void *thread_rule_implementation(void *arg);


/********************************
 * THREAD USER INTERFACE (PTH3)
 ********************************/
void *user_interface(void *arg);


/****************************
 * INIT_LOGIN_IN_THE_SYSTEM
 ****************************/
void init_login_in_the_system(void);


/***********************
 * INIT_TABLE_CREATION
 ***********************/
void init_table_creation(void);


/****************************
 * DIVISION'S CONFIGURATION
 ****************************/
void init_divisions_configuration(void);


/************************
 * RULE'S CONFIGURATION
 ************************/
void init_rules_configuration(void);


/*****************
 * +++++++++++++ *
 * + MAIN CODE + *
 * +++++++++++++ *
 *****************/
/************************
 * FUNCTION MAIN - CODE
 ************************/
int main(int argc, char **argv)
{
	/************************************
	 * INIT ROUTINE - ARGUMENT CHECKING
	 ************************************/
	// ERROR 1 - The channels inserted aren't valid
	if(3 !=argc){
		printf("[MAIN_ERROR %d] The execution of the program implies the definition of the channel that is currently receiving data from the sensors and the channel that it'll be used to send the actuators state to the RGBMatrix.\n", MAIN_ERROR_1);
		printf("Please execute the program by the following steps:\n    ./main \"/dev/pts/x\" \"/dev/pts/y\"\n");
		printf("    Channel x -> communication with the system motes (receives data)\n    Channel y -> communication with the RGBMatrix (sends data)\n");
		exit(-1);
	}
	
	int counter1;
	char directory_channel_sensor[strlen(argv[MAIN_CHANNEL_SENSOR])-1], directory_channel_rgbmat[strlen(argv[MAIN_CHANNEL_RGBMAT])-1];
	for(counter1 = 0; counter1 < strlen(argv[MAIN_CHANNEL_SENSOR]) - 1; counter1++)
		directory_channel_sensor[counter1] = argv[MAIN_CHANNEL_SENSOR][counter1];
	directory_channel_sensor[counter1] = '\0';
	for(counter1 = 0; counter1 < strlen(argv[MAIN_CHANNEL_RGBMAT]) - 1; counter1++)
		directory_channel_rgbmat[counter1] = argv[MAIN_CHANNEL_RGBMAT][counter1];
	directory_channel_rgbmat[counter1] = '\0';
	
	// ERROR 2 - The channel's number selected aren't valid
	if(0 != strcmp(CHANNEL_DEF, directory_channel_sensor) || 0 != strcmp(CHANNEL_DEF, directory_channel_rgbmat) || MAIN_CHANNEL_LIM_INF > (argv[MAIN_CHANNEL_SENSOR][9]-'0') || MAIN_CHANNEL_LIM_SUP < (argv[MAIN_CHANNEL_SENSOR][9]-'0') || MAIN_CHANNEL_LIM_INF > (argv[MAIN_CHANNEL_RGBMAT][9]-'0') || MAIN_CHANNEL_LIM_SUP < (argv[MAIN_CHANNEL_RGBMAT][9]-'0')){
		printf("[MAIN_ERROR %d] The channel's directory aren't correctly characterized.\n", MAIN_ERROR_2);
		printf("Please execute the program by the following steps:\n    ./main \"/dev/pts/x\" \"/dev/pts/y\"\n");
		printf("    Channel x -> communication with the system motes (receives data)\n    Channel y -> communication with the RGBMatrix (sends data)\n");
		exit(-1);
	}
	sensor_data_channel = fopen(argv[MAIN_CHANNEL_SENSOR], "r");
	// ERROR 3
	if(NULL == sensor_data_channel){
		printf("[MAIN_ERROR %d] It's not possible to access the selected channel relative with the sensor's readings\n", MAIN_ERROR_3);
		exit(-1);
	}
	rgb_matrix_channel  = fopen(argv[MAIN_CHANNEL_RGBMAT], "w");
	// ERROR 4
	if(NULL == rgb_matrix_channel){
		printf("[MAIN_ERROR %d] It's not possible to access the selected channel relative with the actuator's state display\n", MAIN_ERROR_4);
		fclose(sensor_data_channel);
		sensor_data_channel = NULL;
		exit(-1);
	}
	pointer_rgb_channel = &argv[MAIN_CHANNEL_RGBMAT];
	
	/*************************
	 * VARIABLES DECLARATION
	 *************************/
	int error_check = 0;
	pthread_t pth1, pth2, pth3;
	
	/**********************************
	 * INIT ROUTINE - WELCOME MESSAGE
	 **********************************/
	printf("\n******************** HAS - HOME AUTOMATIVE SYSTEM ********************\n\n");
	 
	/**************************************
	 * INIT ROUTINE - DATABASE CONNECTION
	 **************************************/
	dbconn = PQconnectdb(conn);
	if (PQstatus(dbconn) == CONNECTION_BAD){
		printf("[DATABASE_ERROR %d] Unable to connect with the database. Please try again by restarting the program or check the ethernet connection\n", DB_ERROR_1);
		exit(-1);
	} else{
		printf("[DATABASE_ERROR %d] Able to connect with the database!\n", DB_OK);
	}
	 
	/**************************************************
	 * INIT ROUTINE - TABLES CREATION IN THE DATABASE
	 **************************************************/
	init_table_creation();
	
	/***************************************
	 * INIT ROUTINE - MOTE'S CONFIGURATION *
	 ***************************************/
	printf("\n++++++++++ MOTE'S CONFIGURATION ++++++++++\n");
	error_check = 0;
	do{
		printf("Insert the number of motes that are considered in the system (needs to be greater than 0): ");
		scanf(" %d", &number_motes);
		getchar();
	} while(1 > number_motes);
	system_motes = mote_vector_creation(number_motes, &error_check);
	if(NULL == system_motes || 0 < error_check){
		printf("[MAIN_ERROR %d] The motes weren't created.\n", MAIN_ERROR_5);
		fclose(sensor_data_channel);
		sensor_data_channel = NULL;
		fclose(rgb_matrix_channel);
		rgb_matrix_channel = NULL;
		PQfinish(dbconn);
		dbconn = NULL;
		exit(-1);
	}
	
	/**************************************
	 * INIT ROUTINE - LOGIN IN THE SYSTEM
	 **************************************/
	printf("\n++++++++++ LOGIN ++++++++++\n");
	init_login_in_the_system();
	
	/*******************************************
	 * INIT ROUTINE - DIVISION'S CONFIGURATION
	 *******************************************/
	printf("\n++++++++++ DIVISION'S CONFIGURATION ++++++++++\n");
	init_divisions_configuration();
	
	/*******************************************
	 * INIT ROUTINE - RULE'S CONFIGURATION
	 *******************************************/
	printf("\n++++++++++ RULE'S CONFIGURATION ++++++++++\n");
	init_rules_configuration();
	
	/*****************************************
	 * INIT ROUTINE - MATRIX'S CONFIGURATION
	 *****************************************/
	
	/*************
	 * CODE TEST
	 *************/
	print_motes_vector(system_motes, number_motes, NULL);
	PGresult *query = PQexec(dbconn, "SELECT * FROM test_table");
	printf("Result Query: %d\n", PQresultStatus(query));
	printf("Fatal Error: %d\n", PGRES_FATAL_ERROR);
	printf("Error Message: %s\n", PQresultErrorMessage(query));
	if (PQresultStatus(query) == PGRES_TUPLES_OK){
		printf("Tuples OK!\n");
		int nt = PQntuples(query);
		if(nt != 0){
			printf("Number of tuples to query: %d\n" , nt);
			
			int i;
			for (i=0;i < nt; i++){
				printf("%s \t %s\n" , PQgetvalue(query,i,0), PQgetvalue(query,i,1));
			}
		}else{
			printf("No rows to fetch! \n");
		}
	}
	PQclear(query);
	
	/***********************
	 * CREATION OF THREADS
	 ***********************//*
	pthread_create(&pth1, NULL, thread_data_processing, NULL);
	pthread_create(&pth2, NULL, thread_rule_implementation, NULL);
	pthread_create(&pth3, NULL, user_interface, NULL);*/
	
	/****************************************
	 * CREATION AND CANCELLATION OF THREADS
	 ****************************************//*
	pthread_join(pth3, NULL);
	pthread_cancel(pth1);
	pthread_cancel(pth2);
	pthread_cancel(pth3);*/
	
	/***********************
	 * TERMINATION ROUTINE
	 ***********************/
	free_mote_memory(system_motes, number_motes, NULL);
	free(buffer_rgb_channel);
	buffer_rgb_channel = NULL;
	free(flag_rules);
	flag_rules = NULL;
	fclose(sensor_data_channel);
	sensor_data_channel = NULL;
	PQfinish(dbconn);
	dbconn = NULL;
	printf("\n**********************************************************************\n");
	printf("************************* END OF PROGRAM HAS *************************\n");
	printf("**********************************************************************\n\n");
	return 0;
	exit(0);
}


/***************************************************
 * +++++++++++++++++++++++++++++++++++++++++++++++ *
 * + CODE RELATIVE TO FUNCTIONS USED BY THE MAIN + *
 * +++++++++++++++++++++++++++++++++++++++++++++++ *
 ***************************************************/
/***********************************
 * INIT_LOGIN_IN_THE_SYSTEM - CODE
 ***********************************/
void init_login_in_the_system(void){
	
}


/******************************
 * INIT_TABLE_CREATION - CODE
 ******************************/
void init_table_creation(void){
	/* DANIEL DA SILVA QUEIRÓS
	 * 
	 * 1-Verificar se cada tabela existe
	 * 		usar TUPPLES_OK caso se utilize o comando SELECT
	 * 2-Criar as tabelas
	 * 		usar o código presente no Github
	 * 3-Enviar numa flag se as tabelas já estavam criadas ou não
	 * 		- criar defines TABLE_ALREADY_CREATED e NOT_CREATED
	 * 		- alterar o argumento para pôr int * flag... (ver um nome para variável)
	 * 4-Se já existir as tabelas, imprimi-las
	 * 		- cria uma função geral, à parte, para efetuar esta operação
	 * 
	 * OBS - criar uma função que só crie as tabelas e uma função que só verifique se já 
	 * estão criadas para o Pedro poder chamá-la
	 * 		- uma função para cada tabela - 1 para criar e 1 para verificar (e depois
	 * nesta função chamar essas funções todas)
	 */
}


/***********************************
 * DIVISION'S CONFIGURATION - CODE
 ***********************************/
void init_divisions_configuration(void){
	/* PEDRO DE CASTRO MOTA CALBERGARIA
	 * 
	 * 1-Perguntar ao utilizador se quer manter o que estava lá antes ou então fazer
	 *  drop a todas as tabelas e criá-las novamente -> preenchendo esses dados
	 * 		- caso queira alterar nao fazer drop ao building e ao apartment, users e
	 *  users activity
	 * 2-Preencher e processar dados do utilizador
	 * 		- sensores
	 * 		- divisões
	 * 		- atuadores
	 * 3-Iniciar os atuadores todos a OFF inicialmente
	 * 
	 * OBS - por cada divisão inserida, faz uma user_activity (deixar para mais tarde)
	 * OBS - não esquecer de atualizar number_maximum_actuators e number_maximum_sensors
	 */
}


/*******************************
 * RULE'S CONFIGURATION - CODE
 *******************************/
void init_rules_configuration(void){
	/* O TÉCNICO BARBOSA
	 * 
	 * 1-Perguntar ao utilizador se quer alterar as regras todas ou não
	 * 		- não fazer drop ao building, apartment, divisions, user_activity, users, sensors e actuatores
	 * 2-Preencher dados na DB
	 * 		- rules
	 * 		- actuator_future_state
	 */
}


/************************************
 * ++++++++++++++++++++++++++++++++ *
 * + CODE RELATIVE TO THE THREADS + *
 * ++++++++++++++++++++++++++++++++ *
 ************************************/
/**********************************************************
 * FUNCTIONS USED BY THREAD DATA PROCESSING (PTH1) - CODE
 **********************************************************/
int multiplication_by_10(int times){
	
	if(0 == times)
		return 1000;
	else if(1 == times)
		return 100;
	else if(2 == times)
		return 10;
	else return 1;
}

int power(int base, int exponent){
	int step = 0, result = 1;
	
	if((base < 0) || (exponent < 0)){
		return 0;
	}
	else{
		for(step= 0; step < exponent; step++){
			result = result * base;
		}
		return result;
	}
}

float calculate_temperature(int number_Temp){
	return -39.6 + 0.01*number_Temp;
}

float calculate_relative_humidity(int number_Humid){
	return -2.0468 + 0.0367*number_Humid - 1.5955*0.000001*power(number_Humid,2);
}

float calculate_humidity_compensated_by_temperature(int number_Humid, float humid_relative, float Temp){
	return (Temp - 25)*(0.01 + 0.00008*number_Humid) + humid_relative;
}

float calculate_visible_light (int number_Visible_Light){
	return 0.625*power(10,6)*(number_Visible_Light/4096.0)*(1.5/power(10,5))*1000;
}


/****************************************
 * THREAD DATA PROCESSING (PTH1) - CODE
 ****************************************/
void *thread_data_processing(void *arg){
	// Variable's declaration
	int step_line = 0, step_word = 0, fill_up = 0, times_10 = 0, times_16 = 0;
	int	decimal_id = 0, convert = 0, flag_invalid = 0, decimal_temp = 0, decimal_humid = 0, decimal_visible_light = 0;
	float temp = 0, relative_humidity = 0, humidity_compensated_by_temperature = 0, visible_light = 0;
	char line[LINE], word[WORD];
	
	// Code
	while(1 == flag_interface){
		fgets(line, LINE, sensor_data_channel);
		// printf("DEBUG: %s", line);
		for(step_line = 0; step_line < strlen(line); step_line++){
			// Beginning of line
			if(INIT_BOTTOM == step_line){
				fill_up = 0;
				for(step_word = INIT_BOTTOM; step_word <= INIT_TOP; step_word++){
					if(step_word != (INIT_BOTTOM + SPACE)){
						word[fill_up] = line[step_word];
						fill_up++;
					}
				}
				word[fill_up] = '\0';
				
				if(0 == strcmp(word,"7E45")){
					//printf("Valid init!\n");
				}
				else {
					printf("ERROR[%d] - The message start bytes are incorrect!\n",ERROR1);
					break;
				}
			}
			// MOTE ID
			else if(MOTE_ID_BOTTOM == step_line){
				flag_invalid = 0;
				times_10 = 0;
				decimal_id = 0;
				for(step_word = MOTE_ID_BOTTOM; step_word <= MOTE_ID_TOP; step_word++){
					convert = line[step_word] - '0';
					if((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9) && (step_word != (MOTE_ID_BOTTOM + SPACE))){
						decimal_id = decimal_id + convert*multiplication_by_10(times_10);
						times_10++;
					}
					else if(!((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9)) && (step_word != (MOTE_ID_BOTTOM + SPACE))){
						flag_invalid = 1;
						break;
					}
					else{
						decimal_id += 0;
					}
				}
				if((decimal_id > 9999) || (flag_invalid)){
					printf("ERROR[%d] - Mote ID invalid!\n",ERROR2);
				}/*
				else{
					printf("Mote ID: %d\n",decimal_id);
				}*/
			}
			// Temperature parameter
			else if(TEMP_BOTTOM == step_line){
				flag_invalid = 0;
				times_16 = 0;
				decimal_temp = 0;
				for(step_word = TEMP_TOP; step_word >= TEMP_BOTTOM; step_word--){
					convert = line[step_word] - '0';
					if((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F) && (step_word != (TEMP_BOTTOM + SPACE))){
						convert = line[step_word] - 'A' + 10;
						decimal_temp = decimal_temp + convert*power(BASE,times_16);
						times_16++;
					}
					else if((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9) && (step_word != (TEMP_BOTTOM + SPACE))){
						decimal_temp = decimal_temp + convert*power(BASE,times_16);
						times_16++;
					}
					else if(!((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F)) && !((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9)) && (step_word != (TEMP_BOTTOM + SPACE))){ 
						flag_invalid = 1;
						break;
					}
					else{
						decimal_temp += 0;
					}
				}
				if(flag_invalid){
					printf("ERROR[%d] - Temperature parameter invalid!\n",ERROR3);
				}
				else{
					//printf("Temperatura decimal = %d\n", decimal_temp);
					temp = calculate_temperature(decimal_temp);
					system_motes[search_mote(system_motes,number_motes,decimal_id,NULL)]->temperature = temp;
					
					//printf("Temperature = %.2f ºC\n",temp);
				}
			}
			// Humidity parameter
			else if(HUMID_BOTTOM == step_line){
				flag_invalid = 0;
				times_16 = 0;
				decimal_humid = 0;
				for(step_word = HUMID_TOP; step_word >= HUMID_BOTTOM; step_word--){
					convert = line[step_word] - '0';
					if((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F) && (step_word != (HUMID_BOTTOM + SPACE))){
						convert = line[step_word] - 'A' + 10;
						decimal_humid = decimal_humid + convert*power(BASE,times_16);
						times_16++;
					}
					else if((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9) && (step_word != (HUMID_BOTTOM + SPACE))){
						decimal_humid = decimal_humid + convert*power(BASE,times_16);
						times_16++;
					}
					else if(!((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F)) && !((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9)) && (step_word != (HUMID_BOTTOM + SPACE))){
						flag_invalid = 1;
						break;
					}
					else{
						decimal_humid += 0;
					}
				}
				if(flag_invalid){
					printf("ERROR[%d] - Humidity parameter invalid!\n",ERROR4);
				}
				else{
					//printf("Humidade decimal = %d\n",decimal_humid);
					relative_humidity = calculate_relative_humidity(decimal_humid);
					//printf("Relative humidity = %.2f %%\n",relative_humidity);
					
					humidity_compensated_by_temperature = calculate_humidity_compensated_by_temperature(decimal_humid,relative_humidity,temp);
					system_motes[search_mote(system_motes,number_motes,decimal_id,NULL)]->humidity = humidity_compensated_by_temperature;
					
					//printf("Humidity compensated by temperature = %.2f %%\n",humidity_compensated_by_temperature);
				}
			}
			// Visible light parameter
			else if(LIGHT_BOTTOM == step_line){
				flag_invalid = 0;
				times_16 = 0;
				decimal_visible_light = 0;
				for(step_word = LIGHT_TOP; step_word >= LIGHT_BOTTOM; step_word--){
					convert = line[step_word] - '0';
					if((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F) && (step_word != (LIGHT_BOTTOM + SPACE))){
						convert = line[step_word] - 'A' + 10;
						decimal_visible_light = decimal_visible_light + convert*power(BASE,times_16);
						times_16++;
					}
					else if((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9) && (step_word != (LIGHT_BOTTOM + SPACE))){
						decimal_visible_light = decimal_visible_light + convert*power(BASE,times_16);
						times_16++;
					}
					else if(!((convert >= ASCII_res_char_A) && (convert <= ASCII_res_char_F)) && !((convert >= ASCII_res_number_0) && (convert <= ASCII_res_number_9)) && (step_word != (LIGHT_BOTTOM + SPACE))){
						flag_invalid = 1;
						break;
					}
					else{
						decimal_visible_light += 0;
					}
				}
				if(flag_invalid){
					printf("ERROR[%d] - Visible light parameter invalid!\n",ERROR5);
				}
				else{
					//printf("Luz visível decimal = %d\n",decimal_visible_light);
					visible_light = calculate_visible_light(decimal_visible_light);
					system_motes[search_mote(system_motes,number_motes,decimal_id,NULL)]->luminosity = visible_light;
					
					//printf("Visible light = %.2f lux\n", visible_light);
				}
			}
			// End of line
			else if (END_BOTTOM == step_line){
				fill_up = 0;
				for(step_word = END_BOTTOM; step_word <= END_TOP; step_word++){
					word[fill_up] = line[step_word];
					fill_up++;
				}
				word[fill_up] = '\0';
				if(0 == strcmp(word,"7E")){
					//printf("Valid end!\n");
					break;
				}
				else {
					printf("ERROR[%d] - Invalid end!\n",ERROR6);
					break;
				}
			}
		}
	}
	pthread_exit(NULL);
}


/**************************************************************
 * FUNCTIONS USED BY THREAD RULE IMPLEMENTATION (PTH2) - CODE
 **************************************************************/
int search_sensor_mote(char * sensor, int number_motes, int * mote_id_sensor, int * type_sensor){
	int integer_aux_1=0, numeration_sensor;
	(*type_sensor)=0;
	switch(sensor[0]){
		case 'L': integer_aux_1=5; (*type_sensor)=TYPE_SENS_LIGHT; break;
		case 'H': integer_aux_1=3; (*type_sensor)=TYPE_SENS_HUM;   break;
		case 'T': integer_aux_1=4; (*type_sensor)=TYPE_SENS_TEMP;  break;
		default: return -1;
	}
	if('\0' == sensor[integer_aux_1]){
		(*mote_id_sensor) = 1;
		return 1;
	}
	numeration_sensor = atoi(&sensor[integer_aux_1]);
	if(numeration_sensor <= number_motes){
		(*mote_id_sensor) = numeration_sensor;
		return 1;
	}
	else{
		(*mote_id_sensor) = -1;
		return 0;
	}
}


/********************************************
 * THREAD RULE IMPLEMENTATION (PTH2) - CODE
 ********************************************/
void *thread_rule_implementation(void *arg){
	
	
	pthread_exit(NULL);
}


/***************************************
 * THREAD USER INTERFACE (PTH3) - CODE
 ***************************************/
void *user_interface(void *arg){
	
	
	pthread_exit(NULL);
}
