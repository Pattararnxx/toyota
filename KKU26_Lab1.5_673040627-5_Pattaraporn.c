#include <stdint.h>
#define STM32F411xE
#include "stm32f4xx.h"

#define DELAY_THRESHOLD (1333333)   // toggle ทุก 1 วิ -> ครบรอบ = 2 วิ = 0.5Hz

int main(void) {
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN);

    /* --- Setup PA5 LED (กระพริบ) --- */
    GPIOA->MODER   &= ~GPIO_MODER_MODER5;
    GPIOA->MODER   |=  (0b01 << GPIO_MODER_MODER5_Pos);
    GPIOA->OTYPER  &= ~GPIO_OTYPER_OT5;
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5;

    /* --- Setup PB6 LED (ตามปุ่ม) --- */
    GPIOB->MODER   &= ~GPIO_MODER_MODER6;
    GPIOB->MODER   |=  (0b01 << GPIO_MODER_MODER6_Pos);
    GPIOB->OTYPER  &= ~GPIO_OTYPER_OT6;
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED6;

    /* --- Setup PB4 BTN --- */
    GPIOB->MODER &= ~GPIO_MODER_MODER4;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4;
    GPIOB->PUPDR |= (0b01 << GPIO_PUPDR_PUPD4_Pos);

    while (1) {
        if ((GPIOB->IDR & GPIO_IDR_ID4) == 0) {
            GPIOB->ODR |= GPIO_ODR_OD6;
        } else {
            GPIOB->ODR &= ~GPIO_ODR_OD6;
        }

        GPIOA->ODR ^= GPIO_ODR_OD5;   // toggle ไฟ PA5

        for (uint32_t iter = 0; iter < DELAY_THRESHOLD; iter++) {
            /* No Action */
        }
    }
    return 0;
}
