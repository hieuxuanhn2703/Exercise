/*
 * matrix_key.h
 *
 *  Created on: Mar 24, 2025
 *      Author: P.HIEU
 */

#ifndef MATRIX_KEY_H_
#define MATRIX_KEY_H_

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

void gpiob_writebit(int pin, uint8_t State);

bool gpiob_readbit(int pin);

void matrix_key_init(void);

char read_keypad(void);

#endif /* MATRIX_KEY_H_ */
