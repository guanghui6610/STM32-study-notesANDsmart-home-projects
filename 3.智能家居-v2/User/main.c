#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "PWM.h"
#include "OLED.h"
#include "ADC.h"
#include "LED.h"
#include "stdio.h"
#include "time.h"
#include "TemperSensor.h"
#include "MYRTC.h"
#include "Clock.h"
#include "Scene.h"
#include "Sensing.h"
#include "SerialPort.h"

extern uint16_t Key_Value;
extern uint8_t Hour,Min,Sec;							//用来调整闹钟的时间变量
extern uint8_t _year,_mon,_day,_hour,_min,_sec;			//用来调整表的时间变量
extern uint32_t Alarm_Time;	//闹钟相关变量，单位都是秒

uint16_t i;
float LightValue;
float TemperValue;

uint16_t MyRTC_Time[] = {2024,12,17,00,00,50};	//定义全局的时间数组，数组内容分别为年、月、日、时、分、秒
uint8_t Flag_Count = 0;							//是否在计时标志，0为不在计时
uint8_t Flag_Change = 3;						//按键调节闹钟/日期/时间，0为调节闹钟，1为调节日期，2为调节时间


int main(void)
{
	OLED_Init();
	MyRTC_Init();
	Key_Init();//按键控制闹钟部分
	AD_Init();
	TemperSensor_Init();
	Sensing_Init();
	SerialPort_Init();
	//按键控制场景切换部分
	Key1Interrupt_Init();//按键1初始化（对应场景1）
	Key2Interrupt_Init();//按键2初始化（对应场景2）
	Key3Interrupt_Init();//两个灯都灭
	
	OLED_ShowString(1, 1, "Date:XXXX-XX-XX");
	OLED_ShowString(2, 1, "Time:XX:XX:XX");
	OLED_ShowString(3, 1, "Alarm:XX:XX:XX");
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	while(1)
	{
		MyRTC_ReadTime();
		Key_Control();//控制闹钟的按键
		check();//要先从RTC中读取时间再进行check操作
		
		/*将光敏电阻和温敏电阻大小转换成0~100*/
		LightValue = (float)AD_GetValue(ADC_Channel_0) / 4095 * 100;
		
		OLED_ShowNum(1, 6, MyRTC_Time[0], 4);		//年
		OLED_ShowNum(1, 11, MyRTC_Time[1], 2);		//月
		OLED_ShowNum(1, 14, MyRTC_Time[2], 2);		//日
		OLED_ShowNum(2, 6, MyRTC_Time[3], 2);		//时
		OLED_ShowNum(2, 9, MyRTC_Time[4], 2);		//分
		OLED_ShowNum(2, 12, MyRTC_Time[5], 2);		//秒
		OLED_ShowNum(4 , 16 , Key_GetValue() , 1);	//显示场景模式几
		
		//Flag_Count反应是否在闹钟计时，1为闹钟计时，0为没有设置闹钟
		
		/*
			2024.12.17感觉这里有问题，逻辑有误，回头更改更改
		*/
		if(Flag_Count)										//正在计时，则显示闹钟响起剩余时间
		{
			if((MyRTC_Time[3] == Hour && MyRTC_Time[4] == Min && MyRTC_Time[5] == Sec))			
			//if(RTC_GetFlagStatus(RTC_FLAG_ALR) == 1)		//闹钟时间到，检查标志位为1
			{
				//RTC_ClearFlag(RTC_FLAG_ALR);				//清除标志位
				Flag_Count = 0;Alarm_Time = 0;				//重置相关参数
				Hour = 0;Min = 0; Sec = 0;
				
				//2024.12.11进行到这一步
				//目前闹钟结束后灯不变化，如何能解决呢？？
				//场景切换和传感器控制没问题？（是否将LED设置成一个单独的函数？？？）预计是可以的！
				
				Key_Value = 3;			//闹钟到了，置该值为4，使之能够实现闹钟时间到，灯灭的功能
				OLED_ShowString(4,1,"Time Out");
			}

			else											//闹钟时间未到
			{
				OLED_ShowString(4,1,"Counting");			//显示正在计时
			}
		}
		else												//不在计时，则显示需要设定的闹钟时间
		{
			OLED_ShowNum(3,7,Hour,2);
			OLED_ShowNum(3,10,Min,2);
			OLED_ShowNum(3,13,Sec,2);
		}
		
		/*
			显示目前的设置界面
			1.设置闹钟（这里为什么没有显示的原因是，显示Set Clock之前要清空最后一行，因为这个代码中的OLED函数不是使用显存数组的形式）
						（如果放在这里的话，程序会一直循环，会有闪烁的画面）
			2.调节日期
			3.调节时间
		*/
		if(Flag_Change == 1)		//显示“调节日期”
		{
			OLED_ShowString(4 , 1 , "Change Date");
		}
		else if(Flag_Change == 2)	//显示“调节时间”
		{
			OLED_ShowString(4 , 1 , "Change Time");
		}
		else if(Flag_Change == 3)	//第四行清空，代表退出设置
		{
			OLED_ShowString(4 , 1 , "              ");
		}
		
		/*
			检测串口接受数据标志位 
			发送1对应场景1
			发送2对应场景2
			发送其他数字对应关灯
		*/
		if(SerialPort_GetRxFlag() == 1)
		{
			if(SerialPort_GetRxData() == 1)
			{
				Key_Value = 1;
			}
			else if(SerialPort_GetRxData() == 2)
			{
				Key_Value = 2;
			}
			else
			{
				Key_Value = 3;
			}	
		}
		
		/* 以下是场景切换以及光敏传感器、温度传感器控制灯光 */
		switch(Key_GetValue())
		{
			case 1:
				if(TemperSensor_Get() == 1)//温度升高，温敏电阻减小，由于电流保持不变，同相输入端电压减小，此时反向输出端电压高于同相输出端，输出低电平
				{
					PWM1_Init();
					PWM1_SetCompare(130 - LightValue);//灯越亮，光敏传感器值越小，该值越大，LED就越暗
				}
				if(TemperSensor_Get() == 0)
				{
					LED1_OFF();
				}
				LED2_OFF();
				break;
			case 2:
				if(Sensing_Get() == 1)
				{
					PWM2_Init();
					PWM2_SetCompare(130 - LightValue);//灯越亮，光敏传感器值越小，该值越大，LED就越暗
				}
				if(Sensing_Get() == 0)
				{
					LED2_OFF();
				}
				LED1_OFF();
				break;
			case 3:
				LED1_OFF();
				LED2_OFF();
				break;
		}
	}
}
