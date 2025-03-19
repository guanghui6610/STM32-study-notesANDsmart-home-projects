#include "RTC.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include <stdint.h>  // 确保包含 uint8_t, uint16_t 等定义
#include <stddef.h>  // 包含 NULL 的定义

// 定义月日的天数表 (平年)
const uint8_t RTC_MonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 辅助函数：判断是否为闰年
static uint8_t RTC_IsLeapYear(uint16_t year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

// 将二进制转换为 BCD 格式
uint8_t ByteToBCD2(uint8_t value) {
    return ((value / 10U) << 4U) | (value % 10U);
}

// 将 BCD 转换为二进制
uint8_t BCD2ToByte(uint8_t value) {
    return ((value >> 4U) * 10U) + (value & 0x0FU);
}

// RTC 初始化函数
void RTC_Init(void) {
    // 启用 PWR 和 BKP 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    // 使用内部低速时钟（LSI）作为 RTC 时钟源
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);  // LSI 时钟源 40kHz
    RCC_RTCCLKCmd(ENABLE); // 启用 RTC 时钟

    // 允许访问 RTC 寄存器
    PWR_BackupAccessCmd(ENABLE);

    // 等待 RTC 寄存器同步
    RTC_WaitForSynchro();

    // 配置 RTC 分频器，设置时间基准为 1 秒
    RTC_SetPrescaler(127); // LSI 时钟为 40kHz，分频器设置为 127
    RTC_WaitForLastTask();

    // RTC 配置完成后可以选择继续进行时间设定（如果需要）
}

// 自定义 RTC 设置计数器
void RTC_SetCounterCustom(uint32_t CounterValue) {
    RTC_SetCounter(CounterValue);
    RTC_WaitForLastTask();
}

// 自定义 RTC 获取计数器
uint32_t RTC_GetCounterCustom(void) {
    return RTC_GetCounter();
}


// RTC 自定义时间设置函数
void RTC_SetTime(RTC_TimeTypeDef *sTime) {
    uint32_t counter_time;

    // 转换时间为秒数
    counter_time = sTime->Hours * 3600U + sTime->Minutes * 60U + sTime->Seconds;

    // 设置时间
    RTC_SetCounter(counter_time);
}

// RTC 自定义时间获取函数
ErrorStatus RTC_GetTime(RTC_TimeTypeDef *sTime, uint8_t Format) {
    uint32_t counter_time;
    uint32_t hours;

    if (sTime == NULL) {
        return ERROR;
    }

    // 读取 RTC 计数值
    counter_time = RTC_GetCounter();

    // 解析小时、分钟、秒
    hours = counter_time / 3600U;
    sTime->Minutes = (counter_time % 3600U) / 60U;
    sTime->Seconds = (counter_time % 3600U) % 60U;
    sTime->Hours = hours % 24U;

    // 如果需要转换为 BCD 格式
    if (Format == RTC_FORMAT_BCD) {
        sTime->Hours = ByteToBCD2(sTime->Hours);
        sTime->Minutes = ByteToBCD2(sTime->Minutes);
        sTime->Seconds = ByteToBCD2(sTime->Seconds);
    }

    return SUCCESS;
}

// RTC 自定义日期设置函数
void RTC_SetDate(RTC_DateTypeDef *sDate) {
    uint32_t days = 0;

    // 计算从 2000 年 1 月 1 日起的天数
    for (uint16_t year = 2000; year < sDate->Year; year++) {
        days += (RTC_IsLeapYear(year) ? 366 : 365);
    }

    for (uint8_t month = 1; month < sDate->Month; month++) {
        days += RTC_MonthDays[month - 1];
        if (month == 2 && RTC_IsLeapYear(sDate->Year)) {
            days += 1; // 闰年 2 月
        }
    }

    days += (sDate->Date - 1);

    // 设置日期（转换为秒数）
    RTC_SetCounter(days * 86400);
}

// RTC 自定义日期获取函数
ErrorStatus RTC_GetDate(RTC_DateTypeDef *sDate) {
    uint32_t counter_time = RTC_GetCounter();
    uint32_t days = counter_time / 86400;
    uint16_t year = 2000;

    if (sDate == NULL) {
        return ERROR;
    }

    // 计算年份
    while (days >= (RTC_IsLeapYear(year) ? 366 : 365)) {
        days -= (RTC_IsLeapYear(year) ? 366 : 365);
        year++;
    }
    sDate->Year = year;

    // 计算月份
    for (uint8_t month = 1; month <= 12; month++) {
        uint8_t month_days = RTC_MonthDays[month - 1];
        if (month == 2 && RTC_IsLeapYear(year)) {
            month_days += 1; // 闰年 2 月
        }

        if (days < month_days) {
            sDate->Month = month;
            sDate->Date = days + 1;
            break;
        }

        days -= month_days;
    }

    // 计算星期
    sDate->WeekDay = (counter_time / 86400 + 6) % 7; // 2000-01-01 是星期六

    return SUCCESS;
}

