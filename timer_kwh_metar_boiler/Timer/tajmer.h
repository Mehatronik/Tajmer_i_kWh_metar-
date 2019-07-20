/*
 * tajmer.h
 *
 * Created: 2.12.2017. 22.09.10
 *  Author: Kovacevic
 */ 


#ifndef TAJMER_H_
#define TAJMER_H_

#include <inttypes.h>

#define DEBOUNCE_TIME 6					//6*1ms vezano je za osnovni tajmer prekid, tj za brojanje prekida
#define DEBOUNCE_TIME_half DEBOUNCE_TIME/2	//3ms

extern volatile uint8_t flag_tajmer0_prekid;
extern volatile uint8_t flag_prekid_50ms;
//extern volatile uint8_t brojac_prekida_tajmera0;
extern volatile uint8_t flag_prekid_debounce_time;
extern volatile uint8_t flag_prekid_debounce_time_half;
extern volatile uint16_t delay_timer;
extern volatile uint16_t timer_disp_cycle;
extern volatile uint8_t flag_prekid_100ms_VAkWh;

extern void tajmer0_init();	//prototip funkcije



#endif /* TAJMER_H_ */