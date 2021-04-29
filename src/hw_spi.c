#include <stm32f103xb.h>
#include "os_func.h"
#include "hw_spi.h"

int SPI1Status;

void SPI1Init()	{
//	SPI1Status	=	5; //init
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN + RCC_APB2ENR_IOPAEN + RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= 1 << 25;	//JTAG disabled, SW enabled
	GPIOA->CRL &= ~0xffff0000;
	GPIOA->CRL |= 0xb4b30000;

	GPIOA->CRH &= ~0xf00;
	GPIOA->CRH |= 0x300;
	GPIOA->BRR = 0x400;

	SPI1->CR1 = 0x304; //master , fPclk/2
	SPI1->CR2 = SPI_CR2_TXDMAEN;
	SPI1->CR1 |= SPI_CR1_SPE;//enable spi
	//SPI1Status	= 0;; //wait
}

void SPI1SendByte(char data)	{
	//SPI1Status = 1; //set busy
	while((SPI1->SR & SPI_SR_BSY) );
	SPI1->DR = data;
	//wait_for_event((short unsigned *) & SPI1->SR, SPI_SR_BSY, 0, 500);
//	while(!(SPI1->SR & SPI_SR_TXE) );
	while((SPI1->SR & SPI_SR_BSY) );
	//(void) SPI1->DR;

}
void SPI1Sendshort(short data)	{
	//SPI1Status = 1; //set busy
	SPI1->DR = data;
	wait_for_event((short unsigned *) & SPI1->SR, SPI_SR_BSY, 0, 500);
	(void) SPI1->DR;

}
