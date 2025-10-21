/***** Base addresses & offsets *****/
#define PERIPH_BASE                 (0x40000000UL)           // Base of peripheral region (on AHB/APB buses)

#define AHB1_OFFSET                 (0x00020000UL)           // Offset of AHB1 block from PERIPH_BASE
#define AHB1_BASE                   (PERIPH_BASE + AHB1_OFFSET)   // = 0x40020000: AHB1 peripheral base

#define GPIOB_OFFSET                (0x0400UL)               // Offset of GPIOB within AHB1 block
#define GPIOB_BASE                  (AHB1_BASE + GPIOB_OFFSET)     // = 0x40020400: GPIOB base

#define RCC_OFFSET                  (0x3800UL)               // Offset of RCC within AHB1 block
#define RCC_BASE                    (AHB1_BASE + RCC_OFFSET)       // = 0x40023800: RCC base

#define RCC_AHB1EN_R_OFFSET         (0x30UL)                 // Offset of RCC_AHB1ENR within RCC
#define RCC_AHB1EN_R                (*(volatile unsigned int *)(RCC_BASE + RCC_AHB1EN_R_OFFSET)) // 0x40023830

#define GPIOB_EN                    (1U << 1)                // Bit mask to enable GPIOB clock

#define MODE_R_OFFSET               (0x00UL)                 // MODER offset within GPIO
#define GPIOB_MODE_R                (*(volatile unsigned int *)(GPIOB_BASE + MODE_R_OFFSET)) // 0x40020400

#define OUTPUT_DR_OFFSET            (0x14UL)                 // ODR offset within GPIO
#define GPIOB_OUTPUT_DR_R           (*(volatile unsigned int *)(GPIOB_BASE + OUTPUT_DR_OFFSET)) //0x40020414

#define PIN5                        (1U << 3)                // Bit for PB3 in ODR
#define LED_PIN                     PIN5                     // Alias for clarity

int main(void)
{
    /* 1) Enable clock access to GPIOA (set RCC_AHB1ENR.GPIOAEN) */
    RCC_AHB1EN_R |= GPIOB_EN;   // Write to 0x40023830, set bit0 → enables GPIOA bus clock

    /* 2) Configure PA5 as general-purpose output: MODER5 = 01b */
    GPIOB_MODE_R |=  (1U << 6);   // Write to 0x40020000: set MODER bit10
    GPIOB_MODE_R &= ~(1U << 7);   // Write to 0x40020000: clear MODER bit11

    while (1)
    {
        /* 3) Toggle PA5 by XORing ODR bit5 (LED blink on Nucleo-F401RE uses PA5) */
        // GPIOA_OUTPUT_DR_R |= LED_PIN;  // (Example) force PA5 high: write 0x40020014 |= (1<<5)

        GPIOB_OUTPUT_DR_R ^= LED_PIN;    // Write to 0x40020014: flip bit5 each loop

        /* Crude delay loop (busy-wait) */
        for (int i = 0; i < 100000; i++) { }  // ~software delay; timing depends on clock
    }
}
