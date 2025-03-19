#include "stm32f10x.h"                  // Device header
#include <time.h>//提供标准时间处理功能
#include "MYRTC.h"

extern uint16_t MyRTC_Time[];//全局变量，用于保存当前的年、月、日、时、分、秒
extern uint8_t _year,_mon,_day,_hour,_min,_sec;//时间修正变量，允许对读取的时间进行偏移调整
uint8_t get_day[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};//每月的天数表
/*
	需要主函数提供如下数组
	uint16_t MyRTC_Time[] = {2024,2,22,10,0,0};	//定义全局的时间数组，数组内容分别为年、月、日、时、分、秒

	使用LSI(Low-Speed Internal oscillator,即低速时钟源)当作RTCCLK，LSI无法由备用电源供电，故主电源掉电时，RTC走时会暂停
	初始化芯片的实时时钟（RTC）模块，以确保RTC模块能正常准确地进行计时工作
*/
void MyRTC_Init(void)
{
	//打开电源和后备接口的时钟，使能对备份寄存器和RTC的访问
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//PWR（电源模块）
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);//BKP（备份模块）在复位或者掉电情况下，保存一些重要数据
	
	/*
		允许访问备份域
	*/
	PWR_BackupAccessCmd(ENABLE);
	
	/*
		强制对备份域进行复位操作
		复位操作会将备份域中的相关寄存器和状态恢复到初始值或者预定义的复位状态
		可以防止之前的配置干扰当前的 RTC 初始化，确保后续配置的正确性。
	*/
	
	RCC_BackupResetCmd(ENABLE);
	/*
		取消之前可能设置的备份域复位状态，允许备份域正常工作。
		这一步通常在完成复位操作后，使得备份域中的功能模块（如备份寄存器和 RTC）能够正常地读写数据和执行其功能。
		由于备份域复位会导致数据丢失，系统可能会因为丢失备份寄存器中的重要数据而无法按照用户之前的设置正常工作。
	*/
	/*
	复位和取消复位结合使用？
		单独复位备份域会清除其内容，但如果不取消复位状态，备份域模块将无法正常工作。
		复位和取消复位的结合使用，可以确保备份域的状态干净，同时能够重新启用其功能。
	*/
	RCC_BackupResetCmd(DISABLE);
	
	/*
		检查备份寄存器中的BKP_DR1中是否存储了标志值0xA5A5
		如果寄存器值不是0xA5A5，说明RTC未初始化，进入首次配置逻辑。
		否则，RTC已配置过，进入非首次配置逻辑。
		首次配置逻辑
	*/
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		/*
			启用低速内部时钟（LSI）并等待其稳定
			LSI时钟是STM32内部提供的低精度时钟，通常用于RTC和看门狗计时
			RCC_GetFlagStatus(RCC_FLAG_LSIRDY)检查LSI是否准备就绪
		*/
		RCC_LSICmd(ENABLE);
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
		
		/*
			配置RTC时钟源为LSI
			RTC模块需要一个时钟源才能工作，这里选择LSI作为时钟源
			启用RTC时钟
		*/
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
		
		/*
			同步RTC寄存器
			RTC_WaitForSynchro()确保RTC寄存器与系统时钟同步
			RTC_WaitForLastTask()等待上一个RTC操作完成，避免冲突
		*/
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		
		/*
			配置RTC的预分频器
			40000-1是预分频值，将LSI频率（约40kHz）分频到1Hz
			RTC计数器每秒递增1，实现秒级计时
		*/
		RTC_SetPrescaler(40000 - 1);
		RTC_WaitForLastTask();
		
		/*
			调用自定义函数MyRTC_SetTime，将全局数组中的初始时间写入RTC
		*/
		MyRTC_SetTime();
		
		/*
			在备份寄存器中写入标志值0xA5A5，表明RTC已完成初始化
		*/
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	
	/*
	非首次配置逻辑:简单重新启用LSI和RTC时钟，确保RTC继续计时
	*/
	else
	{
		RCC_LSICmd(ENABLE);				//即使不是第一次配置，也需要再次开启LSI时钟
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
}

//判断年份是否是闰年
int isLeapYear(int year) 
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
	{
		return 1;  // 是闰年
	} 
	else
	{
		return 0;  // 不是闰年
	}
}

