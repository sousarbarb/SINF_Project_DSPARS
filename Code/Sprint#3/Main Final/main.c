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
#include <math.h>
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
#define MAIN_ERROR_6  6
#define MAIN_ERROR_7  7
#define MAIN_ERROR_8  8

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

// SEARCH SENSOR
#define ERROR_CHECK_SENSOR_ARG_INV	-1
#define	ERROR_CHECK_SENSOR_1	1
#define ERROR_CHECK_SENSOR_2	2
#define ERROR_CHECK_SENSOR_3	3

#define	SENSOR_PRESENT		1
#define	SENSOR_NOT_PRESENT	0

// UPDATE SENSOR
#define ERROR_UPDATE_DATA_SENSOR_1	1
#define ERROR_UPDATE_DATA_SENSOR_2	2
#define ERROR_UPDATE_DATA_SENSOR_3	3
#define ERROR_UPDATE_DATA_SENSOR_4	4
#define	ERROR_UPDATE_DATA_SENSOR_5	5

// UPDATE ACTUATOR

#define ERROR_UPDATE_STATE_ACTUATOR_1	1
#define ERROR_UPDATE_STATE_ACTUATOR_2	2
#define ERROR_UPDATE_STATE_ACTUATOR_3	3
#define ERROR_UPDATE_STATE_ACTUATOR_4	4
#define	ERROR_UPDATE_STATE_ACTUATOR_5	5
#define ERROR_UPDATE_STATE_ACTUATOR_6	6

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

// Tables creation
#define TABLE_NOT_EXIST			0
#define TABLE_EXIST				1
#define TABLE_NOT_CREATED		0
#define TABLE_CREATED			1
#define	ERROR_VERIFYING_TABLE	2
#define	ERROR_CREATING_TABLE	3

// Division's cofiguration defines

#define NEW_DIVISIONS_CONFIGURATION 1
#define OLD_DIVISIONS_CONFIGURATION 0

#define ERROR_CHECK_0 0
#define ERROR_CHECK_1 1
#define ERROR_CHECK_2 2
#define ERROR_CHECK_3 3
#define ERROR_CHECK_4 4
#define ERROR_CHECK_5 5
#define ERROR_CHECK_6 6

#define SIZE_STRING_BUFFER 		100
#define SIZE_STRING_DIVISION 	20
#define SIZE_STRING_SENSOR 		20
#define SIZE_STRING_ACTUATOR	20
#define SIZE_QUERY_STRING       256
#define SIZE_MOTE_ID			1

// Table's name
#define TABNAME_ACT        "actuator"
#define TABNAME_ACTFUTSTAT "actuator_future_state"
#define TABNAME_APART      "apartment"
#define TABNAME_BUILD      "building"
#define TABNAME_DIVIS      "divisions"
#define TABNAME_RULES      "rules"
#define TABNAME_SENS       "sensor"
#define TABNAME_USERS      "users"
#define TABNAME_USEACT     "users_activity"
#define NUM_SPACES	2

// User's activity defines
#define OPTION_LOGIN				1
#define OPTION_LOGOUT				2
#define OPTION_DIVI_CONF			3
#define OPTION_RULES_ALT			4
#define OPTION_INFO					5
#define OPTION_DEL_TABLE_UA			6
#define OPTION_DEL_TABLE_SH			7
#define OPTION_DEL_TABLE_AH			8

// Global variables
FILE *sensor_data_channel = NULL, *rgb_matrix_channel = NULL;
static const char CHANNEL_DEF[]="/dev/pts/";
int number_motes, number_rules, number_divisions, number_maximum_actuators, number_maximum_sensors, matrix_side_size, count_divisions, divisions_configuration, sens_matrix_id, id_sensor, act_matrix_id, id_actuator;
int flag_interface = 1, flag_command = 1;
int present_id_user_activity, option=0;
char **pointer_rgb_channel = NULL;
char *buffer_rgb_channel = NULL;
char *buffer_rgb_channel_aux = NULL;
mote **system_motes = NULL;

// DATABASE
const char *conn = "host='db.fe.up.pt' user='sinfa15' password='DSPARS_sinf2018*'";
PGconn *dbconn_main;
PGconn *dbconn_1;
PGconn *dbconn_2;
PGconn *dbconn_3;

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
void atualize_RGBstring_sensors_state_RGBMatrix(PGconn *dbconn, char * buffer_rgb_pssRGB, int matrix_side);
void atualize_RGBstring_actuators_state_RGBMatrix(PGconn *dbconn, char * buffer_rgb_pssRGB, int matrix_side);


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
void init_login_in_the_system(PGconn *dbconn);


/***********************
   INIT_TABLE_CREATION
 ***********************/
void init_table_creation(PGconn *dbconn);


/****************************
   DIVISION'S CONFIGURATION
 ****************************/
void init_divisions_configuration(PGconn *dbconn);


/************************
   RULE'S CONFIGURATION
 ************************/
void init_rules_configuration(PGconn *dbconn);


/*********************************
   MATRIX'S CONFIGURATION - CODE
 *********************************/
void init_matrix_configuration(PGconn *dbconn);


/*******************************
   SQL CREATION TABLES QUERIES
 *******************************/
int HAS_query_verify_actuator_table(PGconn *dbconn);
int HAS_query_create_actuator_table(PGconn *dbconn);
void HAS_query_show_actuator_table(PGconn *dbconn);

int HAS_query_verify_actuator_future_state_table(PGconn *dbconn);
int HAS_query_create_actuator_future_state_table(PGconn *dbconn);
void HAS_query_show_actuator_future_state_table(PGconn *dbconn);

int HAS_query_verify_apartment_table(PGconn *dbconn);
int HAS_query_create_apartment_table(PGconn *dbconn);
void HAS_query_show_apartment_table(PGconn *dbconn);

int HAS_query_verify_building_table(PGconn *dbconn);
int HAS_query_create_building_table(PGconn *dbconn);
void HAS_query_show_building_table(PGconn *dbconn);

int HAS_query_verify_divisions_table(PGconn *dbconn);
int HAS_query_create_divisions_table(PGconn *dbconn);
void HAS_query_show_divisions_table(PGconn *dbconn);

int HAS_query_verify_rules_table(PGconn *dbconn);
int HAS_query_create_rules_table(PGconn *dbconn);
void HAS_query_show_rules_table(PGconn *dbconn);

int HAS_query_verify_sensor_table(PGconn *dbconn);
int HAS_query_create_sensor_table(PGconn *dbconn);
void HAS_query_show_sensor_table(PGconn *dbconn);

int HAS_query_verify_users_table(PGconn *dbconn);
int HAS_query_create_users_table(PGconn *dbconn);
void HAS_query_show_users_table(PGconn *dbconn);

int HAS_query_verify_users_activity_table(PGconn *dbconn);
int HAS_query_create_users_activity_table(PGconn *dbconn);
void HAS_query_show_users_activity_table(PGconn *dbconn);

void HAS_print_table(PGconn *dbconn, char *name_table);
void HAS_print_table_restrictions(PGconn *dbconn, char *name_table, int id);
void HAS_print_division_info(PGconn *dbconn, char user_answer, int number_divisions);
void HAS_print_users_activity_table(PGconn *dbconn, char *user_name);


/***************
   SQL QUERIES
 ***************/
PGresult* HAS_query_getNumberDivisions(PGconn *dbconn, int id_apartment, int * num_divisions, int * error_check);
PGresult* HAS_query_getNameDivision(PGconn *dbconn, int id_division, char * name_division, int * error_check);
int HAS_query_getMaximumNumberSensorsDivisionsInAnApartment(PGconn *dbconn, int id_apartment, int * id_division_max_sensors, int * error_check);
int HAS_query_getMaximumNumberActuatorsDivisionsInAnApartment(PGconn *dbconn, int id_apartment, int * id_division_max_actuators, int * error_check);
void HAS_query_delete_tables(PGconn *dbconn, int *error_check);
void HAS_query_insert_division_info(PGconn *dbconn, char *str, int id_divisions, int * error_check);
void HAS_query_insert_divisions_sensors(PGconn *dbconn, char **vector_string, int id_division, int number_sensors, int * error_check);
void HAS_query_insert_divisions_actuators(PGconn *dbconn, char **vector_string, int id_division, int number_actuators, int * error_check);
void HAS_query_update_number_divisions_apartament(PGconn *dbconn, int num_divisions);

PGresult* HAS_query_updateNumRules(PGconn *dbconn, int id_division, int num_rules, int * error_check);
void HAS_query_clearRulesTable(PGconn *dbconn);
void HAS_query_insertRule(PGconn *dbconn, int id_rule, char * nam_sens_cond_1, char op_cond_1, int thres_cond_1, char * nam_sens_cond_2, char op_cond_2, int thres_cond_2, int samp_per, char * schedule, int id_division, int num_act_fut_stat, char * logic_op_1_2);
PGresult* HAS_query_getRules(PGconn *dbconn, int * error_check);

void HAS_query_insertActuatorFutureState(PGconn *dbconn, int id_act_fut_stat, char * fut_stat, int id_actuator, int id_rule);
PGresult* HAS_query_getActuatorIdToActuatorFutureState(PGconn *dbconn, int id_division, char * actuator_name, int * id_actuator, int * error_check);
PGresult* HAS_query_getActuatorsFutureStateFromRule(PGconn *dbconn, int id_rule, int * error_check);

void HAS_query_update_dataSensor(PGconn *dbconn, int mote_id, char *type_sens, float value);
PGresult* HAS_query_getSensorsData(PGconn *dbconn, int * error_check);
int HAS_query_search_sensor(PGconn *dbconn, char *name, int mote);
int HAS_query_getSpecificSensorData(PGconn *dbconn, int id_division, int id_mote, int sensor_type, int * error_check);

void HAS_query_update_stateActuator(PGconn *dbconn, int id, char *act_name, char *act_state);
PGresult* HAS_query_getActuatorsState(PGconn *dbconn, int * error_check);
PGresult* HAS_query_getActuatorsStateWithTheirName(PGconn *dbconn, int * error_check);
PGresult* HAS_query_getActuatorsStateDivision(PGconn *dbconn, int id_division, int * error_check);

PGresult* HAS_query_getUserInfo(PGconn *dbconn, int id_user, int * error_check);
void HAS_query_showActiveUsersInfo(PGconn *dbconn);
void HAS_query_insertUser(PGconn *dbconn, int id, char * name, char * permission, char * password, int state, int id_apartment, int * error_check);

void HAS_query_user_activity_insert(PGconn *dbconn, int option, int *error_check);
PGresult* HAS_query_getAllUserInfo(PGconn *dbconn, int * error_check);


/************
   SPRINT#3
 ************/
