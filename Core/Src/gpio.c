/**
 *  auther: shambhu kumar
 */

#include "gpio.h"


void pvgpio_gpiod_init(void)
{
	volatile uint32_t tempreg = 0;

	RCC->AHB1ENR |= (1U<<3);
	tempreg = (RCC->AHB1ENR & (1U<<3));
	(void)tempreg;

	GPIOD->MODER &= (~(0x3U<<24));
	GPIOD->MODER |= (0x1U<<24);		//PD12 output mode

	GPIOD->MODER &= (~(0x3U<<26));
	GPIOD->MODER |= (0x1U<<26);		//PD13 output mode

	GPIOD->OTYPER &= (~(0x3U<<12));	//PD12 PD13 push pull output

	GPIOD->OSPEEDR &= (~(0x3U<<24));//PD12 out speed low
	GPIOD->OSPEEDR &= (~(0x3U<<26));//PD13 out speed low

	GPIOD->PUPDR &= (~(0x3U<<24));	//PD12 no pull up/pull down
	GPIOD->PUPDR &= (~(0x3U<<26));	//PD13 no pull up/pull down

}
