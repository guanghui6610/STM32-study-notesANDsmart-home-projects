#include "stm32f10x.h"                  // Device header


void LED1_ON(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB , GPIO_Pin_6);
}
void LED1_OFF(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	GPIO_SetBits(GPIOB , GPIO_Pin_6);
}
void LED1_Turn(void)
{
	if(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_6) == 0) GPIO_SetBits(GPIOB , GPIO_Pin_6);
	if(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_6) == 1) GPIO_ResetBits(GPIOB , GPIO_Pin_6);
}
void LED2_ON(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB , GPIO_Pin_9);
}
void LED2_OFF(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	GPIO_SetBits(GPIOB , GPIO_Pin_9);
}
void LED2_Turn(void)
{
	if(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_9) == 0) GPIO_SetBits(GPIOB , GPIO_Pin_9);
	if(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_9) == 1) GPIO_ResetBits(GPIOB , GPIO_Pin_9);
}
