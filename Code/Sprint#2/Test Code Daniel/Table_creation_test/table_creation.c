#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <postgresql/libpq-fe.h>

#define TABLE_NOT_EXIST			0
#define TABLE_EXIST				1
#define TABLE_NOT_CREATED		0
#define TABLE_CREATED			1
#define	ERROR_VERIFYING_TABLE	2
#define	ERROR_CREATING_TABLE	3


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




const char *conn = "host='db.fe.up.pt' user='sinfa15' password='DSPARS_sinf2018*'";

int main()
{	
	/* **********************************TEST_CODE********************************************** */
	
/*	printf("Actuator_future_state created: %d\n",HAS_query_create_actuator_future_state_table(dbconn));
	printf("Actuator created: %d\n",HAS_query_create_actuator_table(dbconn));
	printf("Building created: %d\n",HAS_query_create_building_table(dbconn));
	printf("Apartment created: %d\n",HAS_query_create_apartment_table(dbconn));
	printf("Divisions created: %d\n",HAS_query_create_divisions_table(dbconn));
	printf("Sensor created: %d\n",HAS_query_create_sensor_table(dbconn));
	printf("Rules created: %d\n",HAS_query_create_rules_table(dbconn));
	printf("Users created: %d\n",HAS_query_create_users_table(dbconn));
	printf("Users_activity created: %d\n",HAS_query_create_users_activity_table(dbconn));
	
	printf("Actuator_future_state verified: %d\n",HAS_query_verify_actuator_future_state_table(dbconn));
	printf("Actuator verified: %d\n",HAS_query_verify_actuator_table(dbconn));
	printf("Building verified: %d\n",HAS_query_verify_building_table(dbconn));
	printf("Apartment verified: %d\n",HAS_query_verify_apartment_table(dbconn));
	printf("Divisions verified: %d\n",HAS_query_verify_divisions_table(dbconn));
	printf("Sensor verified: %d\n",HAS_query_verify_sensor_table(dbconn));
	printf("Rules verified: %d\n",HAS_query_verify_rules_table(dbconn));
	printf("Users verified: %d\n",HAS_query_verify_users_table(dbconn));
	printf("Users_activity verified: %d\n",HAS_query_verify_users_activity_table(dbconn));*/
	
	/* **************************************END************************************************ */
	
	PGconn *dbconn;
	dbconn = PQconnectdb(conn);
	int res_query;
	
	puts("\n");
	
	switch(HAS_query_verify_building_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_building_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_BUILDING: The table 'building' was successfully created!\n");
				PQexec(dbconn,"INSERT INTO building (address, num_floors, owner, num_apartments) "\
								"VALUES ('Rua Dr. Roberto Frias', 1, 'Daniel,Pedro e Ricardo Lda.', 1)");
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
	
	puts("\n");
	
	switch(HAS_query_verify_apartment_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_apartment_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_APARTMENT: The table 'apartment' was successfully created!\n");
				PQexec(dbconn,"INSERT INTO apartment (id, floor, number, proprietary, num_divisions, address_building) "\
				"VALUES (0,0,1,'Armando Sousa Araújo',4,'Rua Dr. Roberto Frias')");
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
	
	puts("\n");
	
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
	
	puts("\n");
	
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
	
	puts("\n");
	
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
	
	puts("\n");
	
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

	puts("\n");
	
	switch(HAS_query_verify_users_table(dbconn)){
		case TABLE_NOT_EXIST:{
			res_query = HAS_query_create_users_table(dbconn);
			
			if(res_query == TABLE_CREATED){
				printf("INFO_USERS: The table 'users' was successfully created!\n");
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
	
	puts("\n");
	
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
	
	puts("\n");
	
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
	
	puts("\n");
	
	PQfinish(dbconn);
}

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
									"data DOUBLE PRECISION NOT NULL,"\
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
									"date DATE NOT NULL"\
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
