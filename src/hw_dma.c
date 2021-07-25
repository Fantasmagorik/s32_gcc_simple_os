#include <stm32f103xb.h>
#include "common_types.h"
#include "hw_dma.h"
//#include "hw_i2c.h"
//#include "hw_ssd1306.h"
#include "hw_uart.h"
#include "ex_ili9341.h"
#include "logic.h"
#include "os_func.h"

struct _bufferInfo bufferInfo[2];
//char stringBuffer[100];
//unsigned short pX, pY;	//current coordinate to print the next letter
unsigned short colorBack;
char currentPrintBuffer = 0, exclusiveDMATransmitt = 0;
struct _dmaTask exDMATask;	//for exclusive DMA order (string scroll)
//struct _fontInformation *currentFont;

//#define SPACE_WIDTH	10

struct _fontInformation* fontArial24Init(void);

////#define DMA_EN *((unsigned int *)(0x42400D80)) //BitBand -> EN in DMA1_CCR6
//#define MAX_ELEM	I2C_QUEUE_LENHTH - 1
//#define ADC_BUFFER_LENGTH	18
//#define UART_BUFFER_LENGTH	32

#define DMA_MAX_TASK_COUNT	15

short ILI9341BKCOLOR = 0;
//SPI1_TX DMA SECTION



//short ILI9341_XS, ILI9341_XE, ILI9341_YS, ILI9341_YE; //active window coordinate
//char *SPI1TXDMADATA = 0;
//enum graphicOperations SPI1TXDMAOPERATION;
//int  SPI1TXDMACOUNTER = 0;
struct _dmaTask dmaTaskQuery[DMA_MAX_TASK_COUNT];
int currentDMATask = 0;



//unsigned short ADCBuffer[ADC_BUFFER_LENGTH];

//USART3_RX
//int cPutc(struct _terminalWindow *console, char symbol){
//
//}
int cNL(struct _terminalWindow *console)
{
#define DC	15
#define CS	4
#define	LED	8
#define RST	6
#include "os_func.h"
#include "hw_spi.h"
    int offset = 0, i;
    short data;
    if(console->posY + console->font->height * 2 + offset <= (console->position.yPosition + console->position.yHeight) )	//if window has enough place for new string
    {
        console->posY += console->font->height;
    }
    else
    {
        cScrollString(console);

    }

    console->posX = console->position.xPosition;
    return 0;
}
struct _dmaTask* getDMATask()
{
    int i, pos;
    struct _dmaTask *task ;

    for(i = 0, task = (currentDMATask + dmaTaskQuery), pos = currentDMATask; i < DMA_MAX_TASK_COUNT &&( (task->operation == Free) || (task->operation == Blocked)); i++)
    {
        if( (pos + i) >= DMA_MAX_TASK_COUNT )
            pos = 0;
        else
            pos++;
        task = (struct _dmaTask*) (dmaTaskQuery + pos);
//		if(task->operation > Blocked)
//			__ASM("nop");
        if((task->operation == Print || task->operation == Fill ))//|| (task->operation > Blocked) )
            break;
    }
    if( (i == DMA_MAX_TASK_COUNT) &&( task->operation == Free || task->operation == Blocked ) )  	//ckeck last element after cycle
    {
        return (struct _dmaTask*) 0;
    }
    else
    {
        //task->operation = Blocked;
        return task;
    }
}
struct _dmaTask* addDMATask()
{
    int i, pos;
    struct _dmaTask *task ;

