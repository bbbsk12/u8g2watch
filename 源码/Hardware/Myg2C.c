#include "Myg2C.h"
u8g2_t u8g2;

u8 frame_loc;
char Temp[20];
char Week1[6];
s8 LED_State=0, Beep_State=0;
short Timer_Frame_x=20-2,Timer_Frame_targe_x,Timer_Frame_y=58-12,Timer_Frame_targe_y;
short RealTime_Config_now_ui,targe_ui=64,RealTime_interface_ui;
extern u8g2Timer_t Timer;
extern struct RTC_CLOCK rtc_clock;
extern u8 timercounter_flag;
extern Key_t Key[4];
extern uint8_t DHT11_Data[5];
struct RTC_CLOCK timeSet;
signed char state=0,state1=0,interface=0,time0=0,countflag=0,countenterflag=0;
s8 TimeSet_enter_flag=0;
s8 timeset_LREC;
u8 timesetflag11=0;
void Week_Write1(struct RTC_CLOCK *rtc,char *Week)
{
		if(rtc->week == 1)strcpy(Week, "Mon. "); 
		else if(rtc->week==2)strcpy(Week,"Tues. "); 
		else if(rtc->week==3)strcpy(Week,"Wed. "); 
		else if(rtc->week==4)strcpy(Week,"Thur."); 
		else if(rtc->week==5)strcpy(Week,"Fri. "); 
		else if(rtc->week==6)strcpy(Week,"Sat. "); 
		else if(rtc->week==7)strcpy(Week,"Sun. "); 
		else				 strcpy(Week,"ERROR"); 
}


u8 frame_run(short* now,short* targe,u8 step)
{
	if(*now < (*targe)*1)
	{
		u8 temp=(*targe-*now) > step ? step:1;
		*now+=temp;
		
	}
	else if(*now > (*targe)*1)
	{
		u8 temp=*now>step? step:1; 
		*now-=temp;
		
	}
	else if(*now == (*targe)*1)
	{
		return 0;
	}
	return 1;
}


u8 framex2_run(short* now,short* targe,u8 step)
{
	if(*now < (*targe)*1)
	{
		u8 temp=(*targe-*now) > step ? step:4;
		*now+=temp;
		
	}
	else if(*now > (*targe)*1)
	{
		u8 temp=*now>step? step:4; 
		*now-=temp;
		
	}
	else if(*now == (*targe)*1)
	{
		return 0;
	}
	return 1;
}







void RealTime_interface(short offset_x,short offset_y)
{
	u8g2_SetFont(&u8g2,u8g2_font_profont29_mn);
	sprintf(Temp,"%02d:%02d:%02d",rtc_clock.hour,rtc_clock.min,rtc_clock.sec);			
	u8g2_DrawStr(&u8g2,0+offset_x,30+offset_y,Temp);		
	u8g2_SetFont(&u8g2,u8g2_font_profont15_tr);
	Week_Write1(&rtc_clock,Week1);
	sprintf(Temp,"%d/%02d/%02d %s",rtc_clock.year,rtc_clock.mon,rtc_clock.day,Week1);			
	u8g2_DrawStr(&u8g2,0+offset_x,45+offset_y,Temp);

}



