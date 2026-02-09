/**
 *  auther: shambhu kumar
 */

#include "itm.h"


void pvitm_init(void)
{
	volatile uint32_t tempreg = 0;

	RCC->AHB1ENR |= (1U<<1);			//enable GPIOB clk
	tempreg = (RCC->AHB1ENR & (1U<<3));
	(void)tempreg;


	/* init GPIO for ITM trace on PB3 */
	GPIOB->MODER &= (~(0x3U<<6));
	GPIOB->MODER |= (0x2U<<6);			//PB3 AF mode

	GPIOB->OSPEEDR |= (0x3U<<6);		//PB3 out speed very high

	GPIOB->AFR[0] &= (~(0xFU << 12));	//AF0 (system)


	 /**
	 * TPIU uses HCLK on reset, coz. ITM prescaler is 0.
	 *
	 * init DWT, ITM and TPUI for asynchronous trace
	 *
	 */

	// global enable for DWT and ITM features
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	// one physical pin enable on mcu for trace
	TPI->CSPSR |= 0x00000001;

	// set target asynchronous clk freq
	TPI->ACPR = ((SystemCoreClock / TARGET_SWO_FREQ) - 1);

	TPI->FFCR &= (~(0x1U << 1));	//tpiu trace formatter disable


	TPI->SPPR &= (~(0x3U << 0));	//mask trace port protocol bits
	TPI->SPPR |= (0x2U << 0);		//nrz mode (uart type)

	DBGMCU->CR |= (0x1U << 5);		//assign trace pin
	DBGMCU->CR &= (~(0x3U << 6));	//asynchronous mode

	ITM->LAR = 0xC5ACCE55;			//unlock ITM reg write access

	ITM->TPR = 0x1U;				//enable unprivilege acces to stimulus port 0-7
	ITM->TCR &= (~(0x1U << 0));		//disable ITM

	// wait until ITM incative
	while((ITM->TCR & (0x1U << 23)) != 0);

	ITM->TCR &= (~(0x7FU << 16));	//mask tracebusid bits
	ITM->TCR |= (0x1U << 16);		//nonzero tracebusid
//	ITM->TCR |= (0x1U << 2);		//enable synchronization packet transmission
	ITM->TCR |= (0x1U << 3);		//enable hardware packettransfer from DWT to ITM
	ITM->TCR |= (0x1U << 0);		//enable ITM unit
	ITM->TER = 0x1U;				//enable stimulus port 0


}

int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
	  ITM_SendChar(*(ptr++));
  }
  return len;
}
