#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"

// 自定义时间结构体
typedef struct {
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Seconds;
} RTC_TimeTypeDef;

// 自定义日期结构体
typedef struct {
    uint16_t Year;   // 年份
    uint8_t Month;   // 月份 (1~12)
    uint8_t Date;    // 日期 (1~31)
    uint8_t WeekDay; // 星期 (0~6, 0代表周日)
} RTC_DateTypeDef;

// 定义格式选项
#define RTC_FORMAT_BIN 0  // 二进制格式
#define RTC_FORMAT_BCD 1  // BCD 格式

// RTC 配置函数
void RTC_Init(void);
void I2C1_Init(void);

// 自定义 RTC 设置计数器
void RTC_SetCounterCustom(uint32_t CounterValue);
uint32_t RTC_GetCounterCustom(void);

// RTC 自定义时间与日期设置函数
void RTC_SetTime(RTC_TimeTypeDef *sTime);
void RTC_SetDate(RTC_DateTypeDef *sDate);

// RTC 自定义时间与日期获取函数
ErrorStatus RTC_GetTime(RTC_TimeTypeDef *sTime, uint8_t Format);
ErrorStatus RTC_GetDate(RTC_DateTypeDef *sDate);

#endif /* __RTC_H */
