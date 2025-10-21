#include <stdint.h>

/***** Base addresses & offsets *****/
#define PERIPH_BASE                 (0x40000000UL)
#define AHB1_OFFSET                 (0x00020000UL)
#define AHB1_BASE                   (PERIPH_BASE + AHB1_OFFSET)    // 0x40020000

#define GPIOB_OFFSET                (0x0400UL)
#define GPIOB_BASE                  (AHB1_BASE + GPIOB_OFFSET)     // 0x40020400

#define RCC_OFFSET                  (0x3800UL)
#define RCC_BASE                    (AHB1_BASE + RCC_OFFSET)       // 0x40023800

#define RCC_AHB1EN_R_OFFSET         (0x30UL)
#define RCC_AHB1EN_R                (*(volatile uint32_t *)(RCC_BASE + RCC_AHB1EN_R_OFFSET)) // 0x40023830
#define GPIOB_EN                    (1U << 1)    // Enable clock for GPIOB

#define GPIOB_MODE_R_OFFSET          (0x00UL)
#define GPIOB_MODE_R                 (*(volatile uint32_t *)(GPIOB_BASE + GPIOB_MODE_R_OFFSET)) // 0x40020400

#define GPIOB_ODR_OFFSET            (0x14UL)
#define GPIOB_ODR                   (*(volatile uint32_t *)(GPIOB_BASE + GPIOB_ODR_OFFSET)) // 0x40020414

#define LED_PIN                     (1U << 5)    // PB5

int main(void)
{
    // 1. Enable clock for GPIOB
    RCC_AHB1EN_R |= GPIOB_EN;

    // 2. Configure PB5 as output (MODER5 = 01)
    GPIOB_MODE_R |=  (1U << 10);   // Write to 0x40020000: set MODER bit10
    GPIOB_MODE_R &= ~(1U << 11);   // Write to 0x40020000: clear MODER bit11

    // 3. Blink loop
    while (1)
    {
        GPIOB_ODR ^= LED_PIN;          // Toggle PB5
        for (volatile int i = 0; i < 100000; i++);  // Simple delay
    }
}
