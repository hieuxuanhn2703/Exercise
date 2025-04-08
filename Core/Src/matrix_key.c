/*
 * matrix_key.c
 *
 *  Created on: Mar 24, 2025
 *      Author: P.HIEU
 */
#include "matrix_key.h"

#define ROW1      0
#define ROW2      1
#define ROW3      2
#define ROW4      3
#define COLUMN1   4
#define COLUMN2   5
#define COLUMN3   6
#define COLUMN4   7

void gpiob_writebit(int pin, uint8_t State){
	if (State) {
	        GPIOB_ODR |= (1 << pin);
	} else {
	        GPIOB_ODR &= ~(1 << pin);
	}
}

bool gpiob_readbit(int pin){
	if (GPIOB_IDR & (1 << pin)) {
	        return true;
	} else {
	        return false;
	}
}

void matrix_key_init(void){
	//Turn on clock for I2C1 and GPIOB
	RCC_APB2ENR |= RCC_APB2ENR_IOPBENa;

	//configure GPIOB
	GPIOB_CRL &= ~((0xF << (0 * 4)) | (0xF << (1 * 4)) | (0xF << (2 * 4)) | (0xF << (3 * 4))
				     | (0xF << (4 * 4)) | (0xF << (5 * 4)) | (0xF << (6 * 4)) | (0xF << (7 * 4)));
	GPIOB_CRL |= ((0x1 << (0 * 4)) | (0x1 << (1 * 4)) | (0x1 << (2 * 4)) | (0x1 << (3 * 4)));//output push_pull for rows
	GPIOB_CRL |= ((0x8 << (4 * 4)) | (0x8 << (5 * 4)) | (0x8 << (6 * 4)) | (0x8 << (7 * 4)));//input for rows
	GPIOB_ODR |= ((0x1 << 4) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7));//input with pull-up
}

char read_keypad(void){
	/* Make ROW 1 LOW and all other ROWs HIGH */
	gpiob_writebit(ROW1, 0);
	gpiob_writebit(ROW2, 1);
	gpiob_writebit(ROW3, 1);
	gpiob_writebit(ROW4, 1);

	if (!(gpiob_readbit(COLUMN1)))   // if the Col 1 is low
	{
		while (!(gpiob_readbit(COLUMN1)));   // wait till the button is pressed
		return '7';
	}

	if (!(gpiob_readbit(COLUMN2)))   // if the Col 2 is low
	{
		while (!(gpiob_readbit(COLUMN2)));   // wait till the button is pressed
		return '8';
	}

	if (!(gpiob_readbit(COLUMN3)))   // if the Col 3 is low
	{
		while (!(gpiob_readbit(COLUMN3)));   // wait till the button is pressed
		return '9';
	}

	if (!(gpiob_readbit(COLUMN4)))   // if the Col 4 is low
	{
		while (!(gpiob_readbit(COLUMN4)));   // wait till the button is pressed
		return '%';
	}

	/* Make ROW 1 LOW and all other ROWs HIGH */
	gpiob_writebit(ROW1, 1);
	gpiob_writebit(ROW2, 0);
	gpiob_writebit(ROW3, 1);
	gpiob_writebit(ROW4, 1);

	if (!(gpiob_readbit(COLUMN1)))   // if the Col 1 is low
	{
		while (!(gpiob_readbit(COLUMN1)));   // wait till the button is pressed
		return '4';
	}

	if (!(gpiob_readbit(COLUMN2)))   // if the Col 2 is low
	{
		while (!(gpiob_readbit(COLUMN2)));   // wait till the button is pressed
		return '5';
	}

	if (!(gpiob_readbit(COLUMN3)))   // if the Col 3 is low
	{
		while (!(gpiob_readbit(COLUMN3)));   // wait till the button is pressed
		return '6';
	}

	if (!(gpiob_readbit(COLUMN4)))   // if the Col 4 is low
	{
		while (!(gpiob_readbit(COLUMN4)));   // wait till the button is pressed
		return 'X';
	}

	/* Make ROW 1 LOW and all other ROWs HIGH */
	gpiob_writebit(ROW1, 1);
	gpiob_writebit(ROW2, 1);
	gpiob_writebit(ROW3, 0);
	gpiob_writebit(ROW4, 1);

	if (!(gpiob_readbit(COLUMN1)))   // if the Col 1 is low
	{
		while (!(gpiob_readbit(COLUMN1)));   // wait till the button is pressed
		return '1';
	}

	if (!(gpiob_readbit(COLUMN2)))   // if the Col 2 is low
	{
		while (!(gpiob_readbit(COLUMN2)));   // wait till the button is pressed
		return '2';
	}

	if (!(gpiob_readbit(COLUMN3)))   // if the Col 3 is low
	{
		while (!(gpiob_readbit(COLUMN3)));   // wait till the button is pressed
		return '3';
	}

	if (!(gpiob_readbit(COLUMN4)))   // if the Col 4 is low
	{
		while (!(gpiob_readbit(COLUMN4)));   // wait till the button is pressed
		return '-';
	}

	/* Make ROW 1 LOW and all other ROWs HIGH */
	gpiob_writebit(ROW1, 1);
	gpiob_writebit(ROW2, 1);
	gpiob_writebit(ROW3, 1);
	gpiob_writebit(ROW4, 0);

	if (!(gpiob_readbit(COLUMN1)))   // if the Col 1 is low
	{
		while (!(gpiob_readbit(COLUMN1)));   // wait till the button is pressed
		return 'C';
	}

	if (!(gpiob_readbit(COLUMN2)))   // if the Col 2 is low
	{
		while (!(gpiob_readbit(COLUMN2)));   // wait till the button is pressed
		return '0';
	}

	if (!(gpiob_readbit(COLUMN3)))   // if the Col 3 is low
	{
		while (!(gpiob_readbit(COLUMN3)));   // wait till the button is pressed
		return '=';
	}

	if (!(gpiob_readbit(COLUMN4)))   // if the Col 4 is low
	{
		while (!(gpiob_readbit(COLUMN4)));   // wait till the button is pressed
		return '+';
	}

	return 0x01;  // /if nothing is pressed
}

