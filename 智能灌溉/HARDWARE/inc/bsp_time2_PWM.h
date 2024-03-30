#ifndef __BSP_TIME2_PWM_H       //��ֹ�ظ����룺ifndef
#define __BSP_TIME2_PWM_H
#include "stm32f10x.h"//�൱��51��Ƭ���е�#include"reg51.h"
#include "show.h"
#define ADVANCE_TIME2         						 TIM2
#define ADVANCE_TIM2_APB1Clock_Fun         RCC_APB1PeriphClockCmd
#define ADVANCE_TIM2_CLK                   RCC_APB1Periph_TIM2
//PWM�źŵ�Ƶ��F=TIM_CLK/{(ARR+1)*(PSC+1)}  1MHZ  72MHZ/(8*9)=1MHZ
#define ADVANCE_TIM2_Period                (100-1)//ARR�Ĵ������� PWM�����ڣ��ߵ�ƽռ�ձ�+�͵�ƽ��ռ�ձ�=���ڣ�//�ǵ�8   Ȼ���00000�Σ�����(0.000000125*8)��
#define ADVANCE_TIM2_Prescaler             (7200-1)//Ԥ��Ƶ��   //9/72MHZ=0.000000125��һ�����ڼ�0.000000125��ʱ��
#define ADVANCE_TIM2_PULSE								 0//CCR�Ĵ�������PWM�ߵ�ƽ���ߵ͵�ƽ��ռ�ձ�

/*
//��Ϊ���ĸ�ͨ����CH1��CH2��CH3��CH4��������CCR�Ĵ������ĸ�
#define ADVANCE_TIM1_TRQ                   TIM1_UP_IRQn 
*/
#define ADVANCE_GPIO_APB2Clock_Fun         RCC_APB2PeriphClockCmd

 
#define ADVANCE_TIM2_CH2_GPIO_CLK         RCC_APB2Periph_GPIOA
#define ADVANCE_TIM2_CH2_PORT               GPIOA
#define ADVANCE_TIM2_CH2_PIN							 GPIO_Pin_1

 
void TIME2_PWM_Init_10MS(void);
#endif /*_BSP_LED_H */