/* 	
	对MYRTC_Time数组中的各元素进行合理性检查和范围修正，确保时间数据处于合理的取值范围之内。
	（调整时间时候防止出现非常规时间）
*/
void check(void)
{
	if(isLeapYear(MyRTC_Time[0])) get_day[2] = 29;else get_day[2] = 28;					// 判断闰年
	if(MyRTC_Time[1] != 13) MyRTC_Time[1] %= 13;	else MyRTC_Time[1] = 1;				// 月份
	if(MyRTC_Time[1] == 0) MyRTC_Time[1] = 1;
	if(MyRTC_Time[2] != get_day[MyRTC_Time[1]]) MyRTC_Time[2] %= get_day[MyRTC_Time[1]];// 日期
	MyRTC_Time[3] %= 24;// 小时
	MyRTC_Time[4] %= 60;// 分钟
	MyRTC_Time[5] %= 60;// 秒
}

/**
  * 函    数：RTC设置时间
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，全局数组里时间值将刷新到RTC硬件电路
  */
void MyRTC_SetTime(void)
{
/*
	time_t 是一种用于表示时间的基本数据类型，它通常被定义为能够存储从某个特定起始时间
	（例如 Unix 时间戳的起始时间，即 1970 年 1 月 1 日 00:00:00 UTC）到所表示时间点的秒数的数据类型。
*/
	time_t time_cnt;		//定义秒计数器数据类型
/*	
	struct tm 是 C 语言标准库中定义的一个结构体类型，专门用于存放分解后的日期和时间信息。
	它的结构体成员一般包含了年、月、日、时、分、秒以及星期几等多个与时间相关的分量
*/
	struct tm time_date;	//定义日期时间数据类型
	
	//将数组的时间赋值给日期时间结构体
	time_date.tm_year = MyRTC_Time[0] - 1900;//tm_year记录的是从 1900 年开始到当前时间经过的年数，例如 2024 年在这个成员中存储的值就是 124（2024 - 1900），
	time_date.tm_mon = MyRTC_Time[1] - 1 ;	//tm_mon取值范围0-11
	time_date.tm_mday = MyRTC_Time[2];		//tm_mday取值范围1-31
	time_date.tm_hour = MyRTC_Time[3];		//tm_hour取值范围0-23
	time_date.tm_min = MyRTC_Time[4];		//tm_min 取值范围0-59
	time_date.tm_sec = MyRTC_Time[5];		//tm_sec 取值范围0-59
	
	/* 
		mktime 函数是 C 标准库中非常重要的用于时间处理的函数，它的功能是将 struct tm 结构体表示的日期和时间信息转换为 time_t 类型的时间值，
		也就是计算出从 1970 年 1 月 1 日 00:00:00 UTC 开始到给定结构体所表示时间的秒数。 
	*/
	time_cnt = mktime(&time_date);	//调用mktime函数，将日期时间转换为秒计数器格式
	
	/*
		通过调用这个函数，将经过前面转换和时区调整后的时间秒数 time_cnt 写入到 RTC 的计数器（实时时钟芯片内部用于计数时间的寄存器）
		这是整个函数中实现将软件层面处理好的时间数据传递到硬件实时时钟的关键操作
		使得 RTC 能够基于写入的秒数开始准确计时，从而完成时间设置的核心目标。
	*/
	RTC_SetCounter(time_cnt);						//将秒计数器写入到RTC的CNT中
	RTC_WaitForLastTask();							//等待上一次操作完成
}

/**
  * 函    数：RTC读取时间
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，RTC硬件电路里时间值将刷新到全局数组
  */

/*
	该函数主要用于从硬件实时时钟（RTC）中读取当前时间信息
	并将其转换为自定义的 MyRTC_Time 数组格式进行存储
*/
void MyRTC_ReadTime(void)
{
	time_t time_cnt;		//定义秒计数器数据类型
	struct tm time_date;	//定义日期时间数据类型
	
	time_cnt = RTC_GetCounter();		//读取RTC的CNT，获取当前的秒计数器
	
	/* 
		localtime：它接受一个指向time_t类型的指针作为参数，并返回一个指向tm结构的指针，该结构包含了本地时间的详细信息。
		C语言中*表示访问指针指向的值
	*/
	time_date = *localtime(&time_cnt);				//使用localtime函数，将秒计数器转换为日期时间格式
	
	MyRTC_Time[0] = time_date.tm_year + 1900 + _year;		//将日期时间结构体赋值给数组的时间
	MyRTC_Time[1] = time_date.tm_mon  + _mon + 1;
	MyRTC_Time[2] = time_date.tm_mday + _day;
	MyRTC_Time[3] = time_date.tm_hour + _hour;
	MyRTC_Time[4] = time_date.tm_min + _min;
	MyRTC_Time[5] = time_date.tm_sec +_sec;
}

