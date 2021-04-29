#include <stm32f103xb.h>
#include "os_func.h"
#include "hw_dma.h"
#include "hw_uart.h"
#include "logic.h"
//#include "hw_flash.h"

#define RX_BUFFER_LENGTH	50
#define MAX_COMMAND_LENGTH	12

char UARTRXBuffer[RX_BUFFER_LENGTH];
char operationBuffer[RX_BUFFER_LENGTH];
char receivedSymbol[2];
//extern short unsigned PWMPoints[7];
//extern char HALL_MULT, HALL_DIV;
char *ukRXBuffer = UARTRXBuffer;
int PROCESSING, x1;
extern struct _terminalWindow Console;
//extern const unsigned short pwmPoints[7];

struct commandDescription	{
	char *name;
	char *about;
	int 	paramsCount;
};
struct commandDescription terminalKeywords[]	=	{
	{"help"," Set pwm range - 'pwmSet E0 E1 E2 E3 E4 E5 E6', Show pwm range - 'pwmShow'  \
						\r\nSet Hall multiple and divider factor - 'hallSet MULT DIV',  Show - 'hallShow'  \
						\r\nfor save all values in FLASH use 'save'", 0},
	{"pwmSet", "", 7},
	{"pwmShow", "", 0},
	{"hallShow", "", 0},
	{"hallSet", "", 2},
	{"save", "", 0},
	{"reboot", "", 0}
};



void UART3Init()	{
	RCC->APB2ENR	|= RCC_APB2ENR_IOPAEN + RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR	|= RCC_APB1ENR_USART3EN;
	GPIOB->CRH &= ~0xff00;
	GPIOB->CRH |= 0x8b00;
	GPIOB->BSRR =	1	<< 11;

	USART3->CR1 = USART_CR1_UE + USART_CR1_TE + USART_CR1_RE + USART_CR1_RXNEIE;
	USART3->BRR = 313; //115200
	//USART3->CR3 = USART_CR3_DMAR + USART_CR3_DMAT;

	NVIC_EnableIRQ(USART3_IRQn);

	receivedSymbol[1] = 0;
	//brr - 39, mant-
}
void terminalNewLine()	{
	wait_for_event((unsigned short*) &USART3->SR, USART_SR_TXE, 1, 3000);
	USART3->DR = '\r';
	wait_for_event((unsigned short*) &USART3->SR, USART_SR_TXE, 1, 3000);
	USART3->DR = '\n';
}



int checkParamsCount(char *string, struct commandDescription *cd)	{
	int i;
	i = wc(string, ' ');
	if( (i) != cd->paramsCount)	{
		UART3SendString("Too few arguments: ");
		UART3SendString(float_to_string(i, 1, 0));
		UART3SendString(" instead ");
		UART3SendString(float_to_string(cd->paramsCount, 1, 0));
		return 1;
	}
	else
		return 0;
}


int checkString(char *string)	{
	int i, x = sizeof(terminalKeywords) / sizeof(terminalKeywords[0]);

	for(i = 0; i < x; i++)	{
		//ukOperationBuffer = operationBuffer;
		if(stringTest(getOneWord(string, 0), ((char *)terminalKeywords[i].name)) == 0)	{
				break;
		}}

		if(i == x)	{
			UART3SendString("\r\nInvalid string. Type help to see more about syntax\r\n");
			return 1;
		}
		switch(i)	{
			case 0: { UART3SendString(terminalKeywords[0].about); break;} //help
			case 6: {NVIC_SystemReset(); break;}
		}


		for(i = 0; i < RX_BUFFER_LENGTH; i++)
			UARTRXBuffer[i] = 0;
	return 1;
}
void specialSymbol(char code)	{
	switch(code)	{
		case '\r':	{	//received Enter

			terminalNewLine();
			*ukRXBuffer = 0; //end line ('\n')
			ukRXBuffer = UARTRXBuffer;	//RESET address counter
			checkString(UARTRXBuffer);
			//UART3SendString(operationBuffer);


			terminalNewLine();
		}
	};
}
void USART3_IRQHandler()	{	//recieve one byte
	char byte = USART3->DR;
	PROCESSING = 15;
	*receivedSymbol = byte;
	cPrint(&Console, receivedSymbol);
	if(byte < 32)	{
		specialSymbol(byte);
		return;
	}
	else{
		*ukRXBuffer = byte;
		ukRXBuffer++;
		while(DMA1_Channel2->CNDTR);
		wait_for_event((unsigned short*) &USART3->SR, USART_SR_TXE, 1, 3000);
		USART3->DR = byte;
	}

}

void UART3SendString(char *string)	{
	while(*string)	{
		wait_for_event((unsigned short*) &USART3->SR, USART_SR_TXE, 1, 3000);
		USART3->DR = *string;
		string++;
	}
}

void UART1SendByte(char byte)	{
	USART1->DR = byte;
};
