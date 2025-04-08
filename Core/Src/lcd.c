/*
 * lcd.c
 *
 *  Created on: Mar 24, 2025
 *      Author: P.HIEU
 */

#include "lcd.h"

#define LCD_RS      8
#define LCD_RW      9
#define LCD_EN      10


#define LCD_D4      11
#define LCD_D5      12
#define LCD_D6      13
#define LCD_D7      14

void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < (2000 * ms); i++) {
    }
}

void gpio_writebit(int pin, uint8_t State){
	if (State) {
	        GPIOA_ODR |= (1 << pin);
	} else {
	        GPIOA_ODR &= ~(1 << pin);
	}
}

void GPIO_LCD_Config(void)
{
	//configure GPIOA
	RCC_APB2ENR |= RCC_APB2ENR_IOPAENa;
	GPIOA_CRH &= ~((0xF << (0 * 4)) | (0xF << (1 * 4)) | (0xF << (2 * 4)) | (0xF << (3 * 4))
			     | (0xF << (4 * 4)) | (0xF << (5 * 4)) | (0xF << (6 * 4)));
	GPIOA_CRH |= ((0x1 << (0 * 4)) | (0x1 << (1 * 4)) | (0x1 << (2 * 4)) | (0x1 << (3 * 4))
				     | (0x1 << (4 * 4)) | (0x1 << (5 * 4)) | (0x1 << (6 * 4)));

    /*Configure GPIO pin Output Level */
    gpio_writebit(LCD_D4, 0);
    gpio_writebit(LCD_D5, 0);
    gpio_writebit(LCD_D6, 0);
    gpio_writebit(LCD_D7, 0);
    gpio_writebit(LCD_RS, 0);
    gpio_writebit(LCD_RW, 0);
    gpio_writebit(LCD_EN, 0);

}
void LCD_Enable(void)
{
    gpio_writebit(LCD_EN, 1);
//    delay_ms(1);
    gpio_writebit(LCD_EN, 0);
//    delay_ms(1);
}


void LCD_Send4Bit(unsigned char Data)
{
  gpio_writebit(LCD_D4, Data & 0x01);
  gpio_writebit(LCD_D5, (Data>>1)&1);
  gpio_writebit(LCD_D6, (Data>>2)&1);
  gpio_writebit(LCD_D7, (Data>>3)&1);
}


void LCD_SendCommand(unsigned char command)
{
  LCD_Send4Bit(command >> 4);
  LCD_Enable();
  LCD_Send4Bit(command);
  LCD_Enable();


}


void LCD_Clear()
{
  LCD_SendCommand(0x01);
  delay_ms(1);
}


void LCD_Init()
{
  GPIO_LCD_Config();
  LCD_Send4Bit(0x00);
  gpio_writebit(LCD_RS, 0);
  LCD_Send4Bit(0x03);
  LCD_Enable();
  LCD_Enable();
  LCD_Enable();
  LCD_Send4Bit(0x02);
  LCD_Enable();
  LCD_SendCommand(0x28); //turn  on 4 bits protocol
  LCD_SendCommand(0x0E); //enable display
  LCD_SendCommand(0x06);
  LCD_SendCommand(0x01); // clear all
}


void LCD_Gotoxy(unsigned char x, unsigned char y)
{
  unsigned char address;
  if(y == 0)address=(0x80+x);
  else if(y == 1) address=(0xc0+x);
  else if(y == 2) address=(0x94+x);
  else if(y == 3) address=(0xD4+x);
  LCD_SendCommand(address);
}


void LCD_PutChar(unsigned char Data)
{
  gpio_writebit(LCD_RS, 1);
  LCD_SendCommand(Data);
  gpio_writebit(LCD_RS, 0);
}


void LCD_Puts(char *s)
{
  while (*s)
  {
    LCD_PutChar(*s);
    s++;
  }
}

void LCD_Menu (int a){
	LCD_Clear();
	LCD_Puts("--------Menu--------");
	LCD_Gotoxy(1,1);
	LCD_Puts("Change S1, T1");
	LCD_Gotoxy(1,2);
	LCD_Puts("Change S2, T2");
	LCD_Gotoxy(1,3);
	LCD_Puts("Change password");
	LCD_Gotoxy(0,1+a);
	LCD_PutChar('>');
}
