/*
 * manipulate_data.c
 * 
 * Copyright 2018 
 * 		Daniel Queirós da Silva		<up201503071fe.up.pt>
 *		Pedro de Castro Albergaria 	<up201504635@fe.up.pt>
 * 		Ricardo Barbosa Sousa		<up201503004@fe.up.pt>
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
#include "manipulation_mote_struct.h"

#define LINE	70
#define WORD	6

#define BASE	16

#define	ERROR1	1
#define ERROR2	2
#define ERROR3	3
#define	ERROR4	4
#define	ERROR5	5
#define ERROR6	6

#define	ASCII_res_number_0	0
#define ASCII_res_number_9	9
#define	ASCII_res_char_A	17
#define ASCII_res_char_F	22
#define ASCII_res_space		-16

#define	INIT_BOTTOM		0
#define	INIT_TOP		4
#define	MOTE_ID_BOTTOM	15
#define	MOTE_ID_TOP		19
#define	TEMP_BOTTOM		36
#define	TEMP_TOP		40
#define	HUMID_BOTTOM	42
#define	HUMID_TOP		46
#define	LIGHT_BOTTOM	48
#define	LIGHT_TOP		52
#define	END_BOTTOM		66
#define	END_TOP			67

#define SPACE			2

#define NUM_MOTES		5

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
int main(int argc, char **argv)
{
	FILE *f_read;
	
	int nmessages = 1, step_line = 0, step_word = 0, fill_up = 0, times_10 = 0, times_16 = 0;
	int	decimal_id = 0, convert = 0, flag_invalid = 0, decimal_temp = 0, decimal_humid = 0, decimal_visible_light = 0;
	float temp = 0, relative_humidity = 0, humidity_compensated_by_temperature = 0, visible_light = 0;
	
	char *line = NULL, *word = NULL;
	
	mote **syst_motes = NULL;
	
	syst_motes = mote_vector_creation(NUM_MOTES,NULL);
	
	line = (char *) malloc(LINE*sizeof(char));
	word = (char *) malloc(WORD*sizeof(char));
	
	f_read = fopen("/dev/pts/3", "r"); //vai receber strings do programa"file_strings_to_channels"no canal 5
												//compilar apenas com - ./manipulate_data											
												
	while(NULL != fgets(line, LINE, f_read)){
		if(strncmp(line,"\n",3) == 0)
			nmessages+= 0;
		else{
			printf("\n[%d] %s\n", nmessages,line);
			nmessages++;
			
		/*Ciclo que percorre uma linha de informação completa*/	
			for(step_line = 0; step_line < strlen(line); step_line++){

					
			/*Início de cada linha da mensagem*/
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
						printf("Valid init!\n");
					}
					else {
						printf("ERROR[%d] - The message start bytes are incorrect!\n",ERROR1);
						//break;
					}
				}
				
			/*MOTE ID de cada linha da mensagem*/	
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
					}
					else{
						printf("Mote ID: %d\n",decimal_id);
					}
				}
				
			/*Parâmetro de Temperatura*/
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
						syst_motes[search_mote(syst_motes,NUM_MOTES,decimal_id,NULL)]->temperature = temp;
						
						printf("Temperature = %.2f ºC\n",temp);
					}
				}
				
			/*Parâmetro de Humidade*/	
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
					printf("Relative humidity = %.2f % \n",relative_humidity);
					
					humidity_compensated_by_temperature = calculate_humidity_compensated_by_temperature(decimal_humid,relative_humidity,temp);
					syst_motes[search_mote(syst_motes,NUM_MOTES,decimal_id,NULL)]->humidity = humidity_compensated_by_temperature;
					
					printf("Humidity compensated by temperature = %.2f % \n",humidity_compensated_by_temperature);
					}
				}
				
			/*Parâmetro de luz visível*/	
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
					syst_motes[search_mote(syst_motes,NUM_MOTES,decimal_id,NULL)]->luminosity = visible_light;
					
					printf("Visible light = %.2f lux\n", visible_light);
					}
				}
				
			/*Fim da linha*/	
				else if (END_BOTTOM == step_line){
					fill_up = 0;
					for(step_word = END_BOTTOM; step_word <= END_TOP; step_word++){
						word[fill_up] = line[step_word];
						fill_up++;
					}
					word[fill_up] = '\0';
					if(0 == strcmp(word,"7E")){
						printf("Valid end!\n");
						break;
					}
					else {
						printf("ERROR[%d] - Invalid end!\n",ERROR6);
						break;
					}
				}
			}
		//print_motes_vector(syst_motes,NUM_MOTES,NULL); /*Para testar a gravação dos valores na estrutura de motes*/
		}
	}
	free_mote_memory(syst_motes,NUM_MOTES,NULL);
	free(line);
	free(word);
	line = NULL;
	word = NULL;		
	fclose(f_read);
	return 0;
}
