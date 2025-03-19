#include "stm32f10x.h"                  // Device header

void SerialPort_Init(void)
{
	//1.开启RCC外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	//3.初始化USART结构体
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1 , &USART_InitStructure);
	//4.配置中断，开启NVIC（接收数据时候用）
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	//5.开启USART控制
	USART_Cmd(USART1 , ENABLE);
	//6.中断输出配置
	USART_ITConfig(USART1 , USART_IT_RXNE , ENABLE);
}

/*接收数据*/

uint8_t SerialPort_RxData = 0;
uint8_t SerialPort_RxFlag;

uint8_t SerialPort_GetRxFlag(void)
{
	if(SerialPort_RxFlag == 1)
	{
		SerialPort_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t SerialPort_GetRxData(void)
{
	return SerialPort_RxData;
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1 , USART_IT_RXNE) == SET)
	{
		SerialPort_RxFlag = 1;
		SerialPort_RxData = USART_ReceiveData(USART1);
		
		USART_ClearITPendingBit(USART1 , USART_IT_RXNE);
	}
}
