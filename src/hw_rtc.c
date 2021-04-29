#include <stm32f103xb.h>
#include "hw_rtc.h"
#include "os_func.h"
#include "l_ftime.h"
#include "logic.h"
//#include "l_interface.h"
#include "common_types.h"
//#define JD0 2451911 // ???? ?? 01 ??? 2001 ??
#define   JD0 2458850	//jan 2020
ftime_t RTCDate;



void BKPEnable(enum SETState state)	{
	if(state == enable)	{
	RCC->APB1ENR |= RCC_APB1ENR_PWREN ;//+ RCC_APB1ENR_BKPEN;
	PWR->CR |= PWR_CR_DBP;
	}
	else
	{
		PWR->CR &= ~PWR_CR_DBP;
		RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;
	}
}

void RTCSetState(int state)	{
	BKPEnable(enable);
	RCC->BDCR	|= RCC_BDCR_LSEON;
	if(wait_for_event((short*) &RCC->BDCR, RCC_BDCR_LSERDY, 1, 500))
		return;
	RCC->BDCR = 0x101;
	if(state == enable)	{
		RCC->BDCR +=  0x8000;
		//RTC->CRH |= RTC_CRH_SECIE;
		//NVIC_EnableIRQ(RTC_IRQn);
	}
	BKPEnable(disable);
}


int RTCSetTime(char *string)	{

	int counter, i; char *str = string, word[3], *key;
	short dateH , dateL, x;
	ftime_t ft;
	word[2] = 0;
	if(strlen(string) != 13)
		return BADF;

	for(i = 0, key = word; i < 2; i++, key++)
		*key = *str++;
	ft.day = charToInt(word);
	if(ft.day > 31)
		return WDATE;
	for(i = 0, key = word; i < 2; i++, key++)
		*key = *str++;
	ft.month = charToInt(word);
	if(ft.month > 12)
		return WDATE;

	for(i = 0, key = word; i < 2; i++, key++)
		*key = *str++;
	ft.year = 2000 + charToInt(word);
	if(ft.year < 20)
		return WDATE;
	for(i = 0, key = word; i < 2; i++, key++)
		*key = *str++;
	ft.hour = charToInt(word);
	if(ft.hour > 23)
		return WDATE;

	for(i = 0, key = word; i < 2; i++, key++)
		*key = *str++;
	ft.minute = charToInt(word);
	if(ft.minute > 60)
		return WDATE;
	if(*str++ != '.')
		return BADF;
	for(i = 0, key = word; i < 2; i++, key++)
		*key = *str++;
	ft.second = charToInt(word);
	if(ft.second > 60)
		return WDATE;

	counter = ftimeToCounter(&ft);
	dateH = (short) (counter >> 16);
	dateL = (short) counter;
	BKPEnable(enable);

	for(i = 10000; i && ((RTC->CRL & RTC_CRL_CNF) == 0); i--)
		RTC->CRL |= RTC_CRL_CNF;
	//RCC->BDCR &= ~RCC_BDCR_RTCEN;
	RTC->CNTH = dateH;
	while(!(RTC->CRL & RTC_CRL_RTOFF));
	RTC->CNTL = dateL;
	RTC->CRL &= ~RTC_CRL_CNF;
	while(!(RTC->CRL & RTC_CRL_RTOFF));
	BKPEnable(disable);
	x = RTC->CNTL;
	if(x != dateL)
		return WRITEER;
	if(RTC->CNTH != dateH)
		return WRITEER;
	return SUCC;
}


ftime_t *RTCGetTime()	{
	int time = (RTC->CNTH << 16) + RTC->CNTL;
	counterToFtime(time, &RTCDate);
	return &RTCDate;
}

void RTC_IRQHandler()	{
	RTC->CRL &= ~RTC_CRL_SECF;
	//interStatusBarRefresh();
}










