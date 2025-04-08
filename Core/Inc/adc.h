/*
 * adc.h
 *
 *  Created on: Mar 21, 2025
 *      Author: P.HIEU
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <math.h>
#include "config.h"

void adc_config(void);

uint16_t adc_read(void);

float calculate_temperature(void);
#endif /* ADC_H_ */
