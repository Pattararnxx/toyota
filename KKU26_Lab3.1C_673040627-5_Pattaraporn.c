#include <stdint.h>
#define STM32F411xE
#include "stm32f4xx.h"

/* --- Constant Declaration --- */
#define DELAY_THRESHOLD   (1600000U)   // ปรับตัวเลขนี้เพื่อ tune ให้ได้ ~1 วินาทีจริง
#define UART_DATA_ASCII   (75U)        // 'K'

/* --- Macro Declaration --- */
#define DELAY_SEC for (uint32_t iter = 0; iter < DELAY_THRESHOLD; iter++)

int main(void) {
    /* --- Setup Peripheral Clock --- */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;     // เปิด clock GPIOA
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;    // เปิด clock USART2

    /* --- Setup PA2 to AF07 (USART2 Tx) --- */
    GPIOA->MODER   &= ~GPIO_MODER_MODER2;
    GPIOA->MODER   |= (2 << GPIO_MODER_MODER2_Pos);   // 10 = Alternate Function mode
    GPIOA->OTYPER  &= ~GPIO_OTYPER_OT2;                // push-pull
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED2;
    GPIOA->AFR[0]  &= ~GPIO_AFRL_AFRL2;
    GPIOA->AFR[0]  |= (7 << GPIO_AFRL_AFSEL2_Pos);     // AF07 = USART2_TX (ดูตาราง Alternate Function)

    /* --- Setup USART2 --- */
    USART2->CR1 |= USART_CR1_UE;      // 1) เปิดใช้งาน USART
    USART2->CR1 &= ~USART_CR1_M;      // 2) 8-bit data
    USART2->CR2 &= ~USART_CR2_STOP;   // 3) 1 stop bit
    USART2->BRR  = 139;               // 5) baud rate = 115200 (16MHz/139≈115200)
    USART2->CR1 |= USART_CR1_TE;      // 6) เปิด transmitter

    while (1) {
        USART2->DR = UART_DATA_ASCII; // 7) เขียนข้อมูลลง DR เพื่อส่งออก
        DELAY_SEC;
    }
}