void HAS_query_insertNewEntryActuatorHistory(PGconn * dbconn, int id_actuator, char * actuator_name, int id_division, char * actuator_state, int * error_check);
void HAS_query_updateTimeFinalActuatorHistory(PGconn * dbconn, int id_actuator, int * error_check);
void HAS_query_insertNewEntrySensorHistory(PGconn *dbconn, int *error_check, char * sensor_type, int mote_id, int sensor_data);


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
	int error_check = 0, aux1;
	char user_answer;
	pthread_t pth1, pth2, pth3;
	
	
	
	do{
		
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
		dbconn_main = PQconnectdb(conn);
		if (PQstatus(dbconn_main) == CONNECTION_BAD){
			printf("[DATABASE_ERROR %d] Unable to connect with the database. Please try again by restarting the program or check the ethernet connection\n", DB_ERROR_1);
			exit(-1);
		}
		 
		/**************************************************
		   INIT ROUTINE - TABLES CREATION IN THE DATABASE
		 **************************************************/
		init_table_creation(dbconn_main);
		
		/**************************************
		   INIT ROUTINE - LOGIN IN THE SYSTEM
		 **************************************/
		printf("+++++++++++++++++++++++++++\n"
			   "++++++++++ LOGIN ++++++++++\n"
			   "+++++++++++++++++++++++++++\n");
		init_login_in_the_system(dbconn_main);
		
		/***************************************
		   INIT ROUTINE - MOTE'S CONFIGURATION *
		 ***************************************/
		printf("\n"
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
			PQfinish(dbconn_main);
			dbconn_main = NULL;
			exit(-1);
		}
		
		/*******************************************
		   INIT ROUTINE - DIVISION'S CONFIGURATION
		 *******************************************/
		if(0 == strcmp(user_permission, "admin")){
			printf("\n\n\n\n\n\n"
				   "++++++++++++++++++++++++++++++++++++++++++++++\n"
				   "++++++++++ DIVISION'S CONFIGURATION ++++++++++\n"
				   "++++++++++++++++++++++++++++++++++++++++++++++\n");
			init_divisions_configuration(dbconn_main);
		}
		
		/*******************************************
		   INIT ROUTINE - RULE'S CONFIGURATION
		 *******************************************/
		printf("\n\n\n\n\n\n"
			   "++++++++++++++++++++++++++++++++++++++++++\n"
			   "++++++++++ RULE'S CONFIGURATION ++++++++++\n"
			   "++++++++++++++++++++++++++++++++++++++++++\n");
		init_rules_configuration(dbconn_main);
		
		/*****************************************
		   INIT ROUTINE - MATRIX'S CONFIGURATION
		 *****************************************/
		error_check = 0;
		number_maximum_actuators = HAS_query_getMaximumNumberActuatorsDivisionsInAnApartment(dbconn_main, 0, &aux1, &error_check);
		//printf("DEBUG number_maximum_actuators: %d\n", number_maximum_actuators);
		if(0 != error_check){
			printf("[MAIN_ERROR %d] An error has occurred in the number_maximum_actuators calculation.\n", MAIN_ERROR_6);
			fclose(sensor_data_channel);
			sensor_data_channel = NULL;
			fclose(rgb_matrix_channel);
			rgb_matrix_channel = NULL;
			PQfinish(dbconn_main);
			dbconn_main = NULL;
			printf("\n**********************************************************************\n");
			printf("************************* END OF PROGRAM HAS *************************\n");
			printf("**********************************************************************\n\n");
			exit(-1);
		}
		error_check = 0;
		number_maximum_sensors = HAS_query_getMaximumNumberSensorsDivisionsInAnApartment(dbconn_main, 0, &aux1, &error_check);
		//printf("DEBUG number_maximum_sensors: %d\n", number_maximum_sensors);
		if(0 != error_check){
			printf("[MAIN_ERROR %d] An error has occurred in the number_maximum_sensors calculation.\n", MAIN_ERROR_7);
			fclose(sensor_data_channel);
			sensor_data_channel = NULL;
			fclose(rgb_matrix_channel);
			rgb_matrix_channel = NULL;
			PQfinish(dbconn_main);
			dbconn_main = NULL;
			printf("\n**********************************************************************\n");
			printf("************************* END OF PROGRAM HAS *************************\n");
			printf("**********************************************************************\n\n");
			exit(-1);
		}
		write_size_matrix("RGBMatrixConf.txt", number_divisions, number_maximum_sensors + number_maximum_actuators);
		matrix_side_size = determination_of_maximum(number_divisions, number_maximum_sensors + number_maximum_actuators);
		buffer_rgb_channel = alocation_memory_matrix(matrix_side_size);
		
		buffer_rgb_channel_aux = alocation_memory_matrix(matrix_side_size);
		if(NULL == buffer_rgb_channel || NULL == buffer_rgb_channel_aux){
			printf("[MAIN_ERROR %d] An error has occurred in the memory allocation for strings buffer_rgb_channel or buffer_rgb_channel_aux.\n", MAIN_ERROR_8);
			if(NULL == buffer_rgb_channel){
				free(buffer_rgb_channel);
				buffer_rgb_channel = NULL;
			}
			if(NULL == buffer_rgb_channel_aux){
				free(buffer_rgb_channel_aux);
				buffer_rgb_channel_aux = NULL;
			}
			fclose(sensor_data_channel);
			sensor_data_channel = NULL;
			fclose(rgb_matrix_channel);
			rgb_matrix_channel = NULL;
			PQfinish(dbconn_main);
			dbconn_main = NULL;
			printf("\n**********************************************************************\n");
			printf("************************* END OF PROGRAM HAS *************************\n");
			printf("**********************************************************************\n\n");
			exit(-1);
		}
		
		do{
			printf("The program RGBMatrix is already running (Y - yes / N - no, and press Enter)? ");
			user_answer = getchar();
			getchar();
		} while('Y' != user_answer &&'N' != user_answer &&'y' != user_answer && 'n' != user_answer);
		if('n' == user_answer ||'N' == user_answer){
			printf("Then please execute it with the channel's atribuition to communicate with the program.\n");
			free_mote_memory(system_motes, number_motes, NULL);
			system_motes = NULL;
			
			free(buffer_rgb_channel);
			buffer_rgb_channel = NULL;
			free(buffer_rgb_channel_aux);
			buffer_rgb_channel_aux = NULL;
			
			fclose(sensor_data_channel);
			sensor_data_channel = NULL;
			fclose(rgb_matrix_channel);
			rgb_matrix_channel = NULL;
			printf("\n**********************************************************************\n");
			printf("************************* END OF PROGRAM HAS *************************\n");
			printf("**********************************************************************\n\n");
			exit(-1);
		}
		init_matrix_configuration(dbconn_main);
		
		
		PQfinish(dbconn_main);
		dbconn_main = NULL;
		
		
		/***********************
		   CREATION OF THREADS
		 ***********************/
		pthread_create(&pth1, NULL, thread_data_processing, NULL);
		pthread_create(&pth2, NULL, thread_rule_implementation, NULL);
		pthread_create(&pth3, NULL, user_interface, NULL);
		
		/****************************************
		   CREATION AND CANCELLATION OF THREADS
		 ****************************************/
		pthread_join(pth3, NULL);
		pthread_cancel(pth1);
		pthread_cancel(pth2);
		pthread_cancel(pth3);
		
		/***********************
		   TERMINATION ROUTINE
		 ***********************/
		free_mote_memory(system_motes, number_motes, NULL);
		PQfinish(dbconn_1);
		dbconn_1 = NULL;
		PQfinish(dbconn_2);
		dbconn_2 = NULL;
		PQfinish(dbconn_3);
		dbconn_3 = NULL;
		
	}while(1 == flag_command);
	
	free(buffer_rgb_channel);
	buffer_rgb_channel = NULL;
	free(buffer_rgb_channel_aux);
	buffer_rgb_channel_aux = NULL;
	
	fclose(sensor_data_channel);
	sensor_data_channel = NULL;
	
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
void init_login_in_the_system(PGconn *dbconn){
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
					HAS_query_showActiveUsersInfo(dbconn);
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
		
		query = HAS_query_getUserInfo(dbconn, user_id_aux, &error_check);
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
				HAS_query_showActiveUsersInfo(dbconn);
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
			HAS_query_showActiveUsersInfo(dbconn);
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
		if(0 == strcmp("EXIT", str) || 0 == strcmp("exit", str)){
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
				HAS_query_showActiveUsersInfo(dbconn);
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
	
	option = 1;
	HAS_query_user_activity_insert(dbconn, option, NULL);
	
	printf("\n---------- Welcome %s ----------\n", username);
	
	/***** DEBUG ZONE *****//*
	printf("[USER   ID] %d\n"
	       "[USER NAME] %s\n"
	       "[USER_PERM] %s\n"
	       "[USER_PASS] %s\n"
	       "[US_ID_APT] %d\n", user_id, username, user_permission, user_password, user_id_apartment);*/
	/**********************/
}


/******************************
   INIT_TABLE_CREATION - CODE
 ******************************/
void init_table_creation(PGconn *dbconn){
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
	 
	int res_query;
	PGresult *query;
	
	switch(HAS_query_verify_building_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_building_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_BUILDING: The table 'building' was successfully created!\n");
				query = PQexec(dbconn,"INSERT INTO building (address, num_floors, owner, num_apartments) "\
								"VALUES ('Rua Dr. Roberto Frias', 1, 'Daniel,Pedro e Ricardo Lda.', 1)");
				PQclear(query);
			}
			else if(res_query == TABLE_NOT_CREATED){
				printf("INFO_BUILDING: The table 'building' couldn't be created!\n");
			}
			else{
				printf("ERROR_BUILDING: The creation of the table 'building' wans't successfully done!\n");
			}
		}break;
		
		case TABLE_EXIST:{
			printf("INFO_BUILDING: The table 'building' already exists!\n");
		}break;
			
		default:{
			printf("ERROR_BUILDING: The verification of the table 'building' wasn't successfully done!\n");
		}break;
	}
	
	switch(HAS_query_verify_apartment_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_apartment_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_APARTMENT: The table 'apartment' was successfully created!\n");
				query = PQexec(dbconn,"INSERT INTO apartment (id, floor, number, proprietary, num_divisions, address_building) "\
				"VALUES (0,0,1,'Armando Sousa Araújo',4,'Rua Dr. Roberto Frias')");
				PQclear(query);
			}
			else if(res_query == TABLE_NOT_CREATED){
				printf("INFO_APARTMENT: The table 'apartment' couldn't be created!\n");
			}
			else{
				printf("ERROR_APARTMENT: The creation of the table 'apartment' wans't successfully done!\n");
			}
		}break;
		
		case TABLE_EXIST:{
			printf("INFO_APARTMENT: The table 'apartment' already exists!\n");
		}break;
			
		default:{
			printf("ERROR_APARTMENT: The verification of the table 'apartment' wasn't successfully done!\n");
		}break;
	}
	
	switch(HAS_query_verify_divisions_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_divisions_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_DIVISIONS: The table 'divisions' was successfully created!\n");
			}
			else if(res_query == TABLE_NOT_CREATED){
				printf("INFO_DIVISIONS: The table 'divisions' couldn't be created!\n");
			}
			else{
				printf("ERROR_DIVISIONS: The creation of the table 'divisions' wans't successfully done!\n");
			}
		}break;
		
		case TABLE_EXIST:{
			printf("INFO_DIVISIONS: The table 'divisions' already exists!\n");
		}break;
			
		default:{
			printf("ERROR_DIVISIONS: The verification of the table 'divisions' wasn't successfully done!\n");
		}break;
	}
	
	switch(HAS_query_verify_actuator_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_actuator_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_ACTUATOR: The table 'actuator' was successfully created!\n");
			}
			else if(res_query == TABLE_NOT_CREATED){
				printf("INFO_ACTUATOR: The table 'actuator' couldn't be created!\n");
			}
			else{
				printf("ERROR_ACTUATOR: The creation of the table 'actuator' wans't successfully done!\n");
			}
		}break;
		
		case TABLE_EXIST:{
			printf("INFO_ACTUATOR: The table 'actuator' already exists!\n");
		}break;
			
		default:{
			printf("ERROR_ACTUATOR: The verification of the table 'actuator' wasn't successfully done!\n");
		}break;
	}
	
	switch(HAS_query_verify_rules_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_rules_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_RULES: The table 'rules' was successfully created!\n");
			}
			else if(res_query == TABLE_NOT_CREATED){
				printf("INFO_RULES: The table 'rules' couldn't be created!\n");
			}
			else{
				printf("ERROR_RULES: The creation of the table 'rules' wans't successfully done!\n");
			}
		}break;
		
		case TABLE_EXIST:{
			printf("INFO_RULES: The table 'rules' already exists!\n");
		}break;
			
		default:{
			printf("ERROR_RULES: The verification of the table 'rules' wasn't successfully done!\n");
		}break;
	}
	
	switch(HAS_query_verify_sensor_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_sensor_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_SENSOR: The table 'sensor' was successfully created!\n");
			}
			else if(res_query == TABLE_NOT_CREATED){
				printf("INFO_SENSOR: The table 'sensor' couldn't be created!\n");
			}
			else{
				printf("ERROR_SENSOR: The creation of the table 'sensor' wans't successfully done!\n");
			}
		}break;
		
		case TABLE_EXIST:{
			printf("INFO_SENSOR: The table 'sensor' already exists!\n");
		}break;
			
		default:{
			printf("ERROR_SENSOR: The verification of the table 'sensor' wasn't successfully done!\n");
		}break;
	}
	
	switch(HAS_query_verify_users_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_users_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_USERS: The table 'users' was successfully created!\n");
				HAS_query_insertUser(dbconn, 0, "User", "admin", "12345", 1, 0, NULL);
			}
			else if(res_query == TABLE_NOT_CREATED){
				printf("INFO_USERS: The table 'users' couldn't be created!\n");
			}
			else{
				printf("ERROR_USERS: The creation of the table 'users' wans't successfully done!\n");
			}
		}break;
		
		case TABLE_EXIST:{
			printf("INFO_USERS: The table 'users' already exists!\n");
		}break;
			
		default:{
			printf("ERROR_USERS: The verification of the table 'users' wasn't successfully done!\n");
		}break;
	}
	
	switch(HAS_query_verify_users_activity_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_users_activity_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_USERS_ACTIVITY: The table 'users_activity' was successfully created!\n");
			}
			else if(res_query == TABLE_NOT_CREATED){
				printf("INFO_USERS_ACTIVITY: The table 'users_activity' couldn't be created!\n");
			}
			else{
				printf("ERROR_USERS_ACTIVITY: The creation of the table 'users_activity' wans't successfully done!\n");
			}
		}break;
		
		case TABLE_EXIST:{
			printf("INFO_USERS_ACTIVITY: The table 'users_activity' already exists!\n");
		}break;
			
		default:{
			printf("ERROR_USERS_ACTIVITY: The verification of the table 'users_activity' wasn't successfully done!\n");
		}break;
	}
	
	switch(HAS_query_verify_actuator_future_state_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_actuator_future_state_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_ACTUATOR_FUTURE_STATE: The table 'actuator_future_state' was successfully created!\n");
			}
			else if(res_query == TABLE_NOT_CREATED){
				printf("INFO_ACTUATOR_FUTURE_STATE: The table 'actuator_future_state' couldn't be created!\n");
			}
			else{
				printf("ERROR_ACTUATOR_FUTURE_STATE: The creation of the table 'actuator_future_state' wans't successfully done!\n");
			}
		}break;
		
		case TABLE_EXIST:{
			printf("INFO_ACTUATOR_FUTURE_STATE: The table 'actuator_future_state' already exists!\n");
		}break;
			
		default:{
			printf("ERROR_ACTUATOR_FUTURE_STATE: The verification of the table 'actuator_future_state' wasn't successfully done!\n");
		}break;
	}
	printf("\n");
}


/***********************************
   DIVISION'S CONFIGURATION - CODE
 ***********************************/
void init_divisions_configuration(PGconn *dbconn){
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
	 
	char user_answer, str[SIZE_STRING_BUFFER] = {0};
	PGresult *query;
	 
	count_divisions = 0;
	divisions_configuration = 0;
	id_sensor = 0;
	sens_matrix_id = 0;
	id_actuator = 0;
	act_matrix_id = 0;
	
	HAS_print_table(dbconn, TABNAME_DIVIS);
	HAS_print_table(dbconn, TABNAME_SENS);
	HAS_print_table(dbconn, TABNAME_ACT);
	
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
		HAS_query_update_number_divisions_apartament(dbconn, number_divisions);
		do {
			printf("\n-- Division's configuration template: ROOM1:TEMP1,LIGHT1;HEAT,BLINDS,LIGHT\n");
			printf("Insert division configuration nº %d: ", count_divisions+1);
			fgets(str, SIZE_STRING_BUFFER, stdin);
			if('\n' == str[strlen(str) - 1])
				str[strlen(str) - 1] = '\0';
			HAS_query_insert_division_info(dbconn, str, count_divisions, NULL);
			count_divisions++;
		} while (count_divisions < number_divisions);
		option = 3;
		HAS_query_user_activity_insert(dbconn, option, NULL);		
	} else {			// O utilizador que manter a configuração das divisões
		divisions_configuration = OLD_DIVISIONS_CONFIGURATION;
		query = HAS_query_getNumberDivisions(dbconn, 0, &number_divisions, NULL);
		PQclear(query);		
	}
}


/*******************************
   RULE'S CONFIGURATION - CODE
 *******************************/
void init_rules_configuration(PGconn *dbconn){
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
	if(OLD_DIVISIONS_CONFIGURATION == divisions_configuration){
		HAS_print_table(dbconn, TABNAME_RULES);
		HAS_print_table(dbconn, TABNAME_ACTFUTSTAT);
		do{
			fflush(stdin);
			printf("Do you want to change the rules that are stored in the system? (Y - yes; N - no) ");
			user_answer = getchar();
			getchar();
			if(('Y' != user_answer && 'y' != user_answer) && ('N' != user_answer && 'n' != user_answer))
				printf("Please insert a valid character.\n");
		}while(('Y' != user_answer && 'y' != user_answer) && ('N' != user_answer && 'n' != user_answer));
	}
	if('Y' == user_answer || 'y' == user_answer || NEW_DIVISIONS_CONFIGURATION == divisions_configuration){
		number_rules = 0;
		counter_act_fut_sta = 0;
		HAS_query_clearRulesTable(dbconn);
		rules_query = HAS_query_getNumberDivisions(dbconn, 0, &number_divisions, &error_check);
		PQclear(rules_query);
		printf("Number of divisions associated with the apartment 0: %d\n", number_divisions);
		
		for(counter_divisions = 0; counter_divisions < number_divisions; counter_divisions++){
			rules_query = HAS_query_getNameDivision(dbconn, counter_divisions, name_division, &error_check);
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
			
			rules_query = HAS_query_updateNumRules(dbconn, counter_divisions, number_rules_per_division, &error_check);
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

				HAS_query_insertRule(dbconn, number_rules, sensor_condition_1, operator_condition_1, value_condition_1, sensor_condition_2, operator_condition_2, value_condition_2, 0, NULL, counter_divisions, num_actuator_conditions, logic_operator_condition_1_2);

				
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
					
					rules_query = HAS_query_getActuatorIdToActuatorFutureState(dbconn, counter_divisions, act_future_stat, &id_actuator_aux, &error_check);
					PQclear(rules_query);
					if(0 > id_actuator_aux){
						printf("[INIT_RULES_CONFIG %d] Actuator %d of the future states invalid.\n", ERROR_INIT_RULES_CONFIG_11, counter3 + 1);
						printf("    The problem was found in the RULE %d\n", counter_rules + 1);
						return;
					}
					
					HAS_query_insertActuatorFutureState(dbconn, counter_act_fut_sta, act_future_stat_ON_OFF, id_actuator_aux, number_rules);
					
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
		option = 4;
		HAS_query_user_activity_insert(dbconn, option, NULL);
	}
}


/*********************************
   MATRIX'S CONFIGURATION - CODE
 *********************************/
void init_matrix_configuration(PGconn *dbconn){
	/*
	 * Fazer os seguintes queries:
	 * 		- SELECT id, actuator_state, id_divisions, act_matrix_id FROM actuator
	 * 		- SELECT id, mote_id, sensor_type, data, id_divisions_ sens_matrix_id FROM sensors
	 */
	atualize_RGBstring_sensors_state_RGBMatrix(dbconn, buffer_rgb_channel, matrix_side_size);
	atualize_RGBstring_actuators_state_RGBMatrix(dbconn, buffer_rgb_channel, matrix_side_size);
	
	//printf("STRING RGB:\n%s\n", buffer_rgb_channel);
	
	rgb_matrix_channel = fopen((*pointer_rgb_channel), "w");
	fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
	fclose(rgb_matrix_channel);
	rgb_matrix_channel = NULL;
}


/**************************************
   SQL CREATION TABLES QUERIES - CODE
 **************************************/
int HAS_query_verify_actuator_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"SELECT * from actuator");
	if((PQnfields(query) > 0) && (PQresultStatus(query) == PGRES_TUPLES_OK)){
		PQclear(query);
		return TABLE_EXIST;
	}
	else if(PQresultStatus(query) != PGRES_TUPLES_OK){
		PQclear(query);
		return TABLE_NOT_EXIST;
	}
	else{
		PQclear(query);
		return ERROR_VERIFYING_TABLE;
	}
}