    for(i = 0, task = (currentDMATask + dmaTaskQuery), pos = currentDMATask; i < DMA_MAX_TASK_COUNT && task->operation != Free; i++)
    {
        if( (pos + i) >= DMA_MAX_TASK_COUNT )
            pos = 0;
        else
            pos++;
        task = (struct _dmaTask*) (dmaTaskQuery + pos);
//		if(task->operation > Blocked)
//			__ASM("nop");
        if((task->operation == Free) )//|| (task->operation > Blocked) )
            break;
    }
    if( (i == DMA_MAX_TASK_COUNT) && (task->operation != Free) )  	//ckeck last element after cycle
    {
        return (struct _dmaTask*) 0;
    }
    else
    {
        task->operation = Blocked;
        return task;
    }


//	_Task->operation			 = Blocked;
//	if(task->operation == Fill)
//		_Task->data.color	 		 = task->data.color;
//	else
//		_Task->data.addressOfData = task->data.addressOfData;
//	_Task->countOfData		 = task->countOfData;
//	_Task->transmittedData = 0;
//	_Task->coord.xE				 = task->coord.xE;
//	_Task->coord.xS				 = task->coord.xS;
//	_Task->coord.yE				 = task->coord.yE;
//	_Task->coord.yS				 = task->coord.yS;
//	_Task->operation			 = task->operation;
//	if( (SPI1->SR & SPI_SR_BSY) == 0) {		//wake-up DMA Channel, if SPI not under communicate
//		NVIC_SetPendingIRQ(DMA1_Channel3_IRQn);
//	}
//	return i;
}
int nextChar(char symbol)
{

    return 0;
}

void DMAInit()
{
    RCC->AHBENR|= RCC_AHBENR_DMA1EN;
    //usart3
//	DMA1_Channel2->CPAR = (int) &USART3->DR;
//	DMA1_Channel2->CCR = DMA_CCR_MINC + DMA_CCR_DIR; //read from memory, mem increment
    //Channel 6 for I2C transfers
//	DMA1_Channel6->CCR= DMA_CCR6_TCIE+ DMA_CCR6_DIR; //read from memory and full dma transmission interrupt enable
//	DMA1_Channel6->CPAR= (int) &(I2C1->DR);
//	NVIC_SetPriority(DMA1_Channel6_IRQn, 14);
//	NVIC_EnableIRQ(DMA1_Channel6_IRQn);
    //Channel 1 for ADC
//	DMA1_Channel1->CCR 		= DMA_CCR_MSIZE_0 + DMA_CCR_PSIZE_0 + DMA_CCR_MINC + DMA_CCR_TCIE + DMA_CCR_CIRC;
//	DMA1_Channel1->CPAR 	= (int) &ADC1->DR;
//	DMA1_Channel1->CMAR 	= (int) ADCBuffer;
//	DMA1_Channel1->CNDTR 	= ADC_BUFFER_LENGTH;
//	DMA1_Channel1->CCR 	 |= DMA_CCR_EN;
//	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    //*//
    //here some adding startup code for printfBuffer init
    //currentFont = fontArial24Init();
    //Channel 3 for SPI1 transfers (TX)
    DMA1_Channel3->CCR 	= DMA_CCR_DIR + DMA_CCR_MSIZE_0 + DMA_CCR_PSIZE_0 + DMA_CCR_TCIE;
    DMA1_Channel3->CPAR	= (int) &SPI1->DR;
    SPI1->CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 |= SPI_CR1_DFF;
    SPI1->CR1 |= SPI_CR1_SPE;
    //Channel 2 for SPI1 transfer (RX)
    DMA1_Channel2->CCR 	= /*DMA_CCR_MSIZE_0 + DMA_CCR_PSIZE_0 + */DMA_CCR_TCIE + DMA_CCR_MINC;
    DMA1_Channel2->CPAR	= (int) &SPI1->DR;
    DMA1_Channel2->CMAR	= (int) bufferInfo[0].bufferDataStart;

    NVIC_EnableIRQ(DMA1_Channel3_IRQn);
    //NVIC_EnableIRQ(DMA1_Channel2_IRQn);
}
void SPI1DMAFill(short xS, short xE, short yS, short yE, short color)
{
    struct _dmaTask *task = addDMATask();
    if(task == (struct _dmaTask*) 0)
    {
        return;
    }	//no place for the task

    task->countOfData = (xE - xS + 1) * (yE - yS + 1);
    task->xS				    = xS;
    task->xE        			= xE;
    task->yS 			    	= yS;
    task->yE 				    = yE;
    task->data.color			= color;
    task->transmittedData	    = 0;
    task->operation				= Fill;
    if( (SPI1->SR & SPI_SR_BSY) == 0) 	//wake-up DMA Channel, if SPI not under communicate
        NVIC_SetPendingIRQ(DMA1_Channel3_IRQn);
}

