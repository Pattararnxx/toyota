# ติวเข้ม STM32F411RE Coding Exam (15–16 ส.ค. 2569)
### สรุปจาก Guide_Exam.pdf + Guideline_Checksheet.pdf + ตัวอย่างโค้ดจาก repo ของอาจารย์

---

## 0. กติกาข้อสอบ (อ่านก่อนเลย)

- มี **4 ข้อ เลือกทำแค่ 3 ข้อ**
  - **Q1: 7-Segment Control** — ใช้ GPIO (EXTI เป็น *optional*, ไม่ทำก็ได้)
  - **Q2: ADC + GPIO**
  - **Q3: UART (TX/RX) + ADC + GPIO**
  - **Q4: PWM (จะ implement วิธีไหนก็ได้) + ADC + GPIO**
- เปิดหนังสือ/เน็ต/เอกสารได้ทุกอย่าง **ห้ามถาม AI**
- คะแนนรวม 50 แบ่งเป็น 3 มุมมอง และเป็นแบบ **สะสม (cumulative)** คือถ้าอยากได้คะแนนระดับสูง ต้องผ่านเกณฑ์ระดับล่างก่อนทุกอัน (ดูหัวข้อ 1)

### กลยุทธ์เลือก 3 ข้อ (แนะนำ)
Q1 (7-Segment) ง่ายสุดและ EXTI ไม่บังคับ → **ทำแน่นอน**
Q2 (ADC+GPIO) เป็นฐานของ Q3/Q4 อยู่แล้ว → **ทำแน่นอน**
ข้อที่ 3 เลือกระหว่าง Q3 (UART) กับ Q4 (PWM): **ถ้าเวลาน้อย แนะนำ Q4 (PWM)** เพราะกลไกทางไฟฟ้า/timer ตรงไปตรงมากว่า ส่วน UART ต้องเข้าใจ baud rate, TX/RX protocol, polling/interrupt ซึ่งจุกจิกกว่าเวลาไม่พอ

---

## 1. เกณฑ์การให้คะแนน (คะแนนสะสม — สำคัญมาก)

| มุมมอง | เต็ม | ประเด็นหลัก |
|---|---|---|
| **Algorithm development** | 25 | โค้ดทำงานถูกตาม requirement กี่ % (100/80/60/40/20/0%) |
| **Program structure** | 15 | มี template → แยกไฟล์ main.c/.c/.h → 1 ฟังก์ชัน 1 หน้าที่ (Single Responsibility) → main สะอาด |
| **Software Quality** | 10 | ไม่ผิดกฎ MISRA C ทั้ง 22 ข้อ → คอมเมนต์ดี → ตั้งชื่อตัวแปร/ฟังก์ชันสื่อความหมาย |

**กฎทอง**: คะแนนเป็นแบบขั้นบันได ต้องผ่านขั้นล่างสุดก่อนถึงจะได้ขั้นสูงกว่า เช่น "Program structure" ถ้า main.c ยัดโค้ดทั้งหมดไว้ใน `int main()` เดียว (ไม่แยกฟังก์ชันเลย) จะได้ **0 คะแนน** ในหมวดนี้ทันที ต่อให้แยกไฟล์ .h/.c ไว้สวยแค่ไหน

**สิ่งที่ต้องทำเสมอไม่ว่าข้อไหน:**
1. แยกไฟล์เป็น `main.c` + module อื่น ๆ เช่น `gpio.c/.h`, `adc.c/.h`, `sevenseg.c/.h`, `uart.c/.h`, `pwm.c/.h`
2. หนึ่งฟังก์ชัน = หนึ่งหน้าที่ (เช่น `adc_init()`, `adc_read()`, `gpio_set_output()` แยกกันชัดเจน)
3. `main()` ควรมีแค่ `xxx_init()` ต่าง ๆ แล้วก็ `while(1) { ... }` เรียกฟังก์ชันสั้น ๆ — ห้ามยัด logic ยาว ๆ ลงใน main
4. คอมเมนต์แบบ `/* ... */` เท่านั้น (ห้าม `//`)
5. ตั้งชื่อแบบมี prefix บอก type/scope (ดูหัวข้อ MISRA ด้านล่าง) เช่น `u1t_x`, `u4t_count`

