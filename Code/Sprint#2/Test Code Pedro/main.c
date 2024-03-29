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
#include <termios.h>
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

//
#define ERROR_INIT_RULES_CONFIG_1  1
#define ERROR_INIT_RULES_CONFIG_2  2
#define ERROR_INIT_RULES_CONFIG_3  3
#define ERROR_INIT_RULES_CONFIG_4  4
#define ERROR_INIT_RULES_CONFIG_5  5
#define ERROR_INIT_RULES_CONFIG_6  6
#define ERROR_INIT_RULES_CONFIG_7  7
#define ERROR_INIT_RULES_CONFIG_8  8
#define ERROR_INIT_RULES_CONFIG_9  9
#define ERROR_INIT_RULES_CONFIG_10 10
#define ERROR_INIT_RULES_CONFIG_11 11
#define ERROR_INIT_RULES_CONFIG_12 12
#define ERROR_INIT_RULES_CONFIG_13 13
#define ERROR_INIT_RULES_CONFIG_14 14
#define ERROR_INIT_RULES_CONFIG_15 15

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

// User constants
#define SIZE_MAX_USERNAME        64
#define SIZE_MAX_USER_PERMISSION 128
#define SIZE_MAX_USER_PASSWORD   10

// Login constans
#define INIT_LOGIN_IN_THE_SYSTEM_BUFFER_SIZE 150
#define LOGIN_LIM_INVALID_TRIES              3

// Division's cofiguration defines

#define NEW_DIVISIONS_CONFIGURATION 1
#define OLD_DIVISIONS_CONFIGURATION 0

#define ERROR_CHECK_0 0
#define ERROR_CHECK_1 1
#define ERROR_CHECK_2 2
#define ERROR_CHECK_3 3
#define ERROR_CHECK_4 4
#define ERROR_CHECK_5 5

#define SIZE_STRING_BUFFER 		100
#define SIZE_STRING_DIVISION 	20
#define SIZE_STRING_SENSOR 		20
#define SIZE_STRING_ACTUATOR	20
#define SIZE_QUERY_STRING       256
#define SIZE_MOTE_ID			1

// Global variables
FILE *sensor_data_channel = NULL, *rgb_matrix_channel = NULL;
static const char CHANNEL_DEF[]="/dev/pts/";
int number_motes, number_rules, number_divisions, number_maximum_actuators, number_maximum_sensors, matrix_side_size, count_divisions, divisions_configuration, sens_matrix_id, id_sensor, act_matrix_id, id_actuator;
int flag_interface = 1;
int *flag_rules = NULL;
char **pointer_rgb_channel = NULL;
char *buffer_rgb_channel = NULL;
mote **system_motes = NULL;

// DATABASE
const char *conn = "host='db.fe.up.pt' user='sinfa15' password='DSPARS_sinf2018*'";
PGconn *dbconn;

// User information
int user_id, user_id_apartment;
char username[SIZE_MAX_USERNAME], user_permission[SIZE_MAX_USER_PERMISSION], user_password[SIZE_MAX_USER_PASSWORD];


/***************************************************
   FUNCTIONS USED BY THREAD DATA PROCESSING (PTH1)
 ***************************************************/
int multiplication_by_10(int times);
int power(int base, int exponent);
float calculate_temperature(int number_Temp);
float calculate_relative_humidity(int number_Humid);
float calculate_humidity_compensated_by_temperature(int number_Humid, float humid_relative, float Temp);
float calculate_visible_light (int number_Visible_Light);


/*********************************
   THREAD DATA PROCESSING (PTH1)
 *********************************/
void *thread_data_processing(void *arg);


/*******************************************************
   FUNCTIONS USED BY THREAD RULE IMPLEMENTATION (PTH2)
 *******************************************************/
int search_sensor_mote(char * sensor, int number_motes, int * mote_id_sensor, int * type_sensor);


/********************************************
   THREAD RULE IMPLEMENTATION (PTH2)
 ********************************************/
void *thread_rule_implementation(void *arg);


/********************************
   THREAD USER INTERFACE (PTH3)
 ********************************/
void *user_interface(void *arg);


/****************************
   INIT_LOGIN_IN_THE_SYSTEM
 ****************************/
void init_login_in_the_system(void);


/***********************
   INIT_TABLE_CREATION
 ***********************/
void init_table_creation(void);


/****************************
   DIVISION'S CONFIGURATION
 ****************************/
void init_divisions_configuration(void);


/************************
   RULE'S CONFIGURATION
 ************************/
void init_rules_configuration(void);


/***************
   SQL QUERIES
 ***************/
PGresult* HAS_query_getNumberDivisions(int id_apartment, int * num_divisions, int * error_check);
PGresult* HAS_query_getNameDivision(int id_division, char * name_division, int * error_check);

PGresult* HAS_query_updateNumRules(int id_division, int num_rules, int * error_check);
void HAS_query_clearRulesTable(void);
void HAS_query_insertRule(int id_rule, char * nam_sens_cond_1, char op_cond_1, int thres_cond_1, char * nam_sens_cond_2, char op_cond_2, int thres_cond_2, int samp_per, char * schedule, int id_division, int num_act_fut_stat, char * logic_op_1_2);

void HAS_query_insertActuatorFutureState(int id_act_fut_stat, char * fut_stat, int id_actuator, int id_rule);
PGresult* HAS_query_getActuatorIdToActuatorFutureState(int id_division, char * actuator_name, int * id_actuator, int * error_check);

PGresult* HAS_query_getUserInfo(int id_user, int * error_check);
void HAS_query_showActiveUsersInfo(void);

void HAS_query_delete_tables(PGconn *dbconn, int *error_check);
void HAS_query_insert_division_info(PGconn *dbconn, char *str, int * error_check);
void HAS_query_insert_divisions_sensors(PGconn *dbconn, char **vector_string, int id_division, int number_sensors, int * error_check);
void HAS_query_insert_divisions_actuators(PGconn *dbconn, char **vector_string, int id_division, int number_actuators, int * error_check);



/*****************
 * +++++++++++++ *
 * + MAIN CODE + *
 * +++++++++++++ *
 *****************/
/************************
   FUNCTION MAIN - CODE
 ************************/
