/***** Base addresses & offsets *****/
#define PERIPH_BASE                 (0x40000000UL)           // Base of peripheral region (on AHB/APB buses)

#define AHB1_OFFSET                 (0x00020000UL)           // Offset of AHB1 block from PERIPH_BASE
#define AHB1_BASE                   (PERIPH_BASE + AHB1_OFFSET)   // = 0x40020000: AHB1 peripheral base

#define GPIOA_OFFSET                (0x0000UL)               // Offset of GPIOA within AHB1 block
#define GPIOA_BASE                  (AHB1_BASE + GPIOA_OFFSET)     // = 0x40020000: GPIOA base

#define RCC_OFFSET                  (0x3800UL)               // Offset of RCC within AHB1 block
#define RCC_BASE                    (AHB1_BASE + RCC_OFFSET)       // = 0x40023800: RCC base

/*
 * We also learned that activating this peripheral requires enabling clock access through the RCC peripheral.
 * The RCC peripheral provides enable registers per bus.
 * The AHB1 bus clock enable register is RCC_AHB1ENR.
 */

#define RCC_AHB1EN_R_OFFSET         (0x30UL)                 // Offset of RCC_AHB1ENR within RCC
#define RCC_AHB1EN_R                (*(volatile unsigned int *)(RCC_BASE + RCC_AHB1EN_R_OFFSET))
// Address = 0x40023800 + 0x30 = 0x40023830  (RCC_AHB1ENR)

/*
 * Inside RCC_AHB1ENR, bit 0 is GPIOAEN (GPIOA Enable).
 */
#define GPIOA_EN                    (1U << 0)                // Bit mask to enable GPIOA clock

/*
 * Bit helper notes:
 *   Set a bit:     reg |=  (1U << bit)
 *   Clear a bit:   reg &= ~(1U << bit)
 */

/*
 * To configure PA5 mode we write GPIOA_MODER (mode register).
 * For pin 5, the field is MODER5 = bits [11:10].
 *   00=input, 01=general-purpose output, 10=alt func, 11=analog
 * We want output → MODER5 = 01b → bit10=1, bit11=0.
 */
#define MODE_R_OFFSET               (0x00UL)                 // MODER offset within GPIO
#define GPIOA_MODE_R                (*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))
// Address = 0x40020000 + 0x00 = 0x40020000  (GPIOA_MODER)

/*
 * For clarity:
 *   (1U << 10)   // set bit10 = 1
 *   ~(1U << 11)  // clear bit11 = 0
 */

#define OUTPUT_DR_OFFSET            (0x14UL)                 // ODR offset within GPIO
#define GPIOA_OUTPUT_DR_R           (*(volatile unsigned int *)(GPIOA_BASE + OUTPUT_DR_OFFSET))
// Address = 0x40020000 + 0x14 = 0x40020014  (GPIOA_ODR)

#define PIN5                        (1U << 5)                // Bit for PA5 in ODR
#define LED_PIN                     PIN5                     // Alias for clarity

int main(void)
{
    /* 1) Enable clock access to GPIOA (set RCC_AHB1ENR.GPIOAEN) */
    RCC_AHB1EN_R |= GPIOA_EN;   // Write to 0x40023830, set bit0 → enables GPIOA bus clock

    /* 2) Configure PA5 as general-purpose output: MODER5 = 01b */
    GPIOA_MODE_R |=  (1U << 10);   // Write to 0x40020000: set MODER bit10
    GPIOA_MODE_R &= ~(1U << 11);   // Write to 0x40020000: clear MODER bit11

    while (1)
    {
        /* 3) Toggle PA5 by XORing ODR bit5 (LED blink on Nucleo-F401RE uses PA5) */
        // GPIOA_OUTPUT_DR_R |= LED_PIN;  // (Example) force PA5 high: write 0x40020014 |= (1<<5)

        GPIOA_OUTPUT_DR_R ^= LED_PIN;    // Write to 0x40020014: flip bit5 each loop

        /* Crude delay loop (busy-wait) */
        for (int i = 0; i < 100000; i++) { }  // ~software delay; timing depends on clock
    }
}


/*What each instruction accomplishes (super-short bullets)
 * 	PERIPH_BASE: tells the CPU where the entire peripheral address space begins (0x40000000).
 * 	AHB1_OFFSET: where the AHB1 peripheral block starts relative to PERIPH_BASE.
 * 	AHB1_BASE: the computed base for AHB1 peripherals (0x40020000).
 * 	GPIOA_OFFSET: GPIOA’s offset inside AHB1.
 * 	GPIOA_BASE: computed base of GPIOA (0x40020000).
 * 	RCC_OFFSET: RCC’s offset inside AHB1.
 * 	RCC_BASE: computed base of RCC (0x40023800).
 * 	RCC_AHB1EN_R_OFFSET: offset of RCC_AHB1ENR inside RCC.
 * 	RCC_AHB1EN_R: lvalue to directly read/write RCC_AHB1ENR at 0x40023830.
 * 	GPIOA_EN: bit mask for GPIOA clock enable (bit0).
 * 	MODE_R_OFFSET: offset of GPIOx_MODER inside a GPIO port.
 * 	GPIOA_MODE_R: lvalue to access GPIOA_MODER at 0x40020000.
 * 	OUTPUT_DR_OFFSET: offset of GPIOx_ODR.
 * 	GPIOA_OUTPUT_DR_R: lvalue to access GPIOA_ODR at 0x40020014.
 * 	PIN5/LED_PIN: bit for PA5.
 *
 * In main():
 * 	RCC_AHB1EN_R |= GPIOA_EN; enables the GPIOA peripheral clock.
 * 	GPIOA_MODE_R writes set MODER5=01 (PA5 as output).
 * 	Loop toggles PA5 by XORing ODR bit5; delay loop slows the blink.
 *
 *
 * Quick answers you asked for implicitly
 * 	Actual addresses after adding base+offsets:
 * 	AHB1_BASE = 0x40000000 + 0x00020000 = 0x40020000
 * 	GPIOA_BASE = 0x40020000 + 0x0000 = 0x40020000
 * 	RCC_BASE = 0x40020000 + 0x3800 = 0x40023800
 * 	RCC_AHB1EN_R = 0x40023800 + 0x30 = 0x40023830
 * 	GPIOA_MODE_R = 0x40020000 + 0x00 = 0x40020000
 * 	GPIOA_OUTPUT_DR_R (ODR) = 0x40020000 + 0x14 = 0x40020014
 *
 *
 * What is GPIO-A mode in this code?
 * 	MODER5 = 01b → PA5 is configured as general-purpose output.
 * */
