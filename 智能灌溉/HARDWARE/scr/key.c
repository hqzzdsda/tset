#include "key.h"
#include "stm32f10x_exti.h"
/*-------------------------------------------------*/
/*����������ʼ��KEY��������                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                       //����һ������IO�Ľṹ��

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);     //ʹ��PA�˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//ʹ��PORTB��ʱ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //�ر�JTAG��SWD���Ե��Ե�
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;           //׼������PB5,PB6,PB7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   		   //�������룬Ĭ��״̬�ǵ͵�ƽ
    GPIO_Init(GPIOB, &GPIO_InitStructure);            		   //����PA0
}

/*-------------------------------------------------*/
/*�������������ⲿ�жϳ�ʼ������.                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void KEY_Exti_Init(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure; //����һ������IO�Ľṹ��
    EXTI_InitTypeDef   EXTI_InitStructure; //����һ�������жϵĽṹ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	         //ʹ��GPIO���ù���ʱ��
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);   //����KEY1-PB55�ж���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);   //����KEY1-PB55�ж���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);   //����KEY2-PB6�ж���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);   //����KEY3-PB7�ж���

    EXTI_InitStructure.EXTI_Line = EXTI_Line3;	                  //׼�������ⲿ5�ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //�ⲿ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //�½��ش����ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
    EXTI_Init(&EXTI_InitStructure);	 	                          //�����ⲿ�ж�5��

    EXTI_InitStructure.EXTI_Line = EXTI_Line4;	                  //׼�������ⲿ5�ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //�ⲿ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //�½��ش����ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
    EXTI_Init(&EXTI_InitStructure);	 	                          //�����ⲿ�ж�5��

    EXTI_InitStructure.EXTI_Line = EXTI_Line5;	                  //׼�������ⲿ6�ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //�ⲿ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //�½��ش����ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
    EXTI_Init(&EXTI_InitStructure);	 	                          //�����ⲿ�ж�6��

    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //�ⲿ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //�½��ش����ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
    EXTI_Init(&EXTI_InitStructure);	 	                          //�����ⲿ�ж�7��

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			  //׼�������ⲿ�ж�5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		      //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //ʹ���ⲿ�ж�1ͨ��
    NVIC_Init(&NVIC_InitStructure);                               //�����ⲿ�ж�4

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			  //׼�������ⲿ�ж�5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		      //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //ʹ���ⲿ�ж�1ͨ��
    NVIC_Init(&NVIC_InitStructure);                               //�����ⲿ�ж�4

    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			  //׼�������ⲿ�ж�5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		      //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //ʹ���ⲿ�ж�1ͨ��
    NVIC_Init(&NVIC_InitStructure);                               //�����ⲿ�ж�4
}
void EXTI3_IRQHandler(void)//ѡ�񰴼�
{
    Delay_Ms(10);//����
    if(!KEY0_IN_STA)
    {
        switch(screen_num)
        {
        case 0://������
            screen_choose++;
            screen_choose %= 3;
            break;
        case 2://��ֵѡ�����
            switch(admin_screen_page)
            {
            case 1://��һ������
                if(admin_choose < 4)
                    admin_choose++;
                else admin_choose = 0;
                break;
            case 2://�ڶ�������
                if(admin_choose < 8)
                    admin_choose++;
                else admin_choose = 5;
                break;
            }
            break;
        case 3://���ؿ��ƽ���
            pwr_choose++;
            pwr_choose %= 3;
            break;
        case 5://��������
            choose_fan_admin = !choose_fan_admin;
            break;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE5�ϵ��жϱ�־λ
}
u8 DC_MOTOR_flag, water_pwm_flag;
//�ⲿ�ж�4�������
void EXTI4_IRQHandler(void)//ȷ�ϰ���
{
    Delay_Ms(10);//����
    if(!KEY1_IN_STA)
    {
        switch(screen_num)
        {
        case 0://��������
            if(screen_choose == 0)screen_num = 1;//0��ʾ��ͷָ�� �鿴���� ����
            else 	if(screen_choose == 1)screen_num = 2;//1��ʾ��ͷָ�� ��ֵ�޸Ľ���
            else 	if(screen_choose == 2)screen_num = 3;//2��ʾ��ͷָ�� ���ؿ��ƽ���
            break;
        case 2://����ֵ�޸Ľ���
            switch(admin_screen_page)
            {
            case 1 ://��ֵ��һ����
                if(admin_choose == 3)
                {
                    screen_num = 0;//ѡ�з��� �򷵻�������
                    admin_choose = 0;
                }
                else 	if(admin_choose == 4)
                {
                    admin_screen_page = 2;//ѡ����һҳ �������һҳ
                    admin_choose = 5;
                }
                break;
            case 2://��ֵ�ڶ�����
                if(admin_choose == 8)
                {
                    admin_screen_page = 1;//ѡ�з��� �򷵻ص�һ����
                    admin_choose = 0;
                }

                break;
                break;
            }
            break;
        case 1://��������ʾ����
            screen_num = 0;//ѡ�з��� �򷵻�������
            break;
        case 3://���ؿ��ƽ���
            switch(pwr_choose)
            {
            case 0:
                water_pwm_flag = !water_pwm_flag;
                if(water_pwm_flag)
                {
                    water_pwm = 50;
                }
                else water_pwm = 0;
                data_sta(""APP_WATER"", water_pwm);//�ϱ������ݵ�������
                break;
            case 1:
                DC_MOTOR_flag = !DC_MOTOR_flag;
                if(DC_MOTOR_flag)
                {
                    Dc_motor_pwm = 50;                    data_sta(""APP_FAN"", Dc_motor_pwm);//�ϱ������ݵ�������
                }
                else 
								{
Dc_motor_pwm = 0;                    data_sta(""APP_FAN"", Dc_motor_pwm);//�ϱ������ݵ�������
								}
                data_sta(""APP_FAN"", Dc_motor_pwm);//�ϱ������ݵ�������
                break;
            case 2:
                screen_num = 0;//ѡ�з��� �򷵻�������
                break;
            }
            break;
        }

    }
    EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE5�ϵ��жϱ�־λ
}

//�ⲿ�ж�5-6-7�������
void EXTI9_5_IRQHandler(void)  // ++ --
{
    Delay_Ms(10);//����

    if(KEY2_IN_STA == 0) //����������Ӱ����ٴ��жϿ���KEY1��IO�����ƽ���Ƿ��ǵ͵�ƽ������ǽ���if��˵������������
    {
        if(screen_num == 3 && pwr_choose == 1)   //�ڿ��ؿ��ƽ���ѡ�񵽷���
        {
            if(Dc_motor_pwm < 99)
                Dc_motor_pwm++;                    data_sta(""APP_FAN"", Dc_motor_pwm);//�ϱ������ݵ�������
        }
        if(screen_num == 3 && pwr_choose == 0)//�ڿ��ؿ��ƽ���ѡ��ˮ��
        {
            if(water_pwm < 99)
                water_pwm++;  data_sta(""APP_WATER"", water_pwm);//�ϱ������ݵ�������
        }
        if(screen_num == 0)  //���������л�ģʽ
        {
            auto_flag = !auto_flag;   data_sta(""APP_auto_flag"", auto_flag);
        }
        if(screen_num == 2)
            switch(admin_choose)
            {
            case 0:
                tem_up++;
                data_sta(""APP_tem_up"", tem_up);//�ϱ������ݵ�������
                break;
            case 1:
                tem_down++;
                data_sta(""APP_tem_down"", tem_down);
                break;
            case 5:
                humi_up++;
                data_sta(""APP_humi_up"", humi_up);//�ϱ������ݵ�������
                break;
            case 6:
                humi_down++;
                data_sta(""APP_humi_down"", humi_down);//�ϱ������ݵ�������
                break;
            case 7:
                soil_down++;
                data_sta(""APP_soil_down"", soil_down);//�ϱ������ݵ�������
                break;
            case 2:
                soil_up++;
                data_sta(""APP_soil_up"", soil_up);//�ϱ������ݵ�������
                break;

            }
        if(screen_num == 5)//�ڱ�������
        {
					
            switch(ctrl_data())
            {
            case 1:
                if(choose_fan_admin == 1)
                {
                    tem_up++;
                    data_sta(""APP_tem_up"", tem_up);//�ϱ������ݵ�������
                }
                else
                {
                    Dc_motor_pwm++;
                    data_sta(""APP_FAN"", Dc_motor_pwm);//�ϱ������ݵ�������
                }
                break;
            case 2:
                tem_down++;
                data_sta(""APP_tem_down"", tem_down);//�ϱ������ݵ�������
                break;
            case 3:
                humi_up++;
                data_sta(""APP_humi_up"", humi_up);//�ϱ������ݵ�������
                break;
            case 4:
                humi_down++;
                data_sta(""APP_humi_down"", humi_down);//�ϱ������ݵ�������
                break;
            case 6:
                if(choose_fan_admin == 1)
                {
                    if(soil_down <99)
                        soil_down++;
                        data_sta(""APP_soil_down"", soil_down);//�ϱ������ݵ�������
                }
                else
                {
									if(water_pwm <99)
											water_pwm++;			
                        data_sta(""APP_WATER"", water_pwm);//�ϱ������ݵ�������
                }
                break;
            case 5:
                soil_up++;
                data_sta(""APP_soil_up"", soil_up);//�ϱ������ݵ�������
                break;

            }
        }

    }
    if(KEY3_IN_STA == 0) //����������Ӱ����ٴ��жϿ��� KEY2 ��IO�����ƽ���Ƿ��ǵ͵�ƽ������ǽ���if��˵������������
    {
        if(screen_num == 3 && pwr_choose == 1)//�ڿ��ؿ��ƽ���ѡ�񵽷���
        {
            if(Dc_motor_pwm != 0)
                Dc_motor_pwm--;     data_sta(""APP_FAN"", Dc_motor_pwm);//�ϱ������ݵ�������
        }
        if(screen_num == 3 && pwr_choose == 0)//�ڿ��ؿ��ƽ���ѡ��ˮ��
        {
            if(water_pwm != 0)
                water_pwm--;  data_sta(""APP_WATER"", water_pwm);//�ϱ������ݵ�������
        }
        if(screen_num == 0)//����Ļ
        {
            wifi_connect = !wifi_connect;//�Ͽ�������wifi
        }

        if(screen_num == 2)//����ֵ���ڽ���
            switch(admin_choose)
            {
            case 0:
                if(tem_up != 0)
                    tem_up--;
                data_sta(""APP_tem_up"", tem_up);//�ϱ������ݵ�������
                break;
            case 1:
                if(tem_down != 0)
                    tem_down--;
                data_sta(""APP_tem_down"", tem_down);
                break;
            case 5:
                if(humi_up != 0)
                    humi_up--;
                data_sta(""APP_humi_up"", humi_up);//�ϱ������ݵ�������
                break;
            case 6:
                if(humi_down != 0)
                    humi_down--;
                data_sta(""APP_humi_down"", humi_down);//�ϱ������ݵ�������
                break;
            case 7:
                if(soil_down != 0)
                    soil_down--;
                data_sta(""APP_soil_down"", soil_down);//�ϱ������ݵ�������
                break;
            case 2:
                if(soil_up != 0)
                    soil_up--;
                data_sta(""APP_soil_up"", soil_up);//�ϱ������ݵ�������
                break;


            }

        if(screen_num == 5)//�ڱ�������
        {
            switch(ctrl_data())
            {
            case 1:
                if(choose_fan_admin == 1)
                {
                    if(tem_up != 0)
                        tem_up--;

                    data_sta(""APP_tem_up"", tem_up);//�ϱ������ݵ�������
                }
                else
                {
                    if(Dc_motor_pwm != 0)
                        Dc_motor_pwm--;

                    data_sta(""APP_FAN"", Dc_motor_pwm);//�ϱ������ݵ�������
                }
                break;
            case 2:
                if(tem_down != 0)
                    tem_down--;

                data_sta(""APP_tem_down"", tem_down);//�ϱ������ݵ�������
                break;
            case 3:
                if(humi_up != 0)
                    humi_up--;

                data_sta(""APP_humi_up"", humi_up);//�ϱ������ݵ�������
                break;
            case 4:
                if(humi_down != 0)
                    humi_down--;

                data_sta(""APP_humi_down"", humi_down);//�ϱ������ݵ�������
                break;

            case 6:
                if(choose_fan_admin == 1)
                {
                    if(soil_down != 0)
                        soil_down--;
                        data_sta(""APP_soil_down"", soil_down);//�ϱ������ݵ�������
                }
                else
                {
									if(water_pwm != 0)
											water_pwm--;			
                        data_sta(""APP_WATER"", water_pwm);//�ϱ������ݵ�������
                }
                break;
            case 5:
                if(soil_up != 0)
                    soil_up--;
                data_sta(""APP_soil_up"", soil_up);//�ϱ������ݵ�������
                break;

            }
        }

    }
    EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE5�ϵ��жϱ�־λ
    EXTI_ClearITPendingBit(EXTI_Line7);  //���LINE7�ϵ��жϱ�־λ
}