---

## 2. MISRA C 22 ข้อ แบบเข้าใจง่าย (จาก Checksheet)

> ทุกข้อทำตามได้ = ได้เต็ม 10 คะแนนหมวด Software Quality การจำ 22 ข้อ อาจเยอะ แต่ถ้าเขียนตามนิสัยด้านล่างนี้ตั้งแต่แรก จะผ่านเกือบหมดอัตโนมัติ

| # | กฎ | สรุปสั้น | ตัวอย่างถูก |
|---|---|---|---|
| 1 | ห้ามใช้ `//` | ใช้ `/* ... */` เท่านั้น | `/* init GPIO */` |
| 2 | `{` กับ `}` ห้ามอยู่บรรทัดเดียวกับ statement | ขึ้นบรรทัดใหม่เสมอ | `if (x == 2) {`<br>`  y = 4;`<br>`}` |
| 3 | เว้นวรรค operand กับ operator | `a + b` ไม่ใช่ `a+b` | `x = (y + a) / b;` |
| 4 | 1 บรรทัด = 1 assignment | ห้ามเขียนติดกันด้วย `;` | `x = a;`<br>`y = b;` |
| 5 | ห้าม hard-code "magic number" | ใช้ `#define` หรือ `const` | `#define MAX_VAL 63` |
| 6 | เลี่ยง ternary operator `? :` | ใช้ if-else แทน | `if (a==0) {x=y;} else {x=z;}` |
| 7 | ห้ามมี unreachable code | โค้ดหลัง `return` ไม่มีทางรันถึง — อย่าเขียนทิ้งไว้ | ลบโค้ดที่ตายแล้วออก |
| 8 | ห้ามมีตัวแปร/ฟังก์ชันที่ไม่ได้ใช้ | ลบของที่ไม่ใช้ทิ้ง | — |
| 9 | ห้ามใช้เลขฐาน 8 (ขึ้นต้นด้วย 0) | เช่น `012` ห้ามใช้ | ใช้ `10` หรือ `0x0A` แทน |
| 10 | ค่าคงที่ unsigned ต้องมี suffix `u`/`U` | ป้องกันปัญหา sign | `uint16_t x = 32768u;` |
| 11 | Prototype ฟังก์ชันต้องระบุชื่อ parameter และ `(void)` ถ้าไม่มี param | | `uint32_t u4_func(void);`<br>`int16_t s2_func(uint16_t u2_x);` |
| 12 | ห้ามอ่านตัวแปร local ก่อนกำหนดค่า | init ตัวแปรก่อนใช้เสมอ | `uint32_t u4t_y = 0u;` |
| 13 | อย่าใช้ type ผิดประเภทกับ operator | อย่าใช้ `&&`/`!` กับ uint/int ตรง ๆ ต้องเทียบกับค่าก่อน | `if (u4t_x != 0u)` ไม่ใช่ `if (u4t_x)` |
| 14 | ใส่วงเล็บให้ชัดเจนเรื่องลำดับ operator | | `a + ((b / c) - (d * e))` |
| 15 | ห้ามเทียบ float ด้วย `==`/`!=` | ใช้ tolerance เทียบ | `fabs(a-b) < TOLERANCE` |
| 16 | loop counter ห้ามเป็น float | ใช้ int/uint แทน | `for (uint32_t u4t_i = 0u; ...)` |
| 17 | เงื่อนไข if/while ต้องเป็น boolean expression ชัดเจน | ห้าม `if (x)` เฉย ๆ | `if (x == 0u)` / `while (y != 0)` |
| 18 | body ของ if/while/for ต้องมี `{ }` เสมอ | แม้มีบรรทัดเดียว | `if (x<30) {`<br>`  action();`<br>`}` |
| 19 | if...else if ต้องปิดท้ายด้วย else เสมอ | แม้ else จะว่างก็ต้องมี | `} else { /* No action */ }` |
| 20 | ทุก case ใน switch ต้องมี `break` (ยกเว้นตั้งใจ fall-through แบบ case ว่างต่อกัน) | | `case 0: action(); break;` |
| 21 | switch ต้องมี `default` เสมอ | | `default: /* action */ break;` |
| 22 | `default` ต้องอยู่ตำแหน่งแรกหรือสุดท้ายของ switch | ปกติวางไว้ท้ายสุด | — |

