/*
 * uart.c
 *
 *  Created on: Mar 21, 2025
 *      Author: P.HIEU
 */
# include "uart.h"

void uart2_config(void){
	//Enable clock for USART2
	RCC_APB1ENR |= RCC_APB1ENR_USART2ENa; // USART2EN = 1
	USART2_BRR = USART2_BRR_DIV_Mantissa | USART2_BRR_DIV_Fraction;

	//configure: 8bits, no parity, 1 stop bit
	USART2_CR1 = 0;
	USART2_CR1 |= USART_CR1_TEa | USART_CR1_UEa;

	USART2_CR2 = 0;
	USART2_CR2 |= USART_CR2_STOP_1BIT;

	USART2_CR3 = 0; // do not use hardware flow control
}

void uart2_gpio(void){
	RCC_APB2ENR |= RCC_APB2ENR_IOPAENa;
	// PA2 (TX) and PA3 (RX) to Alternate Function
	GPIOA_CRL &= ~(15 << (2 * 4));
	GPIOA_CRL |= (GPIO_CRL_CNF_MODE_2 << (2 * 4));
	GPIOA_CRL &= ~(15 << (3 * 4));
	GPIOA_CRL |= (GPIO_CRL_CNF_MODE_3 << (3 * 4));

}

//send a char
void uart_sendchar(uint8_t data){
	while (!(USART2_SR & USART_SR_TXEa));
	USART2_DR = data;
	while (!(USART2_SR & USART_SR_TCa));
}

//send a string
void uart_sendstring(const char *str){
	while(*str){
		uart_sendchar(*str++);
	}
}


