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

#define eeprom_I2C_address 0x57		//Atmel AT24C32; 32Kbit = 4096 x 8bit = 4Kbyte;	1 million write cycles, 100 years data retention

#define UPALI_HR_ADR   0
#define UPALI_MIN_ADR  1
#define UGASI_HR_ADR   2
#define UGASI_MIN_ADR  3

#define JEDNOK_UPALI_HR_ADR  4
#define JEDNOK_UPALI_MIN_ADR 5
#define JEDNOK_UGASI_HR_ADR  6
#define JEDNOK_UGASI_MIN_ADR 7

/* 4 bajta jer je kWh float, a float je 4 bajta dugacak na AVR-u */
#define KWH_BYTE0_ADR 8
#define KWH_BYTE1_ADR 9
#define KWH_BYTE2_ADR 10
#define KWH_BYTE3_ADR 11

#define BROJILO_ONOF_ADR 12

extern uint8_t EEPROM_read(uint16_t uiAddress);
extern void EEPROM_write(uint16_t uiAddress, uint8_t ucData);


#endif /* EEPROM_H_ */