**นิสัยเขียนโค้ดที่ทำให้ผ่านทุกข้อโดยอัตโนมัติ:**
- คอมเมนต์ `/* */` เสมอ, วงเล็บปีกกาขึ้นบรรทัดใหม่เสมอ, เว้นวรรครอบ operator เสมอ
- ตัวแปรทุกตัวมี `= ...` ตอนประกาศ (init ค่าเริ่มต้นเสมอ)
- ทุก `if/else if` จบด้วย `else`, ทุก `switch` จบด้วย `default: break;`
- ค่าคงที่ทุกตัว → `#define NAME value` ไว้บนสุดของไฟล์

**Naming convention แบบในตัวอย่าง (ไม่บังคับ แต่ช่วยเรื่อง readability):**
`u1_`=uint8_t, `u2_`=uint16_t, `u4_`=uint32_t, `s2_`=int16_t, `s4_`=int32_t, `f4_`=float, ต่อท้ายด้วย `t_` ถ้าเป็นตัวแปร local ใน scope นั้น เช่น `u4t_count`, `u1_getConvFactor()`

---

## 3. โครงสร้างโปรแกรม (Template)

ทำตามแบบนี้ทุกข้อ (จาก repo ตัวอย่างของอาจารย์ ที่เขียนแบบ **bare-metal register-level** ไม่ใช้ HAL):

```
Src/
  main.c
  gpio.c
  sevenseg.c      (หรือ adc.c / uart.c / pwm.c แล้วแต่ข้อ)
  exti.c          (ถ้าใช้)
Inc/
  gpio.h
  sevenseg.h
  exti.h
```

**Header template (.h):**
```c
#ifndef SEVENSEG_H
#define SEVENSEG_H

/* Include ---------------------------------------------------------*/
#include "stm32f4xx.h"          /* CMSIS device header */

/* Private define ----------------------------------------------------*/
#define SEVENSEG_D0_PIN    (7u)     /* PC7 -> 2^0 */
#define SEVENSEG_MAX_DIGIT (9u)

/* Public function prototype ------------------------------------------*/
void sevenseg_init(void);
void sevenseg_display(uint8_t u1_number);

#endif /* SEVENSEG_H */
```

**Source template (.c):**
```c
/***********************************************************************
 * File Name : sevenseg.c
 * Description : Drive the 4-bit BCD lines of the 7-segment shield.
 ************************************************************************/

/* Include ---------------------------------------------------------*/
#include "sevenseg.h"
#include "gpio.h"

/* Private define ----------------------------------------------------*/

/* Public functions ----------------------------------------------------*/

/***********************************************************************
 * @fn      - sevenseg_init
 * @brief   - Configure the 4 BCD pins as output.
 * @param   - void
 * @return  - void
 ************************************************************************/
void sevenseg_init(void)
{
    gpio_set_output(GPIOC, 7u);   /* 2^0 */
    gpio_set_output(GPIOA, 8u);   /* 2^1 */
    gpio_set_output(GPIOB, 10u);  /* 2^2 */
    gpio_set_output(GPIOA, 9u);   /* 2^3 */
}

/***********************************************************************
 * @fn      - sevenseg_display
 * @brief   - Output a 0-9 value on the BCD lines.
 * @param   - u1_number : digit to display, 0-9
 * @return  - void
 ************************************************************************/
void sevenseg_display(uint8_t u1_number)
{
    if (u1_number <= SEVENSEG_MAX_DIGIT)
    {
        gpio_write(GPIOC, 7u,  (uint8_t)(u1_number & 0x01u));
        gpio_write(GPIOA, 8u,  (uint8_t)((u1_number >> 1u) & 0x01u));
        gpio_write(GPIOB, 10u, (uint8_t)((u1_number >> 2u) & 0x01u));
        gpio_write(GPIOA, 9u,  (uint8_t)((u1_number >> 3u) & 0x01u));
    }
    else
    {
        /* No action: invalid digit, ignore */
    }
}

/* Private functions ----------------------------------------------------*/
```

