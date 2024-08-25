#ifndef __SENSOR_H
#define __SENSOR_H

void HumanSensor_Init(void);
s8 HumanSensor_State(void);
void Check_Digit(void);
void HCSR04_Init(void);
uint32_t Get_HCSR04_Distance(void);
void Beep_Init(void);
void LED_Init(void);

#endif
