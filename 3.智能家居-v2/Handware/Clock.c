#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "PWM.h"
#include "OLED.h"

extern uint8_t Flag_Count;							//是否在计时标志，0为不在计时
extern uint8_t Flag_Change ;							//按键调节闹钟/日期/时间，0为调节闹钟，1为调节日期，2为调节时间

uint8_t KeyNum;									//按键键码值
uint8_t Hour,Min,Sec;							//用来调整闹钟时间的变量
uint8_t _year,_mon,_day,_hour,_min,_sec;		//用来调整表的时间变量
uint32_t Alarm_CNT,Alarm_Time;	//设定的闹钟值

//以下是闹钟按键

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	//2024.12.18解决了12.17的问题，在这里未将KeyNum初始化为0
	/*
		在 C 语言中，局部变量（如 KeyNum）在没有初始化时，其初始值是不确定的*******
		如果 KeyNum 没有初始化，它的初始值可能是任何随机值
		草！原来是这个原因
	*/
	uint8_t KeyNum = 0;
	uint32_t Temp = 600000;
	
	//分钟++
	if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_3) == 0)
	{
		Delay_ms(30);
		while(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_3) == 0)//这里如果变成if，长按会特别灵敏
		{
			Temp--;
			if(Temp == 0)
			{
				//加不加这个有什么作用呢？
				//A:按键长按检测
				Delay_ms(20);
				return 2;
			}
		}
		Delay_ms(20);
		KeyNum = 2;
	}
	
	if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_4) == 0)
	{
		Delay_ms(30);
		while(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_4) == 0)
		{
			Temp--;
			if(Temp == 0)
			{
				//加不加这个有什么作用呢？
				Delay_ms(20);
				return 4;
			}
		}
		Delay_ms(20);
		KeyNum = 4;
	}
	return KeyNum;
}

/*按键控制函数*/
void Key_Control(void)
{
	KeyNum = Key_GetNum();	//读取按键键码
	
	if(Flag_Change == 0)	//调节闹钟
	{
		if(KeyNum == 1)		//1号按键调整小时
		{
			Hour++;
			if(Hour > 60)
				Hour = 0;
		}
		else if(KeyNum == 2)	//2号按键调整分钟
		{
			Min++;
			if(Min > 60)
				Min = 0;
		}
		else if(KeyNum == 3)	//3号按键调整秒
		{
			Sec++;
			if(Sec > 60)
				Sec = 0;
		}
		else if(KeyNum == 4)	//4号按键
		{	
			Alarm_Time = Hour*3600 + Min*60 + Sec;			//计算闹钟时长，单位是秒
			if(Alarm_Time > 0)
			{
				Alarm_CNT = RTC_GetCounter()+ Alarm_Time-1;			//设定闹钟值，需要-1
				//RTC_SetAlarm(Alarm_CNT);							//写入闹钟值到RTC的ALR寄存器，该程序中有没有这个无所谓
																	//因为最后判断闹钟到没到是根据时间等不等于闹钟数组中的时间
				Flag_Count = 1;
			}
			else	//若闹钟时长为0，则转到按键调节日期
			{
				Flag_Change = 1;	
			}
		}
	}
	
	else if(Flag_Change == 1)		//调节日期
	{
		if(KeyNum == 1)			//1号按键调整年
		{
			//MyRTC_Time[0]++;
			_year++;
			//MyRTC_SetTime();
		}
		else if(KeyNum == 2)	//2号按键调整月
		{
			//MyRTC_Time[1]++;
			_mon++;
			//MyRTC_SetTime();
		}
		else if(KeyNum == 3)	//3号按键调整日
		{
			//MyRTC_Time[2]++;
			_day++;
			//MyRTC_SetTime();
		}
		else if(KeyNum == 4)	//4号按键，改为调整时间
		{
			Flag_Change = 2;
		}			
	}
	
	else if(Flag_Change == 2)		//调节时间
	{
		if(KeyNum == 1)			//1号按键调整小时
		{
			//MyRTC_Time[3]++;
			_hour++;
			//MyRTC_SetTime();
		}
		else if(KeyNum == 2)	//2号按键调整分钟
		{
			//MyRTC_Time[4]++;
			_min++;
			//MyRTC_SetTime();
		}
		else if(KeyNum == 3)	//3号按键调整秒
		{
			//MyRTC_Time[5]++;
			_sec++;
			//MyRTC_SetTime();
		}
		else if(KeyNum == 4)	//4号按键
		{
			Flag_Change = 3;	//改为调整闹钟
		}			
	}
	else if(Flag_Change == 3)   //退出设置
	{
		if(KeyNum == 4)
		{
			Flag_Change = 0;
			OLED_ShowString(4 , 1 , "                 ");
			OLED_ShowString(4,1,"SetClock");	//刷新oled第四行
		}
	}
}
