#include <stm32f103xb.h>
#include "os_func.h"
#include "hw_spi.h"
#include "ex_ili9341.h"


//(SPI_TypeDef *);
#define SPI_INTERF	SPI1
#define DC	15
#define CS	4
#define	LED	8
#define RST	0

const short unsigned colors16 [] = {RED, YELLOW, GREEN, BLACK, ORANGE, BLUE, DGREEN, DCYAN, BROWN};
//extern char SPI1TXDMAEN;
int countRep = 0;

char fix = 4;
void ILI9341Init()	{
countRep++;
	GPIOA->CRH	&= ~0xf000000f;
	GPIOA->CRL	&= ~0x00f00ff;

	GPIOA->CRH	|= 0x30000003;
	GPIOA->CRL	|= 0x0030033;

	GPIOA->BSRR = 1 << RST;

	delay_ms(200);
	GPIOA->BRR = 1 << RST;
	delay_ms(200);
	GPIOA->BSRR = 1 << RST;
	delay_ms(200);
	GPIOA->BSRR	= 1<< LED;
	delay_ms(100);
	//GPIOA->BRR	= 1<< LED;
	//power control A
	ILI9341SendCmd(0xCB);
	ILI9341SendByte(0x39);
	ILI9341SendByte(0x2C);
	ILI9341SendByte(0x00);
	ILI9341SendByte(0x34);
	ILI9341SendByte(0x02);

	//power control B
	ILI9341SendCmd(0xCF);
	ILI9341SendByte(0x00);
	ILI9341SendByte(0xC1);
	ILI9341SendByte(0x30);

	//driver timing control A
	ILI9341SendCmd(0xE8);
	ILI9341SendByte(0x85);
	ILI9341SendByte(0x00);
	ILI9341SendByte(0x78);

	//driver timing control B
	ILI9341SendCmd(0xEA);
	ILI9341SendByte(0x00);
	ILI9341SendByte(0x00);

	//power on sequence control
	ILI9341SendCmd(0xED);
	ILI9341SendByte(0x64);
	ILI9341SendByte(0x03);
	ILI9341SendByte(0x12);
	ILI9341SendByte(0x81);

	//pump ratio control
	ILI9341SendCmd(0xF7);
	ILI9341SendByte(0x20);

	//power control,VRH[5:0]
	ILI9341SendCmd(0xC0);
	ILI9341SendByte(0x23);

	//Power control,SAP[2:0];BT[3:0]
	ILI9341SendCmd(0xC1);
	ILI9341SendByte(0x10);

	//vcm control
	ILI9341SendCmd(0xC5);
	ILI9341SendByte(0x3E);
	ILI9341SendByte(0x28);

	//vcm control 2
	ILI9341SendCmd(0xC7);
	ILI9341SendByte(0x86);

	//memory access control
	ILI9341SendCmd(0x36);
	ILI9341SendByte(0x68);

	//pixel format
	ILI9341SendCmd(0x3A);
	ILI9341SendByte(0x55);

	//frameration control,normal mode full colors
	ILI9341SendCmd(0xB1);
	ILI9341SendByte(0x00);
	ILI9341SendByte(0x18);

	//display function control
	ILI9341SendCmd(0xB6);
	ILI9341SendByte(0x08);
	ILI9341SendByte(0xA2);	//scan direction
	//ILI9341SendByte(0xc2);

	ILI9341SendByte(0x27);

	//3gamma function disable
	ILI9341SendCmd(0xF2);
	ILI9341SendByte(0x00);

	//gamma curve selected
	ILI9341SendCmd(0x26);
	ILI9341SendByte(0x01);

	//set positive gamma correction
	ILI9341SendCmd(0xE0);
	ILI9341SendByte(0x0F);
	ILI9341SendByte(0x31);
	ILI9341SendByte(0x2B);
	ILI9341SendByte(0x0C);
	ILI9341SendByte(0x0E);
	ILI9341SendByte(0x08);
	ILI9341SendByte(0x4E);
	ILI9341SendByte(0xF1);
	ILI9341SendByte(0x37);
	ILI9341SendByte(0x07);
	ILI9341SendByte(0x10);
	ILI9341SendByte(0x03);
	ILI9341SendByte(0x0E);
	ILI9341SendByte(0x09);
	ILI9341SendByte(0x00);

	//set negative gamma correction
	ILI9341SendCmd(0xE1);
	ILI9341SendByte(0x00);
	ILI9341SendByte(0x0E);
	ILI9341SendByte(0x14);
	ILI9341SendByte(0x03);
	ILI9341SendByte(0x11);
	ILI9341SendByte(0x07);
	ILI9341SendByte(0x31);
	ILI9341SendByte(0xC1);
	ILI9341SendByte(0x48);
	ILI9341SendByte(0x08);
	ILI9341SendByte(0x0F);
	ILI9341SendByte(0x0C);
	ILI9341SendByte(0x31);
	ILI9341SendByte(0x36);
	ILI9341SendByte(0x0F);

	//test
	//set brightness
//	ILI9341SendCmd(0x51);
//	ILI9341SendByte(0);
	//end test
	//
	//exit sleep
	ILI9341SendCmd(0x11);
	delay_ms(200);
	//display on
	ILI9341SendCmd(0x29);
	GPIOA->BSRR = 1 << LED;
	//delay(200);
}
void ILI9341ChipSelect(char level)	{

	if(level)
		GPIOA->BSRR = 1 << CS;
	else
		GPIOA->BRR  = 1 << CS;
}
void ILI9341SendCmd(char cmd)	{
	if(wait_for_event(&SPI1->SR, SPI_SR_TXE, 1, 700))
		return;
	GPIOA->BRR  = 1 << DC;
	GPIOA->BRR = 1 << CS;
	SPI1SendByte(cmd);
	GPIOA->BSRR = 1 << CS;
	GPIOA->BSRR = 1 << DC;
}

void ILI9341SendByte(char byte)	{
	if(wait_for_event(&SPI1->SR, SPI_SR_TXE, 1, 700))
		return;
	GPIOA->BSRR = 1 << DC;
	GPIOA->BRR = 1 << CS;
	SPI1SendByte(byte);
	GPIOA->BSRR = 1 << CS;
}
void ILI9341SendShort(short byte)	{
	if(wait_for_event(&SPI1->SR, SPI_SR_TXE, 1, 700))
		return;
	GPIOA->BSRR = 1 << DC;
	GPIOA->BRR = 1 << CS;
	SPI1Sendshort(byte);
	GPIOA->BSRR = 1 << CS;
}
int ILI9341SetPosition(short xS, short xE, short yS, short yE)	{
	ILI9341SendCmd(0x2A);
//	ILI9341SendByte(xS >> 8);
//	ILI9341SendByte(xS & 0xff);
//	ILI9341SendByte(xE >> 8);
//	ILI9341SendByte(xE & 0xff);
	ILI9341SendShort(xS);
	ILI9341SendShort(xE);
	ILI9341SendCmd(0x2B);
	ILI9341SendShort(yS);
	ILI9341SendShort(yE);
//	ILI9341SendCmd(0x2B);
//	ILI9341SendByte(yS >> 8);
//	ILI9341SendByte(yS & 0xff);
//	ILI9341SendByte(yE >> 8);
//	ILI9341SendByte(yE & 0xff);
	return (xE - xS + 1) * (yE - yS + 1);
}
