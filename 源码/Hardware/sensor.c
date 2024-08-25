#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
  * @brief  人体感应模块初始化
  * @param  无
  * @retval 无
  */
void HumanSensor_Init(void)
{	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
/**
  * @brief  人体感应模块状态检测
  * @param  无
  * @retval 返回1即代表有人 返回0即代表无人
  */
s8 HumanSensor_State(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1);
}

/*******************以下是温湿度传感器子函数*****************/
/*******************以下是温湿度传感器子函数*****************/
/*******************以下是温湿度传感器子函数*****************/
/*******************以下是温湿度传感器子函数*****************/
uint8_t DHT11_Data[5];
void Pin_In()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOA_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIOA_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStructure);


	
}

void Pin_Out()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOA_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIOA_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStructure);	
	
}



void DHT11_Start(void)
{
	Pin_Out();
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	Delay_ms(20);
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	Delay_us(30);
	Pin_In();

}

uint8_t DHT11_Recevice(void)
{
	uint8_t i,Data;
	for(i=0;i<8;i++)
	{
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0);
		Delay_us(30);
		Data <<=1;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1)Data|=1;
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1);
	}
	return Data;
}
/**********以上是温湿度传感器子函数***************************/
/**********以上是温湿度传感器子函数***************************/
/**********以上是温湿度传感器子函数***************************/
/**********以上是温湿度传感器子函数***************************/




/**
  * @brief  温湿度传感器读取数据，读取出来的数据将存入 uint8_t DHT11_Data[5]：
			DHT11_Data[0] 是湿度整数部分 DHT11_Data[1] 是湿度小数部分
			DHT11_Data[2] 是温度整数部分 DHT11_Data[3] 是温度小数部分  
			DHT11_Data[4] 是校验位
  * @param  无
  * @retval 无
  */
void Check_Digit(void)
{
	uint8_t Temp[5];
	DHT11_Start();
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0)
	{
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1);
		Temp[0]=DHT11_Recevice();
		Temp[1]=DHT11_Recevice();
		Temp[2]=DHT11_Recevice();
		Temp[3]=DHT11_Recevice();
		Temp[4]=DHT11_Recevice();

		GPIO_SetBits(GPIOA,GPIO_Pin_3);	
		Delay_us(10);
		if(Temp[4]==Temp[3]+Temp[2]+Temp[1]+Temp[0])
		{
			DHT11_Data[4]=Temp[4];
			DHT11_Data[3]=Temp[3];
			DHT11_Data[2]=Temp[2];
			DHT11_Data[1]=Temp[1];
			DHT11_Data[0]=Temp[0];
		}
	}



}






/**
  * @brief  初始化距离传感器的Trig 和 Echo引脚 并开启TIM4时钟用于计算距离
  * @param  无
  * @retval 无
  */
void HCSR04_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_InternalClockConfig(TIM4);
	TIM_TimeBaseInitTypeDef TIM4_TimeBaseInitStruct;
	TIM4_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM4_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM4_TimeBaseInitStruct.TIM_Period=65535-1;
	TIM4_TimeBaseInitStruct.TIM_Prescaler=7200-1;
	TIM4_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4, &TIM4_TimeBaseInitStruct);
	TIM_Cmd(TIM4, DISABLE);
	
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef Trig_InitStructure;
 	Trig_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	Trig_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	Trig_InitStructure.GPIO_Pin = GPIO_Pin_4;
 	GPIO_Init(GPIOA, &Trig_InitStructure);

	GPIO_InitTypeDef Echo_InitStructure;
 	Echo_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	Echo_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	Echo_InitStructure.GPIO_Pin = GPIO_Pin_5;
 	GPIO_Init(GPIOA, &Echo_InitStructure);
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,(BitAction)0);

}
/**
  * @brief  给距离传感器控制TIM4启动 计数器清零 和TIM4关闭
  * @param  给1启动定时器并清零CNT，给0失能定时器
  * @retval 无
  */
void Timer_State(uint8_t State)
{
	if(State==1)
	{
		TIM4->CNT=0;
		TIM_Cmd(TIM4, ENABLE);	
	}
	else
	{
		TIM_Cmd(TIM4, DISABLE);
	}
}

/**
  * @brief  初始化距离传感器的Trig 和 Echo引脚 并开启TIM4时钟用于计算距离
  * @param  无
  * @retval 返回一个距离值(cm)
  */
uint32_t Get_HCSR04_Distance(void)
{
	uint8_t j=3;
	uint32_t Distance=0,VALUE=0;
	for(uint8_t i=0;i<j;i++)
	{
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)1);
		Delay_us(15);
		GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)0);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0);
		Timer_State(1);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1);
		Timer_State(0);
		VALUE+=((TIM4->CNT)*346/100)/2;

	}
	
	Distance=VALUE/j;
	VALUE=0;
	
	return Distance;
}

/**
  * @brief  初始化蜂鸣器
  * @param  无
  * @retval 无
  */
void Beep_Init(void)
{	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOA,GPIO_Pin_12,(BitAction)1);
}
/**
  * @brief  初始化LED
  * @param  无
  * @retval 无
  */
void LED_Init(void)
{	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