void Timer_interface(short offset_x,short offset_y)
{
	u8g2_SetFont(&u8g2,u8g2_font_profont22_tr);
	sprintf(Temp,"%02d:%02d:%02d",Timer.mins,Timer.sec,Timer.msec);			//接入计时
	u8g2_DrawStr(&u8g2,15+offset_x,25+offset_y,Temp);
	
	u8g2_SetFont(&u8g2,u8g2_font_t0_15_tr);		
	u8g2_DrawStr(&u8g2,42+offset_x,42+offset_y,"Reset");	
	u8g2_DrawStr(&u8g2,20+offset_x,58+offset_y,"Start");
	u8g2_DrawStr(&u8g2,70+offset_x,58+offset_y,"Stop");
	
	if(countflag==2)
	{
		frame_loc--;
		countflag=0;
		
	}
	else if(countflag==3)
	{
		frame_loc++;
		countflag=0;
	}
	
	if(frame_loc==0)frame_loc=3;
	if(frame_loc==4)frame_loc=1;
	if(countenterflag)
	{
		if(frame_loc==1)
		{
			Timer_Frame_targe_x=20-2+offset_y;
			Timer_Frame_targe_y=58-12+offset_y;
			
			frame_run(&Timer_Frame_x,&Timer_Frame_targe_x,6);
			frame_run(&Timer_Frame_y,&Timer_Frame_targe_y,6);
			u8g2_SetDrawColor(&u8g2, 2);
			u8g2_DrawRBox(&u8g2,Timer_Frame_x,Timer_Frame_y,9*5,15,1);//Start
			u8g2_SetDrawColor(&u8g2, 1);
			if(countflag==1)
			{

				u8g2_DrawRBox(&u8g2,Timer_Frame_x,Timer_Frame_y,9*5,15,1);//Start
				timercounter_flag=1;
				countflag=0;
			}
		
		}
		else if(frame_loc==2)
		{
			Timer_Frame_targe_x=70-2+offset_y;
			Timer_Frame_targe_y=58-12+offset_y;
			
			frame_run(&Timer_Frame_x,&Timer_Frame_targe_x,6);
			frame_run(&Timer_Frame_y,&Timer_Frame_targe_y,6);
			u8g2_SetDrawColor(&u8g2, 2);
			u8g2_DrawRBox(&u8g2,Timer_Frame_x,Timer_Frame_y,9*4,15,1);//Stop
			u8g2_SetDrawColor(&u8g2, 1);
			if(countflag==1)
			{
				u8g2_DrawRBox(&u8g2,Timer_Frame_x,Timer_Frame_y,9*4,15,1);//Stop
				timercounter_flag=0;
				countflag=0;
			}
		
		}
		else if(frame_loc==3)
		{ 
			Timer_Frame_targe_x=42-2+offset_y;
			Timer_Frame_targe_y=42-12+offset_y;
			
			frame_run(&Timer_Frame_x,&Timer_Frame_targe_x,6);
			frame_run(&Timer_Frame_y,&Timer_Frame_targe_y,6);
			u8g2_SetDrawColor(&u8g2, 2);
			u8g2_DrawRBox(&u8g2,Timer_Frame_x,Timer_Frame_y,9*5,15,1);//Reset
			u8g2_SetDrawColor(&u8g2, 1);
			if(countflag==1)
			{
				u8g2_DrawRBox(&u8g2,Timer_Frame_x,Timer_Frame_y,9*5,15,1);//Reset
				Timer.mins=Timer.msec=Timer.sec=0;
				countflag=0;
			}
		
		}
	
	}

}

void RealTime_Config(short offset_x,short offset_y)
{
	u8g2_SetFont(&u8g2,u8g2_font_profont15_mr);
	u8g2_DrawXBMP(&u8g2,40+offset_x,0+offset_y,48,48,TimerBmp);
	u8g2_DrawStr(&u8g2,36+offset_x,60+offset_y,"RealTime");
}

void Timer_Config(short offset_x,short offset_y)
{
	u8g2_SetFont(&u8g2,u8g2_font_profont15_mr);
	u8g2_DrawXBMP(&u8g2,40+offset_x,0+offset_y,48,48,CountBmp);
	u8g2_DrawStr(&u8g2,48+offset_x,60+offset_y,"Timer");
}

void TimerConfig(short offset_x,short offset_y)
{
	u8g2_SetFont(&u8g2,u8g2_font_profont15_mr);
	u8g2_DrawXBMP(&u8g2,40+offset_x,0+offset_y,48,48,timeconfig);
	 
	u8g2_DrawStr(&u8g2,48+offset_x,60+offset_y,"Time");
}

