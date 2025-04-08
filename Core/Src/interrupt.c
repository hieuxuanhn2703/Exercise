/*
 * interrupt.c
 *
 *  Created on: Mar 26, 2025
 *      Author: P.HIEU
 */
#include "interrupt.h"

void EXTI0_Init(void){
	EXTI_IMR |= (1 << 0);
	NVIC_ISER0 |= (1 << 6);
}

void Timer3_Init(void) {
    RCC_APB1ENR |= RCC_APB1ENR_TIM3ENa;  //turn on clock for TIM3

    TIM3_PSC = 8000 - 1;    //  1kHz
    TIM3_ARR = 1000 - 1;    //1s
    TIM3_CNT = 0;
    TIM3_DIER |= TIM3_DIER_UIE;
    TIM3_SR &= ~(1 << 0);
    NVIC_ISER0 |= (1 << 29);
//    NVIC_IPR7 |= (1 << (1 * 8 + 4));
    TIM3_CR1 |= TIM3_CR1_CEN; // start Timer
}

void Timer4_Init(void) {
	GPIOA_CRL &= ~(0xF << 4 * 4);
	GPIOA_CRL &= ~(0xF << 5 * 4);
	GPIOA_CRL |= (1 << 4 * 4);
	GPIOA_CRL |= (1 << 5 * 4);
    RCC_APB1ENR |= RCC_APB1ENR_TIM4ENa;  //turn on clock for TIM4
    TIM4_PSC = 8000 - 1;    // 1kHz
    TIM4_ARR = 1000 - 1;    //1s
    TIM4_CNT = 0;
    TIM4_DIER |= TIM4_DIER_UIE;
    TIM4_SR &= ~(1 << 0);
//    NVIC_IPR7 |= (10 << (2 * 8 + 4));
    NVIC_ISER0 |= (1 << 30);

//    TIM4_CR1 |= TIM4_CR1_CEN; // startTimer
}

void Timer1_Init(void) {
    RCC_APB2ENR |= RCC_APB2ENR_TIM1ENa;  // turn on clock for TIM1

    TIM1_PSC = 8000 - 1;    // 1kHz
    TIM1_ARR = 5000 - 1;    //5s
    TIM1_CNT = 0;
    TIM1_SR &= ~(1 << 0);
}
