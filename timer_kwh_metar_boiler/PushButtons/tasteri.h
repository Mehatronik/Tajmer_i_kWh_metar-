/*
 * tasteri.h
 *
 * Created: 30.4.2019. 14.14.43
 *  Author: Kovacevic
 */ 


#ifndef TASTERI_H_
#define TASTERI_H_

#include <inttypes.h>

/* definicije pinova vezane za pinove u odg. portovima */
#define TASTER_NAZAD_pinPort	PIND2
#define TASTER_ENTER_pinPort	PIND3
#define TASTER_LEVO_pinPort		PIND4
#define TASTER_DESNO_pinPort	PIND5
#define TASTER_GORE_pinPort		PIND6
#define TASTER_DOLE_pinPort		PIND7
#define TASTER_NONSTOP_pinPort	PINB0

/* definicije pinova vezane za pinove u promenljivoj u koju smestam njihovo stanje */
#define TASTER_NAZAD	0
#define TASTER_ENTER	1
#define TASTER_LEVO		2
#define TASTER_DESNO	3
#define TASTER_GORE		4
#define TASTER_DOLE		5
#define TASTER_NONSTOP	6

/* makro za zbijanje dva registra sa tasterima u jedan, prosto shift-ovanje.
 *         7   6   5   4   3   2   1   0
 * PIND = LEV DES GOR DOL ENT NAZ  X   X		>>2
 *
 *         7   6   5   4   3   2   1   0
 * PINB =  X   X   X   X   X   X   X   NS		<<6
 *
 *                 7   6   5   4   3   2   1   0 
 * temp_reg_read = X  NS  LEV DES GOR DOL ENT NAZ
 *
 */
#define TEMP_REG_READ   (PIND>>2)|(PINB<<6)



extern void tasteri_init();
extern uint8_t ocitaj_tastere();
extern uint8_t ocitaj_jedan_taster(uint8_t buttons_reg, uint8_t button);

#endif /* TASTERI_H_ */