int HAS_query_create_actuator_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"CREATE TABLE actuator"\
									"("\
									"id INT PRIMARY KEY NOT NULL,"\
									"name character varying(20) NOT NULL,"\
									"actuator_state character varying(5) CHECK (actuator_state = 'ON' OR actuator_state = 'OFF') NOT NULL,"\
									"time TIME NOT NULL,"\
									"id_divisions INT REFERENCES divisions(id) NOT NULL,"\
									"day DATE NOT NULL,"\
									"act_matrix_id INT CHECK (act_matrix_id >= 0) NOT NULL"\
									")");
	if(HAS_query_verify_actuator_table(dbconn) == TABLE_EXIST){
		PQclear(query);
		return TABLE_CREATED;
	}
	else{
		PQclear(query);
		return TABLE_NOT_CREATED;
	}
}

int HAS_query_verify_actuator_future_state_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"SELECT * from actuator_future_state");
	if((PQnfields(query) > 0) && (PQresultStatus(query) == PGRES_TUPLES_OK)){
		PQclear(query);
		return TABLE_EXIST;
	}
	else if(PQresultStatus(query) != PGRES_TUPLES_OK){
		PQclear(query);
		return TABLE_NOT_EXIST;
	}
	else{
		PQclear(query);
		return ERROR_VERIFYING_TABLE;
	}
}

int HAS_query_create_actuator_future_state_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"CREATE TABLE actuator_future_state"\
									"("\
									"id INT PRIMARY KEY NOT NULL,"\
									"future_state character varying(5) CHECK (future_state = 'ON' OR future_state = 'OFF') NOT NULL,"\
									"id_actuator INT REFERENCES actuator(id) NOT NULL,"\
									"id_rules INT REFERENCES rules(id) NOT NULL"\
									")");
	if(HAS_query_verify_actuator_future_state_table(dbconn) == TABLE_EXIST){
		PQclear(query);
		return TABLE_CREATED;
	}
	else{
		PQclear(query);
		return TABLE_NOT_CREATED;
	}
}

int HAS_query_verify_apartment_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"SELECT * from apartment");
	if((PQnfields(query) > 0) && (PQresultStatus(query) == PGRES_TUPLES_OK)){
		PQclear(query);
		return TABLE_EXIST;
	}
	else if(PQresultStatus(query) != PGRES_TUPLES_OK){
		PQclear(query);
		return TABLE_NOT_EXIST;
	}
	else{
		PQclear(query);
		return ERROR_VERIFYING_TABLE;
	}
}

int HAS_query_create_apartment_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"CREATE TABLE apartment"\
									"("\
									"id INT PRIMARY KEY NOT NULL,"\
									"floor INT NOT NULL,"\
									"number INT CHECK (number > 0) NOT NULL,"\
									"proprietary character varying(128) NOT NULL,"\
									"num_divisions INT CHECK (num_divisions > 0) NOT NULL,"\
									"address_building character varying(128) REFERENCES building(address) NOT NULL"\
									")");
	if(HAS_query_verify_apartment_table(dbconn) == TABLE_EXIST){
		PQclear(query);
		return TABLE_CREATED;
	}
	else{
		PQclear(query);
		return TABLE_NOT_CREATED;
	}
}

int HAS_query_verify_building_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"SELECT * from building");
	if((PQnfields(query) > 0) && (PQresultStatus(query) == PGRES_TUPLES_OK)){
		PQclear(query);
		return TABLE_EXIST;
	}
	else if(PQresultStatus(query) != PGRES_TUPLES_OK){
		PQclear(query);
		return TABLE_NOT_EXIST;
	}
	else{
		PQclear(query);
		return ERROR_VERIFYING_TABLE;
	}
}

int HAS_query_create_building_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"CREATE TABLE building"\
									"("\
									"address character varying(256) PRIMARY KEY NOT NULL,"\
									"num_floors INT CHECK (num_floors > 0) NOT NULL,"\
									"owner character varying(128) NOT NULL,"\
									"num_apartments INT CHECK (num_apartments > 0) NOT NULL"\
									")");
	if(HAS_query_verify_building_table(dbconn) == TABLE_EXIST){
		PQclear(query);
		return TABLE_CREATED;
	}
	else{
		PQclear(query);
		return TABLE_NOT_CREATED;
	}
}

int HAS_query_verify_divisions_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"SELECT * from divisions");
	if((PQnfields(query) > 0) && (PQresultStatus(query) == PGRES_TUPLES_OK)){
		PQclear(query);
		return TABLE_EXIST;
	}
	else if(PQresultStatus(query) != PGRES_TUPLES_OK){
		PQclear(query);
		return TABLE_NOT_EXIST;
	}
	else{
		PQclear(query);
		return ERROR_VERIFYING_TABLE;
	}
}

int HAS_query_create_divisions_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"CREATE TABLE divisions"\
									"("\
									"id INT PRIMARY KEY NOT NULL,"\
									"name character varying(64) NOT NULL,"\
									"num_sensors INT CHECK (num_sensors >= 0) NOT NULL,"\
									"num_actuators INT CHECK (num_actuators >= 0) NOT NULL,"\
									"id_apartment INT REFERENCES apartment(id) NOT NULL,"\
									"num_rules INT CHECK(num_rules >= 0) NOT NULL"\
									")");
	if(HAS_query_verify_divisions_table(dbconn) == TABLE_EXIST){
		PQclear(query);
		return TABLE_CREATED;
	}
	else{
		PQclear(query);
		return TABLE_NOT_CREATED;
	}
}

int HAS_query_verify_rules_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"SELECT * from rules");
	if((PQnfields(query) > 0) && (PQresultStatus(query) == PGRES_TUPLES_OK)){
		PQclear(query);
		return TABLE_EXIST;
	}
	else if(PQresultStatus(query) != PGRES_TUPLES_OK){
		PQclear(query);
		return TABLE_NOT_EXIST;
	}
	else{
		PQclear(query);
		return ERROR_VERIFYING_TABLE;
	}
}

int HAS_query_create_rules_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"CREATE TABLE rules"\
									"("\
									"id INT PRIMARY KEY NOT NULL,"\
									"name_sensor_condition1 character varying(20) NOT NULL,"\
									"operator_condition1 character varying(1) CHECK (operator_condition1 ='<' OR operator_condition1 ='>') NOT NULL,"\
									"threshold_condition1 INT CHECK (threshold_condition1 > 0) NOT NULL,"\
									"name_sensor_condition2 character varying(20),"\
									"operator_condition2 character varying(1) CHECK (operator_condition2 ='<' OR operator_condition2 ='>'),"\
									"threshold_condition2 INT CHECK (threshold_condition2 > 0),"\
									"sampling_period INT CHECK (sampling_period > 0),"\
									"schedule character varying(20),"\
									"time TIME NOT NULL,"\
									"id_divisions INT REFERENCES divisions(id) NOT NULL,"\
									"day DATE NOT NULL,"\
									"num_actuators_future_state INT CHECK (num_actuators_future_state > 0) NOT NULL,"\
									"logic_operator character varying(3) CHECK (logic_operator = 'AND' OR logic_operator = 'OR')"\
									")");
	if(HAS_query_verify_rules_table(dbconn) == TABLE_EXIST){
		PQclear(query);
		return TABLE_CREATED;
	}
	else{
		PQclear(query);
		return TABLE_NOT_CREATED;
	}
}

int HAS_query_verify_sensor_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"SELECT * from sensor");
	if((PQnfields(query) > 0) && (PQresultStatus(query) == PGRES_TUPLES_OK)){
		PQclear(query);
		return TABLE_EXIST;
	}
	else if(PQresultStatus(query) != PGRES_TUPLES_OK){
		PQclear(query);
		return TABLE_NOT_EXIST;
	}
	else{
		PQclear(query);
		return ERROR_VERIFYING_TABLE;
	}
}

int HAS_query_create_sensor_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"CREATE TABLE sensor"\
									"("\
									"id INT PRIMARY KEY NOT NULL,"\
									"mote_id INT CHECK (mote_id > 0) NOT NULL,"\
									"sensor_type character varying(20) CHECK (sensor_type = 'LIGHT' OR sensor_type = 'HUM' OR sensor_type = 'TEMP') NOT NULL,"\
									"data INT NOT NULL,"\
									"time TIME NOT NULL,"\
									"id_divisions INT REFERENCES divisions(id) NOT NULL,"\
									"day DATE NOT NULL,"\
									"sens_matrix_id INT CHECK (sens_matrix_id >= 0) NOT NULL"\
									")");
	if(HAS_query_verify_sensor_table(dbconn) == TABLE_EXIST){
		PQclear(query);
		return TABLE_CREATED;
	}
	else{
		PQclear(query);
		return TABLE_NOT_CREATED;
	}
}

int HAS_query_verify_users_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"SELECT * from users");
	if((PQnfields(query) > 0) && (PQresultStatus(query) == PGRES_TUPLES_OK)){
		PQclear(query);
		return TABLE_EXIST;
	}
	else if(PQresultStatus(query) != PGRES_TUPLES_OK){
		PQclear(query);
		return TABLE_NOT_EXIST;
	}
	else{
		PQclear(query);
		return ERROR_VERIFYING_TABLE;
	}
}

int HAS_query_create_users_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"CREATE TABLE users"\
									"("\
									"id INT PRIMARY KEY NOT NULL,"\
									"name character varying(64) NOT NULL,"\
									"permission character varying(128) CHECK (permission = 'admin' OR permission = 'normal') NOT NULL,"\
									"password character varying(10) NOT NULL,"\
									"state INT CHECK (state = 0 OR state = 1) NOT NULL,"\
									"id_apartment INT REFERENCES apartment(id) NOT NULL"\
									")");
	if(HAS_query_verify_users_table(dbconn) == TABLE_EXIST){
		PQclear(query);
		return TABLE_CREATED;
	}
	else{
		PQclear(query);
		return TABLE_NOT_CREATED;
	}
}

int HAS_query_verify_users_activity_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"SELECT * from users_activity");
	if((PQnfields(query) > 0) && (PQresultStatus(query) == PGRES_TUPLES_OK)){
		PQclear(query);
		return TABLE_EXIST;
	}
	else if(PQresultStatus(query) != PGRES_TUPLES_OK){
		PQclear(query);
		return TABLE_NOT_EXIST;
	}
	else{
		PQclear(query);
		return ERROR_VERIFYING_TABLE;
	}
}

int HAS_query_create_users_activity_table(PGconn *dbconn){
	PGresult *query = PQexec(dbconn,"CREATE TABLE users_activity"\
									"("\
									"id INT PRIMARY KEY NOT NULL,"\
									"id_users INT REFERENCES users(id) NOT NULL,"\
									"activity_description character varying(100) NOT NULL,"\
									"time TIME NOT NULL,"\
									"day DATE NOT NULL"\
									")");
	if(HAS_query_verify_users_activity_table(dbconn) == TABLE_EXIST){
		PQclear(query);
		return TABLE_CREATED;
	}
	else{
		PQclear(query);
		return TABLE_NOT_CREATED;
	}
}

void HAS_print_table(PGconn *dbconn, char *name_table){
	
	int i = 0, num_columns = 0, num_lines = 0, step_line = 0, step_col = 0;
	int size_name_col = 0, size_elem = 0, size_elem_max = 0, res_space = 0;
	char str_query[256];
	
	PGresult *query;
	printf("-> %s\n",name_table);
	sprintf(str_query,"SELECT * FROM %s",name_table);	
	query = PQexec(dbconn, str_query);
	
	num_lines = PQntuples(query);
	num_columns = PQnfields(query);
	int max_size_col[num_columns];		
					
	if (PQresultStatus(query) == PGRES_TUPLES_OK){
		if((num_columns != 0) && (strcmp(name_table,"rules") != 0)){		//Todas as tabelas exceto as 'rules'								
			for (step_col = 0; step_col < num_columns; step_col++){
				size_name_col = strlen(PQfname(query,step_col));
				if(size_name_col > size_elem_max){
					size_elem_max = size_name_col;
				}
				else size_elem_max += 0;
				
				for(step_line = 0; step_line < num_lines; step_line++){
					size_elem = strlen(PQgetvalue(query,step_line,step_col));
					if(size_elem > size_elem_max)
						size_elem_max = size_elem;
					else size_elem_max += 0;
				}
				max_size_col[step_col] = size_elem_max + NUM_SPACES;
				size_elem_max = 0;
			}
				
			for(step_col = 0; step_col < num_columns; step_col++){		//Nome das colunas
				res_space = max_size_col[step_col] - strlen(PQfname(query,step_col));
				printf("%s",PQfname(query,step_col));
				for(i = 0; i < abs(res_space); i++){
					printf(" ");
				}
			}
			printf("\n");
			
			for(step_line = 0; step_line < num_lines; step_line++){		//Elementos
				for(step_col = 0; step_col < num_columns; step_col++){
					printf("%s",PQgetvalue(query,step_line,step_col));
					res_space = max_size_col[step_col] - strlen(PQgetvalue(query,step_line,step_col));
					for(i = 0; i < abs(res_space); i++){
						printf(" ");
					}
				}
				printf("\n");
			}
		}
		else if((num_columns != 0) && (strcmp(name_table,"rules") == 0)){		//Tabela das 'rules'
			for (step_col = 0; step_col < num_columns; step_col++){
				size_name_col = strlen(PQfname(query,step_col));
				if(size_name_col > size_elem_max){
					size_elem_max = size_name_col;
				}
				else size_elem_max += 0;
				
				for(step_line = 0; step_line < num_lines; step_line++){
					size_elem = strlen(PQgetvalue(query,step_line,step_col));
					if(size_elem > size_elem_max)
						size_elem_max = size_elem;
					else size_elem_max += 0;
				}
				max_size_col[step_col] = size_elem_max + NUM_SPACES;
				size_elem_max = 0;
			}
			printf("\n");
				
			for(step_col = 0; step_col < PQfnumber(query,"sampling_period"); step_col++){		//Nome das colunas antes do 'sampling_period'
				res_space = max_size_col[step_col] - strlen(PQfname(query,step_col));
				printf("%s",PQfname(query,step_col));
				for(i = 0; i < abs(res_space); i++){
					printf(" ");
				}
			}
			printf("\n");
			
			for(step_line = 0; step_line < num_lines; step_line++){								//Elementos das colunas antes do 'sampling period'
				for(step_col = 0; step_col < PQfnumber(query,"sampling_period"); step_col++){
					printf("%s",PQgetvalue(query,step_line,step_col));
					res_space = max_size_col[step_col] - strlen(PQgetvalue(query,step_line,step_col));
					for(i = 0; i < abs(res_space); i++){
						printf(" ");
					}
				}
				printf("\n");
			}
			printf("\n\n");
			
			for(step_col = PQfnumber(query,"sampling_period"); step_col < num_columns; step_col++){		//Nome das colunas depois do 'sampling_period'
				res_space = max_size_col[step_col] - strlen(PQfname(query,step_col));
				printf("%s",PQfname(query,step_col));
				for(i = 0; i < abs(res_space); i++){
					printf(" ");
				}
			}
			printf("\n");
			
			for(step_line = 0; step_line < num_lines; step_line++){								//Elementos das colunas depois do 'sampling period'
				for(step_col = PQfnumber(query,"sampling_period"); step_col < num_columns; step_col++){
					printf("%s",PQgetvalue(query,step_line,step_col));
					res_space = max_size_col[step_col] - strlen(PQgetvalue(query,step_line,step_col));
					for(i = 0; i < abs(res_space); i++){
						printf(" ");
					}
				}
				printf("\n");
			}			
		}			
		else{
			printf("The table doesn't exist!\n");
		}
	}
	
	else{
		printf("DB query call not ok!\n");
	}
	printf("\n");
}