**main.c template:**
```c
#include "stm32f4xx.h"
#include "gpio.h"
#include "sevenseg.h"

static void rcc_clock_enable(void);

int main(void)
{
    rcc_clock_enable();
    gpio_init();
    sevenseg_init();

    uint8_t u1t_counter = 0u;

    while (1)
    {
        sevenseg_display(u1t_counter);
        u1t_counter = (uint8_t)((u1t_counter + 1u) % 10u);
        delay_ms(500u);
    }
}

static void rcc_clock_enable(void)
{
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);
}
```

---

## 4. GPIO พื้นฐาน (STM32F411, register level)

Register หลักของแต่ละพอร์ต (GPIOA, GPIOB, GPIOC, ...) — ทุกตัวมี 32 บิต แบ่งเป็นช่องละ pin:

| Register | หน้าที่ | บิตต่อ 1 pin |
|---|---|---|
| `MODER` | โหมด: 00=Input, 01=Output, 10=Alternate Function, 11=Analog | 2 bit |
| `OTYPER` | 0=Push-pull, 1=Open-drain | 1 bit |
| `OSPEEDR` | ความเร็วขา (00 low ... 11 very high) | 2 bit |
| `PUPDR` | 00=None, 01=Pull-up, 10=Pull-down | 2 bit |
| `IDR` | อ่านค่าขา input (read-only) | 1 bit/pin |
| `ODR` | ตั้งค่าขา output (เขียนตรง ๆ) | 1 bit/pin |
| `BSRR` | ตั้ง/เคลียร์ขาแบบ atomic (bit 0-15 = SET, bit 16-31 = RESET) — **แนะนำใช้อันนี้แทน ODR** | — |
| `AFR[0]/AFR[1]` | เลือก Alternate Function (สำหรับ Timer/UART/ADC ไม่ต้องใช้ AF, แต่ Timer PWM/UART ต้องใช้) | 4 bit/pin |

**ขั้นตอนตั้งค่า GPIO output ทุกครั้ง:**
```c
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;      /* 1. เปิด clock ของพอร์ตก่อนเสมอ */
GPIOC->MODER &= ~(0x3u << (7u * 2u));     /* 2. clear โหมด pin 7 */
GPIOC->MODER |=  (0x1u << (7u * 2u));     /* 3. set เป็น Output (01) */
GPIOC->BSRR = (1u << 7u);                 /* set PC7 = HIGH */
GPIOC->BSRR = (1u << (7u + 16u));         /* reset PC7 = LOW  */
```

**ขั้นตอนตั้งค่า GPIO input (สำหรับปุ่มกด):**
```c
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
GPIOB->MODER &= ~(0x3u << (10u * 2u));    /* 00 = Input */
GPIOB->PUPDR &= ~(0x3u << (10u * 2u));
GPIOB->PUPDR |=  (0x1u << (10u * 2u));    /* Pull-up (ปุ่มกดแบบ active-low) */

uint32_t u4t_state = (GPIOB->IDR >> 10u) & 0x1u;   /* อ่านค่า */
if (u4t_state == 0u)   /* ปุ่มถูกกด (active-low) */
{
    /* action */
}
else
{
    /* No action */
}
```

---

## 5. Q1: 7-Segment Control (GPIO, EXTI optional)

**เข้าใจจากสไลด์:** บอร์ดชิลด์มี IC ตัวแปลง BCD → 7-Segment ให้แล้ว โจทย์แค่ให้ "ต่อ pin 4 ขา" ไปเป็นเลขฐาน 2 ตรง ๆ (ไม่ต้องคุมทีละ segment เอง!)

