#ifndef __KEY_H
#define __KEY_H	
#define KEY0_IN_STA  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) //PB5����KEY1,��ȡ��ƽ״̬,�����ж�KEY1�Ƿ���
#define KEY1_IN_STA  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) //PB5����KEY1,��ȡ��ƽ״̬,�����ж�KEY1�Ƿ���
#define KEY2_IN_STA  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) //PB6����KEY2,��ȡ��ƽ״̬,�����ж�KEY2�Ƿ���
#define KEY3_IN_STA  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) //PB7����KEY3,��ȡ��ƽ״̬,�����ж�KEY2�Ƿ���
#include "usart1.h"
#include "delay.h"
#include "sys.h"
#include "pub_sub_message.h"


void KEY_Init(void);	   //������ʼ��
void KEY_Exti_Init(void);  //�����ⲿ�жϳ�ʼ������

#endif
