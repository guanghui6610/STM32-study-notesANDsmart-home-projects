#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "PWM.h"

uint16_t Key_Value;

//下面是按键中断

/* 场景1 */
void Key1Interrupt_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB , GPIO_PinSource11);
	
	EXTI_InitTypeDef EXIT_InitStructure;
	EXIT_InitStructure.EXTI_Line = EXTI_Line11;
	EXIT_InitStructure.EXTI_LineCmd = ENABLE;
	EXIT_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXIT_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXIT_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void)//一个单片机上能否出现多次这个？例如A11和A12同时有中断
{

		if(EXTI_GetITStatus(EXTI_Line11) == SET)
		{
			Key_Value = 1;
			EXTI_ClearITPendingBit(EXTI_Line11);
		}
}

/* 场景2 */
void Key2Interrupt_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB , GPIO_PinSource1);
	
	EXTI_InitTypeDef EXIT_InitStructure;
	EXIT_InitStructure.EXTI_Line = EXTI_Line1;
	EXIT_InitStructure.EXTI_LineCmd = ENABLE;
	EXIT_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXIT_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXIT_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI1_IRQHandler(void)//一个单片机上能否出现多次这个？例如A11和A12同时有中断
{

		if(EXTI_GetITStatus(EXTI_Line1) == SET)
		{
			Key_Value = 2;
			EXTI_ClearITPendingBit(EXTI_Line1);
		}
}

/* 关灯 */
void Key3Interrupt_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA , GPIO_PinSource9);
	
	EXTI_InitTypeDef EXIT_InitStructure;
	EXIT_InitStructure.EXTI_Line = EXTI_Line9;
	EXIT_InitStructure.EXTI_LineCmd = ENABLE;
	EXIT_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXIT_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXIT_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI9_5_IRQHandler(void)//一个单片机上能否出现多次这个？例如A11和A12同时有中断
{
		if(EXTI_GetITStatus(EXTI_Line9) == SET)
		{
			Key_Value = 3;
			EXTI_ClearITPendingBit(EXTI_Line9);
		}
}

uint16_t Key_GetValue(void)
{
	return Key_Value;
}

//以下是闹钟按键

/* 分钟增加 */
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
	uint8_t KeyNum;
	uint32_t Temp = 600000;
	
	//分钟++
	if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_3) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_3) == 0)
		{
			Temp--;
			if(Temp == 0)
			{
				//加不加这个有什么作用呢？
				Delay_ms(20);
				return 2;
			}
		}
		Delay_ms(20);
		KeyNum = 2;
	}
	
	if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_4) == 0)
	{
		Delay_ms(20);
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

