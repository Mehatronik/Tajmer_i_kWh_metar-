/*
 * uart.c
 *
 * Created: 1.12.2017. 23.00.35
 *  Author: Jovica
 *  Modified: Kovacevic
 *
 * Potrebno u main funkciji omoguciti interapte globalno: sei()
 *
 */ 

#include "comm.h"
#include "uart.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

volatile unsigned char poslato;
volatile unsigned char input_char;
volatile unsigned char input_buffer[INPUT_BUFF_SIZE], buff_size=0;

ISR(USART_TX_vect)
{
	poslato=0;
}

ISR(USART_RX_vect)
{
	input_char=UDR0;
	
	if (buff_size<INPUT_BUFF_SIZE)
	{
		input_buffer[buff_size]=input_char;
		buff_size++;
		input_buffer[buff_size]='\0';
	}
}

void uart_init(uint32_t baud_rate)
{
	UCSR0A = 0b10;			//dupla brzina slanja
	UCSR0B = 0b11011000;	//RX complete interupt enable; TX complete interupt enable; Receiver enable; Transmitter enable
	UCSR0C = 0b110;			//asinhroni rezim, 8-bit, no parity, 1 stop bit
	

	switch(baud_rate)
	{
		/*za U2Xn=1 tj. za 2x podesenu brzinu slanja */
		case 9600:	  UBRR0 = 207; break;	//error = 0.2%
		case 14400:   UBRR0 = 138; break;	//error = -0.1%
		case 19200:   UBRR0 = 103; break;	//error = 0.2%
		case 38400:   UBRR0 = 51;  break;	//error = 0.2%
		case 76800:	  UBRR0 = 25;  break;	//error = 0.2%
		case 250000:  UBRR0 = 7;   break;	//error = 0%
		case 500000:  UBRR0 = 3;   break;	//error = 0%
		case 1000000: UBRR0 = 1;   break;	//error = 0%
		case 2000000: UBRR0 = 0;   break;	//error = 0%
		default:
			UBRR0 = 207;					//baud 9600 ako nije pogodjen ni jedan case
	}
	
}

void uart_send_str(char *str)
{
	//funkcija za slanje stringova
	
	while (*str!='\0')		//koci dok ne posalje ceo string
	{
		UDR0 = *str;		//salje karakter koji se nalazi na adresi na koju pokazuje str
		poslato = 1;		//flag koji se resetuje u TX ISR-u
		while (poslato);	//koci dok ISR ne kaze da je gotovo
		str++;				//uvecava adresu pokazivaca da dodje do narednog karaktera
	}
}