void DMA1_Channel2_IRQHandler()	 	//SPI1 RX
{
    //if we`re here, all data recieved
    short xS, xE, yS, yE;
    DMA1->IFCR|= DMA_IFCR_CGIF2;	//clear flag
    DMA1_Channel2->CCR &= ~DMA_CCR_EN;				//DISABLE DMA ch
    ILI9341ChipSelect(1);
    SPI1->CR1 = 	0xB04;	//default state
    SPI1->CR1	|=	SPI_CR1_SPE;
//	xS = exDMATask.service.terminalWindow->position.xS;
//	xE = 40;//exDMATask.service.terminalWindow->position.xE;
//	yS = exDMATask.service.terminalWindow->position.yS;
//	yE = yS + exDMATask.service.terminalWindow->font->height - 1;

    xS = exDMATask.service.terminalWindow->position.xPosition;
    xE = 40;//exDMATask.service.terminalWindow->position.xE;
    yS = exDMATask.service.terminalWindow->position.yPosition;
    yE = yS + exDMATask.service.terminalWindow->font->height - 1;
    exclusiveDMATransmitt = 1;

    DMA1_Channel3->CNDTR = ILI9341SetPosition(xS, xE, yS, yE) * 3;
    ILI9341SendCmd(0x2C);
    ILI9341ChipSelect(0);
    DMA1_Channel3->CMAR = (int) bufferInfo[0].bufferDataStart;
    DMA1_Channel3->CCR = DMA_CCR_DIR + /*MA_CCR1_MSIZE_0 + DMA_CCR_PSIZE_0 + */DMA_CCR_TCIE  + DMA_CCR_MINC;
    DMA1_Channel3->CCR |= DMA_CCR_EN;


}

