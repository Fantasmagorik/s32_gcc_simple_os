#include <stm32f103xb.h>

enum RTCResponse	{
	SUCC,
	BADF,
	WDATE,
	WRITEER
};

typedef struct{
uint16_t year;
uint8_t month;
uint8_t day;
uint8_t hour;
uint8_t minute;
uint8_t second;
uint8_t dayName;} ftime_t;







void RTCSetState(int RTCState);
int RTCSetTime(char *string);
ftime_t *RTCGetTime(void);

//void RTCSetDate
//void RTCSetDate(struct date);
//void RTCSetTime(struct time);
