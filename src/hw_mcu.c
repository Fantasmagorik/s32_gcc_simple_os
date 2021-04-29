#include <stm32f103xb.h>
#define BLINK_IO    13

extern int TASK_EN;
volatile uint32_t delayValue;

int RCCInit()
{
    int delay;
    RCC->CR= RCC_CR_HSEON;
    FLASH->ACR = 2; //two tacts will passed for flash

    RCC->CFGR= RCC_CFGR_PLLSRC + (7<< 18) + (1<<10) + (3<<14); //PLL source from HSE, mult on 9 (8 * 9 = 72MHz and prescaller for APB1 to 2 = 72/2 = 36MHZ(APB1limit) and set divider for ADC = PLCLK2/4
    for(delay= 20; delay; delay--) asm("nop");//wait to clear HSERDY bit after init -- ref
    for(delay= 10000; delay && !(RCC->CR& RCC_CR_HSERDY); delay--)
        asm("nop");
    if(!RCC->CR& RCC_CR_HSERDY) return 2;//Fail to switch on HSE
    RCC->CR|= RCC_CR_PLLON;
    for(delay= 100000; delay && !(RCC->CR& RCC_CR_PLLRDY); delay--)
        asm("nop");
    if(!RCC->CR& RCC_CR_PLLRDY) return 4;//Fail to switch on PLL
    RCC->CFGR|= RCC_CFGR_SW_PLL; //set PLL as a freq source
    //RCC->CFGR|= RCC_CFGR_SW_HSE;
    return 0;
}


int hwInit()
{
    RCCInit();
    SysTickInit();
    //SysTimerInit();
    //UART3Init();
    //UART3SendString("\r\nUART Init     DONE\r\n");
    SPI1Init();
    //UART3SendString("\r\nSPI Init      DONE\r\n");
    ILI9341Init();
    //delay_ms(200);
    DMAInit();
    //UART3SendString("\r\nILI9341 Init  FAIL\r\n");

}



int SysTickInit()
{

    //According fresh ideas use 500Hz sys_tick for delay as a part and the full task_manager (later)
    RCC->APB2ENR	|= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH |= 3<<20; //gen push-pull for pc13

    SysTick->CTRL= 0;
    SysTick->LOAD= 72000;///*0x6ddd00;//*/MILLIS; //every 1 ms
    SysTick->CTRL= 7; //enable timer and allow interrupt request

    //NVIC_EnableIRQ(SysTick_IRQn);		//!! System interrupt vectors doesnt need any manipulations
    //__NVIC_SetPriority(SysTick_IRQn, 13);//?


    return 0;

}


void SysTick_Handler()	 	//every 1 microsecond
{
    SysTick->VAL = 0;	//it`s clear Pending bit
    //GPIOC->ODR ^= BLINK_IO;
    if(delayValue)
        delayValue--;
    if(TASK_EN)
        TaskTick();
}

void delay_ms(uint32_t timeout) {
    delayValue = timeout;
    while(delayValue > 0)
        asm("nop");
}

int wait_for_event(uint16_t *byte_addr, uint16_t mask, uint8_t expected_value, uint32_t timeout)
{
    delayValue = timeout;
    while(delayValue> 0)
    {
        if( ((*byte_addr) & mask )> 0 ==( expected_value > 0) ) //
            return 0;
    }
    return 1;
}