void ADCConfig(short offset_x,short offset_y)
{
	u8g2_SetFont(&u8g2,u8g2_font_profont15_mr);
	u8g2_DrawXBMP(&u8g2,40+offset_x,0+offset_y,48,48,ADCconfig);
	u8g2_DrawStr(&u8g2,38+offset_x,60+offset_y,"ADCValue");
}

void TimeSetConfig(short offset_x,short offset_y)
{
	TimeSet_enter_flag=0;
	u8g2_SetFont(&u8g2,u8g2_font_profont15_mr);
	u8g2_DrawXBMP(&u8g2,40+offset_x,0+offset_y,48,48,timeset);
	u8g2_DrawStr(&u8g2,42+offset_x,60+offset_y,"TimeSet");
}

void SensorConfig(short offset_x,short offset_y)
{
	u8g2_SetFont(&u8g2,u8g2_font_profont15_mr);
	u8g2_DrawXBMP(&u8g2,40+offset_x,0+offset_y,48,48,sensor_config);
	u8g2_DrawStr(&u8g2,45+offset_x,60+offset_y,"Sensor");
}	


void hardwareConfig(short offset_x,short offset_y)
{
	u8g2_SetFont(&u8g2,u8g2_font_profont15_mr);
	u8g2_DrawXBMP(&u8g2,40+offset_x,0+offset_y,48,48,hardware_config);
	u8g2_DrawStr(&u8g2,38+offset_x,60+offset_y,"HardWare");
}






short timeset_RBOX_XY[]={29,27,14,20};
s8 Timeset_Frame,Timeset_Frame_len;
short timeset_temp_frameX,timeset_temp_frameY,Timeset_len;

void TimeSet_interface(short offset_x,short offset_y)
{
	if(TimeSet_enter_flag==0){timeset_LREC=TIMERSET_NOP;TimeSet_enter_flag=1;}

	if(timesetflag11==0)//进来先校时一次方便在原来的时间基础上修改
	{
		timeSet=rtc_clock;
		timesetflag11=1;//置一防止每次都要校时，待结束函数任务再将标志位置0
	}

	u8g2_SetFont(&u8g2,u8g2_font_profont29_mn);
	sprintf(Temp,"%02d:%02d:%02d",timeSet.hour,timeSet.min,timeSet.sec);			
	u8g2_DrawStr(&u8g2,0+offset_x,30+offset_y,Temp);	
	u8g2_SetFont(&u8g2,u8g2_font_profont15_tr);
	
	Week_Write1(&timeSet,Week1);
	sprintf(Temp,"%d/%02d/%02d %s",timeSet.year,timeSet.mon,timeSet.day,Week1);			
	u8g2_DrawStr(&u8g2,0+offset_x,55+offset_y,Temp);
	
	timeset_RBOX_XY[0]=29;
	
	frame_run(&timeset_temp_frameX,&timeset_frameXY[Timeset_Frame][0],10);
	frame_run(&timeset_temp_frameY,&timeset_frameXY[Timeset_Frame][1],5);
	frame_run(&Timeset_len,&timeset_RBOX_XY[Timeset_Frame_len],3);
	
	u8g2_DrawRBox(&u8g2,timeset_temp_frameX+offset_x,timeset_temp_frameY+offset_y,Timeset_len,3,1);
	
	if(timeset_LREC==TIMERSET_R)
	{

		timeset_LREC=TIMERSET_NOP;
		Timeset_Frame++;	
	}	
	else if(timeset_LREC==TIMERSET_L)
	{

		timeset_LREC=TIMERSET_NOP;
		Timeset_Frame--;
		
	}
		if(Timeset_Frame==-1)Timeset_Frame=13;
		if(Timeset_Frame==14)Timeset_Frame=0;
		if	   (Timeset_Frame==6 || Timeset_Frame== 7) Timeset_Frame_len=1;
		else if(Timeset_Frame>=8 && Timeset_Frame<=11) Timeset_Frame_len=2;
		else if(Timeset_Frame==12|| Timeset_Frame==13) Timeset_Frame_len=3;
		else 										   Timeset_Frame_len=0;	
	if(timeset_LREC==TIMERSET_E)
	{
		timeset_LREC=TIMERSET_NOP;
		if(Timeset_Frame==0)timeSet.hour++;
		if(Timeset_Frame==1)timeSet.hour--;
		
		if(Timeset_Frame==2)timeSet.min++;
		if(Timeset_Frame==3)timeSet.min--;
		
		if(Timeset_Frame==4)timeSet.sec++;
		if(Timeset_Frame==5)timeSet.sec--;
		
		if(Timeset_Frame==6)timeSet.year++;
		if(Timeset_Frame==7)timeSet.year--;
		
		if(Timeset_Frame==8)timeSet.mon++;
		if(Timeset_Frame==9)timeSet.mon--;
		
		if(Timeset_Frame==10)timeSet.day++;
		if(Timeset_Frame==11)timeSet.day--;
		
		if(Timeset_Frame==12 && timeSet.week<7)timeSet.week++;
		if(Timeset_Frame==13 && timeSet.week>1)timeSet.week--;
	}
}

