#include "stm32f103xb.h"
#include "hw_mcu.h"
#include "os_func.h"
#include "common_types.h"
#include "ex_ili9341.h"
#include "hw_rtc.h"
#include "logic.h"

ftime_t *time;

struct _fontInformation* fontArial24Init(void);
struct _fontInformation* fontCentury16Init();
struct _fontInformation* fontLucida10Init();
struct _fontInformation *curFont;


struct _terminalWindow Console, Info, Clock;
extern const short unsigned colors16 [] ;

uint16_t currColor = 0;
uint16_t currColor1 = 13;

void printTime()    {
    char *str;
    cClear(&Clock);
    time = RTCGetTime();
    str = arrayToString(&time->minute, 2, ':', decimal_2);
    cPrint(&Clock, str);

    }

void colorWindows() {
    Console.colorB = colors16[currColor];
    currColor++;
    if(currColor >= 15)
        currColor = 0;
    cClear(&Console);
    cPrint(&Console, "CONSOLE window present");
}

void colorWindows1() {
    Info.colorB = colors16[currColor1];
    currColor1++;
    Info.colorA = colors16[currColor1];
    if(currColor1 >= 15)
        currColor1 = 0;
    cClear(&Info);
    cPrint(&Info, "INFO window here");
}

void blink()    {
    GPIOC->ODR ^= 1<< 13;
}
void testSpi()  {
    while(SPI1->SR & SPI_SR_BSY);
    SPI1->DR = 200;
}

void main()	{
	int i;
	struct coordinates coord, coord1, coord2, *pos = &coord;
    hwInit();
    //delay_ms(200);



	Console.colorA = 0xffff;//0xca82;
	Console.colorB = 0x0;
	Console.font = fontCentury16Init();
	Console.position.xS	=	0;
	Console.position.xE	=	319;
	Console.position.yS	=	110;
	Console.position.yE	=	220;
	Info.colorA = 0xffff;
	Info.colorB = 0x03EF;
	Info.font = fontLucida10Init();
	Info.position.xS = 20;
	Info.position.xE = 230;
	Info.position.yE = 80;
	Info.position.yS = 5;

	Clock.colorA = 0xffb0;
	Clock.colorB = 0x0;
	Clock.font = Console.font;
	Clock.position.xS = 255;
	Clock.position.xE = 312;
	Clock.position.yS = 5;
	Clock.position.yE = 35;


	//delay_ms(500);
	TaskInit();

	//addTask(testSpi, 2000, 700);
	addTask(colorWindows, 800, 30);
	addTask(blink, 1000, 100);
	addTask(colorWindows1, 1000, 29);
	addTask(printTime, 1200, 1000);
	//addTask(ILI9341Init, 10000, 3000);
	//addTask(colorWindows, 1200, 3000);

	while(1)	{
        TaskDispatch(); //Launch the one of ready task, if it exist

}


}
