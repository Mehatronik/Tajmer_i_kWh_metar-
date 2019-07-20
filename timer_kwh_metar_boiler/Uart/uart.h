/*
 * uart.h
 *
 * Created: 1.12.2017. 23.00.49
 *  Author: Jovica
 *  Modified: Kovacevic
 */ 


#ifndef UART_H_
#define UART_H_

#define INPUT_BUFF_SIZE 8

/* prototipovi */

extern void uart_init(uint32_t baud_rate);
extern void uart_send_str(char *str);



#endif /* UART_H_ */