/*
 * pwm.c
 *
 *  Created on: Mar 26, 2025
 *      Author: P.HIEU
 */
#include "pwm.h"

void pwm_init(void) {

    RCC_APB1ENR |= RCC_APB1ENR_TIM2ENa;  // Bật clock cho TIM2
    RCC_APB2ENR |= RCC_APB2ENR_IOPAENa; // Bật clock cho GPIOA

    /* 2. Cấu hình PA0 là chân đầu ra PWM (AF1 - Alternate Function) */
    GPIOA_CRL &= ~(0xF << (0 * 4)); // Xóa cấu hình cũ
    GPIOA_CRL |= (0x9 << (0 * 4));  // Cấu hình PA0 (AF Push-Pull, max speed 50MHz)

    /* 3. Cấu hình Timer 2 */
    TIM2_PSC = 160 - 1;    // Chia clock xuống 50kHz
    TIM2_ARR = 1000 - 1;  // (tần số PWM = 50Hz)
//    TIM2_CCR1 = 1000;      // Độ rộng xung 50%

    /* 4. Chọn chế độ PWM Mode 1 */
    TIM2_CCMR1 |= TIM2_CCMR1_OC1M_Pos; // Chọn PWM Mode 1
    TIM2_CCMR1 |= TIM2_CCMR1_OC1PE; // Bật preload (CC1 được cập nhật khi có sự kiện cập nhật)

    /* 5. Bật kênh PWM */
    TIM2_CCER |= TIM2_CCER_CC1E; // Bật đầu ra PWM trên kênh CC1

    /* 6. Khởi động Timer */
//    TIM2_CR1 |= TIM2_CR1_CEN; // Bắt đầu Timer
}