void HAS_print_table_restrictions(PGconn *dbconn, char *name_table, int id){
	
	int i = 0, num_columns = 0, num_lines = 0, step_line = 0, step_col = 0;
	int size_name_col = 0, size_elem = 0, size_elem_max = 0, res_space = 0;
	char str_query[256];
	
	if(0==strcmp(name_table, TABNAME_SENS)) {
		printf("\n-> Sensors");
	} else if(0==strcmp(name_table, TABNAME_ACT)) {
		printf("\n-> Actuators");
	} else if(0==strcmp(name_table, TABNAME_RULES))
		printf("\n-> Rules");
	
	PGresult *query;
	sprintf(str_query,"SELECT * FROM %s WHERE id_divisions = %d", name_table, id);	
	query = PQexec(dbconn, str_query);
	
	num_lines = PQntuples(query);
	num_columns = PQnfields(query);
	int max_size_col[num_columns];		
					
	if (PQresultStatus(query) == PGRES_TUPLES_OK){
		if((num_columns != 0) && (strcmp(name_table,"rules") != 0)){		//Todas as tabelas exceto as 'rules'								
			for (step_col = 0; step_col < num_columns; step_col++){
				size_name_col = strlen(PQfname(query,step_col));
				if(size_name_col > size_elem_max){
					size_elem_max = size_name_col;
				}
				else size_elem_max += 0;
				
				for(step_line = 0; step_line < num_lines; step_line++){
					size_elem = strlen(PQgetvalue(query,step_line,step_col));
					if(size_elem > size_elem_max)
						size_elem_max = size_elem;
					else size_elem_max += 0;
				}
				max_size_col[step_col] = size_elem_max + NUM_SPACES;
				size_elem_max = 0;
			}
			printf("\n");
				
			for(step_col = 0; step_col < num_columns; step_col++){		//Nome das colunas
				res_space = max_size_col[step_col] - strlen(PQfname(query,step_col));
				printf("%s",PQfname(query,step_col));
				for(i = 0; i < abs(res_space); i++){
					printf(" ");
				}
			}
			printf("\n");
			
			for(step_line = 0; step_line < num_lines; step_line++){		//Elementos
				for(step_col = 0; step_col < num_columns; step_col++){
					printf("%s",PQgetvalue(query,step_line,step_col));
					res_space = max_size_col[step_col] - strlen(PQgetvalue(query,step_line,step_col));
					for(i = 0; i < abs(res_space); i++){
						printf(" ");
					}
				}
				printf("\n");
			}
		}
		else if((num_columns != 0) && (strcmp(name_table,"rules") == 0)){		//Tabela das 'rules'
			for (step_col = 0; step_col < num_columns; step_col++){
				size_name_col = strlen(PQfname(query,step_col));
				if(size_name_col > size_elem_max){
					size_elem_max = size_name_col;
				}
				else size_elem_max += 0;
				
				for(step_line = 0; step_line < num_lines; step_line++){
					size_elem = strlen(PQgetvalue(query,step_line,step_col));
					if(size_elem > size_elem_max)
						size_elem_max = size_elem;
					else size_elem_max += 0;
				}
				max_size_col[step_col] = size_elem_max + NUM_SPACES;
				size_elem_max = 0;
			}
			printf("\n");
				
			for(step_col = 0; step_col < PQfnumber(query,"sampling_period"); step_col++){		//Nome das colunas antes do 'sampling_period'
				res_space = max_size_col[step_col] - strlen(PQfname(query,step_col));
				printf("%s",PQfname(query,step_col));
				for(i = 0; i < abs(res_space); i++){
					printf(" ");
				}
			}
			printf("\n");
			
			for(step_line = 0; step_line < num_lines; step_line++){								//Elementos das colunas antes do 'sampling period'
				for(step_col = 0; step_col < PQfnumber(query,"sampling_period"); step_col++){
					printf("%s",PQgetvalue(query,step_line,step_col));
					res_space = max_size_col[step_col] - strlen(PQgetvalue(query,step_line,step_col));
					for(i = 0; i < abs(res_space); i++){
						printf(" ");
					}
				}
				printf("\n");
			}
			printf("\n\n");
			
			for(step_col = PQfnumber(query,"sampling_period"); step_col < num_columns; step_col++){		//Nome das colunas depois do 'sampling_period'
				res_space = max_size_col[step_col] - strlen(PQfname(query,step_col));
				printf("%s",PQfname(query,step_col));
				for(i = 0; i < abs(res_space); i++){
					printf(" ");
				}
			}
			printf("\n");
			
			for(step_line = 0; step_line < num_lines; step_line++){								//Elementos das colunas depois do 'sampling period'
				for(step_col = PQfnumber(query,"sampling_period"); step_col < num_columns; step_col++){
					printf("%s",PQgetvalue(query,step_line,step_col));
					res_space = max_size_col[step_col] - strlen(PQgetvalue(query,step_line,step_col));
					for(i = 0; i < abs(res_space); i++){
						printf(" ");
					}
				}
				printf("\n");
			}			
		}			
		else{
			printf("The table doesn't exist!\n");
		}
	}
	
	else{
		printf("DB query call not ok!\n");
	}
}

void HAS_print_division_info(PGconn *dbconn, char user_answer, int num_divisions) {
	int counter1 = 0, counter2 = 0;
	char query_string[SIZE_QUERY_STRING], division_name[SIZE_STRING_DIVISION];
	PGresult *query;
	
	for(counter1 = 0; counter1 < num_divisions; counter1++) {
		sprintf(query_string, "SELECT name FROM divisions WHERE id = %d", counter1);
		query = PQexec(dbconn, query_string);
	
		if(PQresultStatus(query) != PGRES_TUPLES_OK) {
			printf("Query failed.\n");
			PQclear(query);
			PQfinish(dbconn);
			exit(-1);
		}
		
		query_string[0] = '\0'; 
		
		for(counter2 = 0; counter2 < PQntuples(query); counter2++) {
			strcpy(division_name, PQgetvalue(query, counter2, 0));
			printf("\n------ %s ------\n", division_name);
			
			switch(user_answer) {
				case '2':
					HAS_print_table_restrictions(dbconn, TABNAME_RULES, counter1);
					break;
				case '3':
					HAS_print_table_restrictions(dbconn, TABNAME_SENS, counter1);
					break;
				case '4':
					HAS_print_table_restrictions(dbconn, TABNAME_ACT, counter1);
					break;
				case '5':
					HAS_print_table_restrictions(dbconn, TABNAME_SENS, counter1);
					HAS_print_table_restrictions(dbconn, TABNAME_ACT, counter1);
					break;
				default:
					printf("No info needed.\n");
					break;
			}
		}
	}
	PQclear(query);	
}

void HAS_print_users_activity_table(PGconn *dbconn, char *user_name){
	
	int i = 0, num_columns = 0, num_lines = 0, step_line = 0, step_col = 0;
	int size_name_col = 0, size_elem = 0, size_elem_max = 0, res_space = 0;
	char str_query[256];
	
	PGresult *query;
	printf("-> users_activity - %s\n",user_name);
	sprintf(str_query,"SELECT users_activity.id, users_activity.activity_description, users_activity.time, users_activity.day "
						"FROM users, users_activity WHERE users.name = '%s' AND users_activity.id_users = users.id",user_name);	
	query = PQexec(dbconn, str_query);
	
	num_lines = PQntuples(query);
	num_columns = PQnfields(query);
	int max_size_col[num_columns];		
					
	if (PQresultStatus(query) == PGRES_TUPLES_OK){
		if(num_columns != 0){								
			for (step_col = 0; step_col < num_columns; step_col++){
				size_name_col = strlen(PQfname(query,step_col));
				if(size_name_col > size_elem_max){
					size_elem_max = size_name_col;
				}
				else size_elem_max += 0;
				
				for(step_line = 0; step_line < num_lines; step_line++){
					size_elem = strlen(PQgetvalue(query,step_line,step_col));
					if(size_elem > size_elem_max)
						size_elem_max = size_elem;
					else size_elem_max += 0;
				}
				max_size_col[step_col] = size_elem_max + NUM_SPACES;
				size_elem_max = 0;
			}
				
			for(step_col = 0; step_col < num_columns; step_col++){		//Nome das colunas
				res_space = max_size_col[step_col] - strlen(PQfname(query,step_col));
				printf("%s",PQfname(query,step_col));
				for(i = 0; i < abs(res_space); i++){
					printf(" ");
				}
			}
			printf("\n");
			
			for(step_line = 0; step_line < num_lines; step_line++){		//Elementos
				for(step_col = 0; step_col < num_columns; step_col++){
					printf("%s",PQgetvalue(query,step_line,step_col));
					res_space = max_size_col[step_col] - strlen(PQgetvalue(query,step_line,step_col));
					for(i = 0; i < abs(res_space); i++){
						printf(" ");
					}
				}
				printf("\n");
			}
		}			
		else{
			printf("The table doesn't exist!\n");
		}
	}
	
	else{
		printf("DB query call not ok!\n");
	}
	printf("\n");	
}

/*************************
   SQL DIVISIONS QUERIES 
 *************************/
PGresult* HAS_query_getNumberDivisions(PGconn *dbconn, int id_apartment, int * num_divisions, int * error_check){
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

PGresult* HAS_query_getNameDivision(PGconn *dbconn, int id_division, char * name_division, int * error_check){
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

int HAS_query_getMaximumNumberSensorsDivisionsInAnApartment(PGconn *dbconn, int id_apartment, int * id_division_max_sensors, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getMaximumNumberSensorsDivisionsInAnApartment ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return -1;
	}
	else if(0 > id_apartment){
		printf("[HAS_query_getMaximumNumberSensorsDivisionsInAnApartment ERROR] Id of the apartment invalid.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return -1;
	}
	
	int num_max_sensors = 0, counter, n_div;
	char str[100];
	PGresult *query_divisions;
	sprintf(str, "SELECT id, num_sensors  FROM divisions WHERE id_apartment = %d", id_apartment);
	query_divisions = PQexec(dbconn, str);
	if(PQresultStatus(query_divisions) == PGRES_TUPLES_OK){
		(*id_division_max_sensors) = -1;
		n_div = PQntuples(query_divisions);
		for(counter = 0; counter < n_div; counter++){
			if(num_max_sensors < atoi(PQgetvalue(query_divisions, counter, 1))){
				(*id_division_max_sensors) = atoi(PQgetvalue(query_divisions, counter, 0));
				num_max_sensors = atoi(PQgetvalue(query_divisions, counter, 1));
			}
		}
		if(NULL != error_check)
			(*error_check) = 0;
		PQclear(query_divisions);
		return num_max_sensors;
	}
	else{
		printf("[HAS_query_getMaximumNumberSensorsDivisionsInAnApartment Error Message] %s\n", PQresultErrorMessage(query_divisions));
		if(NULL != id_division_max_sensors)
			(*id_division_max_sensors) = -1;
		if(NULL != error_check)
			(*error_check) = 3;
		return -1;
	}
}

int HAS_query_getMaximumNumberActuatorsDivisionsInAnApartment(PGconn *dbconn, int id_apartment, int * id_division_max_actuators, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getMaximumNumberSensorsDivisionsInAnApartment ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return -1;
	}
	else if(0 > id_apartment){
		printf("[HAS_query_getMaximumNumberSensorsDivisionsInAnApartment ERROR] Id of the apartment invalid.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return -1;
	}
	
	int num_max_actuators = 0, counter, n_div;
	char str[100];
	PGresult *query_divisions;
	sprintf(str, "SELECT id, num_actuators  FROM divisions WHERE id_apartment = %d", id_apartment);
	query_divisions = PQexec(dbconn, str);
	if(PQresultStatus(query_divisions) == PGRES_TUPLES_OK){
		(*id_division_max_actuators) = -1;
		n_div = PQntuples(query_divisions);
		for(counter = 0; counter < n_div; counter++){
			if(num_max_actuators < atoi(PQgetvalue(query_divisions, counter, 1))){
				(*id_division_max_actuators) = atoi(PQgetvalue(query_divisions, counter, 0));
				num_max_actuators = atoi(PQgetvalue(query_divisions, counter, 1));
			}
		}
		if(NULL != error_check)
			(*error_check) = 0;
		PQclear(query_divisions);
		return num_max_actuators;
	}
	else{
		printf("[HAS_query_getMaximumNumberSensorsDivisionsInAnApartment Error Message] %s\n", PQresultErrorMessage(query_divisions));
		if(NULL != id_division_max_actuators)
			(*id_division_max_actuators) = -1;
		if(NULL != error_check)
			(*error_check) = 3;
		return -1;
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

void HAS_query_insert_division_info(PGconn *dbconn, char *str, int id_division, int * error_check) {
	//Argument check
	if(NULL == str) {
		printf("[ERROR MESSAGE %d] Divisions configuration string empty.\n", ERROR_CHECK_1);
		if(NULL != error_check)
			(*error_check) = ERROR_CHECK_1;
	}
	
	//Query code
	int counter1 = 0, counter2 = 0, counter3 = 0, count_sensors = 0, count_actuators = 0;
	int id_sensor = 0, id_actuator = 0, number_sensors = 1, number_actuators = 1;
	char str_division[SIZE_STRING_DIVISION], str_sensors_aux[SIZE_STRING_SENSOR], str_actuators_aux[SIZE_STRING_ACTUATOR], **str_sensors, **str_actuators;
	char query_string[SIZE_QUERY_STRING];
	PGresult *division_query;
	
	number_maximum_actuators = 0;
	number_maximum_sensors = 0;
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
			number_sensors++;
		}
	}
	
	if(number_sensors > number_maximum_sensors)
		number_maximum_sensors = number_sensors;
	
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
			number_actuators++;
	}
	
	if(number_actuators > number_maximum_actuators)
		number_maximum_actuators = number_actuators;
		
		
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
	
	sprintf(query_string, "UPDATE divisions SET num_sensors = %d, num_actuators = %d WHERE id = %d", number_sensors, number_actuators, id_division);
	division_query = PQexec(dbconn, query_string);
	PQclear(division_query);
	
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
	
	sens_matrix_id = 0;
	
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
		PQclear(division_query);
	}
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
	
	act_matrix_id = 0;
	
	for(counter1 = 0; counter1 < number_actuators; counter1++) {
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
		PQclear(division_query);
	}
}

