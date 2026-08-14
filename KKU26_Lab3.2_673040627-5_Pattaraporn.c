#include <stdint.h>
#define STM32F411xE
#include "stm32f4xx.h"

#define DELAY_THRESHOLD (1600000U)
#define DELAY_SEC for (uint32_t iter = 0; iter < DELAY_THRESHOLD; iter++)

char stringOut[50] = "Hello world!\n\r";

static void UART2_TxString(char strOut[]);

int main(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER   &= ~GPIO_MODER_MODER2;
    GPIOA->MODER   |= (2 << GPIO_MODER_MODER2_Pos);
    GPIOA->OTYPER  &= ~GPIO_OTYPER_OT2;
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED2;
    GPIOA->AFR[0]  &= ~GPIO_AFRL_AFRL2;
    GPIOA->AFR[0]  |= (7 << GPIO_AFRL_AFSEL2_Pos);

    USART2->CR1 |= USART_CR1_UE;
    USART2->CR1 &= ~USART_CR1_M;
    USART2->CR2 &= ~USART_CR2_STOP;
    USART2->BRR  = 139;
    USART2->CR1 |= USART_CR1_TE;

    while (1) {
        UART2_TxString(stringOut);
        DELAY_SEC;
    }
}

/* ส่งข้อความทีละตัวจนเจอ null terminator '\0' */
static void UART2_TxString(char strOut[]) {
    for (uint8_t idx = 0; strOut[idx] != '\0'; idx++) {
        while ((USART2->SR & USART_SR_TXE) == 0);  // รอจน TXE=1 (ส่งตัวก่อนหน้าเสร็จแล้ว)
        USART2->DR = strOut[idx];                    // ค่อยเขียนตัวถัดไป
    }
}
