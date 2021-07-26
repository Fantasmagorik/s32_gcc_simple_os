#include "stm32f103xb.h"
#include "hw_mcu.h"
#include "os_func.h"
#include "common_types.h"
#include "ex_ili9341.h"
#include "hw_rtc.h"
#include "logic.h"
#include "ex_dht22.h"

extern uint32_t    lastTimeStatisticsCollect, COUNT_OF_WAIT_PROCESSES, COUNT_OF_ALL_PROCESSES;
ftime_t *time;

struct _fontInformation* fontArial24Init(void);
struct _fontInformation* fontCentury16Init();
struct _fontInformation* fontLucida10Init();
struct _fontInformation *curFont;

char buffStatistics[300];


struct _terminalWindow Console, Info, Clock, DHT, DHT1, statistics;
extern const short unsigned colors16[] ;

uint16_t currColor = 0;
uint16_t currColor1 = 13;

union DHT22Data dhtData;
    char *str;

void printTime()    {
    char *str;
    cClear(&Clock);
    time = RTCGetTime();
    str = arrayToString(&time->minute, 2, ':', decimal_2);
    cPrint(&Clock, str);

    }
void printDHT22()   {

    dhtData.value= DHT22GetValue();
    //str = float_to_string(dhtData.structShort.temp, decimal_1);
    cClear(&DHT);
    cPrint(&DHT, stradd(float_to_string(dhtData.structShort.temp, 10, 1), " C"));
    cClear(&DHT1);
    cPrint(&DHT1, stradd(float_to_string(dhtData.structShort.humid, 10, 1), " %"));

}
void colorWindows() {
    Console.colorB = colors16[currColor];
    currColor++;
    if(currColor >= 9)
        currColor = 0;
    Console.colorA = colors16[currColor];
    cClear(&Console);
    cPrint(&Console, "CONSOLE window present");
}

void colorWindows1() {
    Info.colorB = colors16[currColor1];
    currColor1++;
    Info.colorA = colors16[currColor1];
    if(currColor1 >= 9)
        currColor1 = 0;
    cClear(&Info);
    //cPrint(&Info, "INFO window here");
}

void blink()    {
    GPIOC->ODR ^= 1<< 13;
}
void testSpi()  {
    while(SPI1->SR & SPI_SR_BSY);
    SPI1->DR = 200;
}
void printStatistics()  {
    int i;
    char *ptrBuffer = buffStatistics;
    buffStatistics[20] = 0;
    cClear(&statistics);
    *ptrBuffer = 0;
    //stradd(ptrBuffer, "All tasks: ");

    //stradd(ptrBuffer, "\nWait: ");
    //stradd(ptrBuffer, float_to_string(COUNT_OF_WAIT_PROCESSES, 1, 1));
    *(ptrBuffer + bytesInChar(buffStatistics) + 1) = 0;
    cPrint(&statistics, "Statistics\nwindow");


}
void main()	{
	int i;
	struct coordinates coord, coord1, coord2, *pos = &coord;
    hwInit();
    //delay_ms(200);


    SPI1DMAFill(0, 319, 0, 239, CYAN);
	Console.colorA                  = WHITE;//0xca82;
	Console.colorB                  = BLACK;
	Console.font                    = fontCentury16Init();
	Console.position.xPosition	    = 0;
	Console.position.xWidth	        = 319;
	Console.position.yPosition	    = 110;
	Console.position.yHeight        = 110;
	Info.colorA                     = BLACK;
	Info.colorB                     = DCYAN;
	Info.font                       = fontLucida10Init();
	Info.position.xPosition         = 0;
	Info.position.xWidth            = 319;
	Info.position.yPosition         = 221;
	Info.position.yHeight           = 18;

	Clock.colorA                    = BLACK;
	Clock.colorB                    = CYAN;
	Clock.font                      = Console.font;
	Clock.position.xPosition        = 255;
	Clock.position.xWidth           = 57;
	Clock.position.yPosition        = 5;
	Clock.position.yHeight          = 30;

	statistics.position.xPosition   = 0;
	statistics.position.yPosition   = 0;
	statistics.position.xWidth      = 230;
	statistics.position.yHeight     = 90;
	statistics.font                 = Clock.font;
	statistics.colorA               = BLACK;
	statistics.colorB               = DCYAN;

    DHT = Clock;
    DHT.position.yPosition += 32;
    DHT1 = DHT;
    DHT1.position.yPosition += 30;

	/*DHT.colorA                      = 0xffb0;
	DHT.colorB                      = 0x0;
	DHT.font                        = Console.font;
	DHT.position.xPosition          = 235;
	DHT.position.xWidth             = 319;
	DHT.position.yPosition          = 40;
	DHT.position.yHeight            = 75;*/

	//cClear(&Console);
	//delay_ms(500);
	TaskInit();

	//addTask(testSpi, 2000, 700);
	addTask(colorWindows, 800, 300);
	addTask(blink, 1000, 100);
	addTask(colorWindows1, 1000, 19);
	addTask(printTime, 1200, 1000);
	//addTask(DHT22Start, 1000, 1100);
	addTask(printDHT22, 1200, 1100);
	addTask(statisticsPrepare, 900, 1000);
	addTask(printStatistics, 1200, 1000);
	//addTask(ILI9341Init, 10000, 3000);
	//addTask(colorWindows, 1200, 3000);

	while(1)	{
        TaskDispatch(); //Launch the one of ready task, if it exist

}


}