void HAS_query_update_number_divisions_apartament(PGconn *dbconn, int num_divisions) {
	PGresult *query;
	char query_string[SIZE_QUERY_STRING];
	
	sprintf(query_string, "UPDATE apartment SET num_divisions = %d WHERE id=0", num_divisions);
	query = PQexec(dbconn, query_string);
	PQclear(query);
}

/*********************
   SQL RULES QUERIES 
 *********************/
PGresult* HAS_query_updateNumRules(PGconn *dbconn, int id_division, int num_rules, int * error_check){
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

void HAS_query_clearRulesTable(PGconn *dbconn){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_clearRulesTable ERROR] Connection to the database is bad.\n");
		return;
	}
	
	PGresult *query;
	query = PQexec(dbconn, "TRUNCATE rules CASCADE");
	PQclear(query);
}

void HAS_query_insertRule(PGconn *dbconn, int id_rule, char * nam_sens_cond_1, char op_cond_1, int thres_cond_1, char * nam_sens_cond_2, char op_cond_2, int thres_cond_2, int samp_per, char * schedule, int id_division, int num_act_fut_stat, char * logic_op_1_2){
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

PGresult* HAS_query_getRules(PGconn *dbconn, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getRules ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	
	PGresult *query = PQexec(dbconn, 
		"SELECT \
		id, \
		name_sensor_condition1, operator_condition1, threshold_condition1, \
		name_sensor_condition2, operator_condition2, threshold_condition2, \
		logic_operator, \
		num_actuators_future_state, \
		id_divisions, \
		sampling_period, \
		schedule, \
		time, \
		day \
		FROM rules");
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		printf("[HAS_query_getRules Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
}


/************************
   SQL ACTUATOR QUERIES 
 ************************/
void HAS_query_update_stateActuator(PGconn *dbconn, int id, char *act_name, char *act_state){
	if(PQstatus(dbconn) == CONNECTION_BAD){
		printf("ERROR_UPDATE_STATE_ACTUATOR %d: Connection failed!\n",ERROR_UPDATE_STATE_ACTUATOR_1);
		return;
	}
	else if(id < 0){
		printf("ERROR_UPDATE_STATE_ACTUATOR %d: Id divisions can't be less than zero!\n",ERROR_UPDATE_STATE_ACTUATOR_2);
		return;
	}
	else if(act_state == NULL){
		printf("ERROR_UPDATE_STATE_ACTUATOR %d: Actuator's state is invalid!\n",ERROR_UPDATE_STATE_ACTUATOR_3);
		return;
	}
	else if(act_name == NULL){
		printf("ERROR_UPDATE_STATE_ACTUATOR %d: Actuator's name is invalid!\n",ERROR_UPDATE_STATE_ACTUATOR_4);
		return;		
	}
	char str_query[256];
	sprintf(str_query,"UPDATE actuator SET actuator_state='%s' "
						"WHERE id_divisions=%d AND name='%s'",act_state,id,act_name);
	PGresult *query = PQexec(dbconn,str_query);
	if(PQresultStatus(query) != PGRES_COMMAND_OK){
		printf("ERROR_UPDATE_STATE_ACTUATOR %d: The update query of the actuator's state is incorrect!\n",ERROR_UPDATE_STATE_ACTUATOR_5);
		PQclear(query);
		return;
	}
	// else printf("The state of the sensor named '%s' with the id %d was successfully updated to the state: '%s'\n",act_name,id,act_state);
	PQclear(query);
	
	sprintf(str_query,"UPDATE actuator SET time=(CURRENT_TIME(0) AT TIME ZONE 'GMT-1'),day=CURRENT_DATE "
						"WHERE id_divisions=%d AND name='%s'",id,act_name);
	query = PQexec(dbconn,str_query);
	if(PQresultStatus(query) != PGRES_COMMAND_OK){
		printf("ERROR_UPDATE_DATA_SENSOR %d: Query incorrect!\n",ERROR_UPDATE_STATE_ACTUATOR_6);
		PQclear(query);
		return;
	}
	// else printf("Date and time were successfully updated to current time and day!\n");
	PQclear(query);
	return;
}

PGresult* HAS_query_getActuatorsState(PGconn *dbconn, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getActuatorsState ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	
	PGresult *query = PQexec(dbconn, "SELECT id, actuator_state, id_divisions, act_matrix_id FROM actuator");
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		printf("[HAS_query_getActuatorsState Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
}

PGresult* HAS_query_getActuatorsStateWithTheirName(PGconn *dbconn, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getActuatorsStateWithTheirName ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	
	PGresult *query = PQexec(dbconn, "SELECT id, name, actuator_state, id_divisions, act_matrix_id FROM actuator");
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		printf("[HAS_query_getActuatorsStateWithTheirName Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
}

PGresult* HAS_query_getActuatorsStateDivision(PGconn *dbconn, int id_division, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getActuatorsStateDivision ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	else if(0 > id_division){
		printf("[HAS_query_getActuatorsStateDivision ERROR] Division id invalid.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
	
	char str[150];
	PGresult *query;
	sprintf(str, "SELECT id, name, actuator_state, act_matrix_id FROM actuator WHERE id_divisions = %d", id_division);
	query = PQexec(dbconn, str);
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		printf("[HAS_query_getActuatorsStateDivision Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
}


/***********************
   SQL SENSORS QUERIES 
 ***********************/
void HAS_query_update_dataSensor(PGconn *dbconn, int mote_id, char *type_sens, float value){
	if(PQstatus(dbconn) == CONNECTION_BAD){
		printf("ERROR_UPDATE_DATA_SENSOR %d: Connection failed!\n",ERROR_UPDATE_DATA_SENSOR_1);
		return;
	}
	else if(mote_id < 0){
		printf("ERROR_UPDATE_DATA_SENSOR %d: Mote id can't be less than zero!\n",ERROR_UPDATE_DATA_SENSOR_2);
		return;
	}
	else if(type_sens == NULL){
		printf("ERROR_UPDATE_DATA_SENSOR %d: Sensor's type is invalid!\n",ERROR_UPDATE_DATA_SENSOR_3);
		return;
	}
	char str_query[256];
	sprintf(str_query,"UPDATE sensor SET data=%d "
						"WHERE mote_id=%d AND sensor_type='%s'",(int)value,mote_id,type_sens);
	PGresult *query = PQexec(dbconn,str_query);
	if(PQresultStatus(query) != PGRES_COMMAND_OK){
		printf("ERROR_UPDATE_DATA_SENSOR %d: The update query of sensor's data is incorrect!\n",ERROR_UPDATE_DATA_SENSOR_4);
		PQclear(query);
		return;
	}
	//else printf("The data of the sensor named '%s' from the mote %d was successfully updated to the value: %d\n",type_sens,mote_id,(int)value);
	PQclear(query);
	
	sprintf(str_query,"UPDATE sensor SET time=(CURRENT_TIME(0) AT TIME ZONE 'GMT-1'),day=CURRENT_DATE "
						"WHERE mote_id=%d AND sensor_type='%s'",mote_id,type_sens);
	query = PQexec(dbconn,str_query);
	if(PQresultStatus(query) != PGRES_COMMAND_OK){
		printf("ERROR_UPDATE_DATA_SENSOR %d: The update query of sensor's day and time is incorrect!\n",ERROR_UPDATE_DATA_SENSOR_5);
		PQclear(query);
		return;
	}
	//else printf("Date and time were successfully updated to current time and day!\n");
	PQclear(query);
	return;
}

PGresult* HAS_query_getSensorsData(PGconn *dbconn, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getSensorsData ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	
	PGresult *query = PQexec(dbconn, "SELECT id, mote_id, sensor_type, data, id_divisions, sens_matrix_id FROM sensor");
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		printf("[HAS_query_getSensorsData Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
}

int HAS_query_search_sensor(PGconn *dbconn, char *name, int mote){
	PGresult *query;
	char str_query[256];
	if(PQstatus(dbconn) != CONNECTION_OK){
		printf("ERROR_CHECK_SENSOR %d: The connection to the database is offline!\n",ERROR_CHECK_SENSOR_1);
		return ERROR_CHECK_SENSOR_ARG_INV;
	}
	else if(name == NULL){
		printf("ERROR_CHECK_SENSOR %d: The sensor's name is invalid\n",ERROR_CHECK_SENSOR_2);
		return ERROR_CHECK_SENSOR_ARG_INV;
	}
	else if(mote < 0){
		printf("ERROR_CHECK_SENSOR %d: The mote id must be higher than zero!\n",ERROR_CHECK_SENSOR_3);
		return ERROR_CHECK_SENSOR_ARG_INV;
	}
	sprintf(str_query,"SELECT * FROM sensor WHERE sensor_type='%s' AND mote_id=%d",name,mote);
	query = PQexec(dbconn,str_query);
	if(PQntuples(query) != SENSOR_NOT_PRESENT){
		//printf("Sensor %s from mote %d can be updated!\n",name,mote);
		return SENSOR_PRESENT;
	}
	else{
		//printf("Sensor %s from mote %d can't be updated! This sensor doesn't exist!\n",name,mote);
		return SENSOR_NOT_PRESENT;
	}
}

int HAS_query_getSpecificSensorData(PGconn *dbconn, int id_division, int id_mote, int sensor_type, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getSpecificSensorData ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return -1;
	}
	else if(0 > id_division){
		printf("[HAS_query_getSpecificSensorData ERROR] Division id invalid.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return -1;
	}
	else if(id_mote < 0){
		printf("[HAS_query_getSpecificSensorData ERROR] Mote id can't be less than zero!\n");
		if(NULL != error_check)
			(*error_check) = 3;
		return -1;
	}
	else if(sensor_type != TYPE_SENS_HUM && sensor_type != TYPE_SENS_LIGHT && sensor_type != TYPE_SENS_TEMP){
		printf("[HAS_query_getSpecificSensorData ERROR] Sensor's type is invalid!\n");
		if(NULL != error_check)
			(*error_check) = 3;
		return -1;
	}
	
	char str[120];
	PGresult *query;
	switch(sensor_type){
	case TYPE_SENS_TEMP: 
		sprintf(str, "SELECT data FROM sensor WHERE mote_id = %d AND sensor_type = 'TEMP' AND id_divisions = %d", id_mote, id_division);
		break;
	case TYPE_SENS_HUM: 
		sprintf(str, "SELECT data FROM sensor WHERE mote_id = %d AND sensor_type = 'HUM' AND id_divisions = %d", id_mote, id_division);
		break;
	case TYPE_SENS_LIGHT: 
		sprintf(str, "SELECT data FROM sensor WHERE mote_id = %d AND sensor_type = 'LIGHT' AND id_divisions = %d", id_mote, id_division);
		break;
	default:
		printf("[HAS_query_getSpecificSensorData ERROR] Sensor's type is invalid!\n");
		if(NULL != error_check)
			(*error_check) = 3;
		return -1;
	}
	query = PQexec(dbconn, str);
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		if(NULL != error_check)
			(*error_check) = 0;
		return atoi(PQgetvalue(query, 0, 0));
	}
	else{
		printf("[HAS_query_getSpecificSensorData Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 4;
		return -1;
	}
}


/*************************************
   SQL ACTUATOR FUTURE STATE QUERIES 
 *************************************/
void HAS_query_insertActuatorFutureState(PGconn *dbconn, int id_act_fut_stat, char * fut_stat, int id_actuator, int id_rule){
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
	PQclear(query);
}

PGresult* HAS_query_getActuatorIdToActuatorFutureState(PGconn *dbconn, int id_division, char * actuator_name, int * id_actuator, int * error_check){
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

PGresult* HAS_query_getActuatorsFutureStateFromRule(PGconn *dbconn, int id_rule, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getActuatorsFutureStateFromRule ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	else if(0 > id_rule){
		printf("[HAS_query_getActuatorIdToActuatorFutureState ERROR] Rule id invalid.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
	
	char str[120];
	PGresult *query;
	sprintf(str, "SELECT id, future_state, id_actuator FROM actuator_future_state WHERE id_rules = %d", id_rule);
	query = PQexec(dbconn, str);
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		if(NULL != error_check)
			(*error_check) = 0;
		return query;
	}
	else{
		printf("[HAS_query_getActuatorsFutureStateFromRule Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 2;
		return NULL;
	}
}


/*********************
   SQL USERS QUERIES 
 *********************/
PGresult* HAS_query_getUserInfo(PGconn *dbconn, int id_user, int * error_check){
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

void HAS_query_showActiveUsersInfo(PGconn *dbconn){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_showActiveUsersInfo ERROR] Connection to the database is bad.\n");
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
			printf("[HAS_query_showActiveUsersInfo ERROR] There isn't any information about any user in the database.\n");
		}
	}
	else{
		printf("[HAS_query_showActiveUsersInfo ERROR] Query call ins't ok.\n");
		printf("Result Query: %d\n", PQresultStatus(query));
		printf("Error Message: %s\n", PQresultErrorMessage(query));
	}
	printf("\n");
}

void HAS_query_insertUser(PGconn *dbconn, int id, char * name, char * permission, char * password, int state, int id_apartment, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_insertUser ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return;
	}
	else if(0 > id){
		printf("[HAS_query_insertUser ERROR] User_id invalid (it needs to be equal or higher than zero).\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return;
	}
	else if(NULL == name){
		printf("[HAS_query_insertUser ERROR] Invalid string: name.\n");
		if(NULL != error_check)
			(*error_check) = 3;
		return;
	}
	else if(NULL == permission){
		printf("[HAS_query_insertUser ERROR] Invalid string: permission.\n");
		if(NULL != error_check)
			(*error_check) = 4;
		return;
	}
	else if(NULL == password){
		printf("[HAS_query_insertUser ERROR] Invalid string: password.\n");
		if(NULL != error_check)
			(*error_check) = 5;
		return;
	}
	else if(0 != state && 1 != state){
		printf("[HAS_query_insertUser ERROR] state invalid (it needs to be 0 - ACTIVE - or 1 - INACTIVE).\n");
		if(NULL != error_check)
			(*error_check) = 6;
		return;
	}
	else if(0 > id_apartment){
		printf("[HAS_query_insertUser ERROR] id_apartment invalid (it needs to be equal or higher than zero).\n");
		if(NULL != error_check)
			(*error_check) = 7;
		return;
	}
	
	char str[300];
	PGresult *query;
	sprintf(str, "INSERT INTO users(id, name, permission, password, state, id_apartment) VALUES (%d, '%s', '%s', '%s', %d, %d)", id, name, permission, password, state, id_apartment);
	query = PQexec(dbconn, str);
	if(PQresultStatus(query) == PGRES_COMMAND_OK){
		if(NULL != error_check)
			(*error_check) = 0;
		return;
	}
	else{
		printf("[HAS_query_insertUser Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 8;
		return;
	}
}

void HAS_query_user_activity_insert(PGconn *dbconn, int option, int *error_check) {
	PGresult *query;
	char query_string[SIZE_QUERY_STRING], user_name[SIZE_MAX_USERNAME];
	int present_id_user_activity = 0;

	sprintf(query_string, "SELECT id FROM users_activity");
	query = PQexec(dbconn, query_string);
	present_id_user_activity = PQntuples(query);
	PQclear(query);
	query = HAS_query_getUserInfo(dbconn, user_id, NULL);
	strcpy(user_name, PQgetvalue(query, 0, 0)); 
	PQclear(query);
	
	switch(option) {
		case OPTION_LOGIN: //Log in de um utilizador
			sprintf(query_string, "INSERT INTO users_activity (id, id_users, activity_description, time, day) VALUES (%d, %d, '%s - Log in', CURRENT_TIME(0) AT TIME ZONE 'GMT-1', CURRENT_DATE)", present_id_user_activity, user_id, user_name);	
			query = PQexec(dbconn, query_string);
			
			if(PQresultStatus(query) != PGRES_COMMAND_OK) {
				printf("[ERROR MESSAGE %d]: Failed executing query.\n", ERROR_CHECK_6);
				printf("ERROR MESSAGE: %s\n", PQresultErrorMessage(query)); 
				if(NULL != error_check)
					(*error_check) = ERROR_CHECK_6;
				PQfinish(dbconn);
				dbconn = NULL;
				query = NULL;
				exit(-1);
			}
			
			present_id_user_activity++;
			
			break;
		case OPTION_LOGOUT: //Log out de um utilizador
			sprintf(query_string, "INSERT INTO users_activity (id, id_users, activity_description, time, day) VALUES (%d, %d, '%s - Log out', CURRENT_TIME(0) AT TIME ZONE 'GMT-1', CURRENT_DATE)", present_id_user_activity, user_id, user_name);
			query = PQexec(dbconn, query_string);
			
			if(PQresultStatus(query) != PGRES_COMMAND_OK) {
				printf("[ERROR MESSAGE %d]: Failed executing query.\n", ERROR_CHECK_6);			
				printf("ERROR MESSAGE: %s\n", PQresultErrorMessage(query)); 
				if(NULL != error_check)
					(*error_check) = ERROR_CHECK_6;
				PQfinish(dbconn);
				dbconn = NULL;
				query = NULL;
				exit(-1);
			}
			present_id_user_activity++;
			
			break;
		case OPTION_DIVI_CONF: //Alteração da configuração das divisões
			sprintf(query_string, "INSERT INTO users_activity (id, id_users, activity_description, time, day) VALUES (%d, %d, '%s - New division configuration', CURRENT_TIME(0) AT TIME ZONE 'GMT-1', CURRENT_DATE)", present_id_user_activity, user_id, user_name);
			query = PQexec(dbconn, query_string);
			
			if(PQresultStatus(query) != PGRES_COMMAND_OK) {
				printf("[ERROR MESSAGE %d]: Failed executing query.\n", ERROR_CHECK_6);
				printf("ERROR MESSAGE: %s\n", PQresultErrorMessage(query)); 
				if(NULL != error_check)
					(*error_check) = ERROR_CHECK_6;
				PQfinish(dbconn);
				dbconn = NULL;
				query = NULL;
				exit(-1);
			}
			
			present_id_user_activity++;
			
			break;
		case OPTION_RULES_ALT: //Regras alteradas
			sprintf(query_string, "INSERT INTO users_activity (id, id_users, activity_description, time, day) VALUES (%d, %d, '%s - Divisions rules changed', CURRENT_TIME(0) AT TIME ZONE 'GMT-1', CURRENT_DATE)", present_id_user_activity, user_id, user_name);
			query = PQexec(dbconn, query_string);
			
			if(PQresultStatus(query) != PGRES_COMMAND_OK) {
				printf("[ERROR MESSAGE %d]: Failed executing query.\n", ERROR_CHECK_6);
				printf("ERROR MESSAGE: %s\n", PQresultErrorMessage(query)); 
				if(NULL != error_check)
					(*error_check) = ERROR_CHECK_6;
				PQfinish(dbconn);
				dbconn = NULL;
				query = NULL;
				exit(-1);
			}
			
			present_id_user_activity++;
			
			break;
		case OPTION_INFO: //Informações da user interface
			sprintf(query_string, "INSERT INTO users_activity (id, id_users, activity_description, time, day) VALUES (%d, %d, '%s - General information', CURRENT_TIME(0) AT TIME ZONE 'GMT-1', CURRENT_DATE)", present_id_user_activity, user_id, user_name);
			query = PQexec(dbconn, query_string);
			
			if(PQresultStatus(query) != PGRES_COMMAND_OK) {
				printf("[ERROR MESSAGE %d]: Failed executing query.\n", ERROR_CHECK_6);
				printf("ERROR MESSAGE: %s\n", PQresultErrorMessage(query)); 
				if(NULL != error_check)
					(*error_check) = ERROR_CHECK_6;
				PQfinish(dbconn);
				dbconn = NULL;
				query = NULL;
				exit(-1);
			}
			
			present_id_user_activity++;
			
			break;
		case OPTION_DEL_TABLE_UA: //Informações da user interface
			sprintf(query_string, "INSERT INTO users_activity (id, id_users, activity_description, time, day) VALUES (%d, %d, '%s - Delete users activity', CURRENT_TIME(0) AT TIME ZONE 'GMT-1', CURRENT_DATE)", present_id_user_activity, user_id, user_name);
			query = PQexec(dbconn, query_string);
			
			if(PQresultStatus(query) != PGRES_COMMAND_OK) {
				printf("[ERROR MESSAGE %d]: Failed executing query.\n", ERROR_CHECK_6);
				printf("ERROR MESSAGE: %s\n", PQresultErrorMessage(query)); 
				if(NULL != error_check)
					(*error_check) = ERROR_CHECK_6;
				PQfinish(dbconn);
				dbconn = NULL;
				query = NULL;
				exit(-1);
			}
			
			present_id_user_activity++;
			
			break;
		case OPTION_DEL_TABLE_SH: //Informações da user interface
			sprintf(query_string, "INSERT INTO users_activity (id, id_users, activity_description, time, day) VALUES (%d, %d, '%s - Delete sensor history', CURRENT_TIME(0) AT TIME ZONE 'GMT-1', CURRENT_DATE)", present_id_user_activity, user_id, user_name);
			query = PQexec(dbconn, query_string);
			
			if(PQresultStatus(query) != PGRES_COMMAND_OK) {
				printf("[ERROR MESSAGE %d]: Failed executing query.\n", ERROR_CHECK_6);
				printf("ERROR MESSAGE: %s\n", PQresultErrorMessage(query)); 
				if(NULL != error_check)
					(*error_check) = ERROR_CHECK_6;
				PQfinish(dbconn);
				dbconn = NULL;
				query = NULL;
				exit(-1);
			}
			
			present_id_user_activity++;
			
			break;
		case OPTION_DEL_TABLE_AH: //Informações da user interface
			sprintf(query_string, "INSERT INTO users_activity (id, id_users, activity_description, time, day) VALUES (%d, %d, '%s - Delete actuator history', CURRENT_TIME(0) AT TIME ZONE 'GMT-1', CURRENT_DATE)", present_id_user_activity, user_id, user_name);
			query = PQexec(dbconn, query_string);
			
			if(PQresultStatus(query) != PGRES_COMMAND_OK) {
				printf("[ERROR MESSAGE %d]: Failed executing query.\n", ERROR_CHECK_6);
				printf("ERROR MESSAGE: %s\n", PQresultErrorMessage(query)); 
				if(NULL != error_check)
					(*error_check) = ERROR_CHECK_6;
				PQfinish(dbconn);
				dbconn = NULL;
				query = NULL;
				exit(-1);
			}
			
			present_id_user_activity++;
			
			break;
	}
	PQclear(query);
}

PGresult* HAS_query_getAllUserInfo(PGconn *dbconn, int * error_check){
	if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_getUserInfo ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return NULL;
	}
	
	PGresult *query = PQexec(dbconn, "SELECT id, name, permission, password, state, id_apartment FROM users");
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
	PGconn *dbconn_2;
	
	dbconn_2 = PQconnectdb(conn);
	
	if (PQstatus(dbconn_2) == CONNECTION_BAD){
		printf("[dbconn_2] Unable to connect\n");
		pthread_exit(NULL);
	}
	
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
					//system_motes[search_mote(system_motes,number_motes,decimal_id,NULL)]->temperature = temp;
					if(HAS_query_search_sensor(dbconn_2,"TEMP",decimal_id) == SENSOR_PRESENT){
						HAS_query_update_dataSensor(dbconn_2,decimal_id,"TEMP",temp);
						HAS_query_insertNewEntrySensorHistory(dbconn_2, NULL, "TEMP", decimal_id, (int)temp);
					}
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
					//system_motes[search_mote(system_motes,number_motes,decimal_id,NULL)]->humidity = humidity_compensated_by_temperature;
					if(HAS_query_search_sensor(dbconn_2,"HUM",decimal_id) == SENSOR_PRESENT){
						HAS_query_update_dataSensor(dbconn_2,decimal_id,"HUM",humidity_compensated_by_temperature);
						HAS_query_insertNewEntrySensorHistory(dbconn_2, NULL, "HUM", decimal_id, (int)humidity_compensated_by_temperature);
					}		
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
					//system_motes[search_mote(system_motes,number_motes,decimal_id,NULL)]->luminosity = visible_light;
					if(HAS_query_search_sensor(dbconn_2,"LIGHT",decimal_id) == SENSOR_PRESENT){
						HAS_query_update_dataSensor(dbconn_2,decimal_id,"LIGHT",visible_light);
						HAS_query_insertNewEntrySensorHistory(dbconn_2, NULL, "LIGHT", decimal_id, (int)visible_light);
					}
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

void atualize_RGBstring_sensors_state_RGBMatrix(PGconn *dbconn, char * buffer_rgb_pssRGB, int matrix_side){
	int counter_rows, numb_rows, error_check;
	PGresult *query;
	query = HAS_query_getSensorsData(dbconn, &error_check);
	if(NULL != query)
		numb_rows = PQntuples(query);
	else
		numb_rows = 0;
	if(NULL != query && 0 < numb_rows){
		for(counter_rows = 0; counter_rows < numb_rows; counter_rows++){
			switch(PQgetvalue(query, counter_rows, 2)[0]){
				case 'L':
				buffer_rgb_pssRGB = configuration_matrix_sensors(
					buffer_rgb_pssRGB, 
					matrix_side, 
					atoi(PQgetvalue(query, counter_rows, 4)) + 1, 
					atoi(PQgetvalue(query, counter_rows, 5)) + 1, 
					atoi(PQgetvalue(query, counter_rows, 3)), 
					TYPE_SENS_LIGHT
					);
				break;
				case 'H':
				buffer_rgb_pssRGB = configuration_matrix_sensors(
					buffer_rgb_pssRGB, 
					matrix_side, 
					atoi(PQgetvalue(query, counter_rows, 4)) + 1, 
					atoi(PQgetvalue(query, counter_rows, 5)) + 1, 
					atoi(PQgetvalue(query, counter_rows, 3)), 
					TYPE_SENS_HUM
					);
				break;
				case 'T':
				buffer_rgb_pssRGB = configuration_matrix_sensors(
					buffer_rgb_pssRGB, 
					matrix_side, 
					atoi(PQgetvalue(query, counter_rows, 4)) + 1, 
					atoi(PQgetvalue(query, counter_rows, 5)) + 1, 
					atoi(PQgetvalue(query, counter_rows, 3)), 
					TYPE_SENS_TEMP
					);
				break;
				default:
				printf("[atualize_RGBstring_sensors_state_RGBMatrix Error Message] Sensor's (%d) type (%s) of the division %d not founded\n", atoi(PQgetvalue(query, counter_rows, 0)), PQgetvalue(query, counter_rows, 2), atoi(PQgetvalue(query, counter_rows, 4)));
			}
			
		}
		PQclear(query);
	}
	else
		printf("[atualize_RGBstring_sensors_state_RGBMatrix Error Message] There are none sensors to pass their state to the RGBMatrix\n");
}

void atualize_RGBstring_actuators_state_RGBMatrix(PGconn *dbconn, char * buffer_rgb_pssRGB, int matrix_side){
	int counter_rows, numb_rows, error_check;
	PGresult *query;
	query = HAS_query_getActuatorsState(dbconn, &error_check);
	if(NULL != query)
		numb_rows = PQntuples(query);
	else
		numb_rows = 0;
	if(NULL != query && 0 < numb_rows){
		for(counter_rows = 0; counter_rows < numb_rows; counter_rows++){
			buffer_rgb_pssRGB = configuration_matrix_actuators(
				buffer_rgb_pssRGB, 
				matrix_side, 
				atoi(PQgetvalue(query, counter_rows, 2)) + 1, 
				atoi(PQgetvalue(query, counter_rows, 3)) + 1, 
				PQgetvalue(query, counter_rows, 1)
				);
		}
		PQclear(query);
	}
	else
		printf("[atualize_RGBstring_actuators_state_RGBMatrix Error Message] There are none actuators to pass their state to the RGBMatrix\n");
}


/********************************************
   THREAD RULE IMPLEMENTATION (PTH2) - CODE
 ********************************************/
void *thread_rule_implementation(void *arg){
	// Variables Declaration
	int rule_index, number_rules_thread, error_check;
	int mote_id_sensor_1, type_sensor_1, mote_id_sensor_2, type_sensor_2;
	int logic_value_condition_1 = 0, logic_value_condition_2 = 0;
	int value_sensor_1, value_sensor_2;
	
	int act_index, number_act;
	int actFutStat_index, number_actFutStat;
	int error_check_aux = 0;
	
	PGresult *query_imp_rules;
	PGresult *query_getActualStateDivision;
	PGresult *query_getFutureStateFromRule;
	
	dbconn_3 = PQconnectdb(conn);
	
	if (PQstatus(dbconn_3) == CONNECTION_BAD){
		printf("[dbconn_3] Unable to connect\n");
		pthread_exit(NULL);
	}
	
	strcpy(buffer_rgb_channel_aux, buffer_rgb_channel);
	
	while(1 == flag_interface){
		// Gets the rules information
		query_imp_rules = HAS_query_getRules(dbconn_3, &error_check);
		if(NULL == query_imp_rules){
			printf("[thread_rule_implementation ERROR] Query HAS_query_getRules not executed\n");
			pthread_exit(NULL);
		}
		number_rules_thread = PQntuples(query_imp_rules);
		
		// Rule's implementation
		for(rule_index = 0; rule_index < number_rules_thread; rule_index++){
			
			// ONLY EXIST CONDITION 1
			if(1 == PQgetisnull(query_imp_rules, rule_index, 7)){
				// Search for the characteristics of the SENSOR 1 - MOTE_ID, SENSOR_TYPE
				search_sensor_mote(PQgetvalue(query_imp_rules, rule_index, 1), number_motes, &mote_id_sensor_1, &type_sensor_1);
				
				// Get sensor_CONDITION_1 value
				value_sensor_1 = HAS_query_getSpecificSensorData(dbconn_3, atoi(PQgetvalue(query_imp_rules, rule_index, 9)), mote_id_sensor_1, type_sensor_1, &error_check);
				
				// Implementation of the future states
				if(('>' == PQgetvalue(query_imp_rules, rule_index, 2)[0] && (value_sensor_1 > atoi(PQgetvalue(query_imp_rules, rule_index, 3)))) 
						|| ('<' == PQgetvalue(query_imp_rules, rule_index, 2)[0] && (value_sensor_1 < atoi(PQgetvalue(query_imp_rules, rule_index, 3))))){
					// Execute the future states relative to the present rule
					// .....
					//printf("[RULE %d] TRUE\n", atoi(PQgetvalue(query_imp_rules, rule_index, 0)));
/***** GET RULES					***** GET ACT STATE		***** GET ACTFUTSTATE
SELECT \							SELECT					SELECT 
0	id, \							0	id, 				0	id, 
1	name_sensor_condition1, 		1	name, 				1	future_state, 
2	operator_condition1, 			2	actuator_state, 	2	id_actuator 
3	threshold_condition1, \			3	act_matrix_id  		FROM actuator_future_state WHERE id_rules = %d
4	name_sensor_condition2, 		FROM actuator			
5	operator_condition2, 			WHERE id_divisions = %d
6	threshold_condition2, \			
7	logic_operator, \				
8	num_actuators_future_state, \	
9	id_divisions, \					
sampling_period, \					
schedule, \							
time, \								
day \								
FROM rules							
******/
					// Get present actuators state
					query_getActualStateDivision = HAS_query_getActuatorsStateDivision(
						dbconn_3, 
						atoi(PQgetvalue(query_imp_rules, rule_index, 9)), 
						&error_check
						);
					if(NULL == query_getActualStateDivision){
						printf("[thread_rule_implementation ERROR] Query HAS_query_getActuatorsStateDivision not executed\n");
						pthread_exit(NULL);
					}
					
					// Get future states relative to a single rule (id = rule_index)
					query_getFutureStateFromRule = HAS_query_getActuatorsFutureStateFromRule(
						dbconn_3, 
						atoi(PQgetvalue(query_imp_rules, rule_index, 0)), 
						&error_check
						);
					if(NULL == query_getFutureStateFromRule){
						printf("[thread_rule_implementation ERROR] Query HAS_query_getActuatorsFutureStateFromRule not executed\n");
						PQclear(query_getActualStateDivision);
						pthread_exit(NULL);
					}
					
					number_act = PQntuples(query_getActualStateDivision);
					number_actFutStat = PQntuples(query_getFutureStateFromRule);
					
					// Check present state of the actuators and atualize them if needed
					for(actFutStat_index = 0; actFutStat_index < number_actFutStat; actFutStat_index++){
						// Discovering the actuator's id
						for(act_index = 0; act_index < number_act; act_index++)
							if(atoi(PQgetvalue(query_getActualStateDivision, act_index, 0)) == 
									atoi(PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 2)))
								break;
								
						// OBS - now the act_index guards the row for the present actuator
						
						// Update the actuator's state if needed
						if(0 != strcmp(PQgetvalue(query_getActualStateDivision, act_index, 2), 
								PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 1)
								)){
							HAS_query_update_stateActuator(
								dbconn_3, 
								atoi(PQgetvalue(query_imp_rules, rule_index, 9)), 
								PQgetvalue(query_getActualStateDivision, act_index, 1), 
								PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 1)
								);
							HAS_query_updateTimeFinalActuatorHistory(
								dbconn_3, 
								atoi(PQgetvalue(query_getActualStateDivision, act_index, 0)), 
								NULL
								);
							HAS_query_insertNewEntryActuatorHistory(
								dbconn_3, 
								atoi(PQgetvalue(query_getActualStateDivision, act_index, 0)), 
								PQgetvalue(query_getActualStateDivision, act_index, 1), 
								atoi(PQgetvalue(query_imp_rules, rule_index, 9)), 
								PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 1), 
								&error_check_aux
								);
						}
					}
					
					// PQclear QUERIES per rule
					PQclear(query_getActualStateDivision);
					PQclear(query_getFutureStateFromRule);
				}
			}


			// CONDITION 1 AND CONDITION 2
			else if('A' == (PQgetvalue(query_imp_rules, rule_index, 7)[0])){
				// Search for the characteristics of the SENSOR 1 - MOTE_ID, SENSOR_TYPE
				search_sensor_mote(PQgetvalue(query_imp_rules, rule_index, 1), number_motes, &mote_id_sensor_1, &type_sensor_1);
				
				// Search for the characteristics of the SENSOR 2 - MOTE_ID, SENSOR_TYPE
				search_sensor_mote(PQgetvalue(query_imp_rules, rule_index, 4), number_motes, &mote_id_sensor_2, &type_sensor_2);
				
				// Get sensor_CONDITION_1 value
				value_sensor_1 = HAS_query_getSpecificSensorData(dbconn_3, atoi(PQgetvalue(query_imp_rules, rule_index, 9)), mote_id_sensor_1, type_sensor_1, &error_check);
				
				// Get sensor_CONDITION_2 value
				value_sensor_2 = HAS_query_getSpecificSensorData(dbconn_3, atoi(PQgetvalue(query_imp_rules, rule_index, 9)), mote_id_sensor_2, type_sensor_2, &error_check);
				
				// Discovering the compare operator in CONDITION_1
				switch(PQgetvalue(query_imp_rules, rule_index, 2)[0]){
				case '>':
					if(value_sensor_1 > atoi(PQgetvalue(query_imp_rules, rule_index, 3)))
						logic_value_condition_1 = 1;
					else
						logic_value_condition_1 = 0;
					break;
				case '<':
					if(value_sensor_1 < atoi(PQgetvalue(query_imp_rules, rule_index, 3)))
						logic_value_condition_1 = 1;
					else
						logic_value_condition_1 = 0;
					break;
				default:
					printf("[thread_rule_implementation Error] Operator in condition 1 invalid.\n");
					pthread_exit(NULL);
				}
				
				// Discovering the compare operator in CONDITION_2
				switch(PQgetvalue(query_imp_rules, rule_index, 5)[0]){
				case '>':
					if(value_sensor_2 > atoi(PQgetvalue(query_imp_rules, rule_index, 6)))
						logic_value_condition_2 = 1;
					else
						logic_value_condition_2 = 0;
					break;
				case '<':
					if(value_sensor_2 < atoi(PQgetvalue(query_imp_rules, rule_index, 6)))
						logic_value_condition_2 = 1;
					else
						logic_value_condition_2 = 0;
					break;
				default:
					printf("[thread_rule_implementation Error] Operator in condition 2 invalid.\n");
					pthread_exit(NULL);
				}
				
				// Implementation of the future states
				if(1==logic_value_condition_1 && 1==logic_value_condition_2){
					// Execute the future states relative to the present rule
					// .....
					//printf("[RULE %d] TRUE\n", atoi(PQgetvalue(query_imp_rules, rule_index, 0)));
/***** GET RULES					***** GET ACT STATE		***** GET ACTFUTSTATE
SELECT \							SELECT					SELECT 
0	id, \							0	id, 				0	id, 
1	name_sensor_condition1, 		1	name, 				1	future_state, 
2	operator_condition1, 			2	actuator_state, 	2	id_actuator 
3	threshold_condition1, \			3	act_matrix_id  		FROM actuator_future_state WHERE id_rules = %d
4	name_sensor_condition2, 		FROM actuator			
5	operator_condition2, 			WHERE id_divisions = %d
6	threshold_condition2, \			
7	logic_operator, \				
8	num_actuators_future_state, \	
9	id_divisions, \					
sampling_period, \					
schedule, \							
time, \								
day \								
FROM rules							
******/
					// Get present actuators state
					query_getActualStateDivision = HAS_query_getActuatorsStateDivision(
						dbconn_3, 
						atoi(PQgetvalue(query_imp_rules, rule_index, 9)), 
						&error_check
						);
					if(NULL == query_getActualStateDivision){
						printf("[thread_rule_implementation ERROR] Query HAS_query_getActuatorsStateDivision not executed\n");
						pthread_exit(NULL);
					}
					
					// Get future states relative to a single rule (id = rule_index)
					query_getFutureStateFromRule = HAS_query_getActuatorsFutureStateFromRule(
						dbconn_3, 
						atoi(PQgetvalue(query_imp_rules, rule_index, 0)), 
						&error_check
						);
					if(NULL == query_getFutureStateFromRule){
						printf("[thread_rule_implementation ERROR] Query HAS_query_getActuatorsFutureStateFromRule not executed\n");
						PQclear(query_getActualStateDivision);
						pthread_exit(NULL);
					}
					
					number_act = PQntuples(query_getActualStateDivision);
					number_actFutStat = PQntuples(query_getFutureStateFromRule);
					
					// Check present state of the actuators and atualize them if needed
					for(actFutStat_index = 0; actFutStat_index < number_actFutStat; actFutStat_index++){
						// Discovering the actuator's id
						for(act_index = 0; act_index < number_act; act_index++)
							if(atoi(PQgetvalue(query_getActualStateDivision, act_index, 0)) == 
									atoi(PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 2)))
								break;
								
						// OBS - now the act_index guards the row for the present actuator
						
						// Update the actuator's state if needed
						if(0 != strcmp(PQgetvalue(query_getActualStateDivision, act_index, 2), 
								PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 1)
								)){
							HAS_query_update_stateActuator(
								dbconn_3, 
								atoi(PQgetvalue(query_imp_rules, rule_index, 9)), 
								PQgetvalue(query_getActualStateDivision, act_index, 1), 
								PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 1)
								);
							HAS_query_updateTimeFinalActuatorHistory(
								dbconn_3, 
								atoi(PQgetvalue(query_getActualStateDivision, act_index, 0)), 
								NULL
								);
							HAS_query_insertNewEntryActuatorHistory(
								dbconn_3, 
								atoi(PQgetvalue(query_getActualStateDivision, act_index, 0)), 
								PQgetvalue(query_getActualStateDivision, act_index, 1), 
								atoi(PQgetvalue(query_imp_rules, rule_index, 9)), 
								PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 1), 
								&error_check_aux
								);
						}
					}
					
					// PQclear QUERIES per rule
					PQclear(query_getActualStateDivision);
					PQclear(query_getFutureStateFromRule);
				}
			}


			// CONDITION 1 OR CONDITION 2
			else if('O' == (PQgetvalue(query_imp_rules, rule_index, 7)[0])){
				// Search for the characteristics of the SENSOR 1 - MOTE_ID, SENSOR_TYPE
				search_sensor_mote(PQgetvalue(query_imp_rules, rule_index, 1), number_motes, &mote_id_sensor_1, &type_sensor_1);
				
				// Search for the characteristics of the SENSOR 2 - MOTE_ID, SENSOR_TYPE
				search_sensor_mote(PQgetvalue(query_imp_rules, rule_index, 4), number_motes, &mote_id_sensor_2, &type_sensor_2);
				
				// Get sensor_CONDITION_1 value
				value_sensor_1 = HAS_query_getSpecificSensorData(dbconn_3, atoi(PQgetvalue(query_imp_rules, rule_index, 9)), mote_id_sensor_1, type_sensor_1, &error_check);
				
				// Get sensor_CONDITION_2 value
				value_sensor_2 = HAS_query_getSpecificSensorData(dbconn_3, atoi(PQgetvalue(query_imp_rules, rule_index, 9)), mote_id_sensor_2, type_sensor_2, &error_check);
				
				// Discovering the compare operator in CONDITION_1
				switch(PQgetvalue(query_imp_rules, rule_index, 2)[0]){
				case '>':
					if(value_sensor_1 > atoi(PQgetvalue(query_imp_rules, rule_index, 3)))
						logic_value_condition_1 = 1;
					else
						logic_value_condition_1 = 0;
					break;
				case '<':
					if(value_sensor_1 < atoi(PQgetvalue(query_imp_rules, rule_index, 3)))
						logic_value_condition_1 = 1;
					else
						logic_value_condition_1 = 0;
					break;
				default:
					printf("[thread_rule_implementation Error] Operator in condition 1 invalid.\n");
					pthread_exit(NULL);
				}
				
				// Discovering the compare operator in CONDITION_2
				switch(PQgetvalue(query_imp_rules, rule_index, 5)[0]){
				case '>':
					if(value_sensor_2 > atoi(PQgetvalue(query_imp_rules, rule_index, 6)))
						logic_value_condition_2 = 1;
					else
						logic_value_condition_2 = 0;
					break;
				case '<':
					if(value_sensor_2 < atoi(PQgetvalue(query_imp_rules, rule_index, 6)))
						logic_value_condition_2 = 1;
					else
						logic_value_condition_2 = 0;
					break;
				default:
					printf("[thread_rule_implementation Error] Operator in condition 2 invalid.\n");
					pthread_exit(NULL);
				}
				
				// Implementation of the future states
				if(1==logic_value_condition_2 || 1==logic_value_condition_2){
					// Execute the future states relative to the present rule
					// .....
					//printf("[RULE %d] TRUE\n", atoi(PQgetvalue(query_imp_rules, rule_index, 0)));
/***** GET RULES					***** GET ACT STATE		***** GET ACTFUTSTATE
SELECT \							SELECT					SELECT 
0	id, \							0	id, 				0	id, 
1	name_sensor_condition1, 		1	name, 				1	future_state, 
2	operator_condition1, 			2	actuator_state, 	2	id_actuator 
3	threshold_condition1, \			3	act_matrix_id  		FROM actuator_future_state WHERE id_rules = %d
4	name_sensor_condition2, 		FROM actuator			
5	operator_condition2, 			WHERE id_divisions = %d
6	threshold_condition2, \			
7	logic_operator, \				
8	num_actuators_future_state, \	
9	id_divisions, \					
sampling_period, \					
schedule, \							
time, \								
day \								
FROM rules							
******/
					// Get present actuators state
					query_getActualStateDivision = HAS_query_getActuatorsStateDivision(
						dbconn_3, 
						atoi(PQgetvalue(query_imp_rules, rule_index, 9)), 
						&error_check
						);
					if(NULL == query_getActualStateDivision){
						printf("[thread_rule_implementation ERROR] Query HAS_query_getActuatorsStateDivision not executed\n");
						pthread_exit(NULL);
					}
					
					// Get future states relative to a single rule (id = rule_index)
					query_getFutureStateFromRule = HAS_query_getActuatorsFutureStateFromRule(
						dbconn_3, 
						atoi(PQgetvalue(query_imp_rules, rule_index, 0)), 
						&error_check
						);
					if(NULL == query_getFutureStateFromRule){
						printf("[thread_rule_implementation ERROR] Query HAS_query_getActuatorsFutureStateFromRule not executed\n");
						PQclear(query_getActualStateDivision);
						pthread_exit(NULL);
					}
					
					number_act = PQntuples(query_getActualStateDivision);
					number_actFutStat = PQntuples(query_getFutureStateFromRule);
					
					// Check present state of the actuators and atualize them if needed
					for(actFutStat_index = 0; actFutStat_index < number_actFutStat; actFutStat_index++){
						// Discovering the actuator's id
						for(act_index = 0; act_index < number_act; act_index++)
							if(atoi(PQgetvalue(query_getActualStateDivision, act_index, 0)) == 
									atoi(PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 2)))
								break;
								
						// OBS - now the act_index guards the row for the present actuator
						
						// Update the actuator's state if needed
						if(0 != strcmp(PQgetvalue(query_getActualStateDivision, act_index, 2), 
								PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 1)
								)){
							HAS_query_update_stateActuator(
								dbconn_3, 
								atoi(PQgetvalue(query_imp_rules, rule_index, 9)), 
								PQgetvalue(query_getActualStateDivision, act_index, 1), 
								PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 1)
								);
							HAS_query_updateTimeFinalActuatorHistory(
								dbconn_3, 
								atoi(PQgetvalue(query_getActualStateDivision, act_index, 0)), 
								NULL
								);
							HAS_query_insertNewEntryActuatorHistory(
								dbconn_3, 
								atoi(PQgetvalue(query_getActualStateDivision, act_index, 0)), 
								PQgetvalue(query_getActualStateDivision, act_index, 1), 
								atoi(PQgetvalue(query_imp_rules, rule_index, 9)), 
								PQgetvalue(query_getFutureStateFromRule, actFutStat_index, 1), 
								NULL
								);
						}
					}
					
					// PQclear QUERIES per rule
					PQclear(query_getActualStateDivision);
					PQclear(query_getFutureStateFromRule);
				}
			}


			// ERROR HAS OCURRED
			else{
				printf("[thread_rule_implementation Error] Logic operator of the conditions 1 and 2 is invalid ([RULE %d]).\n", rule_index);
				pthread_exit(NULL);
			}
		}
		
		// RGBMatrix Update
		atualize_RGBstring_sensors_state_RGBMatrix(dbconn_3, buffer_rgb_channel, matrix_side_size);
		atualize_RGBstring_actuators_state_RGBMatrix(dbconn_3, buffer_rgb_channel, matrix_side_size);
		
		// Comparation between the previous state and the new one
		if(0 != strcmp(buffer_rgb_channel_aux, buffer_rgb_channel)){
			strcpy(buffer_rgb_channel_aux, buffer_rgb_channel);
			rgb_matrix_channel = fopen((*pointer_rgb_channel), "w");
			fprintf(rgb_matrix_channel, "%s\n", buffer_rgb_channel);
			fclose(rgb_matrix_channel);
			rgb_matrix_channel = NULL;
		}
		
		PQclear(query_imp_rules);
	}
	pthread_exit(NULL);
}


