
#include "Delay.h"

#include "stm32f10x_it.h"



Key_t Key[4]={0};

/**
  * @brief  用定时器扫描按键,每个按键的状态存入 Key_t 结构体，四个按键用 Key[4]数组存储
  * @param  无
  * @retval 无
  */
void TIM3_IRQHandler(void)
{
	Key[0].TrailKey=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
	Key[1].TrailKey=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
	Key[2].TrailKey=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	Key[3].TrailKey=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		for(u8 i=0;i<4;i++)
		{
			switch(Key[i].state)
			{
				case 0:
				{
					if(Key[i].TrailKey==0)Key[i].state++;
					else {;}  
				}break;
				
				case 1:
				{
					if(Key[i].TrailKey==0){Key[i].state++;}
					else {Key[i].state=0;}  
				}break;
				case 2:
				{
					if(Key[i].TrailKey==1){Key[i].state=0;Key[i].Keytouch=1;}
					else {   ;}  
				}break;
			}
		}
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}



/**
  * @brief  给计时器开启定时器
  * @param  无
  * @retval 无
  */
u8g2Timer_t Timer;
u8 timercounter_flag;
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if(timercounter_flag==1)
		{
			Timer.msec++;
			if(Timer.msec==100)
			{
				Timer.msec=0;
				Timer.sec++;
			}
			if(Timer.sec==60)
			{
				Timer.sec=0;
				Timer.mins++;
			}
			if(Timer.mins==60)
			{
				Timer.mins=0;
			
			}
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}






































/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {



  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
