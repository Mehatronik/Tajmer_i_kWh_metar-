/*
 * ad_konverzija.c
 *
 * Created: 1.12.2017. 22.09.18
 *  Author: Ko''
 */ 

#include "comm.h"
#include "ad_konverzija.h"

/* RAW vrednosti sa AD ulaza */
uint16_t adc_napon_raw = 0;			
uint16_t adc_struja_raw = 0;			

/* obradjene vrednosti */
uint16_t napon = 0;		//celobrojno u V
float struja = 0;		//realno u A
float snaga = 0;		//realno u kW
float energija = 0;		//realno u kWh

uint8_t ad_kanal = 0;
volatile uint8_t isr_adc = 0;


void ADC_init()
{
	
	PRR = 0;						//power reduction off
	
	ADMUX = 0b11000000;		        //ref internal 1.1V, kanal A0
	ADCSRA = 0b11101110;			//ADC enable, start conversion, auto trigger enable, ADC conv. complete interrupt enable, 64 prescaler = 250kHz, a preporuka je do 200kHz
	ADCSRB = 0b0;					//0b11
	
	ad_kanal = 0; //prvo citam A0
	
}


/* TODO: u ISR-u neka bude samo flag, a obrada podataka izdvojeno, jer je ovo glup nacin, da racun radim u prekidnoj rutini */

ISR(ADC_vect)
{
	/*
	 * AD konverzija
	 * ISR okine kada je gotova konverzija
	 */
	isr_adc = 1;	//set flag

}

void adc_read()
{
	
	if (isr_adc)
	{
		isr_adc = 0;	//reset flag
	
	
		//upis ad konverzije oba kanala u odgovarajuce promenljive
		if(ad_kanal == 0)
			adc_napon_raw = (uint16_t)ADC;		//potrebno je jos skalirati u zavisnosti od Vref i ulaznog napona i dole isto za struju
		else if(ad_kanal == 1)
			adc_struja_raw = (uint16_t)ADC;
	
		//npr: napon 1023 = 280V
		//     struja 1023 = 25A
		napon = adc_napon_raw/3.65;
		struja = adc_struja_raw/40.92;
		snaga = (napon * struja)/1000.0;	//kW
		//snaga se integrali(sumira) svaki sekund u main-u cime se racuna elektricna energija
		
		/**************************************************************************************************************/
		ADCSRA &= ~(1<<ADEN);	//ISKLJUCIM ADC da bi promena u ADMUX bila sigurna, po preporuci iz datasheet-a
	
	
		ad_kanal++;			//inkrementiraj kanal
		if(ad_kanal > 1)	//kreni opet od nule kad dodjes do poslednjeg; 1, da citam samo prva dva
			ad_kanal = 0;
	


	
		//multipleksiranje ad ulaza; tj. promena ad kanala
		switch(ad_kanal)
		{
			case 0:
			ADMUX &= ~(0b11);		//00;ref internal 1.1V, kanal A0
			break;
		
			case 1:
			ADMUX &= ~(0b10);		//01;ref internal 1.1V, kanal A1
			ADMUX |= (0b1);
			break;
		
			//case 2:
			//ADMUX &= ~(0b1);		//10;ref internal 1.1V, kanal A2
			//ADMUX |= (0b10);
			//break;
			//
			//case 3:
			//ADMUX |= (0b11);		//11;ref internal 1.1V, kanal A3
			//break;
		
			default: {}
		
			//postojao je problem ako hocu da menjam i referencu kada prebacujem kanal, da mi daje bezveze rezultate.
			//mislim da u datasheetu pise da prvo citanje posle menjanja reference moze biti lose
		}
	
	
		//napomena: promena ulaza se vrsi ovde, a kad naredni put uleti u ISR vrsi se konverzija tog kanala.
	
		ADCSRA |= (1<<ADEN)|(1<<ADSC);	//ponovo dozvolim adc posle promene u ADMUX i pokrenem opet prvu konverziju da bi htelo da radi u Free running
		
	}
}