| BCD bit | STM32F411RE Pin |
|---|---|
| 2^0 | PC7 |
| 2^1 | PA8 |
| 2^2 | PB10 |
| 2^3 | PA9 |

⚠️ **ถ้าใช้ shield สีฟ้า ป้ายบนบอร์ดผิด ให้ยึด mapping ตารางนี้ (แบบ shield สีแดง) เป็นหลัก**

ตารางแปลงเลข → บิต (จำง่าย: ก็คือเลขฐาน 2 ปกติของ 0-9):

| เลข | 2^3 | 2^2 | 2^1 | 2^0 |
|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 |
| 1 | 0 | 0 | 0 | 1 |
| 2 | 0 | 0 | 1 | 0 |
| 3 | 0 | 0 | 1 | 1 |
| 4 | 0 | 1 | 0 | 0 |
| 5 | 0 | 1 | 0 | 1 |
| 6 | 0 | 1 | 1 | 0 |
| 7 | 0 | 1 | 1 | 1 |
| 8 | 1 | 0 | 0 | 0 |
| 9 | 1 | 0 | 0 | 1 |

โค้ดตัวอย่างอยู่ในหัวข้อ 3 แล้ว (`sevenseg_display()`) — ใช้ shift ขวา + `& 0x01u` ดึงแต่ละบิตออกมาสั่ง pin

**ถ้าจะทำ EXTI (optional, เพิ่มคะแนน algorithm ได้):** ใช้ปุ่มบนบอร์ด (เช่น D2/PA10, D3/PB3, D4/PB5, D5/PB4) ผูก interrupt เพื่อเพิ่ม/ลดเลขที่แสดง — ดูโค้ดตัวอย่างสไตล์ EXTI ด้านล่าง

```c
/* exti.c – ตัวอย่างตั้งปุ่มเพิ่มเลข ผ่าน EXTI line */
void exti_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* Route PB3 -> EXTI line 3 (EXTICR[0], bits 15:12 สำหรับ pin3) */
    SYSCFG->EXTICR[0] &= ~(0xFu << 12u);
    SYSCFG->EXTICR[0] |=  (0x1u << 12u);   /* 1 = Port B */

    EXTI->FTSR |= (1u << 3u);   /* falling edge trigger (ปุ่มกด active-low) */
    EXTI->IMR  |= (1u << 3u);   /* unmask line */

    NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI3_IRQHandler(void)
{
    if ((EXTI->PR & (1u << 3u)) != 0u)
    {
        EXTI->PR = (1u << 3u);   /* clear pending: เขียน 1 เพื่อเคลียร์ */
        u1_counter = (uint8_t)((u1_counter + 1u) % 10u);
        sevenseg_display(u1_counter);
    }
    else
    {
        /* No action */
    }
}
```

---

## 6. Q2: ADC + GPIO

**หลักการ ADC บน STM32F411 (ADC1, 12-bit, single conversion, polling mode ง่ายสุดสำหรับสอบ):**

```c
/* adc.c */
void adc_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    /* ตั้ง PA0 (ADC1_IN0) เป็น Analog mode (11) */
    GPIOA->MODER |= (0x3u << (0u * 2u));

    ADC1->CR2 &= ~ADC_CR2_CONT;         /* Single conversion mode */
    ADC1->SQR3 = 0u;                    /* channel 0 เป็นลำดับแรก (regular sequence) */
    ADC1->SMPR2 |= (0x7u << (0u * 3u)); /* sampling time ยาวขึ้น (channel 0) */
    ADC1->CR2 |= ADC_CR2_ADON;          /* เปิด ADC */
}

uint16_t adc_read(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;               /* เริ่มแปลงค่า */
    while ((ADC1->SR & ADC_SR_EOC) == 0u)        /* รอจนกว่าจะแปลงเสร็จ */
    {
        /* wait */
    }
    return (uint16_t)ADC1->DR;                   /* อ่านผลลัพธ์ 0-4095 */
}
```

