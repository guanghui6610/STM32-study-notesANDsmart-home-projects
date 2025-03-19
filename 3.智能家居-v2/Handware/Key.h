#ifndef __KEY_H
#define __KEY_H

void Key1Interrupt_Init(void);
void Key2Interrupt_Init(void);
void Key3Interrupt_Init(void);
uint16_t Key_GetValue(void);

void Key_Init(void);
uint8_t Key_GetNum(void);

void Key_Control(void);

#endif
