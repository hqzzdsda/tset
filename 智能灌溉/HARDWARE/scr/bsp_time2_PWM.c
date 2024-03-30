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
	/*C89��ʽ����������ֻ������ǰ���*/
  /*C99��ʽ�������������ں���������κεط�*/
  TIM_OCInitTypeDef   TIME2_OUTPUT_InitStruct;
	TIM_TimeBaseInitTypeDef  TIME2_PWM_InitStruct;
		NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	ADVANCE_TIM2_APB1Clock_Fun(ADVANCE_TIM2_CLK, ENABLE);
	
	/*--------------ʱ���ṹ���ʼ����PWM�Ļ�׼ʱ�ӣ�------------------*/
	TIME2_PWM_InitStruct.TIM_Period=ADVANCE_TIM2_Period;
	TIME2_PWM_InitStruct.TIM_Prescaler=ADVANCE_TIM2_Prescaler;
	TIME2_PWM_InitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ�ӵķ�Ƶ���ӣ���������ʱ���ʱ����Ҫ�õ���1��Ƶ�ڲ�ʱ�Ӹ�CKD��72MHZ��
	TIME2_PWM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ�����
	TIME2_PWM_InitStruct.TIM_RepetitionCounter=0;
 
	TIM_TimeBaseInit(ADVANCE_TIME2,&TIME2_PWM_InitStruct);
	
  /*--------------����ȽϽṹ���ʼ��------------*/
	
	//����ΪPWMģʽ1
	TIME2_OUTPUT_InitStruct.TIM_OCMode=TIM_OCMode_PWM1;/*ģʽ1���ǣ��ߵ�ƽ��Ч��ģʽ2���ǵ͵�ƽ��Ч*/
	//���ʹ��
	TIME2_OUTPUT_InitStruct.TIM_OutputState=TIM_OutputState_Enable;

	TIME2_OUTPUT_InitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	//��ʼ�����ͨ���ͻ���ͨ��������ͨ����������ͨ��һ���ʼ����


/*-------------------CH2���PWM------------------*/	
	TIME2_OUTPUT_InitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIME2_OUTPUT_InitStruct.TIM_Pulse=ADVANCE_TIM2_PULSE;/**/

  TIM_OC2Init(ADVANCE_TIME2,&TIME2_OUTPUT_InitStruct);/**/
	//�Զ���װ��ʹ�ܣ��ǵ�ARR��ֵʱ������װ��
	TIM_OC2PreloadConfig(ADVANCE_TIME2,TIM_OCPreload_Enable);/**/


	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //�������жϱ�־λ
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;                 //����TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
	TIM_Cmd(TIM2,ENABLE);   //��TIM2 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                      //ʹ��TIM2����ж�   
}
void TIME2_PWM_Init_10MS(void)
{
	TIME2_GPIO_PWM_Config();
	TIME2_PWM_MODE_Config();
}

u32 SystemTimer,Dc_motor_pwm_cnt;
/*-------------------------------------------------*/
/*����������ʱ��2�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){  //���TIM_IT_Update��λ����ʾTIM2����жϣ�����if	
		/*���PWM��������*/  
			Dc_motor_pwm_cnt++;		//����
		if(Dc_motor_pwm_cnt>Dc_motor_pwm/10)
			FAN=0;//�رշ���
		else
			FAN=1;//�򿪷���
		if(Dc_motor_pwm_cnt>10)Dc_motor_pwm_cnt=0;
		/*���PWM��������*/
			SystemTimer++;                                  //ȫ�ּ�ʱ�ı���+1,��ʾ��ȥ��10������
			if(SystemTimer>1000)SystemTimer=0;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //���TIM2����жϱ�־ 	
	}
}