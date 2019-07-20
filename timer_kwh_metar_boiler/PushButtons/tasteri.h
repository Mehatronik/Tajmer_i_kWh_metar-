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
#define TASTER_LEVO_pinPort		PIND4
#define TASTER_DESNO_pinPort	PIND5
#define TASTER_GORE_pinPort		PIND6
#define TASTER_DOLE_pinPort		PIND7
#define TASTER_NAZAD_pinPort	PINB0
#define TASTER_ENTER_pinPort	PINB1

/* definicije pinova vezane za pinove u promenljivoj u koju smestam njihovo stanje */
#define TASTER_LEVO		0
#define TASTER_DESNO	1
#define TASTER_GORE		2
#define TASTER_DOLE		3
#define TASTER_NAZAD	4
#define TASTER_ENTER	5


extern void tasteri_init();
extern uint8_t ocitaj_tastere();
extern uint8_t ocitaj_jedan_taster(uint8_t buttons_reg, uint8_t button);

#endif /* TASTERI_H_ */