/*
 * manipulate_data.c
 * 
 * Copyright 2018 Daniel Silva <daniel@debian>
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

#define LINE	100
#define WORD	10

#define BASE	16

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
#define	END_TOP			68 //contando com o parágrafo ("\n")

FILE *f = NULL;

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
	int nmessages = 1, step_line = 0, step_word = 0, fill_up = 0, times_10 = 0, times_16 = 0;
	int	decimal_id = 0, convert = 0, flag_invalid = 0, decimal_temp = 0, decimal_humid = 0, decimal_visible_light = 0;
	float temperature = 0, relative_humidity = 0, humidity_compensated_by_temperature = 0, visible_light = 0;
	char line[LINE]={0}, word[WORD]={0};
	f = fopen("/dev/pts/3", "r");
	while(1){
		if(NULL != fgets(line, LINE, f)){
			printf("\n[%d] %s\n", nmessages, line);
			nmessages++;
		}
		
	/*Ciclo que percorre uma linha de informação completa*/	
		for(step_line = 0; step_line < strlen(line); step_line++){
				
		/*Início de cada linha da mensagem*/
			if(INIT_BOTTOM == step_line){
				fill_up = 0;
				for(step_word = INIT_BOTTOM; step_word <= INIT_TOP; step_word++){
					word[fill_up] = line[step_word];
					fill_up++;
				}
				word[fill_up] = '\0';
				
				if(0 == strcmp(word,"7E 45")){
					printf("Início válido!\n");
				}
				else {
					printf("Início inválido!\n");
					break;
				}
			}
			
		/*MOTE ID de cada linha da mensagem*/	
			else if(MOTE_ID_BOTTOM == step_line){
				flag_invalid = 0;
				times_10 = 0;
				decimal_id = 0;
				for(step_word = MOTE_ID_BOTTOM; step_word <= MOTE_ID_TOP; step_word++){
					convert = line[step_word] - '0';
					if((convert >= 0) && (convert <= 9)){
						decimal_id = decimal_id + convert*multiplication_by_10(times_10);
						times_10++;
					}
					else if(convert != -16){
						flag_invalid = 1;
						break;
					}
				}
				if((decimal_id > 9999) || (flag_invalid)){
					printf("Mote ID inválido!\n");
					break;
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
					if((convert >= 17) && (convert <= 22)){
						convert = line[step_word] - 'A' + 10;
						decimal_temp = decimal_temp + convert*power(BASE,times_16);
						times_16++;
					}
					else if((convert >= 0) && (convert <= 9)){
						decimal_temp = decimal_temp + convert*power(BASE,times_16);
						times_16++;
					}
					else if(convert == -16){ 
						decimal_temp += 0;
					}
					else{
						flag_invalid = 1;
						break;
					}
				}
				if(flag_invalid){
					printf("Parâmetro de temperatura inválido!\n");
					break;
				}
				else{
					//printf("Temperatura decimal = %d\n", decimal_temp);
					temperature = calculate_temperature(decimal_temp);
					printf("Temperatura = %.2f ºC\n",temperature);
				}
			}
			
		/*Parâmetro de Humidade*/	
			else if(HUMID_BOTTOM == step_line){
				flag_invalid = 0;
				times_16 = 0;
				decimal_humid = 0;
				for(step_word = HUMID_TOP; step_word >= HUMID_BOTTOM; step_word--){
					convert = line[step_word] - '0';
					if((convert >= 17) && (convert <= 22)){
						convert = line[step_word] - 'A' + 10;
						decimal_humid = decimal_humid + convert*power(BASE,times_16);
						times_16++;
					}
					else if((convert >= 0) && (convert <= 9)){
						decimal_humid = decimal_humid + convert*power(BASE,times_16);
						times_16++;
					}
					else if(convert == -16){
						decimal_humid += 0;
					}
					else{
						flag_invalid = 1;
						break;
					}
				}
				if(flag_invalid){
					printf("Parâmetro de Humidade inválido!\n");
					break;
				}
				else{
				//printf("Humidade decimal = %d\n",decimal_humid);
				relative_humidity = calculate_relative_humidity(decimal_humid);
				printf("Humidade relativa = %.2f %\n",relative_humidity);
				humidity_compensated_by_temperature = calculate_humidity_compensated_by_temperature(decimal_humid,relative_humidity,temperature);
				printf("Humidade compensada com a temperatura = %.2f %\n",humidity_compensated_by_temperature);
				}
			}
			
		/*Parâmetro de luz visível*/	
			else if(LIGHT_BOTTOM == step_line){
				flag_invalid = 0;
				times_16 = 0;
				decimal_visible_light = 0;
				for(step_word = LIGHT_TOP; step_word >= LIGHT_BOTTOM; step_word--){
					convert = line[step_word] - '0';
					if((convert >= 17) && (convert <= 22)){
						convert = line[step_word] - 'A' + 10;
						decimal_visible_light = decimal_visible_light + convert*power(BASE,times_16);
						times_16++;
					}
					else if((convert >= 0) && (convert <= 9)){
						decimal_visible_light = decimal_visible_light + convert*power(BASE,times_16);
						times_16++;
					}
					else if(convert == -16){
						decimal_visible_light += 0;
					}
					else{
						flag_invalid = 1;
						break;
					}
				}
				if(flag_invalid){
					printf("Parâmetro de luz visível inválido!\n");
					break;
				}
				else{
				//printf("Luz visível decimal = %d\n",decimal_visible_light);
				visible_light = calculate_visible_light(decimal_visible_light);
				printf("Luz visível = %.2f lux\n", visible_light);
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
				if(0 == strcmp(word,"7E\n")){
					printf("Fim válido!\n");
					break;
				}
				else {
					printf("Fim inválido!\n");
					break;
				}
			}
		}
	}		
	return 0;
}
