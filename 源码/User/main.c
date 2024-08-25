#include "main.h"

extern u8g2_t u8g2;
int main(void)
{
	My_RTC_Init();//初始化RTC
	RTC_Set(2024,7,29,23,59,50);//设置RTC时间
	RTC_Get();//更新RTC时钟

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级组
	Timer_Init();//定时器初始化
	Key_Init();//按键初始化
	u8g2_Init(&u8g2);//u8g2初始化
	AD_Init();//ADC初始化
	HumanSensor_Init();//人体感应模块初始化
	HCSR04_Init();//距离模块初始化
	Beep_Init();//蜂鸣器初始化
	LED_Init();//LED初始化
	while(1)
	{
		Control_System();//核心代码
	}
	
}