short function_x=0,function_y=2 ,function_x1,function_y1,targe_x1,targe_y1,function_x2,function_y2,targe_x2,targe_y2;
s8 function_LREC;

void ADC_interface(short offset_x,short offset_y)
{
	u8g2_DrawXBMP(&u8g2,5+offset_x,12+offset_y,40,40,ADC_ValueShow);
	u8g2_SetFont(&u8g2,u8g2_font_profont29_mn);
	sprintf(Temp,"%.2f",AD_GetValue()/4095*3.3);	
	u8g2_DrawStr(&u8g2,55+offset_x,40+offset_y,Temp);	
}

void human_sensor_interface(short offset_x,short offset_y)
{
	u8g2_SetFont(&u8g2,u8g2_font_courB12_tr);
	u8g2_DrawXBMP(&u8g2,10+offset_x,20+offset_y,30,30,human_sensor);

	if(HumanSensor_State()==1)
	{
		sprintf(Temp,"Occupied");
	}
	else if(HumanSensor_State()==0)
	{
		sprintf(Temp,"No one");
	}
	u8g2_DrawStr(&u8g2,45+offset_x,40+offset_y,Temp);
}
void temperature_humidity_sensor_interface(short offset_x,short offset_y)
{
	u8g2_SetFont(&u8g2,u8g2_font_courB12_tr);
	Check_Digit( );
	u8g2_DrawXBMP(&u8g2,5+offset_x,15+offset_y,40,40,temperature_humidity_sensor);
	sprintf(Temp,"%02d.%02dC",DHT11_Data[2],DHT11_Data[3]);
	u8g2_DrawStr(&u8g2,45+offset_x,30+offset_y,Temp);
	sprintf(Temp,"%02d.%02dRH",DHT11_Data[0],DHT11_Data[1]);
	u8g2_DrawStr(&u8g2,45+offset_x,48+offset_y,Temp);

}


void Distance_sensor_interface(short offset_x,short offset_y)
{
	int distance=Get_HCSR04_Distance();
	u8g2_SetFont(&u8g2,u8g2_font_courB14_tr);
	u8g2_DrawXBMP(&u8g2,5+offset_x,15+offset_y,40,40,distance_sensor);
	sprintf(Temp,"%02dCM",distance);
	u8g2_DrawStr(&u8g2,60+offset_x,40+offset_y,Temp);


}

void Beep_interface(short offset_x,short offset_y)
{
	u8g2_DrawXBMP(&u8g2,offset_x,10+offset_y,45,45,Beep_hardware);
	u8g2_SetFont(&u8g2,u8g2_font_courB12_tr);
	u8g2_DrawStr(&u8g2,55+offset_x,38+offset_y,"ON/OFF");
	u8g2_DrawRFrame(&u8g2,50+offset_x,38-14+offset_y,5*12+9,18,5);
	if(Beep_State) GPIO_WriteBit(GPIOA,GPIO_Pin_12,(BitAction)0);
	else		  GPIO_WriteBit(GPIOA,GPIO_Pin_12,(BitAction)1);
}

