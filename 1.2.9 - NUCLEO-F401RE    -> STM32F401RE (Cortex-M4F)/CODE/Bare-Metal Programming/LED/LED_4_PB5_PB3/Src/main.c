// Toggle PB5 + PB3

#include <stdint.h>

#define PERIPH_BASE												(0x40000000UL)
#define AHB1_OFFSET												(0x00020000UL)
#define AHB1_BASE													(PERIPH_BASE + AHB1_OFFSET)	//	0x40020000

#define GPIOB_OFFSET											(0x0400UL)
#define GPIOB_BASE													(AHB1_BASE +GPIOB_OFFSET ) // 0x40020400

#define RCC_OFFSET												(0x3800UL)
#define RCC_BASE														(AHB1_BASE + RCC_OFFSET) // 0x40023800

#define RCC_AHB1EN_R_OFFSET 						(0x30UL)
#define RCC_AHB1EN_R											(*(volatile uint32_t *)(RCC_BASE + RCC_AHB1EN_R_OFFSET)) // 0x40023830

#define GPIOB_EN														(1U<<1)

#define GPIOB_MODE_R_OFFSET						(0x00UL)
#define GPIOB_MODE_R											(*(volatile uint32_t *)(GPIOB_BASE + GPIOB_MODE_R_OFFSET)) // 0x40020400

#define GPIOB_OD_R_OFFSET								(0x14UL)
#define GPIOB_OD_R												(*(volatile uint32_t*)(GPIOB_BASE + GPIOB_OD_R_OFFSET)) // 0x40020414

#define PB3																	(1U<<3)
#define PB5 																	(1U<<5)
#define LED_MASK                          							(PB3 | PB5)

int main(void)
{
	RCC_AHB1EN_R |= GPIOB_EN;

	GPIOB_MODE_R &= ~(1U<<7) | (1U<<11);
	GPIOB_MODE_R |=  (1U<<6) | (1U<<10);

	while(1)
	{
		GPIOB_OD_R ^= LED_MASK;
		for (volatile int i = 0; i < 100000; i++) { }

	}
}


