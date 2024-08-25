#include "main.h"

extern u8g2_t u8g2;
int main(void)
{
	My_RTC_Init();//��ʼ��RTC
	RTC_Set(2024,7,29,23,59,50);//����RTCʱ��
	RTC_Get();//����RTCʱ��

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ���
	Timer_Init();//��ʱ����ʼ��
	Key_Init();//������ʼ��
	u8g2_Init(&u8g2);//u8g2��ʼ��
	AD_Init();//ADC��ʼ��
	HumanSensor_Init();//�����Ӧģ���ʼ��
	HCSR04_Init();//����ģ���ʼ��
	Beep_Init();//��������ʼ��
	LED_Init();//LED��ʼ��
	while(1)
	{
		Control_System();//���Ĵ���
	}
	
}
