#include "bsp_time2_PWM.h"


static void TIME2_GPIO_PWM_Config(void)
{
	GPIO_InitTypeDef  GPIO_TIME_InitStruct;
	ADVANCE_GPIO_APB2Clock_Fun(ADVANCE_TIM2_CH2_GPIO_CLK,ENABLE);	
	GPIO_TIME_InitStruct.GPIO_Pin=ADVANCE_TIM2_CH2_PIN; 

	GPIO_TIME_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;	
	GPIO_TIME_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;		
	GPIO_Init(ADVANCE_TIM2_CH2_PORT, &GPIO_TIME_InitStruct);
  
}
static void TIME2_PWM_MODE_Config(void)
{
	/*C89格式，变量定义只能在最前面的*/
  /*C99格式，变量定义能在函数体里的任何地方*/
  TIM_OCInitTypeDef   TIME2_OUTPUT_InitStruct;
	TIM_TimeBaseInitTypeDef  TIME2_PWM_InitStruct;
		NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	ADVANCE_TIM2_APB1Clock_Fun(ADVANCE_TIM2_CLK, ENABLE);
	
	/*--------------时基结构体初始化（PWM的基准时钟）------------------*/
	TIME2_PWM_InitStruct.TIM_Period=ADVANCE_TIM2_Period;
	TIME2_PWM_InitStruct.TIM_Prescaler=ADVANCE_TIM2_Prescaler;
	TIME2_PWM_InitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟的分频因子，配置死区时间的时候需要用到，1分频内部时钟给CKD（72MHZ）
	TIME2_PWM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数，
	TIME2_PWM_InitStruct.TIM_RepetitionCounter=0;
 
	TIM_TimeBaseInit(ADVANCE_TIME2,&TIME2_PWM_InitStruct);
	
  /*--------------输出比较结构体初始化------------*/
	
	//配置为PWM模式1
	TIME2_OUTPUT_InitStruct.TIM_OCMode=TIM_OCMode_PWM1;/*模式1就是，高电平有效，模式2就是低电平有效*/
	//输出使能
	TIME2_OUTPUT_InitStruct.TIM_OutputState=TIM_OutputState_Enable;

	TIME2_OUTPUT_InitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	//初始化输出通道和互补通道（互补通道会跟随输出通道一起初始化）


/*-------------------CH2输出PWM------------------*/	
	TIME2_OUTPUT_InitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIME2_OUTPUT_InitStruct.TIM_Pulse=ADVANCE_TIM2_PULSE;/**/

  TIM_OC2Init(ADVANCE_TIME2,&TIME2_OUTPUT_InitStruct);/**/
	//自动重装载使能，记到ARR的值时，重新装载
	TIM_OC2PreloadConfig(ADVANCE_TIME2,TIM_OCPreload_Enable);/**/


	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //清除溢出中断标志位
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;                 //设置TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
	TIM_Cmd(TIM2,ENABLE);   //开TIM2 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                      //使能TIM2溢出中断   
}
void TIME2_PWM_Init_10MS(void)
{
	TIME2_GPIO_PWM_Config();
	TIME2_PWM_MODE_Config();
}

u32 SystemTimer,Dc_motor_pwm_cnt;
/*-------------------------------------------------*/
/*函数名：定时器2中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){  //如果TIM_IT_Update置位，表示TIM2溢出中断，进入if	
		/*软件PWM驱动风扇*/  
			Dc_motor_pwm_cnt++;		//计数
		if(Dc_motor_pwm_cnt>Dc_motor_pwm/10)
			FAN=0;//关闭风扇
		else
			FAN=1;//打开风扇
		if(Dc_motor_pwm_cnt>10)Dc_motor_pwm_cnt=0;
		/*软件PWM驱动风扇*/
			SystemTimer++;                                  //全局计时的变量+1,表示过去了10毫秒钟
			if(SystemTimer>1000)SystemTimer=0;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //清除TIM2溢出中断标志 	
	}
}