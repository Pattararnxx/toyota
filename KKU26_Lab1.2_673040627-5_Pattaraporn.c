#include <stdint.h>

#define GPIOA_Base       0x40020000
#define GPIOA_MODER      (*(volatile uint32_t*)(GPIOA_Base + 0x00))
#define GPIOA_OTYPER     (*(volatile uint32_t*)(GPIOA_Base + 0x04))
#define GPIOA_OSPEEDR    (*(volatile uint32_t*)(GPIOA_Base + 0x08))
#define GPIOA_ODR        (*(volatile uint32_t*)(GPIOA_Base + 0x14))

#define DELAY_THRESHOLD  (1333333)   // 16,000,000 / 12 ≈ 1,333,333

int main(void) {
    (*(volatile uint32_t*) 0x40023830) |= 0b1;

    GPIOA_MODER   &= ~(0b11 << 10);
    GPIOA_MODER   |=  (0b01 << 10);
    GPIOA_OTYPER  &= ~(0b1  << 5);
    GPIOA_OSPEEDR &= ~(0b11 << 10);

    while (1) {
        /* --- Toggle PA5 LED --- */
        GPIOA_ODR ^= (0b1 << 5);   // สลับสถานะ 0<->1

        for (uint32_t iter = 0; iter < DELAY_THRESHOLD; iter++) {
            /* No Action */
        }
    }
    return 0;
}
