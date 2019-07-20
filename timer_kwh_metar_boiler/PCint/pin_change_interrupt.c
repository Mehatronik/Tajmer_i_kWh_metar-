/*
 * pin_change_interrupt.c
 *
 * Created: 28.4.2019. 17.30.32
 *  Author: Kovacevic
 */ 

#include "comm.h"
#include "pin_change_interrupt.h"


volatile uint8_t flag_pc_int = 0;
volatile uint8_t flag_pc_int_pomocni = 0;

/* za potrebe rtc modula, da bi vreme bilo ispisivano svake sekunde. 
   Rtc modul daje na izlazu 1HZ (ako je podesen) na posebnom pinu.
   Taj pin cu vezati na PC3 (PCINT11), tj. A3 na Arduino Uno ploci.
   
   PCINT radi tako sto daje prekid na toggle pina (ne razlikuje uzlaznu i silaznu ivicu),
   tako da je u ISR-u potrebno proveriti logicki nivo pina posle prekida.
*/
void pc_init()
{
	//PCINT-ovi su podeljeni u 3 grupe
	
	PCICR |= 1<<PCIE1;		//interrupt enable, u PCIE1 spadaju PCINT 14 - 8. Meni treba PCINT11 pin
	PCMSK1 |= 1<<PCINT11;   //interrupt enable na zeljenom pinu - PCINT11
	
	DDRC &= ~(1<<PINC3);  //PCINT11 je na pinu PC3, ulaz
	
}

ISR(PCINT1_vect) 
{
	/* posle izazivanja prekida proverim logicki nivo, jer ce prekid ici na 
	   pola sekunde zbog toga sto ne razlikuje ivice, proverim samo kada je HIGH
	   cime dobijam 1 sekund.
	*/
	
	//moze i  (PINC & 1<<PINC3) == 0b1000, ali je redundantno
	if( PINC & (1<<PINC3) )		
	{
		flag_pc_int = 1;
		flag_pc_int_pomocni = 1;
	}
		
}