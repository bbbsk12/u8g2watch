#ifndef __MY_RTC_H
#define __MY_RTC_H

#include <stm32f10x.h>
struct RTC_CLOCK
{
		u32 year;
	  u32 mon;
	  u32 day;
		u32 hour;
		u32 min;
	  u32 sec;
		u32 week;
};				
extern struct RTC_CLOCK rtc_clock;

u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 My_RTC_Init(void);
u8 RTC_Get(void);
#endif
