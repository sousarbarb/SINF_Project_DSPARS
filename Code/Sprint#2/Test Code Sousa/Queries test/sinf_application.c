#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <postgresql/libpq-fe.h>

const char *conn = "host='db.fe.up.pt' user='sinfa15' password='DSPARS_sinf2018*'";

PGconn *dbconn;

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

int HAS_query_getMaximumNumberSensorsDivisions(int id_apartment, int * error_check){
	return 0;
}

int main()
{
	int id, error_check;
	int return_func;
	dbconn = PQconnectdb(conn);
	
	if (PQstatus(dbconn) == CONNECTION_BAD){
		printf("Unable to connect\n");
		exit(-1);
	}else{
		printf("Able to connect!\n"); 
	}
	
	printf("Number Maximum Sensors: %d\n", HAS_query_getMaximumNumberSensorsDivisions(0, &error_check));
	
	
	
	/*
	while(1){
		printf("Please insert your user id: ");
		scanf(" %d", &id);
		query = HAS_query_getUserInfo(id, &error_check);
		switch(error_check){
			case 0:
			printf("USER INFO:\n"
			       "id: %d\t__%s\t__%s\t\t__%s\t__%d\t__%d\n", 
			       id,
			       PQgetvalue(query, 0, 0),
			       PQgetvalue(query, 0, 1),
			       PQgetvalue(query, 0, 2),
			       atoi(PQgetvalue(query, 0, 3)),
			       atoi(PQgetvalue(query, 0, 4)));
				break;
			case 3:
			printf("User not founded\n[%d] %s\n", PQresultStatus(query), PQresultErrorMessage(query));
			break;
			default:
			printf("ASDFQEWRF\n");
		}
		PQclear(query);
	}*/
	
	
	PQfinish(dbconn);	
}