/***************************************
   THREAD USER INTERFACE (PTH3) - CODE
 ***************************************/
void *user_interface(void *arg){
	int user_index, numb_users;
	char user_answer, user_answer_sub_menu;
	PGconn *dbconn_1;
	PGresult *query, *restart_sequence;
	
	dbconn_1 = PQconnectdb(conn);
	
	if (PQstatus(dbconn_1) == CONNECTION_BAD){
		printf("[dbconn_1] Unable to connect\n");
		pthread_exit(NULL);
	}
	flag_interface = 1;
	
	while(1 == flag_interface) {
		if(0 == strcmp(user_permission, "admin")) {
			printf("\n");
			printf("++++++++++++++++++++++++++++++++++++++++\n");
			printf("+++++++++++++++ HAS MENU +++++++++++++++\n");
			printf("++++++++++++++++++++++++++++++++++++++++\n");
			printf("\n");
			printf("1. Display division's information       \n");
			printf("2. Display division's rules information \n");
			printf("3. Display sensor's information         \n");
			printf("4. Display actuators's information      \n");
			printf("5. Display actuators's and sensors information		\n");
			printf("6. Display user's activity information				\n");
			printf("7. Delete user's activity information				\n");
			printf("8. Delete sensor's history information				\n");
			printf("9. Delete actuator's history information				\n");
			printf("A. Logout from the system				\n");
			printf("B. Exit Home Automative System          \n");
			printf("\n");
			printf("Insert the desired option (press Enter after): ");
			user_answer = getchar();
			getchar();
			switch(user_answer){
				case '1':
					HAS_print_table(dbconn_1, TABNAME_DIVIS);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '2':
					HAS_print_division_info(dbconn_1, user_answer, number_divisions);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '3':
					HAS_print_division_info(dbconn_1, user_answer, number_divisions);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '4':
					HAS_print_division_info(dbconn_1, user_answer, number_divisions);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '5':
					HAS_print_division_info(dbconn_1, user_answer, number_divisions);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '6':
					query = HAS_query_getAllUserInfo(dbconn_1, NULL);
					if(NULL == query){
						printf("[user_interface Error] Query HAS_query_getAllUserInfo not executed\n");
						pthread_exit(NULL);
					}
					// Print for the option all users activity
					numb_users = PQntuples(query);
					for(user_index = 0; user_index < numb_users; user_index++){
						/*****
						 * SELECT 
						 * 	0 id, 
						 * 	1 name, 
						 * 	2 permission, 
						 *  3 password, 
						 *  4 state, 
						 *  5 id_apartment
						 * 		FROM users
						 *****/
						printf("\t[%d] Username: %s\n", user_index+1, PQgetvalue(query, user_index, 1));
					}
					printf("\t[%d] All Users info\n\tAnswer: ", user_index+1);
					
					user_answer_sub_menu = getchar();
					getchar();
					
					printf("\n");
					
					if(0 == numb_users){
						printf("\n\tThere is no users present in the database.\n");
					}
					else if(((int)(user_answer_sub_menu - '0')) < numb_users){
						HAS_print_users_activity_table(dbconn_1, PQgetvalue(query, ((int)(user_answer_sub_menu - '0')) - 1, 1));
					}
					else{
						HAS_print_table(dbconn_1,"users_activity");
					}
					
					
					PQclear(query);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '7':
					query = PQexec(dbconn_1,"TRUNCATE users_activity");
					PQclear(query);
					option = 6;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '8':
					query = PQexec(dbconn_1,"TRUNCATE sensor_history");
					PQclear(query);
					option = 7;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					restart_sequence = PQexec(dbconn_1, "ALTER SEQUENCE sensor_id_count RESTART");
					PQclear(restart_sequence);
					break;
				case '9':
					query = PQexec(dbconn_1,"TRUNCATE actuator_history");
					PQclear(query);
					option = 8;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					restart_sequence = PQexec(dbconn_1, "ALTER SEQUENCE actuator_id_count RESTART");
					PQclear(restart_sequence);
					break;
				case 'A':
					flag_interface = 0;
					option = 2;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case 'B':
					flag_interface = 0;
					flag_command = 0;
					break;
				default:
					printf("Insert a valid option.\n");
					break;
			}			
		} 
		else {
			printf("\n");
			printf("++++++++++++++++++++++++++++++++++++++++\n");
			printf("+++++++++++++++ HAS MENU +++++++++++++++\n");
			printf("++++++++++++++++++++++++++++++++++++++++\n");
			printf("\n");
			printf("1. Display division's information       \n");
			printf("2. Display division's rules information \n");
			printf("3. Display sensor's information         \n");
			printf("4. Display actuators's information      \n");
			printf("5. Display actuators's and sensors information		\n");
			printf("6. Logout from the system				\n");
			printf("7. Exit Home Automative System          \n");
			printf("\n");
			printf("Insert the desired option (press Enter after): ");
			user_answer = getchar();
			getchar();
			switch(user_answer){
				case '1':
					HAS_print_table(dbconn_1, TABNAME_DIVIS);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '2':
					HAS_print_division_info(dbconn_1, user_answer, number_divisions);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '3':
					HAS_print_division_info(dbconn_1, user_answer, number_divisions);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '4':
					HAS_print_division_info(dbconn_1, user_answer, number_divisions);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '5':
					HAS_print_division_info(dbconn_1, user_answer, number_divisions);
					option = 5;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '6':
					flag_interface = 0;
					option = 2;
					HAS_query_user_activity_insert(dbconn_1, option, NULL);
					break;
				case '7':
					flag_interface = 0;
					flag_command = 0;
					break;
				default:
					printf("Insert a valid option.\n");
					break;
			}
		}
	}
	pthread_exit(NULL);
}

