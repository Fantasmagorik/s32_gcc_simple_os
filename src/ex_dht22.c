#include <stm32f103xb.h>
#include "os_func.h"
#include "ex_dht22.h"
//using TIM2 !!!
#define outPP		0x3
#define inputF	4
#define PORTDATA	1 //GPIOA 1
volatile short tDMABuffer[50];
//int DHT22Value;


	union DHT22Data DHT22Data;


void DHT22Init()	{
//	GPIOA->CRL &= ~0xf << PORTDATA * 4;
//	GPIOA->CRL |= outPP << PORTDATA * 4;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC	=	71;	//1 uS
	TIM2->ARR	=	0xffff;
	TIM2->CCMR1 = TIM_CCMR1_CC2S_0;	//channel 2 as TI2
	TIM2->CCER = TIM_CCER_CC2E;			//channel 2 enable
	TIM2->SMCR = 101; //gated mode for ti2f

	DMA1_Channel7->CCR = 0;
	DMA1_Channel7->CPAR = (int) &TIM2->CCR2;
	DMA1_Channel7->CMAR = (int) tDMABuffer;
	TIM2->DIER = TIM_DIER_CC2DE;

}

unsigned int DHT22GetValue()	{
	int i, mask;
	char checksum, *byte, DHT22CRC;

	short *pulseTime, value;
	DHT22Data.value = 0;
	DHT22CRC = 0;

	//calculate temp and humidity
	for(i = 32, pulseTime = tDMABuffer + 1, value = *(pulseTime + 1) - *pulseTime; i; --i, pulseTime++, value = *(pulseTime + 1) - *pulseTime){
		if((value > 65) && (value < 80))
			DHT22Data.value += (1 << (i - 1));
	}
	//calculate CRC
	for(i = 8, pulseTime = tDMABuffer + 33, value = *(pulseTime + 1) - *pulseTime; i; --i, pulseTime++, value = *(pulseTime + 1) - *pulseTime)	{
		if((value > 65) && (value < 80))
			DHT22CRC += (1 << (i - 1));
	}
	//check CRC
	addTask(DHT22Start, 0, 0);
	for(i = 0, byte = (char*)&DHT22Data.value, checksum = 0; i < 4; i++)
		checksum += *(byte + i);
	if(checksum != DHT22CRC)
		return 0;
	//if(DHT22Data.structShort.temp & 0x8000)

	return DHT22Data.value;
}

void DHT22Start()	{
    int i;
	TIM2->CR1 = 0;
	TIM2->CNT = 0;
//	DHT22Bits = 0;
//	curVal = tempBuffer;
//	GPIOA->CRL &= ~0xf << (PORTDATA * 4);
//	GPIOA->CRL |= outPP << (PORTDATA * 4);
	GPIOA->CRL	&= ~0xf0;
	GPIOA->CRL |= 0x30;
	//[ull down DATA pin for at least 2 millis
	GPIOA->BRR = 2;
	//delay_ms(3);
	for(i = 1000; i; i--);
	GPIOA->BSRR = 2;
	//reconfigure DAtA pin for using timer
	GPIOA->CRL	&= ~0xf0;
	GPIOA->CRL |= 0x40;
	TIM2->CR1 |= TIM_CR1_CEN + TIM_CR1_URS;
	DMA1_Channel7->CCR = 0;
	DMA1_Channel7->CNDTR = 50;
	DMA1_Channel7->CCR	 = DMA_CCR_MINC + DMA_CCR_MSIZE_0 + DMA_CCR_PSIZE_0 + DMA_CCR_EN;
}
