#ifndef __SERIALPORT_H
#define __SERIALPORT_H

void SerialPort_Init(void);
void SerialPort_SendByte(uint8_t SendByte);
void SerialPort_SendArray(uint16_t *SendArray , uint8_t SendSize);
void SerialPort_SendString(char *SendString);
uint32_t SerialPow(uint32_t X , uint32_t Y);
void SerialPort_SendNum(uint32_t SendNum , uint16_t SendSize);

uint8_t SerialPort_GetRxFlag(void);
uint8_t SerialPort_GetRxData(void);

#endif
