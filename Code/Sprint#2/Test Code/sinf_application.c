#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <postgresql/libpq-fe.h>

const char *conn = "host='db.fe.up.pt' user='sinfa15' password='DSPARS_sinf2018*'";

int main()
{	
	PGconn *dbconn;
	
	dbconn = PQconnectdb(conn);
	
	if (PQstatus(dbconn) == CONNECTION_BAD){
		printf("Unable to connect\n");
		exit(-1);
	}else{
		printf("Able to connect!\n"); 
	}
	
	PGresult *query = PQexec(dbconn, "SELECT * FROM \"HAS\".\"test_table\""); 	
	
	printf("Result Query: %d\n", PQresultStatus(query));
	
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
	}else{
		printf("DB query call not ok!\n");
	}
	
	PQclear(query);
	PQfinish(dbconn);	
}
