#include <stdint.h>
#define STM32F411xE
#include "stm32f4xx.h"

void USART2_IRQHandler(void) {
    if ((USART2->SR & USART_SR_RXNE) != 0) {

        char data = USART2->DR;

        if (data == '1') {
            GPIOA->ODR |= GPIO_ODR_OD5;
        } else {
            GPIOA->ODR &= ~(GPIO_ODR_OD5);
        }
    }
}

int main(void) {

    /* --- Setup peripheral clock --- */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* --- Setup GPIO PA5 --- */
    GPIOA->MODER &= ~(GPIO_MODER_MODER5);
    GPIOA->MODER |= (0b01 << GPIO_MODER_MODER5_Pos);

    /* --- Setup GPIO PA2, PA3 --- */
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (0b10 << GPIO_MODER_MODER2_Pos)
                  | (0b10 << GPIO_MODER_MODER3_Pos);

    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3);
    GPIOA->AFR[0] |= (0b0111 << GPIO_AFRL_AFSEL2_Pos)
                   | (0b0111 << GPIO_AFRL_AFSEL3_Pos);

    /* --- USART2 Setup --- */
    USART2->CR1 |= USART_CR1_UE;
    USART2->CR1 &= ~USART_CR1_M;
    USART2->CR2 &= ~USART_CR2_STOP;
    USART2->BRR = 139;

    USART2->CR1 |= USART_CR1_RXNEIE;
    USART2->CR1 |= USART_CR1_RE;

    /* --- Enable USART2 interrupt --- */
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 0);

    while(1) {

    }
}
