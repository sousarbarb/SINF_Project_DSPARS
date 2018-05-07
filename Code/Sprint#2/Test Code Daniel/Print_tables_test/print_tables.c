#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <postgresql/libpq-fe.h>

const char *conn = "host='db.fe.up.pt' user='sinfa15' password='DSPARS_sinf2018*'";

#define NUM_SPACES	2

void HAS_print_table(PGconn *dbconn, char *name_table){
	
	int i = 0, num_columns = 0, num_lines = 0, step_line = 0, step_col = 0;
	int size_name_col = 0, size_elem = 0, size_elem_max = 0, res_space = 0;
	char str_query[256];
	
	PGresult *query;
	printf("Table -> %s\n",name_table);
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

int main()
{
	system("clear");
	PGconn *dbconn;
	dbconn = PQconnectdb(conn);
	HAS_print_table(dbconn,"users_activity");
}

				