int main(int argc, char **argv)
{
	/************************************
	   INIT ROUTINE - ARGUMENT CHECKING
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
	   VARIABLES DECLARATION
	 *************************/
	int error_check = 0;
	pthread_t pth1, pth2, pth3;
	
	/**********************************
	   INIT ROUTINE - WELCOME MESSAGE
	 **********************************/
	printf("\n\n"
	       "**********************************************************************\n"
	       "******************** HAS - HOME AUTOMATIVE SYSTEM ********************\n"
	       "**********************************************************************\n\n");
	 
	/**************************************
	   INIT ROUTINE - DATABASE CONNECTION
	 **************************************/
	dbconn = PQconnectdb(conn);
	if (PQstatus(dbconn) == CONNECTION_BAD){
		printf("[DATABASE_ERROR %d] Unable to connect with the database. Please try again by restarting the program or check the ethernet connection\n", DB_ERROR_1);
		exit(-1);
	}
	
	/**************************************
	   INIT ROUTINE - LOGIN IN THE SYSTEM
	 **************************************/
	printf("+++++++++++++++++++++++++++\n"
	       "++++++++++ LOGIN ++++++++++\n"
	       "+++++++++++++++++++++++++++\n");
	init_login_in_the_system();
	 
	/**************************************************
	   INIT ROUTINE - TABLES CREATION IN THE DATABASE
	 **************************************************/
	init_table_creation();
	
	/***************************************
	   INIT ROUTINE - MOTE'S CONFIGURATION *
	 ***************************************/
	printf("\n\n\n\n\n\n"
	       "++++++++++++++++++++++++++++++++++++++++++\n"
	       "++++++++++ MOTE'S CONFIGURATION ++++++++++\n"
	       "++++++++++++++++++++++++++++++++++++++++++\n");
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
	
	/*******************************************
	   INIT ROUTINE - DIVISION'S CONFIGURATION
	 *******************************************/
	printf("\n\n\n\n\n\n"
	       "++++++++++++++++++++++++++++++++++++++++++++++\n"
	       "++++++++++ DIVISION'S CONFIGURATION ++++++++++\n"
	       "++++++++++++++++++++++++++++++++++++++++++++++\n");
	init_divisions_configuration();
	
	/*******************************************
	   INIT ROUTINE - RULE'S CONFIGURATION
	 *******************************************/
	printf("\n\n\n\n\n\n"
	       "++++++++++++++++++++++++++++++++++++++++++\n"
	       "++++++++++ RULE'S CONFIGURATION ++++++++++\n"
	       "++++++++++++++++++++++++++++++++++++++++++\n");
	init_rules_configuration();
	
	/*****************************************
	   INIT ROUTINE - MATRIX'S CONFIGURATION
	 *****************************************/
	
	
	/***********************
	   CREATION OF THREADS
	 ***********************//*
	pthread_create(&pth1, NULL, thread_data_processing, NULL);
	pthread_create(&pth2, NULL, thread_rule_implementation, NULL);
	pthread_create(&pth3, NULL, user_interface, NULL);*/
	
	/****************************************
	   CREATION AND CANCELLATION OF THREADS
	 ****************************************//*
	pthread_join(pth3, NULL);
	pthread_cancel(pth1);
	pthread_cancel(pth2);
	pthread_cancel(pth3);*/
	
	/***********************
	   TERMINATION ROUTINE
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
   INIT_LOGIN_IN_THE_SYSTEM - CODE
 ***********************************/
void init_login_in_the_system(void){
	int user_id_aux, error_check, invalid_tries = 0, login_status = 0;
	char str[INIT_LOGIN_IN_THE_SYSTEM_BUFFER_SIZE];
	char str_aux[INIT_LOGIN_IN_THE_SYSTEM_BUFFER_SIZE];
	PGresult *query;
	// GET ID
	do{
		do{
			printf("    Id (greater or equal than 0; press -1 to exit the program): ");
			scanf("%d", &user_id_aux);
			getchar();
			if(-1 == user_id_aux){
				PQfinish(dbconn);
				dbconn = NULL;
				printf("\n**********************************************************************\n"
						 "************************* END OF PROGRAM HAS *************************\n"
						 "**********************************************************************\n\n");
				exit(-1);
			}
			else if(user_id_aux < 0){
				printf("    User id invalid! ");
				invalid_tries++;
					
				if(LOGIN_LIM_INVALID_TRIES <= invalid_tries){
					printf("\nLimit of invalid tries reached! The program will now close.\n");
					HAS_query_showActiveUsersInfo();
					PQfinish(dbconn);
					dbconn = NULL;
					printf("\n**********************************************************************\n"
							 "************************* END OF PROGRAM HAS *************************\n"
							 "**********************************************************************\n\n");
					exit(-1);
				}
				printf("Please try again.\n\n");
			}
		} while(user_id_aux < 0);
		
		query = HAS_query_getUserInfo(user_id_aux, &error_check);
		if(1 == error_check){
			printf("Unable to connect with the database! The program will now close.\n");
			PQclear(query);
			PQfinish(dbconn);
			dbconn = NULL;
			printf("\n**********************************************************************\n"
					 "************************* END OF PROGRAM HAS *************************\n"
					 "**********************************************************************\n\n");
				exit(-1);
		}
		else if(2 == error_check || 3 == error_check || PGRES_TUPLES_OK != PQresultStatus(query) || 0 == PQntuples(query)){
			printf("    User id invalid! ");
			PQclear(query);
			invalid_tries++;
			if(LOGIN_LIM_INVALID_TRIES <= invalid_tries){
				printf("\n    Limit of invalid tries reached! The program will now close.\n");
				HAS_query_showActiveUsersInfo();
				PQfinish(dbconn);
				dbconn = NULL;
				printf("\n**********************************************************************\n"
						 "************************* END OF PROGRAM HAS *************************\n"
						 "**********************************************************************\n\n");
				exit(-1);
			}
			printf("Please try again.\n\n");
			continue;
		}
		else if(1 != atoi(PQgetvalue(query, 0, 3))){
			printf("\n    The intended user ins't active at the moment. Please talk with the administrators of the system.\n");
			HAS_query_showActiveUsersInfo();
			PQclear(query);
			PQfinish(dbconn);
			dbconn = NULL;
			printf("\n**********************************************************************\n"
					 "************************* END OF PROGRAM HAS *************************\n"
					 "**********************************************************************\n\n");
				exit(-1);
		}
		login_status = 1;
	} while(0 == login_status);
	
	login_status = 0;
	invalid_tries = 0;
	
	// GET PASSWORD
	do{
		sprintf(str_aux, "    Password (max. number of letter: %d; write 'EXIT' to terminate): *", INIT_LOGIN_IN_THE_SYSTEM_BUFFER_SIZE);
		strcpy(str, getpass(str_aux));
		if(0 == strcmp("EXIT", str)){
			PQclear(query);
			PQfinish(dbconn);
			dbconn = NULL;
			printf("\n**********************************************************************\n"
					 "************************* END OF PROGRAM HAS *************************\n"
					 "**********************************************************************\n\n");
			exit(-1);
		}
		else if(0 != strcmp(str, PQgetvalue(query, 0, 2)) || strlen(str) != strlen(PQgetvalue(query, 0, 2))){
			printf("    Password invalid! ");
			invalid_tries++;
			if(LOGIN_LIM_INVALID_TRIES <= invalid_tries){
				printf("\nLimit of invalid tries reached! The program will now close.\n");
				PQclear(query);
				HAS_query_showActiveUsersInfo();
				PQfinish(dbconn);
				dbconn = NULL;
				printf("\n**********************************************************************\n"
						 "************************* END OF PROGRAM HAS *************************\n"
						 "**********************************************************************\n\n");
				exit(-1);
			}
			else
				printf("Please try again.\n\n");
			continue;
		}
		login_status = 1;
	} while(0 == login_status);
	
	user_id = user_id_aux;
	strcpy(username, PQgetvalue(query, 0, 0));
	strcpy(user_permission, PQgetvalue(query, 0, 1));
	strcpy(user_password, PQgetvalue(query, 0, 2));
	user_id_apartment = atoi(PQgetvalue(query, 0, 4));
	
	/***** DEBUG ZONE *****/
	printf("[USER   ID] %d\n"
	       "[USER NAME] %s\n"
	       "[USER_PERM] %s\n"
	       "[USER_PASS] %s\n"
	       "[US_ID_APT] %d\n", user_id, username, user_permission, user_password, user_id_apartment);
	/**********************/
}


/******************************
   INIT_TABLE_CREATION - CODE
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
   DIVISION'S CONFIGURATION - CODE
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
	 * 
	 * 1 - Depois de saber o nome da divisão, inserir informação na tablea
	 * 2 - Depois de saber os resto inserir também
	 */
	 
	 char user_answer, str[SIZE_STRING_BUFFER] = {0};
	 
	 count_divisions = 0;
	 divisions_configuration = 0;
	 id_sensor = 0;
	 sens_matrix_id = 0;
	 id_actuator = 0;
	 act_matrix_id = 0;
	 
	 do {
		fflush(stdin);
		printf("Do you want to mantain the present configuration? (Y - yes; N- no): ");
		user_answer = getchar();
		getchar();
		if(('Y'!= user_answer && 'y' != user_answer) && ('N' != user_answer && 'n' != user_answer))
			printf("	Please insert a valid answer.\n");
	} while (('Y' != user_answer && 'y' != user_answer) && ('N' != user_answer && 'n' != user_answer));
	
	if('N' == user_answer || 'n' == user_answer) {			// O utilizador quer alterar a configuração das divisões
		divisions_configuration = NEW_DIVISIONS_CONFIGURATION;	
		HAS_query_delete_tables(dbconn, NULL);
		
		fflush(stdin);
		printf("\nInsert the number of divisions: ");
		scanf("%d", &number_divisions);
		getchar();
		
		do {
			printf("\n-- Division's configuration template: ROOM1:TEMP1,LIGHT1;HEAT,BLINDS,LIGHT\n");
			printf("Insert division configuration nº %d: ", count_divisions+1);
			fgets(str, SIZE_STRING_BUFFER, stdin);
			printf("Divisions configuration: %s", str);
			if('\n' == str[strlen(str) - 1])
				str[strlen(str) - 1] = '\0';
			HAS_query_insert_division_info(dbconn, str, NULL);	
		
			//COLOCAR A VARIÁVEL CONTADORA DO ID DAS DIVISÕES A ZERO AQUI
		
			count_divisions++;
		} while (count_divisions < number_divisions);
			
	} else {			// O utilizador que manter a configuração das divisões
		divisions_configuration = OLD_DIVISIONS_CONFIGURATION;
	}
}


