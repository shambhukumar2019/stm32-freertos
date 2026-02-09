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

// pointers to task block
TaskHandle_t pxtask1_handle;
TaskHandle_t pxtask2_handle;

void pvmaintask1(void *);
void pvmaintask2(void *);


int main(void)
{
	BaseType_t xret_val = 0;

	SystemClock_Config();

	pvitm_init();
	pvgpio_gpiod_init();



	xret_val = xTaskCreate(pvmaintask1, "blink-led-fast", 1024, NULL, 1, &pxtask1_handle);
	if(xret_val == pdFAIL)
	{
		configASSERT(0);
	}

	xret_val = xTaskCreate(pvmaintask2, "blink-led-slow", 1024, NULL, 1, &pxtask2_handle);
	if(xret_val == pdFAIL)
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
void pvmaintask1(void *pvParameters)
{
	const TickType_t xdelay_ms1 = pdMS_TO_TICKS(4000);

	for(;;)
	{
		printf("task a\n");

		GPIOD->ODR ^= (0x1U<<12);	//toggle PD12
		vTaskDelay(xdelay_ms1);
	}
}

/**
 *  blink led slow
 */
void pvmaintask2(void *pvParameters)
{
	const TickType_t xdelay_ms2 = pdMS_TO_TICKS(2000);

	for(;;)
	{
		printf("task b\n");

		GPIOD->ODR ^= (0x1U<<13);	//toggle PD13
		vTaskDelay(xdelay_ms2);
	}
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  //HSI = 16MHz default, PLL input clock frequency
  RCC->PLLCFGR &= (~(1U<<22));	//HSI as PLL source clock
  userConfigASSERT(RCC->PLLCFGR, (1U<<22), (0U<<22));

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

  //AHB clk out = 168MHz, HCLK
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
//  while((RCC->CFGR & (0x3U<<2)) != (0x2U<<2));	//wait for pll select as sys clk

  RCC->CR |= (1U<<0);	//hsi on
  while((RCC->CR & (0x1U<<1)) != (0x1U<<1));	//wait for hsi stable

  RCC->CR |= (1U<<24);	//main PLL on
  while((RCC->CR & (0x1U<<25)) != (0x1U<<25));	//wait for main PLL stable

  SystemCoreClockUpdate();

}


