#include <stdint.h>
#define STM32F411xE
#include "stm32f4xx.h"

#define DELAY_THRESHOLD (1600000U)
#define DELAY_SEC for (uint32_t iter = 0; iter < DELAY_THRESHOLD; iter++)

int main(void) {
    /* --- Setup peripheral clock --- */
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN);
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* --- Setup PA5 LED (Blue) --- */
    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOA->MODER |= (1 << GPIO_MODER_MODER5_Pos);   // 01 = output mode

    /* --- Setup PB6 LED (Green) --- */
    GPIOB->MODER &= ~GPIO_MODER_MODER6;
    GPIOB->MODER |= (1 << GPIO_MODER_MODER6_Pos);

    /* --- Setup USART2 Rx Pin (PA3) --- */
    GPIOA->MODER   &= ~GPIO_MODER_MODER3;
    GPIOA->MODER   |= (2 << GPIO_MODER_MODER3_Pos);  // AF mode
    GPIOA->OTYPER  &= ~GPIO_OTYPER_OT3;
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED3;
    GPIOA->AFR[0]  &= ~GPIO_AFRL_AFRL3;
    GPIOA->AFR[0]  |= (7 << GPIO_AFRL_AFSEL3_Pos);   // AF07 = USART2_RX (ตามตาราง PA3)

    /* --- Setup USART2 --- */
    USART2->CR1 |= USART_CR1_UE;
    USART2->CR1 &= ~USART_CR1_M;
    USART2->CR2 &= ~USART_CR2_STOP;
    USART2->BRR  = 139;
    USART2->CR1 |= USART_CR1_RE;      // เปิดตัวรับ (Receiver Enable)

    while (1) {
        if ((USART2->SR & USART_SR_RXNE) != 0) {      // มีข้อมูลมาใหม่หรือยัง
            if (USART2->DR == '1') {
                GPIOA->ODR |= GPIO_ODR_OD5;            // เปิดไฟ Blue
            } else if (USART2->DR == 'G') {
                GPIOB->ODR ^= GPIO_ODR_OD6;             // toggle ไฟ Green
            } else {
                GPIOA->ODR &= ~GPIO_ODR_OD5;            // ตัวอักษรอื่น → ปิดไฟ Blue
            }
        }
        DELAY_SEC;
    }
}
