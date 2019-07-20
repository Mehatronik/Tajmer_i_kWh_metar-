/*
 * pin_change_interrupt.h
 *
 * Created: 28.4.2019. 17.30.21
 *  Author: Kovacevic
 */ 


#ifndef PIN_CHANGE_INTERRUPT_H_
#define PIN_CHANGE_INTERRUPT_H_

extern void pc_init();

extern volatile uint8_t flag_pc_int;
extern volatile uint8_t flag_pc_int_pomocni;

#endif /* PIN_CHANGE_INTERRUPT_H_ */