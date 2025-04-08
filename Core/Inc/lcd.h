/*
 * lcd.h
 *
 *  Created on: Mar 24, 2025
 *      Author: P.HIEU
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include "config.h"

void delay_ms(uint32_t ms);

void gpio_writebit(int pin, uint8_t State);

void GPIO_LCD_Config(void);

void LCD_Enable(void);

void LCD_Send4Bit(unsigned char Data);

void LCD_SendCommand(unsigned char command);

void LCD_Clear();

void LCD_Init();

void LCD_Gotoxy(unsigned char x, unsigned char y);

void LCD_PutChar(unsigned char Data);

void LCD_Puts(char *s);

void LCD_Menu(int a);

#endif /* LCD_H_ */