/*******************************
   RULE'S CONFIGURATION - CODE
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
	// General variables
	int error_check = 0, counter_divisions, counter_rules, number_rules_per_division;
	char user_answer, name_division[SIZE_LIB_MAN_RULE_LABEL_DIVISION_NAME];
	
	// Variables to the rules manipulation
	int   counter2, counter3, counter4, integer_aux1, id_actuator_aux, counter_act_fut_sta = 0;
	char  string_aux1[SIZE_STRING_BUFFER_1], string_aux2[SIZE_STRING_BUFFER_2];
	char  sensor_condition_1[SIZE_LIB_MAN_RULE_LABEL_SENSOR_CONDITION];
	char  operator_condition_1;
	int   value_condition_1;
	char  logic_operator_condition_1_2[SIZE_LIB_MAN_RULE_LABEL_LOG_OPERATORS];
	char  sensor_condition_2[SIZE_LIB_MAN_RULE_LABEL_SENSOR_CONDITION];
	char  operator_condition_2;
	int   value_condition_2;
	int   num_actuator_conditions;
	char  act_future_stat[SIZE_LIB_MAN_RULE_LABEL_ACTUAT_FUT_STATE];
	char  act_future_stat_ON_OFF[SIZE_LIB_MAN_RULE_LABEL_ACTUAT_FUT_STATE];
	
	PGresult *rules_query;
	do{
		fflush(stdin);
		printf("Do you want to change the rules that are stored in the system? (Y - yes; N - no) ");
		user_answer = getchar();
		getchar();
		if(('Y' != user_answer && 'y' != user_answer) && ('N' != user_answer && 'n' != user_answer))
			printf("Please insert a valid character.\n");
	}while(('Y' != user_answer && 'y' != user_answer) && ('N' != user_answer && 'n' != user_answer));
	if('Y' == user_answer || 'y' == user_answer){
		number_rules = 0;
		counter_act_fut_sta = 0;
		HAS_query_clearRulesTable();
		rules_query = HAS_query_getNumberDivisions(0, &number_divisions, &error_check);
		PQclear(rules_query);
		printf("Number of divisions associated with the apartment 0: %d\n", number_divisions);
		
		for(counter_divisions = 0; counter_divisions < number_divisions; counter_divisions++){
			rules_query = HAS_query_getNameDivision(counter_divisions, name_division, &error_check);
			PQclear(rules_query);
			printf("\n************************************************************"
			       "\n************************************************************"
			       "\n\n[DIVISION_ID %d] %s\n", counter_divisions, name_division);
		
			printf("\n+++++ TEMPLATE APLIED TO THE RULES CREATION +++++\n");
			printf("Type of comparasion operators that are available to choose: '<' or '>'\n");
			printf("Type of logic operators that are available to choose: \"AND\" or \"OR\"\n");
			printf("Possibles actuator's state: \"ON\" or \"OFF\"");
			printf("Maximum ratings:\n");
			printf("    Size of division's name:            %d characters\n", SIZE_LIB_MAN_RULE_LABEL_DIVISION_NAME);
			printf("    Size of sensor's name:              %d characters\n", SIZE_LIB_MAN_RULE_LABEL_SENSOR_CONDITION);
			printf("    Size of actuator's pretended state: %d characters\n", SIZE_LIB_MAN_RULE_LABEL_ACTUAT_FUT_STATE);
			printf("    Size of rule's characterization:    %d characters\n", SIZE_STRING_BUFFER_1);
			printf("Examples of rules definition in the file:\n\n");
			printf("    (rule 1)    <room_name>: <sensor_name_1><condition_1><value_1> [AND / OR <sensor_name_2><condition_2><value_2>] <actuator>:<state>[,<actuator>:<state>]\n");
			printf("    (rule 2)    <room_name>: <sensor_name_1><condition_1><value_1> [AND / OR <sensor_name_2><condition_2><value_2>] <actuator>:<state>[,<actuator>:<state>]\n");
			printf("...\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
			
			do{
				printf("Insert the number of rules that are going to be applied to this division (>=0):");
				scanf("%d", &number_rules_per_division);
				getchar();
				if(0 > number_rules_per_division)
					printf("Please insert a valid number of rules.\n");
			}while(0 > number_rules_per_division);
			
			rules_query = HAS_query_updateNumRules(counter_divisions, number_rules_per_division, &error_check);
			PQclear(rules_query);
			
			for(counter_rules = 0; counter_rules < number_rules_per_division; counter_rules++){
				/**********************
				 * RULES MANIPULATION
				 **********************/
								
				// Code of manipulation of rules

				// - Reading the rule from the file
				printf("[INSERT RULE %d APPLIED TO %s] ", counter_rules+1, name_division);
				fgets(string_aux1, SIZE_STRING_BUFFER_1, stdin);

				integer_aux1 = strlen(string_aux1);
				if('\n' == string_aux1[integer_aux1 - 1])
					string_aux1[integer_aux1 - 1] = '\0';
				if(0 == (integer_aux1 - 1)){
					printf("[INIT_RULES_CONFIG %d] Rule's description invalid.\n", ERROR_INIT_RULES_CONFIG_1);
					printf("    The problem was found in the RULE %d\n", counter_rules + 1);
					return;
				}
				integer_aux1--;

				// - Processing division's name
				for(counter2 = 0; ':' != string_aux1[counter2] && counter2 < integer_aux1 && counter2 < SIZE_STRING_BUFFER_2; counter2++)
					string_aux2[counter2] = string_aux1[counter2];
				string_aux2[counter2] = '\0';
				if(integer_aux1 == counter2){
					printf("[INIT_RULES_CONFIG %d] Rule's description invalid.\n", ERROR_INIT_RULES_CONFIG_2);
					printf("    The problem was found in the RULE %d\n", counter_rules + 1);
					return;
				}
				counter2++;

				// - Processing condition 1
				while(' ' == string_aux1[counter2] && counter2 < integer_aux1)
					counter2++;
				if(integer_aux1 == counter2){
					printf("[INIT_RULES_CONFIG %d] Rule's description invalid.\n", ERROR_INIT_RULES_CONFIG_3);
					printf("    The problem was found in the RULE %d\n", counter_rules + 1);
					return;
				}
				else if(counter2 < integer_aux1 && (':' == string_aux1[counter2] || comparison_operators[1] == string_aux1[counter2] || comparison_operators[2] == string_aux1[counter2])){
					printf("[INIT_RULES_CONFIG %d] Rule's description invalid.\n", ERROR_INIT_RULES_CONFIG_4);
					printf("    The problem was found in the RULE %d\n", counter_rules + 1);
					return;
				}

				for(counter3 = 0; (comparison_operators[1] != string_aux1[counter2] && comparison_operators[2] != string_aux1[counter2]) && counter2 < integer_aux1 && counter3 < SIZE_STRING_BUFFER_2; counter3++){
					string_aux2[counter3] = string_aux1[counter2];
					counter2++;
				}
				string_aux2[counter3] = '\0';
				if(integer_aux1 == counter2){
					printf("[INIT_RULES_CONFIG %d] Rule's description invalid.\n", ERROR_INIT_RULES_CONFIG_5);
					printf("    The problem was found in the RULE %d\n", counter_rules + 1);
					return;
				}

				if(1 != validation_of_sensor(string_aux2, number_motes)){
					printf("[INIT_RULES_CONFIG %d] The sensor especified in the condition doesn't exist.\n", ERROR_INIT_RULES_CONFIG_6);
					printf("    The problem was found in the RULE %d\n", counter_rules + 1);
					return;
				}

				strcpy(sensor_condition_1, string_aux2);
				operator_condition_1 = string_aux1[counter2];
				counter2++;
				value_condition_1 = conversion_of_a_piece_of_a_string_into_integer(string_aux1, counter2, &counter2, integer_aux1);


				// - Processing condition 2
				while(' ' == string_aux1[counter2] && counter2 < integer_aux1)
					counter2++;
				if(integer_aux1 == counter2){
					printf("[INIT_RULES_CONFIG %d] Rule's description invalid.\n", ERROR_INIT_RULES_CONFIG_6);
					printf("    The problem was found in the RULE %d\n", counter_rules + 1);
					return;
				}

				if((lib_man_rule_logic_operators[1][0] == string_aux1[counter2] && lib_man_rule_logic_operators[1][1] == string_aux1[counter2+1] && lib_man_rule_logic_operators[1][2] == string_aux1[counter2+2] && ' ' == string_aux1[counter2+3])  ||  (lib_man_rule_logic_operators[2][0] == string_aux1[counter2] && lib_man_rule_logic_operators[2][1] == string_aux1[counter2+1] && ' ' == string_aux1[counter2+2])){
					if(lib_man_rule_logic_operators[1][0] == string_aux1[counter2] && lib_man_rule_logic_operators[1][1] == string_aux1[counter2+1] && lib_man_rule_logic_operators[1][2] == string_aux1[counter2+2] && ' ' == string_aux1[counter2+3]){
						// LOGIC OPERATOR: AND
						strcpy(logic_operator_condition_1_2, lib_man_rule_logic_operators[1]);
						counter2 = counter2 + 3;
					}
					else if(lib_man_rule_logic_operators[2][0] == string_aux1[counter2] && lib_man_rule_logic_operators[2][1] == string_aux1[counter2+1] && ' ' == string_aux1[counter2+2]){
						// LOGIC OPERATOR: OR
						strcpy(logic_operator_condition_1_2, lib_man_rule_logic_operators[2]);
						counter2 = counter2 + 2;
					}
					while(' ' == string_aux1[counter2] && counter2 < integer_aux1)
						counter2++;
					if(integer_aux1 == counter2){
						printf("[INIT_RULES_CONFIG %d] Rule's description invalid.\n", ERROR_INIT_RULES_CONFIG_7);
						printf("    The problem was found in the RULE %d\n", counter_rules + 1);
						return;
					}
					for(counter3 = 0; (comparison_operators[1] != string_aux1[counter2] && comparison_operators[2] != string_aux1[counter2]) && counter2 < integer_aux1 && counter3 < SIZE_STRING_BUFFER_2; counter3++){
						string_aux2[counter3] = string_aux1[counter2];
						counter2++;
					}
					string_aux2[counter3] = '\0';
					if(integer_aux1 == counter2){
						printf("[INIT_RULES_CONFIG %d] Rule's description invalid.\n", ERROR_INIT_RULES_CONFIG_8);
						printf("    The problem was found in the RULE %d\n", counter_rules + 1);
						return;
					}
					
					if(1 != validation_of_sensor(string_aux2, number_motes)){
						printf("[INIT_RULES_CONFIG %d] The sensor especified in the condition doesn't exist.\n", ERROR_INIT_RULES_CONFIG_9);
						printf("    The problem was found in the RULE %d\n", counter_rules + 1);
						return;
					}
					
					strcpy(sensor_condition_2, string_aux2);
					operator_condition_2 = string_aux1[counter2];
					counter2++;
					value_condition_2 = conversion_of_a_piece_of_a_string_into_integer(string_aux1, counter2, &counter2, integer_aux1);
				}
				else{
					strcpy(logic_operator_condition_1_2, lib_man_rule_logic_operators[0]);
					sensor_condition_2[0] = '\0';
					operator_condition_2 = comparison_operators[0];
					value_condition_2 = 0;
				}
				
				
				// - Processing actuators
				while(' ' == string_aux1[counter2] && counter2 < integer_aux1)
					counter2++;
				if(integer_aux1 == counter2){
					printf("[INIT_RULES_CONFIG %d] Rule's description invalid.\n", ERROR_INIT_RULES_CONFIG_10);
					printf("    The problem was found in the RULE %d\n", counter_rules + 1);
					return;
				}
				counter4 = 0;
				for(counter3 = counter2; counter3 < integer_aux1 && '\0' != string_aux1[counter3]; counter3++)
					if(':' == string_aux1[counter3])
						counter4++;
				num_actuator_conditions = counter4;

				HAS_query_insertRule(number_rules, sensor_condition_1, operator_condition_1, value_condition_1, sensor_condition_2, operator_condition_2, value_condition_2, 0, NULL, counter_divisions, num_actuator_conditions, logic_operator_condition_1_2);

				
				for(counter3 = 0; (counter3 < num_actuator_conditions) && ('\0' != string_aux1[counter2]); counter3++){
					counter4 = 0;
					while(':' != string_aux1[counter2] && '\0' != string_aux1[counter2]){
						act_future_stat[counter4] = string_aux1[counter2];
						counter2++;
						counter4++;
					}
					act_future_stat[counter4] = '\0';
					counter2++;
					
					counter4 = 0;
					while(' ' != string_aux1[counter2] && '\0' != string_aux1[counter2]){
						act_future_stat_ON_OFF[counter4] = string_aux1[counter2];
						counter2++;
						counter4++;
					}
					act_future_stat_ON_OFF[counter4] = '\0';
					
					rules_query = HAS_query_getActuatorIdToActuatorFutureState(counter_divisions, act_future_stat, &id_actuator_aux, &error_check);
					PQclear(rules_query);
					if(0 > id_actuator_aux){
						printf("[INIT_RULES_CONFIG %d] Actuator %d of the future states invalid.\n", ERROR_INIT_RULES_CONFIG_11, counter3 + 1);
						printf("    The problem was found in the RULE %d\n", counter_rules + 1);
						return;
					}
					
					HAS_query_insertActuatorFutureState(counter_act_fut_sta, act_future_stat_ON_OFF, id_actuator_aux, number_rules);
					
					while(' ' == string_aux1[counter2] && '\0' != string_aux1[counter2] && counter2 < integer_aux1)
						counter2++;
					counter_act_fut_sta++;
				}
				if(counter3 < num_actuator_conditions){
					printf("[INIT_RULES_CONFIG %d] Rule's description invalid.\n", ERROR_INIT_RULES_CONFIG_12);
					printf("    The problem was found in the RULE %d\n", counter_rules + 1);
					return;
				}
				/************************************************************************************************************/
				number_rules++;
			}
		}
	}
}