**นำไปใช้กับ GPIO** เช่น อ่านค่า potentiometer แล้วเทียบระดับสั่ง LED ติด/ดับ:
```c
uint16_t u2t_value = adc_read();
if (u2t_value > ADC_THRESHOLD)
{
    gpio_write(GPIOA, LED_PIN, 1u);
}
else
{
    gpio_write(GPIOA, LED_PIN, 0u);
}
```

> ตรวจดู pin ตัวต้านทานปรับค่า (potentiometer) บนชิลด์จริงของคุณ (มักจะเป็น A0/PA0 ตามภาพสไลด์) ก่อนเขียนโค้ดจริง

---

## 7. Q3: UART (TX/RX) + ADC + GPIO

Nucleo-F411RE ปกติใช้ **USART2 บน PA2 (TX) / PA3 (RX)** ต่อผ่าน ST-Link เป็น Virtual COM Port (เปิดดูผ่าน Serial Monitor / PuTTY ที่ baud 115200 ได้เลยไม่ต้องต่อสาย)

```c
/* uart.c */
#define UART_BAUD      115200u
#define APB1_CLK_HZ    16000000u   /* ค่า default HSI ของ Nucleo ถ้าไม่ได้ตั้ง clock พิเศษ */

void uart_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* PA2/PA3 -> Alternate function AF7 (USART2) */
    GPIOA->MODER &= ~((0x3u << (2u*2u)) | (0x3u << (3u*2u)));
    GPIOA->MODER |=  ((0x2u << (2u*2u)) | (0x2u << (3u*2u)));
    GPIOA->AFR[0] &= ~((0xFu << (2u*4u)) | (0xFu << (3u*4u)));
    GPIOA->AFR[0] |=  ((0x7u << (2u*4u)) | (0x7u << (3u*4u)));

    USART2->BRR = (uint16_t)(APB1_CLK_HZ / UART_BAUD);
    USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
}

void uart_send_char(uint8_t u1_data)
{
    while ((USART2->SR & USART_SR_TXE) == 0u)
    {
        /* wait until TX buffer empty */
    }
    USART2->DR = u1_data;
}

void uart_send_string(const char *pc_str)
{
    while (*pc_str != '\0')
    {
        uart_send_char((uint8_t)*pc_str);
        pc_str++;
    }
}

uint8_t uart_receive_char(void)
{
    while ((USART2->SR & USART_SR_RXNE) == 0u)
    {
        /* wait until data received */
    }
    return (uint8_t)USART2->DR;
}
```

**Flow ที่ตรงกับโจทย์ (UART+ADC+GPIO):** อ่านค่า ADC → แปลงเป็นข้อความ → ส่งออกทาง UART → หรือรับคำสั่งจาก UART มาสั่งเปิด/ปิด GPIO/LED

---

## 8. Q4: PWM (implementation ยืดหยุ่น) + ADC + GPIO

**"implementation method is flexible"** หมายถึงใช้ได้ทั้ง Timer PWM hardware หรือ software PWM (bit-bang ด้วย delay) — แต่ **แนะนำใช้ Timer PWM (ง่ายกว่า แม่นกว่า คะแนน algorithm สูงกว่า)**

ตัวอย่างด้วย TIM3 Channel 1 (มักออกที่ PA6 บนหลาย shield — เช็ค datasheet AF table ของบอร์ดจริงก่อนใช้):