void LED_interface(short offset_x,short offset_y)
{
	u8g2_DrawXBMP(&u8g2,offset_x,10+offset_y,45,45,LED_hardware);
	u8g2_SetFont(&u8g2,u8g2_font_courB12_tr);
	u8g2_DrawStr(&u8g2,55+offset_x,38+offset_y,"ON/OFF");
	u8g2_DrawRFrame(&u8g2,50+offset_x,38-14+offset_y,5*12+9,18,5);
	if(LED_State) GPIO_WriteBit(GPIOA,GPIO_Pin_11,(BitAction)1);
	else		  GPIO_WriteBit(GPIOA,GPIO_Pin_11,(BitAction)0);
}
s8 function_handoff_flag=1;

void Control_System(void)
{
		if(Key[3].Keytouch==1)
		{

			if(function_x==2 && function_y==2)timeset_LREC=TIMERSET_E;
			if(function_x==1 && function_y==2)countflag=1;
			function_LREC=1;
			if(function_handoff_flag==2 && function_y==1 && function_x==3)	function_LREC=0;
			if(function_handoff_flag==3 && function_y==1 && function_x>=4)	function_LREC=0;
			if(function_handoff_flag==4 && function_y==1 && function_x>=7)	function_LREC=0;
			Key[3].Keytouch=0;
				if(function_x==8 && function_y==1)LED_State=!LED_State;
			if(function_x==7 && function_y==1)Beep_State=!Beep_State;
		}
		else if(Key[0].Keytouch==1)
		{
			if(function_x==2 && function_y==2)timeset_LREC=TIMERSET_C;
			function_LREC=2;
			Key[0].Keytouch=0;

		}
		else if(Key[1].Keytouch==1)
		{ 
			if(function_x==2 && function_y==2)timeset_LREC=TIMERSET_L;

			if(function_x==1 && function_y==2)countflag=2;
			function_LREC=4;
			if(function_handoff_flag==2 && function_y==1 && function_x==3)	function_LREC=0;
			if(function_handoff_flag==3 && function_y==1 && function_x==4)	function_LREC=0;
			
	
			if(function_handoff_flag==4 && function_y==1 && function_x==7)	function_LREC=0;
			Key[1].Keytouch=0;

		}
		else if(Key[2].Keytouch==1)
		{ 

			if(function_x==2 && function_y==2)timeset_LREC=TIMERSET_R;
			if(function_x==1 && function_y==2)countflag=3;
			
			function_LREC=3;
			if(function_handoff_flag==2 && function_y==1 && function_x==3)	function_LREC=0;//防止进入ADC测量界面按下右键越界跳转 
			if(function_handoff_flag==1 && function_y==1 && function_x==2)	function_LREC=0;
			
			if(function_handoff_flag==3 && function_y==1 && function_x==6)	function_LREC=0;
			
			if(function_handoff_flag==4 && function_y==1 && function_x==8)	function_LREC=0;
			if(function_handoff_flag==4 && function_y==0 && function_x>=7)	function_LREC=0;
			Key[2].Keytouch=0;

		}
		
		if(function_LREC==1 && function_y<2)
		{
			function_y++;
			function_LREC=0;
			while(1)
			{
				u8g2_ClearBuffer(&u8g2);
				targe_y1=64;
				function_x1=0;
				frame_run(&function_y1,&targe_y1,8);
				
				main_function[function_x][function_y-1](function_x1,function_y1);
				if(function_y1>=32)
				{
					function_x2=0;
					targe_y2=64;
					frame_run(&function_y2,&targe_y2,8);
					main_function[function_x][function_y](function_x1,-64+function_y2);
				}
				u8g2_SendBuffer(&u8g2);
				if(function_y2==64)
				{
					function_y1=function_y2=0;
					break;
				}
			}
		}
		if(function_LREC==2 && function_y>0)
		{
			if(function_x==2 && function_y==2)
			{
				//u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
				RTC_Set(timeSet.year,timeSet.mon,timeSet.day,timeSet.hour,timeSet.min,timeSet.sec);
				RTC_Get();
				timesetflag11=0;
				timeset_LREC=TIMERSET_NOP;
			}
			if(function_x==1 && function_y==2)countenterflag=0;
			function_y--;
			function_LREC=0;
			while(1)
			{
				u8g2_ClearBuffer(&u8g2);
				targe_y1=64;
				function_x1=0;
				frame_run(&function_y1,&targe_y1,8);
				main_function[function_x][function_y+1](function_x1,-function_y1);
				if(function_y1>=32)
				{
					function_x2=0;
					targe_y2=64;
					frame_run(&function_y2,&targe_y2,8);
					main_function[function_x][function_y](function_x1,64-function_y2);
				}
				u8g2_SendBuffer(&u8g2);
				if(function_y2==64)
				{
					
					function_y1=function_y2=0;
					break;
				}
			}
		}
		if(function_LREC==3 && function_y<=1 && function_x<8 )/**/
		{
			if(function_y==0 && function_x<=2)
			{
				function_handoff_flag=2;
				function_x=2;
			}
			else if (function_y==0 && function_x==3)
			{
				
				function_handoff_flag=3;
			}
			else if (function_y==0 && function_x>=4)
			{
				function_handoff_flag=4;
				function_x=6;
			}

			function_x++;function_LREC=0;
				
			while(1)
			{
				u8g2_ClearBuffer(&u8g2);
				targe_y1=0;
				targe_x1=128;
				framex2_run(&function_x1,&targe_x1,16);
				main_function[function_x-1][function_y](-function_x1,function_y1);
				if(function_x1>=64)
				{
					targe_x2=128;
					targe_y2=0;
					framex2_run(&function_x2,&targe_x2,16);
					main_function[function_x][function_y](128-function_x2,function_y2);
				}
				u8g2_SendBuffer(&u8g2);
				if(function_x2==128)
				{
					function_x1=function_x2=0;
					break;
				}
			}

		}

		if(function_LREC==4&& function_y<=1 && function_x>0) /**/
		{
			if(function_y==0 && function_x>=4 && function_handoff_flag==3)function_x=4;
			if(function_y==0 && function_x>=4 && function_handoff_flag==3)
			{
				function_handoff_flag=2;
			}
			if(function_y==0 && function_x>=7 && function_handoff_flag==4)
			{
				function_handoff_flag=9;
				function_x=7;
			}
			function_x--;function_LREC=0;
			/***************动画*******************************************/			
			while(1)
			{
				u8g2_ClearBuffer(&u8g2);
				targe_y1=0;
				targe_x1=128;
				framex2_run(&function_x1,&targe_x1,16);
				main_function[function_x+1][function_y](function_x1,function_y1);
				if(function_x1>=64)
				{
					targe_x2=128;
					targe_y2=0;
					framex2_run(&function_x2,&targe_x2,16);
					main_function[function_x][function_y](-128+function_x2,function_y2);
				}
				u8g2_SendBuffer(&u8g2);
				if(function_x2==128)
				{
					function_x1=function_x2=0;
					break;
				}
			}
			/*************************************************************/
			if(function_handoff_flag==9)
			{
				function_handoff_flag=3;
				function_x=4;
			}
			if(function_y==0 && function_x==2 && function_handoff_flag==2)
			{
				function_handoff_flag=1;
				function_x=0;
			}
			


		}
		if(function_x==1 && function_y==2)countenterflag=1;
		
		u8g2_ClearBuffer(&u8g2);
		main_function[function_x][function_y](0,0);
	
		u8g2_SendBuffer(&u8g2);




}
