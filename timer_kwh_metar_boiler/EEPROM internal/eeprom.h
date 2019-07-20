/*
 * eeprom.h
 *
 * Created: 14.5.2019. 08.12.01
 *  Author: Kovacevic
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

/* Citanje i pisanje EEPROM-a koji se nalazi unutar uC-a ATmega 328p.
 * Velicine je 1024 byte-a, tj. 1024 adrese * 8 bita svaka vrednost.
 * Funkcije za citanje i pisanje su preuzete iz datasheet-a sa minornom
 * modifikacijom
 */


#include "comm.h"

#define UPALI_HR_ADR   0
#define UPALI_MIN_ADR  1
#define UGASI_HR_ADR   2
#define UGASI_MIN_ADR  3

#define JEDNOK_UPALI_HR_ADR  4
#define JEDNOK_UPALI_MIN_ADR 5
#define JEDNOK_UGASI_HR_ADR  6
#define JEDNOK_UGASI_MIN_ADR 7

extern uint8_t EEPROM_read(uint16_t uiAddress);
extern void EEPROM_write(uint16_t uiAddress, uint8_t ucData);


#endif /* EEPROM_H_ */