void DMA1_Channel3_IRQ_Task()
{
    struct _dmaTask *task = (dmaTaskQuery + currentDMATask);
    struct _bufferInfo *pCurrentBuffer;
    int i, dataLast, x;
    char isNotPrinted = 0;
    unsigned short pX, pY;

    dataLast = (task->countOfData - task->transmittedData);
    if(task->operation == Print)
        ILI9341ChipSelect(1);;	//after every letter
    if(dataLast == 0)	 	//it was last part of transaction
    {
        task->operation = Free;
        ILI9341ChipSelect(1);				//Free ili9341
        i = ( (currentDMATask + 1) == DMA_MAX_TASK_COUNT) ? 0: currentDMATask + 1;	//query position through 0 if POSITION = MAX_VALUE
        if(currentDMATask + 1 == DMA_MAX_TASK_COUNT)
        task = getDMATask();
        if(task == (struct _dmaTask*) 0)
            return;
        //have another task
        currentDMATask 	= i;
        task 						= dmaTaskQuery + currentDMATask;
        dataLast 				= task->countOfData;
    }
    switch(task->operation)
    {
    case Fill:
    {
        if(task->transmittedData == 0)   //first transaction in that task
        {
            ILI9341SetPosition(task->xS, task->xE, task->yS, task->yE);
            ILI9341SendCmd(0x2C);
            ILI9341ChipSelect(0);
        }
        DMA1_Channel3->CNDTR 	= 	(dataLast > 65535) ? 65535 : dataLast;
        task->transmittedData += 	DMA1_Channel3->CNDTR;
        DMA1_Channel3->CMAR		=		(int) &(task->data.color);
        DMA1_Channel3->CCR = DMA_CCR_DIR + DMA_CCR_MSIZE_0 + DMA_CCR_PSIZE_0 + DMA_CCR_TCIE + DMA_CCR_EN;
        break;
    }
    case Print:
    {
        pCurrentBuffer = (struct _bufferInfo*) bufferInfo + currentPrintBuffer;
        i = (int) *(task->data.addressOfData + task->transmittedData);
        if( (i == '\n') || (i == 0xd) )
        {
            task->transmittedData++;
            cNL(task->service.terminalWindow);
            isNotPrinted = 1;
            if(dataLast - 1 == 0)
            {
                task->transmittedData = task->countOfData;
                if(i == '\n')
                    NVIC_SetPendingIRQ(DMA1_Channel3_IRQn);
                return;
            }
        }
        if(/*dataLast && */(task->transmittedData == 0) || isNotPrinted)   //first transaction in that task
        {
            getLetterImage(*(task->data.addressOfData + task->transmittedData), pCurrentBuffer, task->service.terminalWindow);
        }
        pX = task->service.terminalWindow->posX;
        pY = task->service.terminalWindow->posY;
        ILI9341ChipSelect(1); //free display
        i = ILI9341SetPosition(pX, pX + pCurrentBuffer->letterWide -1, pY, pY + task->service.terminalWindow->font->height - 1);
        ILI9341SendCmd(0x2C);
        ILI9341ChipSelect(0);
        task->service.terminalWindow->posX += (pCurrentBuffer->letterWide +
                                               task->service.terminalWindow->font->letterOffset);	//nextChar position
        pX = task->service.terminalWindow->posX;
        DMA1_Channel3->CMAR  = (int) pCurrentBuffer->bufferDataStart;
        DMA1_Channel3->CNDTR = i;
        task->transmittedData++;
        DMA1_Channel3->CCR = DMA_CCR_DIR + DMA_CCR_MSIZE_0 + DMA_CCR_PSIZE_0 + DMA_CCR_TCIE + DMA_CCR_EN + DMA_CCR_MINC;
        //prepare next transmit
        currentPrintBuffer ^= 1;
        i = (int) *(task->data.addressOfData + task->transmittedData);	//char number
        if(i == 0) //it was the last char
            return;

        if(i == 32)
            x = task->service.terminalWindow->font->letterOffset * 4;
        else
            x = (task->service.terminalWindow->font->lettersInfoStart + (i - 33))->width;	//charToInt width
        if((i == '\n')  || ((pX + task->service.terminalWindow->font->letterOffset + x) >= (task->service.terminalWindow->position.xPosition + task->service.terminalWindow->position.xWidth) ) )	//new string symbol
        {
            cNL(task->service.terminalWindow);
            if( (i == '\n') || (i == 0xd) )
                task->transmittedData ++;
        }
        getLetterImage(*(task->data.addressOfData + task->transmittedData), (struct _bufferInfo*) bufferInfo + currentPrintBuffer, task->service.terminalWindow);
    }

        //set
        // some like int newSymbole( char ) - return - break, 0- success, new_string,
    }
}

void DMA1_Channel3_IRQHandler()	 			//SPI1 DMA TC (Transfer Complete)
{


    DMA1->IFCR|= DMA_IFCR_CGIF3;	//clear flag
    DMA1_Channel3->CCR = 0;				//DISABLE DMA ch
    DMA1_Channel3_IRQ_Task();
    //addTask(DMA1_Channel3_IRQ_Task, 0, 0);

}

