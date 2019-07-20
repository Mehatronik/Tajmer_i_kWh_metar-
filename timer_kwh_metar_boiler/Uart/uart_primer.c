/*
 * BuckConverter.c
 *
 * Created: 24.8.2018. 01.14.39
 * Author : Kovacevic
 */ 

#define  F_CPU 16000000


#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include <util/delay.h>



int mainIlimozdaNE(void)
{
    
	char bafer[15];
	int a=10, b=5, c=255;

	uart_init();
	sei();

	
    while (1) 
    {
		/* preko uarta se salju stringovi, pa da bi se int poslao mora se prvo konvertovati preko itoa() f-je */
		/* moguce je koristiti i ostale slicne f-je: utoa(), ltoa()..... */
		/* umesto itoa() bolje je koristiti sprintf(buffer, "%3d", int); */
		
		send_str("  ");
		itoa( a , bafer, 10);
		send_str(bafer);
		
		send_str("  ");
		itoa( b , bafer, 10);
		send_str(bafer);
		
		send_str("  ");
		itoa( c , bafer, 10);
		send_str(bafer);

		
		
		send_str("\n");  //novi red

		_delay_ms(100);
		
	
		
		
    }
}

