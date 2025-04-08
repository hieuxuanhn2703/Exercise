/*
 * uart.h
 *
 *  Created on: Mar 21, 2025
 *      Author: P.HIEU
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "config.h"
void uart2_config(void);//8mHz, baudrate=9600

void uart2_gpio(void);

void uart_sendchar(uint8_t data);

void uart_sendstring(const char *str);


#endif /* UART_H_ */

