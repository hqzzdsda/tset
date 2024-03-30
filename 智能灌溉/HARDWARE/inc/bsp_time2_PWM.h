#ifndef __BSP_TIME2_PWM_H       //防止重复编译：ifndef
#define __BSP_TIME2_PWM_H
#include "stm32f10x.h"//相当于51单片机中的#include"reg51.h"
#include "show.h"
#define ADVANCE_TIME2         						 TIM2
#define ADVANCE_TIM2_APB1Clock_Fun         RCC_APB1PeriphClockCmd
#define ADVANCE_TIM2_CLK                   RCC_APB1Periph_TIM2
//PWM信号的频率F=TIM_CLK/{(ARR+1)*(PSC+1)}  1MHZ  72MHZ/(8*9)=1MHZ
#define ADVANCE_TIM2_Period                (100-1)//ARR寄存器决定 PWM的周期（高电平占空比+低电平的占空比=周期）//记到8   然后记00000次，就是(0.000000125*8)秒
#define ADVANCE_TIM2_Prescaler             (7200-1)//预分频器   //9/72MHZ=0.000000125，一次周期记0.000000125的时间
#define ADVANCE_TIM2_PULSE								 0//CCR寄存器决定PWM高电平或者低电平的占空比

/*
//因为有四个通道（CH1、CH2、CH3、CH4），所以CCR寄存器有四个
#define ADVANCE_TIM1_TRQ                   TIM1_UP_IRQn 
*/
#define ADVANCE_GPIO_APB2Clock_Fun         RCC_APB2PeriphClockCmd

 
#define ADVANCE_TIM2_CH2_GPIO_CLK         RCC_APB2Periph_GPIOA
#define ADVANCE_TIM2_CH2_PORT               GPIOA
#define ADVANCE_TIM2_CH2_PIN							 GPIO_Pin_1

 
void TIME2_PWM_Init_10MS(void);
#endif /*_BSP_LED_H */
