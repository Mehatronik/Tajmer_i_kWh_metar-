/*
 * eeprom.c
 *
 * Created: 14.5.2019. 08.12.30
 *  Author: Kovacrvic
 */ 

#include "eeprom.h"



uint8_t EEPROM_read(uint16_t uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	
	/* Set up address register */
	EEAR = uiAddress;
	
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	
	/* Return data from Data Register */
	return EEDR;
}

void EEPROM_write(uint16_t uiAddress, uint8_t ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	
	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;
	
	/**********************!!!!!!!!!!!!!!!!!!!!!!***************************/
	cli();									//zabrana interapta, po preporuci iz datasheet-a, doci ce do write-faila ako se desi interapt izmedju ova dva koraka
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
	
	sei();									//dozvola interapta
	/**********************!!!!!!!!!!!!!!!!!!!!!!***************************/
}