/*************************
   SQL DIVISIONS QUERIES 
 *************************/
PGresult* HAS_query_getNumberDivisions(int id_apartment, int * num_divisions, int * error_check){
	if(NULL == num_divisions){
		printf("[HAS_query_getNumberDivisions ERROR] Pointer to the num_divisions invalid.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	else if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getNumberDivisions ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
	else if(0 > id_apartment){
		printf("[HAS_query_getNumberDivisions ERROR] Id of the apartment invalid.\n");
		if(NULL != error_check)
			(*error_check) = 3;
		return NULL;
	}
	
	char str[100];
	PGresult *query;
	sprintf(str, "SELECT num_divisions FROM apartment WHERE id=%d", id_apartment);
	query = PQexec(dbconn, str);
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		(*num_divisions) = atoi(PQgetvalue(query, 0, 0));
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		printf("[HAS_query_getNumberDivisions Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 4;
		return NULL;
	}
}

PGresult* HAS_query_getNameDivision(int id_division, char * name_division, int * error_check){
	if(NULL == name_division){
		printf("[HAS_query_getNameDivision ERROR] Pointer to the name_division invalid.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	else if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getNameDivision ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
	else if(0 > id_division){
		printf("[HAS_query_getNameDivision ERROR] Id of the division invalid.\n");
		if(NULL != error_check)
			(*error_check) = 3;
		return NULL;
	}
	
	char str[150];
	PGresult *query;
	sprintf(str, "SELECT name FROM apartment, divisions WHERE apartment.id = divisions.id_apartment AND divisions.id=%d", id_division);
	query = PQexec(dbconn, str);
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		strcpy(name_division, PQgetvalue(query, 0, 0));
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		printf("[HAS_query_getNameDivision Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 4;
		return NULL;
	}
}

void HAS_query_delete_tables(PGconn *dbconn, int * error_check) {
	int num_rows = 0;
	
	PGresult *divisions_query = PQexec(dbconn, "TRUNCATE divisions CASCADE");
	
	divisions_query = PQexec(dbconn, "SELECT * FROM divisions");
	
	if (PQresultStatus(divisions_query) == PGRES_TUPLES_OK) {
		
		num_rows = PQntuples(divisions_query);
		
		if(num_rows != 0){
			printf("Number of tuples to query: %d\n" , num_rows);
			printf("[ERROR_MESSAGE %d] TABLE ROWS NOT DELETED\n", ERROR_CHECK_0);
			if(NULL == error_check)
				(*error_check) = ERROR_CHECK_0;
		}
	}
	PQclear(divisions_query);
}

void HAS_query_insert_division_info(PGconn *dbconn, char *str, int * error_check) {
	//Argument check
	if(NULL == str) {
		printf("[ERROR MESSAGE %d] Divisions configuration string empty.\n", ERROR_CHECK_1);
		if(NULL != error_check)
			(*error_check) = ERROR_CHECK_1;
	}
	
	//Query code
	int counter1 = 0, counter2 = 0, counter3 = 0, count_sensors = 0, count_actuators = 0;
	int id_sensor = 0, id_actuator = 0;
	char str_division[SIZE_STRING_DIVISION], str_sensors_aux[SIZE_STRING_SENSOR], str_actuators_aux[SIZE_STRING_ACTUATOR], **str_sensors, **str_actuators;
	char query_string[SIZE_QUERY_STRING];
	PGresult *division_query;
	
	number_maximum_actuators = 1;
	number_maximum_sensors = 1;	
	// DIVISION NAME
	for(counter1 = 0; ':' != str[counter1]; counter1++) {
			str_division[counter1] = str[counter1];
	}
	str_division[counter1] = '\0';
	counter1++;
	sprintf(query_string, "INSERT INTO divisions (id, name, num_sensors, num_actuators, id_apartment, num_rules) VALUES (%d, '%s', 0, 0, 0, 0)", count_divisions, str_division);
	division_query = PQexec(dbconn, query_string);
	
	if(PQresultStatus(division_query) != PGRES_COMMAND_OK) {
		printf("Failed executing query.\n");
		if(NULL != error_check)
			(*error_check) = ERROR_CHECK_3;
		PQfinish(dbconn);
		dbconn = NULL;
		division_query = NULL;
		exit(-1);
	}
	
	PQclear(division_query);
	
	// NAME AND NUMBER OF SENSORS
	for(counter2 = counter1; ';' != str[counter2]; counter2++) {
		if(',' == str[counter2]) {
			number_maximum_sensors++;
		}
	}

	str_sensors = string_vector_sensors_creation(number_maximum_sensors, NULL);
	if(NULL == str_sensors) {
		printf("[ERROR MESSAGE %d] String vector not created.\n", ERROR_CHECK_2);
		free_vector_string_memory(str_sensors, number_maximum_sensors, NULL);
		if(NULL == error_check)
			(*error_check) = ERROR_CHECK_2;
	}
	
	for(counter2 = counter1; ';' != str[counter2]; counter2++) {
		str_sensors_aux[count_sensors] = str[counter2];
		count_sensors++;
		if((',' == str[counter2])) {
			str_sensors_aux[count_sensors-1] = '\0';
			strcpy(str_sensors[id_sensor], str_sensors_aux);
			str_sensors_aux[0] = '\0';
			count_sensors = 0;
			id_sensor++;
		}
		else if(';' == str[counter2+1]) {
			str_sensors_aux[count_sensors] = '\0';
			strcpy(str_sensors[id_sensor], str_sensors_aux);
			str_sensors_aux[0] = '\0';
			count_sensors = 0;
			id_sensor++;
		}
	}
			
	counter2++;
	
	HAS_query_insert_divisions_sensors(dbconn, str_sensors, count_divisions, number_maximum_sensors, NULL);
	
	// NAME AND NUMBER OF ACTUATORS
	for(counter3 = counter2; '\0' != str[counter3]; counter3++) {
		if(',' == str[counter3])
			number_maximum_actuators++;
	}
		
	str_actuators = string_vector_sensors_creation(number_maximum_actuators, NULL);
	
	if(NULL == str_actuators) {
		printf("[ERROR MESSAGE %d] String vector not created.\n", ERROR_CHECK_2);
		free_vector_string_memory(str_sensors, number_maximum_actuators, NULL);
		if(NULL == error_check)
			(*error_check) = ERROR_CHECK_2;
	}
	
	for(counter3 = counter2; '\0' != str[counter3]; counter3++) {
		str_actuators_aux[count_actuators] = str[counter3];
		count_actuators++;
		if((',' == str[counter3])) {
			str_actuators_aux[count_actuators-1] = '\0';
			strcpy(str_actuators[id_actuator], str_actuators_aux);
			str_actuators_aux[0] = '\0';
			count_actuators = 0;
			id_actuator++;
		}
		else if('\0' == str[counter3+1]) {
			str_actuators_aux[count_actuators] = '\0';
			strcpy(str_actuators[id_actuator], str_actuators_aux);
			str_actuators_aux[0] = '\0';
			count_actuators = 0;
			id_actuator++;
		}
	}
	
	HAS_query_insert_divisions_actuators(dbconn, str_actuators, count_divisions, number_maximum_actuators, NULL);
	
	free_vector_string_memory(str_actuators, number_maximum_actuators, NULL);
}

void HAS_query_insert_divisions_sensors(PGconn *dbconn, char **vector_string, int id_division, int number_sensors, int * error_check) {
	//Argument check
	if(NULL == vector_string) {
		printf("[ERROR MESSAGE %d] Vector string empty.\n", ERROR_CHECK_4);
		if(NULL == error_check)
			(*error_check) = ERROR_CHECK_4;
	}
	
	// Query code
	int counter1= 0, counter2 = 0;
	PGresult *division_query;
	char query_string[SIZE_QUERY_STRING], sensor_name[SIZE_STRING_SENSOR], sensor_mote;
	
	for(counter1 = 0; counter1 < number_sensors; counter1++) {
		for(counter2 = 0; '\0' != vector_string[counter1][counter2]; counter2++) {
			sensor_name[counter2] = vector_string[counter1][counter2];
		}
		sensor_mote = sensor_name[counter2-1];
		sensor_name[counter2 - 1] = '\0';
		sprintf(query_string, "INSERT INTO sensor (id, mote_id, sensor_type, data, time, id_divisions, day, sens_matrix_id) "
		"VALUES (%d, %c, '%s', 0, CURRENT_TIME(0) AT TIME ZONE 'GMT-1', %d,CURRENT_DATE, %d)", id_sensor, sensor_mote, sensor_name, id_division, sens_matrix_id);
		division_query = PQexec(dbconn, query_string);
	
		if(PQresultStatus(division_query) != PGRES_COMMAND_OK) {
			printf("[ERROR MESSAGE %d]: Failed executing query.\n", ERROR_CHECK_5);
			if(NULL != error_check)
				(*error_check) = ERROR_CHECK_5;
			PQfinish(dbconn);
			dbconn = NULL;
			division_query = NULL;
			exit(-1);
		}
		sens_matrix_id++;
		id_sensor++;
		sensor_name[0] = '\0';
		query_string[0] = '\0';
	}
	PQclear(division_query);
}

void HAS_query_insert_divisions_actuators(PGconn *dbconn, char **vector_string, int id_division, int number_actuators, int * error_check) {
	//Argument check
	if(NULL == vector_string) {
		printf("[ERROR MESSAGE %d] Vector string empty.\n", ERROR_CHECK_4);
		if(NULL == error_check)
			(*error_check) = ERROR_CHECK_4;
	}
	
	// Query code
	int counter1= 0;
	PGresult *division_query;
	char query_string[SIZE_QUERY_STRING];
	
	for(counter1 = 0; counter1 < number_actuators; counter1++) {
		printf("Nome do atuador: %s\n", vector_string[counter1]);
		sprintf(query_string, "INSERT INTO actuator (id, name, actuator_state, time, id_divisions, day, act_matrix_id) VALUES (%d, '%s', 'OFF', CURRENT_TIME(0) AT TIME ZONE 'GMT-1', %d,CURRENT_DATE, %d)", id_actuator, vector_string[counter1], id_division, act_matrix_id);
		division_query = PQexec(dbconn, query_string);
	
		if(PQresultStatus(division_query) != PGRES_COMMAND_OK) {
			printf("[ERROR MESSAGE %d]: Failed executing query.\n", ERROR_CHECK_5);
			if(NULL != error_check)
				(*error_check) = ERROR_CHECK_5;
			PQfinish(dbconn);
			dbconn = NULL;
			division_query = NULL;
			exit(-1);
		}
		act_matrix_id++;
		id_actuator++;
		query_string[0] = '\0';
	}
	PQclear(division_query);
}

/*********************
   SQL RULES QUERIES 
 *********************/
PGresult* HAS_query_updateNumRules(int id_division, int num_rules, int * error_check){
	if(0 > id_division){
		printf("[HAS_query_updateNumRules ERROR] Division id invalid.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	else if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_updateNumRules ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
	else if(0 > num_rules){
		printf("[HAS_query_updateNumRules ERROR] Number of rules aplied to the division invalid.\n");
		if(NULL != error_check)
			(*error_check) = 3;
		return NULL;
	}
	
	char str[80];
	PGresult *query;
	sprintf(str, "UPDATE divisions SET num_rules=%d WHERE id=%d", num_rules, id_division);
	query = PQexec(dbconn, str);
	if(PQresultStatus(query) == PGRES_COMMAND_OK){
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		printf("[HAS_query_updateNumRules Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 4;
		return NULL;
	}
}

void HAS_query_clearRulesTable(void){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_clearRulesTable ERROR] Connection to the database is bad.\n");
		return;
	}
	
	PGresult *query;
	query = PQexec(dbconn, "TRUNCATE rules CASCADE");
	PQclear(query);
}

void HAS_query_insertRule(int id_rule, char * nam_sens_cond_1, char op_cond_1, int thres_cond_1, char * nam_sens_cond_2, char op_cond_2, int thres_cond_2, int samp_per, char * schedule, int id_division, int num_act_fut_stat, char * logic_op_1_2){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_insertRule ERROR] Connection to the database is bad.\n");
		return;
	}
	else if(0 > id_division){
		printf("[HAS_query_insertRule ERROR] Id of the division invalid.\n");
		return;
	}
	else if(0 > id_rule){
		printf("[HAS_query_insertRule ERROR] Id of the rule invalid.\n");
		return;
	}
	else if(1 > num_act_fut_stat){
		printf("[HAS_query_insertRule ERROR] Number of actuator future state invalid.\n");
		return;
	}
	
	char str[500];
	char nam_sens_cond_2_aux[SIZE_LIB_MAN_RULE_LABEL_SENSOR_CONDITION+5]={}, op_cond_2_aux[10], thres_cond_2_aux[10], samp_per_aux[10], schedule_aux[20], logic_op_1_2_aux[10];
	PGresult * query;
	
	if(0 == strcmp(lib_man_rule_logic_operators[0], logic_op_1_2)){
		strcpy(nam_sens_cond_2_aux, "NULL");
		strcpy(op_cond_2_aux, "NULL");
		strcpy(thres_cond_2_aux, "NULL");
		strcpy(logic_op_1_2_aux, "NULL");
	}
	else{
		sprintf(nam_sens_cond_2_aux, "'%s'", nam_sens_cond_2);
		sprintf(op_cond_2_aux, "'%c'", op_cond_2);
		sprintf(thres_cond_2_aux, "%d", thres_cond_2);
		sprintf(logic_op_1_2_aux, "'%s'", logic_op_1_2);
	}
	if(0 >= samp_per)
		strcpy(samp_per_aux, "NULL");
	else
		sprintf(samp_per_aux, "'%d'", samp_per);
	if(NULL == schedule)
		strcpy(schedule_aux, "NULL");
	else if('\0' == schedule[0])
		strcpy(schedule_aux, "NULL");
	else
		sprintf(schedule_aux, "'%s'", schedule);
	
	sprintf(str, "INSERT INTO rules(id, name_sensor_condition1, operator_condition1, threshold_condition1, name_sensor_condition2, operator_condition2, threshold_condition2, sampling_period, schedule, time, id_divisions, day, num_actuators_future_state, logic_operator) VALUES (%d, '%s', '%c', %d, %s, %s, %s, %s, %s, CURRENT_TIME(0) AT TIME ZONE 'GMT-1', %d, CURRENT_DATE, %d, %s)", id_rule, nam_sens_cond_1, op_cond_1, thres_cond_1, nam_sens_cond_2_aux, op_cond_2_aux, thres_cond_2_aux, samp_per_aux, schedule_aux, id_division, num_act_fut_stat, logic_op_1_2_aux);
	
	query = PQexec(dbconn, str);
	if(!(PQresultStatus(query) == PGRES_COMMAND_OK))
		printf("[HAS_query_insertRule Error Message] %s\n", PQresultErrorMessage(query));
}



/*************************************
   SQL ACTUATOR FUTURE STATE QUERIES 
 *************************************/
void HAS_query_insertActuatorFutureState(int id_act_fut_stat, char * fut_stat, int id_actuator, int id_rule){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_insertActuatorFutureState ERROR] Connection to the database is bad.\n");
		return;
	}
	else if(0 > id_act_fut_stat){
		printf("[HAS_query_insertActuatorFutureState ERROR] Id of the actuator_future_state invalid.\n");
		return;
	}
	else if(0 > id_actuator){
		printf("[HAS_query_insertActuatorFutureState ERROR] Id of the actuator invalid.\n");
		return;
	}
	else if(0 > id_rule){
		printf("[HAS_query_insertActuatorFutureState ERROR] Id of the rule invalid.\n");
		return;
	}
	else if(NULL == fut_stat){
		printf("[HAS_query_insertActuatorFutureState ERROR] Invalid string: fut_stat.\n");
		return;
	}
	
	char str[150];
	PGresult * query;
	sprintf(str, "INSERT INTO actuator_future_state (id, future_state, id_actuator, id_rules) VALUES (%d, '%s', %d, %d)", id_act_fut_stat, fut_stat, id_actuator, id_rule);
	query = PQexec(dbconn, str);
	if(!(PQresultStatus(query) == PGRES_COMMAND_OK))
		printf("[HAS_query_insertRule Error Message] %s\n", PQresultErrorMessage(query));
}

PGresult* HAS_query_getActuatorIdToActuatorFutureState(int id_division, char * actuator_name, int * id_actuator, int * error_check){
	if(0 > id_division){
		printf("[HAS_query_getActuatorIdToActuatorFutureState ERROR] Division id invalid.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	else if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getActuatorIdToActuatorFutureState ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
	else if(NULL == actuator_name){
		printf("[HAS_query_getActuatorIdToActuatorFutureState ERROR] Invalid string: actuator_name.\n");
		if(NULL != error_check)
			(*error_check) = 3;
		return NULL;
	}
	
	char str[150];
	PGresult *query;
	sprintf(str, "SELECT id FROM actuator WHERE actuator.name = '%s' AND actuator.id_divisions = %d", actuator_name, id_division);
	query = PQexec(dbconn, str);
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		(*id_actuator) = atoi(PQgetvalue(query,0,0));
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		(*id_actuator) = -1;
		if(NULL != error_check)
			(*error_check) = 4;
		return NULL;
	}
}



/*********************
   SQL USERS QUERIES 
 *********************/
PGresult* HAS_query_getUserInfo(int id_user, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getUserInfo ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	else if(0 > id_user){
		printf("[HAS_query_getUserInfo ERROR] User_id invalid (it needs to be equal or higher than zero).\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
	
	char str[120];
	PGresult *query;
	sprintf(str, "SELECT name, permission, password, state, id_apartment FROM users WHERE id = %d", id_user);
	query = PQexec(dbconn, str);
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		if(NULL != error_check)
			(*error_check) = 3;
		return NULL;
	}
}

void HAS_query_showActiveUsersInfo(void){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_showUsersInfo ERROR] Connection to the database is bad.\n");
		exit(-1);
	}
	
	int numb_rows, counter;
	PGresult *query = PQexec(dbconn, "SELECT id, name, permission, id_apartment FROM users WHERE state = 1");
	if(PGRES_TUPLES_OK == PQresultStatus(query)){
		numb_rows = PQntuples(query);
		if(0 != numb_rows){
			printf("\n+++++ ACTIVE USERS IN HAS SYSTEM +++++\n");
			printf("Id\t| Username\t| Permission\t| Id_apartment\n");
			for(counter = 0; counter < numb_rows; counter++)
				printf("%d\t| %s\t| %s\t| %d\n", 
				        atoi(PQgetvalue(query, counter, 0)), 
				              PQgetvalue(query, counter, 1),
				                    PQgetvalue(query, counter, 2),
				                          atoi(PQgetvalue(query, counter, 3)));
		}
		else{
			printf("[HAS_query_showUsersInfo ERROR] There isn't any information about any user in the database.\n");
		}
	}
	else{
		printf("[HAS_query_showUsersInfo ERROR] Query call ins't ok.\n");
		printf("Result Query: %d\n", PQresultStatus(query));
		printf("Error Message: %s\n", PQresultErrorMessage(query));
	}
	printf("\n");
}



/************************************
 * ++++++++++++++++++++++++++++++++ *
 * + CODE RELATIVE TO THE THREADS + *
 * ++++++++++++++++++++++++++++++++ *
 ************************************/
/**********************************************************
   FUNCTIONS USED BY THREAD DATA PROCESSING (PTH1) - CODE
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
   THREAD DATA PROCESSING (PTH1) - CODE
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
   FUNCTIONS USED BY THREAD RULE IMPLEMENTATION (PTH2) - CODE
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
   THREAD RULE IMPLEMENTATION (PTH2) - CODE
 ********************************************/
void *thread_rule_implementation(void *arg){
	
	
	pthread_exit(NULL);
}


/***************************************
   THREAD USER INTERFACE (PTH3) - CODE
 ***************************************/
void *user_interface(void *arg){
	
	
	pthread_exit(NULL);
}
