#ifndef __KEY_H
#define __KEY_H	
#define KEY0_IN_STA  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) //PB5控制KEY1,读取电平状态,可以判断KEY1是否按下
#define KEY1_IN_STA  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) //PB5控制KEY1,读取电平状态,可以判断KEY1是否按下
#define KEY2_IN_STA  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) //PB6控制KEY2,读取电平状态,可以判断KEY2是否按下
#define KEY3_IN_STA  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) //PB7控制KEY3,读取电平状态,可以判断KEY2是否按下
#include "usart1.h"
#include "delay.h"
#include "sys.h"
#include "pub_sub_message.h"


void KEY_Init(void);	   //按键初始化
void KEY_Exti_Init(void);  //按键外部中断初始化程序

#endif