```c
/* pwm.c – TIM3_CH1 บน PA6 */
#define PWM_ARR_VALUE   999u    /* Auto-reload: กำหนดความละเอียด PWM (1000 steps) */

void pwm_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    /* PA6 -> Alternate Function AF2 (TIM3_CH1) */
    GPIOA->MODER &= ~(0x3u << (6u*2u));
    GPIOA->MODER |=  (0x2u << (6u*2u));
    GPIOA->AFR[0] &= ~(0xFu << (6u*4u));
    GPIOA->AFR[0] |=  (0x2u << (6u*4u));

    TIM3->PSC = 15u;              /* prescaler: 16MHz/(15+1) = 1MHz timer clock */
    TIM3->ARR = PWM_ARR_VALUE;    /* period = 1000 counts -> 1kHz PWM */
    TIM3->CCR1 = 0u;              /* duty cycle เริ่มต้น 0% */

    TIM3->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM3->CCMR1 |= (0x6u << 4u);  /* PWM mode 1 */
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;

    TIM3->CCER |= TIM_CCER_CC1E;  /* enable output channel 1 */
    TIM3->CR1  |= TIM_CR1_CEN;    /* start timer */
}

void pwm_set_duty(uint16_t u2_percent)
{
    if (u2_percent <= 100u)
    {
        TIM3->CCR1 = (uint16_t)((PWM_ARR_VALUE * u2_percent) / 100u);
    }
    else
    {
        /* No action: invalid input, keep previous duty */
    }
}
```

**Flow ที่ตรงกับโจทย์ (PWM+ADC+GPIO):** อ่านค่า ADC (potentiometer) → map ค่า 0-4095 เป็น duty cycle 0-100% → สั่ง `pwm_set_duty()` → ควบคุมความสว่าง LED เป็นตัวอย่างคลาสสิก

```c
uint16_t u2t_adc = adc_read();                          /* 0-4095 */
uint16_t u2t_duty = (uint16_t)((u2t_adc * 100u) / 4095u); /* map เป็น % */
pwm_set_duty(u2t_duty);
```

---

## 9. Checklist ก่อนส่งโค้ด (เช็คทุกข้อ)

- [ ] แยกไฟล์ main.c + module.c/.h อย่างน้อย 1-2 module
- [ ] ไม่มี logic ยาว ๆ ใน `main()` — main เรียกแค่ init + loop เรียกฟังก์ชัน
- [ ] ทุกฟังก์ชันทำหน้าที่เดียว ตั้งชื่อสื่อความหมาย (`adc_read`, `gpio_set_output`, ...)
- [ ] คอมเมนต์ `/* */` ทุกจุด ไม่มี `//`
- [ ] `{`/`}` ขึ้นบรรทัดใหม่เสมอ, เว้นวรรครอบ operator
- [ ] ไม่มี magic number เปล่า ๆ → ใช้ `#define`
- [ ] ตัวแปร local init ค่าตอนประกาศเสมอ
- [ ] ทุก if/else if จบด้วย else, ทุก switch มี default + break ทุก case
- [ ] เช็คทุก `if(...)`/`while(...)` ว่าเทียบค่าเสมอ (`== 0u`, `!= 0u`) ไม่ใช้ตัวแปรตรง ๆ
- [ ] เปิด clock (`RCC->...EN`) ก่อนตั้งค่า peripheral ทุกครั้ง (ลืมข้อนี้บ่อยที่สุด แล้วจะงงว่าทำไมโค้ดไม่ทำงาน)

---

## 10. สรุปสูตรจำ Pin/Register สำคัญของบอร์ด

| อุปกรณ์ | Pin (Red shield) |
|---|---|
| 7-Segment 2^0 / 2^1 / 2^2 / 2^3 | PC7 / PA8 / PB10 / PA9 |
| ปุ่มกด D2–D5 | PA10 / PB3 / PB5 / PB4 |
| LED D10–D13 | PB6 / PA7 / PA6 / PA5 |
| ADC ตัวอย่าง (เช็คบอร์ดจริง) | A0=PA0, A1=PA1, A2=PA4 |
| UART (ST-Link VCP) | USART2: PA2 (TX) / PA3 (RX) |

---

**ขอให้สอบผ่านนะครับ 💪 ถ้าเวลาน้อย ให้ไล่ตามลำดับ: (1) ทำ Q1 ให้รันติด 7-segment ได้จริงก่อน → (2) ทำ ADC อ่านค่าได้ → (3) ต่อยอดเป็น PWM หรือ UART ตามที่ถนัด → สุดท้ายไล่ MISRA checklist ก่อนส่ง**