/************
   SPRINT#3
 ************/
void HAS_query_insertNewEntryActuatorHistory(PGconn * dbconn, int id_actuator, char * actuator_name, int id_division, char * actuator_state, int * error_check){
	if(NULL == dbconn){
		printf("[HAS_query_insertNewEntryActuatorHistory ERROR] PGconn pointer invalid.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return;
	}
	else if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_insertNewEntryActuatorHistory ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return;
	}
	else if(NULL == actuator_name){
		printf("[HAS_query_insertNewEntryActuatorHistory ERROR] Invalid string: actuator_name.\n");
		if(NULL != error_check)
			(*error_check) = 3;
		return;
	}
	else if(NULL == actuator_state){
		printf("[HAS_query_insertNewEntryActuatorHistory ERROR] Invalid string: actuator_state.\n");
		if(NULL != error_check)
			(*error_check) = 4;
		return;
	}
	else if(0 > id_actuator){
		printf("[HAS_query_insertNewEntryActuatorHistory ERROR] Invalid id: id_actuator.\n");
		if(NULL != error_check)
			(*error_check) = 5;
		return;
	}
	else if(0 > id_division){
		printf("[HAS_query_insertNewEntryActuatorHistory ERROR] Invalid id: id_division.\n");
		if(NULL != error_check)
			(*error_check) = 6;
		return;
	}
	
	char str[300];
	PGresult * query;
	sprintf(str, "INSERT INTO actuator_history(id_actuator, actuator_name, id_division, actuator_state, time_start) VALUES (%d, '%s', %d, '%s', CURRENT_TIMESTAMP(0) AT TIME ZONE 'GMT-1')", id_actuator, actuator_name, id_division, actuator_state);
	query = PQexec(dbconn, str);
	if(!(PQresultStatus(query) == PGRES_COMMAND_OK)){
		printf("[HAS_query_insertNewEntryActuatorHistory Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 7;
		return;
	}
	if(NULL != error_check)
		(*error_check) = 0;
	PQclear(query);
}

void HAS_query_updateTimeFinalActuatorHistory(PGconn * dbconn, int id_actuator, int * error_check){
	if(NULL == dbconn){
		printf("[HAS_query_updateTimeFinalActuatorHistory ERROR] PGconn pointer invalid.\n");
		if(NULL != error_check)
			(*error_check) = 1;
		return;
	}
	else if(CONNECTION_BAD == PQstatus(dbconn)){
		printf("[HAS_query_updateTimeFinalActuatorHistory ERROR] Connection to the database is bad.\n");
		if(NULL != error_check)
			(*error_check) = 2;
		return;
	}
	else if(0 > id_actuator){
		printf("[HAS_query_updateTimeFinalActuatorHistory ERROR] Invalid id: id_actuator.\n");
		if(NULL != error_check)
			(*error_check) = 3;
		return;
	}
	
	char str[150];
	PGresult *query, *query_aux;
	sprintf(str, "SELECT id, id_actuator, actuator_name, id_division, actuator_state, time_start, time_final FROM actuator_history WHERE id_actuator = %d ORDER BY time_start DESC", id_actuator);
	query = PQexec(dbconn, str);
	if(PQresultStatus(query) == PGRES_TUPLES_OK){
		if(0 != PQntuples(query)){
			if(0 == PQgetisnull(query, 0, 5)){
				sprintf(str, "UPDATE actuator_history SET time_final = CURRENT_TIMESTAMP(0) AT TIME ZONE 'GMT-1' WHERE id = %d", atoi(PQgetvalue(query, 0, 0)));
				query_aux = PQexec(dbconn, str);	
				if(!(PQresultStatus(query_aux) == PGRES_COMMAND_OK))
					printf("[HAS_query_updateTimeFinalActuatorHistory Error Message] %s\n", PQresultErrorMessage(query_aux));
				PQclear(query_aux);
			}
		}
		/*****
		else if(0 == PQntuples(query))
			printf("[HAS_query_updateTimeFinalActuatorHistory Info] No register about the %d actuator\n", id_actuator);
		*****/
		PQclear(query);
		if(NULL != error_check)
			(*error_check) = 0;
	}
	else{
		printf("[HAS_query_updateTimeFinalActuatorHistory Error Message] %s\n", PQresultErrorMessage(query));
		if(NULL != error_check)
			(*error_check) = 4;
	}
}

void HAS_query_insertNewEntrySensorHistory(PGconn *dbconn, int *error_check, char * sensor_type, int mote_id, int sensor_data) {
	// ERROR CHECK
	if(NULL == dbconn) {
		printf("[HAS_query_insertNewEntrySensorHistory ERROR MESSAGE] DB connection not OK!");
		if(NULL != error_check)
			(*error_check) = 1;
		return;
	} else if(1 > mote_id) {
		printf("[HAS_query_insertNewEntrySensorHistory ERROR MESSAGE] mote_id not correct!");
		if(NULL != error_check)
			(*error_check) = 2;
		return;
	}
	
	// Function's code
	char query_string[SIZE_QUERY_STRING];
	PGresult *sensor_history;
	
	sprintf(query_string, "SELECT id, mote_id, sensor_type, id_divisions FROM sensor WHERE mote_id = %d AND sensor_type = '%s'", mote_id, sensor_type);
	sensor_history = PQexec(dbconn, query_string);
	
	if(PQresultStatus(sensor_history) == PGRES_TUPLES_OK) {	
		if(0 == PQntuples(sensor_history)) {
			//[HAS_query_insertNewEntrySensorHistory ERROR MESSAGE] There's no sensor with this specification!
			PQclear(sensor_history);
			return;
		} else {
			sprintf(query_string, "INSERT INTO sensor_history(id_sensor, sensor_type, mote_id, id_division, sensor_data, time_update) VALUES (%d, '%s', %d, %d, %d, CURRENT_TIMESTAMP(0) AT TIME ZONE 'GMT-1')", atoi(PQgetvalue(sensor_history, 0,0)), PQgetvalue(sensor_history, 0,2), atoi(PQgetvalue(sensor_history, 0,1)), atoi(PQgetvalue(sensor_history, 0,3)), sensor_data);
			sensor_history = PQexec(dbconn, query_string);
			if(!(PQresultStatus(sensor_history) == PGRES_COMMAND_OK)) {
				printf("ERRO AO EXECTUAR QUERY: %s\n", PQresultErrorMessage(sensor_history));
				return;
			}
		}
	}
	PQclear(sensor_history);
}