int getLetterImage(char symbol, struct _bufferInfo *buff, struct _terminalWindow *console)
{
    int  x, y, SPACE_WIDTH = console->font->letterOffset * 4;
    unsigned char i;
    struct _lettersInfo	*position;
    unsigned short colorActive = 0xffff;//, colorBack ;//= 0x03EF;
    char 	 *currentByteFont;
    unsigned short  *currentByteMem;

    currentByteMem = (unsigned short*) buff->bufferDataStart;
    if(symbol > 96)
        --symbol;
    if(symbol < 32)	 	//not printed
    {
        return 0;
    }
    if(symbol == 32)	 	//space
    {
        for(x = 0; x < console->font->height; x++)
        {
            for(y = 0; y < SPACE_WIDTH; y++, currentByteMem++)
            {
                *currentByteMem = console->colorB;
            }
        }
        buff->letterWide = SPACE_WIDTH;
        buff->bufferState = ready;
        return 0;
    }
    position =  (struct _lettersInfo*) console->font->lettersInfoStart + symbol - 33;
    currentByteFont = (char*) console->font->fontDataStart + position->position;
    buff->letterWide = position->width;
    x = position->width * console->font->height;	// total pixel to send
    if(x > PRINTBUFFERLENGTH / 2)
        return 0;
    for(x = console->font->height; x; x--)
    {
        for(y = position->width, i = 128; y; y--)
        {
            if(*currentByteFont & i)
                *currentByteMem = console->colorA;
            else
                *currentByteMem = console->colorB;

            i >>= 1;
            if(i == 0)
            {
                if(y > 1)
                    currentByteFont++;
                i = 128;
            }
            currentByteMem++;
        }
        currentByteFont++;
    }
    //here we should check if we have a real picture of asked symbol- check bits resolution in fontType
    //---
}

int cClear(struct _terminalWindow *console)
{
    SPI1DMAFill(console->position.xPosition, console->position.xPosition + console->position.xWidth, console->position.yPosition, console->position.yPosition + console->position.yHeight, console->colorB);
    console->posX = console->position.xPosition;
    console->posY = console->position.yPosition;
    return 0;
}
int cPrint(struct _terminalWindow *console, char *string)
{
    struct _dmaTask *task = addDMATask();
    if(task == (struct _dmaTask*) 0)
    {
        return 0;
    }	//
    task ->countOfData 						= strlen(string);
    task->data.addressOfData 			    = string;
    task->service.terminalWindow	        = console;
    task->transmittedData					= 0;
    task->operation							= Print;
    if( (SPI1->SR & SPI_SR_BSY) == 0) 	//wake-up DMA Channel, if SPI not under communicate
        NVIC_SetPendingIRQ(DMA1_Channel3_IRQn);
    return 0;
}

int cScrollString(struct _terminalWindow *console)
{
    int count;
    short xS, xE, yS, yE;
    exclusiveDMATransmitt = 1;
    //PRINTBUFFERLENGTH //buffer length in word
    //console->font->height	// 	line height
    exDMATask.service.terminalWindow = console;
    xS = console->position.xPosition;
    xE = 40;
    yS = console->position.yPosition + console->font->height * 7;
    yE = yS + console->font->height - 1;

//	xS = 0;
//	xE = 30;
//	yS = 00;
//	yE = 30;

    //console->position.xE -console->position.xS +1;	// line length

    DMA1_Channel2->CNDTR = ILI9341SetPosition(xS, xE, yS, yE) *3;
    ILI9341SendCmd(0x3A);
    ILI9341SendShort(0x6600);
    //DMA1_Channel1->CMAR = (int) bufferInfo[0].bufferDataStart;
    GPIOA->BSRR = 1 << DC;
    GPIOA->BRR  = 1 << DC;
    GPIOA->BRR = 1 << CS;
    //SPI1SendByte(46);
    //wait_for_event(&SPI1->SR, SPI_SR_BSY, 0, 70);
//	GPIOA->BSRR = 1 <<  CS;
//	GPIOA->BRR = 1 << CS;
//	GPIOA->BSRR = 1 << DC;
//	SPI1SendByte(0x2e);
    //SPI1DMARead(320);

    DMA1_Channel2->CCR |= DMA_CCR_EN;
    //exclusiveDMATransmitt = 1;
    SPI1->CR1 |= SPI_CR1_RXONLY;

}

//int SPI1DMARead(int count)	{
//	exclusiveDMATransmitt = 1;
//	DMA1_Channel2->CNDTR = count;
//	DMA1_Channel2->CCR |= DMA_CCR_EN;
//	SPI1->CR1 |= SPI_CR1_RXONLY;
//
//
//}

