/*
 * tasteri.c
 *
 * Created: 30.4.2019. 14.14.55
 *  Author: Kovacevic
 */ 

#include "comm.h"
#include "tasteri.h"
#include "tajmer.h"
#include "uart.h"

char buff[15];
extern const char *byte_to_binary(int x);

void tasteri_init()
{
	/* tasteri input */
	DDRD &= ~( (1<<TASTER_LEVO_pinPort) | (1<<TASTER_DESNO_pinPort) | (1<<TASTER_GORE_pinPort) | (1<<TASTER_DOLE_pinPort) );
	DDRD &= ~( (1<<TASTER_NAZAD_pinPort) | (1<<TASTER_ENTER_pinPort) );
	DDRB &= ~( 1<<TASTER_NONSTOP_pinPort );
	
	/* internal pull ups ON */
	PORTD |= (1<<TASTER_LEVO_pinPort) | (1<<TASTER_DESNO_pinPort) | (1<<TASTER_GORE_pinPort) | (1<<TASTER_DOLE_pinPort);
	PORTD |= (1<<TASTER_NAZAD_pinPort) | (1<<TASTER_ENTER_pinPort);
	PORTB |= (1<<TASTER_NONSTOP_pinPort);
}


uint8_t ocitaj_tastere()
{
	/* posto imam 7 tastera spakovacu ih u jedan "registar" tj. 8-bit promenljivu
	   gde ce svaki bit predstavljati stanje tastera, a prva dva MSB su visak     */
	
	/* posto postoji problem bouncing-a, tastere ocitavam sa periodom npr. 50ms, tj.
	   koristim polling sistem. Ako je ocitavanje isto u uzastopna dva citanja, tasteri su
	   debouncovani i stanje u registru predstavlja stvarno stanje tastera		*/
	
	
	/* u tajmeru imam zapravo "2 tajmera" tj. procitam sve tastere prvi put pa posle 3ms (tj. Deb.Perioda/2)
	   opet procitam stanje. Ako se poklapaju konacno upisujem stanje iz dve pomocne promenljive u glavnu promenljivu
	   koja preslikava stvano stanje	*/

	static uint8_t tasteri_reg = 0xFF;
	uint8_t temp_reg1 = 0xFF;
	uint8_t temp_reg2 = 0xFF;
	
	if (flag_prekid_debounce_time_half)		//prvo citanje 3ms; zapravo perioda je 3ms jer je to razlika izmedju prvog i drugog citanja
	{
		flag_prekid_debounce_time_half = 0;	//reset flag
		
		//procitam stanje svih tastera i spakujem u jednu promenljivu
		//prvo citanje
		temp_reg1 = TEMP_REG_READ;

	}
	if (flag_prekid_debounce_time)		//drugo citanje 6ms, tj 3ms posle drugog
	{
		flag_prekid_debounce_time = 0;
		
		//drugo citanje
		temp_reg2 = TEMP_REG_READ;
		
		if (temp_reg1 == temp_reg2)
			tasteri_reg = temp_reg2;	//nebitno da li je 1 ili 2 jer su ovde isti
		
	}
	
	return tasteri_reg;
}

uint8_t ocitaj_jedan_taster(uint8_t buttons_reg, uint8_t button)
{
	/* f-ja prima vrednost, tj. broj tastera koji je definisan #define-om u tasteri.h fajlu*/
	/* VAZNO: ova f-ja samo proverava stanje jednog tastera u registru, a u programu je neophodno pozivati
	   funkciju za citanje tastera, tj. potrebno je polling-ovati	*/
	/* potrebno je otpustiti taster da bi se uvazilo njegovo ili stiskanje bilo kog drugog tastera */
	/* promenljiva "button" se prakticno menja pri svakom pozivu od strane main programa, tako da je potrebna
	   paznja pri programiranju */
	
	uint8_t stisnut = 0;
	static uint8_t temp_tast = 0;
	static uint8_t flag_stisnut = 0;
	static uint8_t delay_enable = 1;
	uint8_t pom;
	
	if ( (~buttons_reg & (1<<button)) && flag_stisnut == 0 )		//taster stisnut a prethodno otpusten
	{
		flag_stisnut = 1;		//specava ponovni ulazak ako je ostao stisnut
		temp_tast = button;		//zapamtim koji je taster stisnut
		stisnut = 1;
	}		
	pom  =	( ~buttons_reg & (1<<temp_tast)) ;	//maskiram i proveravam da li je stisnut
	if (  pom  && (temp_tast==TASTER_DOLE || temp_tast==TASTER_GORE) && button == temp_tast)	//ako je taster gore ili dole ostao stisnut, sacekaj malo pa ga toggle-uj nekom periodom
	{
		/* ako je ostao stisnut taster gore ili dole, sacekaj npr. 0.3s pa ga togluj periodom od npr 0.05s;
		   ovo radim da bi bilo moguce brze menjati sate ako je taster ostao stisnut, da ne bi morao 30 puta stiskati i pustati taster */
		//startuj "delay" tajmer, ali samo prvi put
		//ako je delay istekao periodicno togluj promenljivu "stisnut", jer ce to napraviti efekat brzog stiskanja
		//kada je taster otpusten opet dozvoli startovanje "delay" tajmera
		
		if (delay_enable == 1)
		{
			delay_enable = 0;	//spreci ponovni ulazak
			delay_timer = 0;	//start, timer ISR ga uvecava
		}
		if (delay_timer >= 400)	//ako je delay veci od 400ms togluj "stisnut"
		{
			if(flag_prekid_50ms)
			{
				flag_prekid_50ms = 0; //reset flag-a
				stisnut ^= 0b1;	//toggle
			}
			
				
		}		
	}
	else if ( (~buttons_reg & (1<<temp_tast)) == 0 )		//provera da li je otpusten taster koji je stisnut
	{
		flag_stisnut = 0;	//resetujem flag tek kada je otpusten taster koji je stisnut
		delay_enable = 1;	//dozvoli ponovno startovanje "delay" tajmera ako je taster otpusten
	}
	
	/*
	sprintf(buff, "butt_reg:%s  ", byte_to_binary(buttons_reg));
	uart_send_str(buff);
							         
	sprintf(buff, "butt:%s  ", byte_to_binary(button));						
	uart_send_str(buff);		
	
	sprintf(buff, "tmp_t:%s  ", byte_to_binary(1<<temp_tast));
	uart_send_str(buff);	
	
	sprintf(buff, "flag_stis:%d  ", flag_stisnut);
	uart_send_str(buff);		
	uart_send_str("\n");
	*/
	
	return stisnut;
}