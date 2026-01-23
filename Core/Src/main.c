/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"



//#if !defined(__SOFT_FP__) && defined(__ARM_FP)
//  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
//#endif

void vtask1(void *);
void vtask2(void *);

int main(void)
{
	BaseType_t ret_val = 0;

	SystemClock_Config();

	ret_val = xTaskCreate(vtask1, "blink-led-fast", 512, NULL, 1, NULL);
	if(ret_val == pdFAIL)
	{
		configASSERT(0);
	}

	ret_val = xTaskCreate(vtask2, "blink-led-slow", 512, NULL, 1, NULL);
	if(ret_val == pdFAIL)
	{
		configASSERT(0);
	}

	vTaskStartScheduler();

    /* Loop forever */
	for(;;);
}


/**
 *  blink led fast
 */
void vtask1(void *pvParameters)
{
	for(;;);
}

/**
 *  blink led slow
 */
void vtask2(void *pvParameters)
{
	for(;;);
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  //HSI = 16MHz default, PLL input clock frequency
  RCC->PLLCFGR |= (1U<<22);	//HSI as PLL source clock
  userConfigASSERT(RCC->PLLCFGR, (1U<<22), (1U<<22));

  //VCO input frequency = PLL input clock frequency / M , 8MHz
  RCC->PLLCFGR &= (~(0x3FU<<0));	//mask PLLM bits
  RCC->PLLCFGR |= (0x8U<<0);		//M = 8
  userConfigASSERT(RCC->PLLCFGR, (0x3FU<<0), (0x8U<<0));

  //VCO output frequency [336MHz] = VCO input frequency * N ,336MHz
  RCC->PLLCFGR &= (~(0x1FFU<<6));	//mask PLLN bits
  RCC->PLLCFGR |= (0xA8U<<6);		//N = 168
  userConfigASSERT(RCC->PLLCFGR, (0x1FFU<<6), (0xA8U<<6));

  //PLL output frequency [168MHz] = VCO output frequency / P , 168MHz SYSCLK
  RCC->PLLCFGR &= (~(0x3U<<16));	//mask PLLP bits
  RCC->PLLCFGR |= (0x0U<<16);		//P = 2
  userConfigASSERT(RCC->PLLCFGR, (0x3U<<16), (0x0U<<16));

  //USB, RNG, SDIO, OTG FS frequency = VCO output frequency / Q , 48MHz
  RCC->PLLCFGR &= (~(0xFU<<24));	//mask PLLQ bits
  RCC->PLLCFGR |= (0x7U<<24);		//Q = 7
  userConfigASSERT(RCC->PLLCFGR, (0xFU<<24), (0x7U<<24));

  //AHB clk out = 168MHz
  RCC->CFGR &= (~(0xFU<<4));		//mask AHB prescaler bits
  RCC->CFGR |= (0x0U<<4);			//AHB prescaler = null
  userConfigASSERT(RCC->CFGR, (0xFU<<4), (0x0U<<4));

  //APB1 clk out = 42MHz, APB1 clk = AHB clk / 4
  RCC->CFGR &= (~(0x7U<<10));		//mask APB1 prescaler bits
  RCC->CFGR |= (0x5U<<10);			//APB1 prescaler = 4
  userConfigASSERT(RCC->CFGR, (0x7U<<10), (0x5U<<10));

  //APB2 clk out = 84MHz, APB2 clk = AHB clk / 2
  RCC->CFGR &= (~(0x7U<<13));		//mask APB2 prescaler bits
  RCC->CFGR |= (0x4U<<13);			//APB2 prescaler = 2
  userConfigASSERT(RCC->CFGR, (0x7U<<13), (0x4U<<13));

  //configure PLL as system clock
  RCC->CFGR &= (~(0x3U<<0));
  RCC->CFGR |= (0x2U<<0);
  userConfigASSERT(RCC->CFGR, (0x3U<<0), (0x2U<<0));
  while((RCC->CFGR & (0x3U<<2)) != (0x2U<<2));	//wait for pll select as sys clk

  RCC->CR |= (1U<<0);	//hsi on
  while((RCC->CR & (0x1U<<1)) != (0x1U<<1));	//wait for hsi stable

  RCC->CR |= (1U<<24);	//main PLL on
  while((RCC->CR & (0x1U<<25)) != (0x1U<<25));	//wait for main PLL stable

}