//int SSD1306Print(char *data, char xS, char yS, struct protoFontInformation *fontInfo)	{
//	struct I2CTask cTask;
//	struct protoFontDescriptors *letterInfo;
//	while (DMA_EN);//wait dma free status
//	DMA_EN = 1;
//	//200 means that print continues the current string
//	curX = (xS == 200)? curX: xS;
//	curY = (yS == 200)? curY: yS;
//	currentChar = data;
//	font = fontInfo;
//	currentOperation = PRINT;
//	NVIC_SetPendingIRQ(DMA1_Channel6_IRQn);
//
//}

//void DMA1_Channel6_IRQHandler()	{	//Transfer completed /- 6 ch- I2C
//	//(void) DMA_EN;
//		struct protoFontDescriptors *letterInfo;
//		char *addr, CCR;
//		DMA1->IFCR|= DMA_IFCR_CGIF6; //clear interrupt flag global
//		DMA1_Channel6->CCR= 0;//~DMA_CCR6_EN; // dma transmission interrupt disable
//		I2C1Stop();
//
//	//Last operation?
//		if (currentOperation == FILLEND)	{
//			DMA_EN = 0;
//			return;
//		}

//		if (currentOperation == PRINT)	{
//
//			if (*currentChar == 0)	{	//last char ?
//				DMA_EN = 0;
//				return;
//			}
//
//			letterInfo = (struct protoFontDescriptors*) (font->addrFontDescriptor + (*currentChar - font->firstCharCode) );
//			addr = (*currentChar == 32)? &t0: (char *)(font->addrFontDataStart + letterInfo->position);
//			if (*currentChar == 32) {		//SPACE ?
//				addr = &t0;
//				xE = curX + 7;
//				CCR = 0x13;
//			}
//			else	{
//				addr = (char *)(font->addrFontDataStart + letterInfo->position);
//				xE = curX + letterInfo->width - 1;
//				yE = curY + font->height / 8 - 1;
//				CCR = 0x93;
//			}
//	}
//		else		//Fill
//			if (currentOperation == FILL)	{
//				addr = currentChar;
//				CCR	 = 0x13;
//				currentOperation = FILLEND;
//			}
//			I2C1SendAddress(SSD1306_Address);
//			DMA1_Channel6->CMAR = (int) addr;
//			DMA1_Channel6->CNDTR= SSD1306SetWindow(curX, xE, curY, yE);
//			I2C1SendByte(SSD1306_MODE_DATs);
//			DMA1_Channel6->CCR = CCR;
//			if(currentOperation == PRINT)	{
//				currentChar++;
//				curX = xE+ 1;
//			}

//		}

//void SSD1306Fill(char xS, char x_E, char yS, char y_E, char data)	{
//	while (DMA_EN);//wait dma free status
//	DMA_EN = 1;
//	currentChar = &data;
//	curX = xS;
//	curY = yS;
//	xE	 = x_E;
//	yE	 = y_E;
//	currentOperation = FILL;
//	NVIC_SetPendingIRQ(DMA1_Channel6_IRQn);
//
//	}
////ADC handler
//void DMA1_Channel1_IRQHandler()	{	//
//	int fcounter = 0, adcValue1 = 0, adcValue2 = 0, adcValue3 = 0;
//	DMA1->IFCR|= DMA_IFCR_CGIF1; //clear interrupt flag global
//	while(fcounter < ADC_BUFFER_LENGTH)	{
//		adcValue1+= *(ADCBuffer + fcounter++);
//		adcValue2+= *(ADCBuffer + fcounter++);
//		adcValue3+= *(ADCBuffer + fcounter++);
//	}
//	ADC_DATA1 = adcValue1 /(ADC_BUFFER_LENGTH / 3);
//	ADC_DATA2 = adcValue2 /(ADC_BUFFER_LENGTH / 3);
//	ADC_DATA3 = adcValue3 /(ADC_BUFFER_LENGTH / 3);
//	__ASM("nop");
//
//
//}
