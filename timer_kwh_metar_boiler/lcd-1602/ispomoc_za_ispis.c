/*
 * ispomoc_za_ispis.c
 *
 * Created: 2.5.2019. 19.17.11
 *  Author: Kovacevic
 */ 
#include "comm.h"
#include "lcd1602.h"

/* funkcije za ispisivanje sata/minuta/sekundi tokom podesavanja istih u meniju */
void sati_ispis(uint8_t *sat, char *buff, int8_t *cursor, uint8_t inc_dec);
void minuti_ispis(uint8_t *minut, char *buff, int8_t *cursor, uint8_t inc_dec);
void sekundi_ispis(uint8_t *sekund, char *buff, int8_t *cursor, uint8_t inc_dec);




void sati_ispis(uint8_t *sat, char *buff, int8_t *cursor, uint8_t inc_dec)
{
	
	if (inc_dec == UVECAJ)
	{
		(*sat)++;
		if(*sat >= 24 && *sat < 30)		//<30 posto je hr = unsigned int
			*sat = 0;
		
		sprintf(buff, "%02d", *sat);
		lcd1602_goto_xy(*cursor-1,1);
		lcd1602_send_string(buff);
		lcd1602_goto_xy(*cursor,1);
	}
	else if (inc_dec == UMANJI)
	{
		(*sat)--;
		if(*sat >= 250 && *sat <= 255)	//posto je hr = unsigned int 8 bit i overflow se desi
			*sat = 23;
		
		sprintf(buff, "%02d", *sat);
		lcd1602_goto_xy(*cursor-1,1);
		lcd1602_send_string(buff);
		lcd1602_goto_xy(*cursor,1);
	}
	
	
}

void minuti_ispis(uint8_t *minut, char *buff, int8_t *cursor, uint8_t inc_dec)
{
	
	if (inc_dec == UVECAJ)
	{
		(*minut)++;
		if(*minut >= 60 && *minut < 65)		//<30 posto je hr = unsigned int
			*minut = 0;
		
		sprintf(buff, "%02d", *minut);
		lcd1602_goto_xy(*cursor-1,1);
		lcd1602_send_string(buff);
		lcd1602_goto_xy(*cursor,1);
	}
	else if (inc_dec == UMANJI)
	{
		(*minut)--;
		if(*minut >= 250 && *minut <= 255)	//posto je hr = unsigned int 8 bit i overflow se desi
			*minut = 59;
		
		sprintf(buff, "%02d", *minut);
		lcd1602_goto_xy(*cursor-1,1);
		lcd1602_send_string(buff);
		lcd1602_goto_xy(*cursor,1);
	}
	
	
}

void sekundi_ispis(uint8_t *sekund, char *buff, int8_t *cursor, uint8_t inc_dec)
{
	
	if (inc_dec == UVECAJ)
	{
		(*sekund)++;
		if(*sekund >= 60 && *sekund < 65)		//<30 posto je hr = unsigned int
			*sekund = 0;
		
		sprintf(buff, "%02d", *sekund);
		lcd1602_goto_xy(*cursor-1,1);
		lcd1602_send_string(buff);
		lcd1602_goto_xy(*cursor,1);
	}
	else if (inc_dec == UMANJI)
	{
		(*sekund)--;
		if(*sekund >= 250 && *sekund <= 255)	//posto je hr = unsigned int 8 bit i overflow se desi
			*sekund = 59;
		
		sprintf(buff, "%02d", *sekund);
		lcd1602_goto_xy(*cursor-1,1);
		lcd1602_send_string(buff);
		lcd1602_goto_xy(*cursor,1);
	}
	
	
}