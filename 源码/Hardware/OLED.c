#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "Delay.h"
#include "OLED.h"
#include <string.h>
#include <math.h>
void OLED_WriteCommand(uint8_t Command)
{

	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(I2C1, 0x00);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	
	I2C_SendData(I2C1, Command);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	
	I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Transmitter);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
	
	I2C_SendData(I2C1, 0x40);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
	
	I2C_SendData(I2C1, Data);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}


void OLED_Init(void)
{
	MyI2C_Init();
	
	Delay_ms(100);
	
	OLED_WriteCommand(0xAE);
	
	OLED_WriteCommand(0xD5);
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);
	
	OLED_WriteCommand(0xA1);
	
	OLED_WriteCommand(0xC8);

	OLED_WriteCommand(0xDA);
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);

	OLED_WriteCommand(0xA6);

	OLED_WriteCommand(0x8D);
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);
	
	Delay_ms(100);
}
uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      /* only support for software I2C*/
    
        OLED_Init();
      break;
    case U8X8_MSG_DELAY_NANO:
      /* not required for SW I2C */
      break;
    
    case U8X8_MSG_DELAY_10MICRO:
      /* not used at the moment */
      break;
    
    case U8X8_MSG_DELAY_100NANO:
      /* not used at the moment */
      break;
   
    case U8X8_MSG_DELAY_MILLI:
      Delay_ms(arg_int);
      break;
    case U8X8_MSG_DELAY_I2C:
      /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
      Delay_us(arg_int<=2?5:1);
      break;
    
    case U8X8_MSG_GPIO_I2C_CLOCK:
      
      if ( arg_int == 0 )
      {
		  GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			
      }
      else
      {
		 GPIO_SetBits(GPIOB,GPIO_Pin_8);
	  }
      break;
    case U8X8_MSG_GPIO_I2C_DATA:
      
      if ( arg_int == 0 )
      {
	    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
      }
      else
      {
	//GPIOA->BSRR = GPIO_BSRR_BS_14;		/* atomic set PA10 */
	// input mode
		GPIO_SetBits(GPIOB,GPIO_Pin_9);
      }
      break;
/*
    case U8X8_MSG_GPIO_MENU_SELECT:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_SELECT_PORT, KEY_SELECT_PIN));
      break;
    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_NEXT_PORT, KEY_NEXT_PIN));
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_PREV_PORT, KEY_PREV_PIN));
      break;
    
    case U8X8_MSG_GPIO_MENU_HOME:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_HOME_PORT, KEY_HOME_PIN));
      break;
*/
    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}

void u8g2_Init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2,U8G2_R0,u8x8_byte_hw_i2c,u8x8_gpio_and_delay_stm32l0);
	u8g2_InitDisplay(u8g2);
	u8g2_SetPowerSave(u8g2,0);
	u8g2_ClearBuffer(u8g2);
}
