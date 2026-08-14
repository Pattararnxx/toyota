#include <stdint.h>
#include <stdio.h>
#define STM32F411xE
#include "stm32f4xx.h"

char stringOut[50];

static void UART2_TxString(char strOut[]);

int main(void) {
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN);
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* --- Setup PB6 LED --- */
    GPIOB->MODER &= ~GPIO_MODER_MODER6;
    GPIOB->MODER |= (1 << GPIO_MODER_MODER6_Pos);

    /* --- Setup USART2 Tx (PA2) / Rx (PA3) --- */
    GPIOA->MODER   &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER   |= (2 << GPIO_MODER_MODER2_Pos) | (2 << GPIO_MODER_MODER3_Pos);
    GPIOA->OTYPER  &= ~(GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3);
    GPIOA->AFR[0]  &= ~(GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3);
    GPIOA->AFR[0]  |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);

    /* --- Setup USART2 --- */
    USART2->CR1 |= USART_CR1_UE;
    USART2->CR1 &= ~USART_CR1_M;
    USART2->CR2 &= ~USART_CR2_STOP;
    USART2->BRR  = 139;
    USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);   // เปิดทั้งส่งและรับ

    while (1) {
        if ((USART2->SR & USART_SR_RXNE) != 0) {
            char received = (char)USART2->DR;   // อ่านค่าที่รับมาเก็บไว้ก่อน (ป้องกันอ่านซ้ำผิดค่า)

            if (received == '1') {
                GPIOB->ODR |= GPIO_ODR_OD6;
            } else {
                GPIOB->ODR &= ~GPIO_ODR_OD6;
            }

            sprintf(stringOut, "You've entered: %c\n\r", received);
            UART2_TxString(stringOut);
        }
    }
}

static void UART2_TxString(char strOut[]) {
    for (uint8_t idx = 0; strOut[idx] != '\0'; idx++) {
        while ((USART2->SR & USART_SR_TXE) == 0);
        USART2->DR = strOut[idx];
    }
}
