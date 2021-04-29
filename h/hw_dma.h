#include "common_types.h"


void DMAInit(void);
//int setTaskI2C(struct I2CTask);
void DMA1_Channel6_IRQHandler(void);
//is that place good?
void SSD1306Fill(char xS, char xE, char yS, char yE, char data);
int SSD1306Print(char *data, char xS, char yS, struct protoFontInformation *fontInfo);
int SPI1DMASetPosition(short x, short y);

void UART3DMASendString(char *string);
void SPI1DMAFill(short xS, short xE, short yS, short yE, short color);
void SPI1DMAPrint(char *string, struct coordinates *pos, struct _fontInformation *font);
int getLetterImage(char symbol, struct _bufferInfo*, struct _terminalWindow *console);
enum graphicOperations	{
	Free = 0, Fill, Print, Draw, Blocked
};

struct _dmaTask{

	enum graphicOperations operation;
	int countOfData;
	union data{
		char *addressOfData;
		short color;
	}data;
	union service	{
		struct _terminalWindow *terminalWindow;
	}service;
	int transmittedData;
	struct coordinates coord;
};


int cPutc(struct _terminalWindow *console, char symbol);
int cPrint(struct _terminalWindow *console, char *string);
int cClear(struct _terminalWindow *console);

//int SPI1DMARead(int count);
int cScrollString(struct _terminalWindow *console);