#include <stdint.h>

/* ----- Register Definition ----- */
#define GPIOA_Base       0x40020000
#define GPIOA_MODER      (*(volatile uint32_t*)(GPIOA_Base + 0x00))
#define GPIOA_OTYPER     (*(volatile uint32_t*)(GPIOA_Base + 0x04))
#define GPIOA_OSPEEDR    (*(volatile uint32_t*)(GPIOA_Base + 0x08))
#define GPIOA_ODR        (*(volatile uint32_t*)(GPIOA_Base + 0x14))

int main(void) {
    (*(volatile uint32_t*) 0x40023830) |= 0b1;   // เปิด clock ให้ GPIOA

    /* --- Setup PA5 LED --- */
    GPIOA_MODER   &= ~(0b11 << 10);
    GPIOA_MODER   |=  (0b01 << 10);   // ตั้ง PA5 เป็น output
    GPIOA_OTYPER  &= ~(0b1  << 5);    // push-pull
    GPIOA_OSPEEDR &= ~(0b11 << 10);   // low speed

    /* --- Turn on PA5 LED --- */
    GPIOA_ODR |= (0b1 << 5);          // PA5 = HIGH (ไฟติด)

    while (1) { /* No Action */ }
    return 0;
}
