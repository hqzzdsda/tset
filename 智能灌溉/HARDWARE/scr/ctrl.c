#include "ctrl.h"
u8 APP_alarm;
u8 one_ctrl[2] = "12"; //�������ƶ����������������ֻ����һ�ζ���
/*
alarm_num=1 �¶ȴ�������
alarm_num=2 �¶�С������

alarm_num=3 ʪ�ȴ�������
alarm_num=4 ʪ��С������

alarm_num=5 ����ʪ�ȴ�������
alarm_num=6 ����ʪ��С������

*/
#define LOG  0
u8 ctrl_data(void)
{
    u8 alarm_num = 0;
    if(tem > tem_up) //�¶ȴ�������
    {
#if LOG
        u1_printf( "�¶ȴ�������\r\n");
#endif

        alarm_num = 1; //�������
        if(auto_flag && one_ctrl[0]) //�Զ�ģʽ���ҵ�һ�ν���  
        {
            Dc_motor_pwm = 50;     
            one_ctrl[0] = 0; //���㣬��һ�ν�������Ҫ������һ����ܽ���
        }
    }
    else if(tem < tem_down) //�¶�С������
    {
#if LOG
        u1_printf( "�¶�С������\r\n");
#endif
        alarm_num = 2; //�������
        if(auto_flag && one_ctrl[0]) //�Զ�ģʽ
        {
            Dc_motor_pwm = 0;
            one_ctrl[0] = 0; //���㣬��һ�ν�������Ҫ������һ����ܽ���
        }
    }
    else if(tem > tem_down && tem < tem_up) //�¶�����
    {
#if LOG
        u1_printf( "�¶�����\r\n");
#endif
        one_ctrl[0] = 1;
        if(auto_flag)//�Զ�ģʽ
            Dc_motor_pwm = 0;
    }

    if(humi > humi_up) //ʪ�ȴ�������
    {
#if LOG
        u1_printf( "ʪ�ȴ�������\r\n");
#endif
        alarm_num = 3; //�������
    }
    else if(humi < humi_down) //ʪ��С������
    {
#if LOG
        u1_printf( "ʪ��С������\r\n");
#endif
        alarm_num = 4; //�������
    }
    if(soil > soil_up) //����ʪ�ȴ�������
    {
#if LOG
        u1_printf( "����ʪ�ȴ�������\r\n");
#endif
        alarm_num = 5; //�������
        if(auto_flag && one_ctrl[1]) //�Զ�ģʽ
        {
            water_pwm = 0;//�ر�ˮ��
            one_ctrl[1] = 0; //���㣬��һ�ν�������Ҫ������һ����ܽ���
        }
    }
    else if(soil < soil_down) //����ʪ��С������
    {
#if LOG
        u1_printf( "����ʪ��С������\r\n");
#endif
        alarm_num = 6; //�������
        if(auto_flag && one_ctrl[1]) //�Զ�ģʽ
        {
            water_pwm = 50;//��ˮ��
            one_ctrl[1] = 0; //���㣬��һ�ν�������Ҫ������һ����ܽ���
        }
    }
    else if(soil >=soil_down && soil < soil_up) //����ʪ������
    {
#if LOG
        u1_printf( "����ʪ������\r\n");
#endif
        one_ctrl[1] = 1;
        if(auto_flag)//�Զ�ģʽ
            water_pwm = 0;//�ر�ˮ��
    }
    if(alarm_num == 0) //�������Ϊ0��ʾû�б���
    {
#if LOG
        u1_printf( "����\r\n");
#endif
        APP_alarm = 0; //app������־λ
        BEEP = 0; //ֹͣ����
    }
		else
		{
        BEEP = !BEEP; 
        screen_num = 5; //��������
        APP_alarm = 1; //app����
        Delay_Ms(100);
		}
    if(wifi_connect && SubcribePack_flag) //��־λ��λ������wifi�ͷ�����
        On_off_state_send();//�ϴ����ص�����
    return alarm_num;
}
void On_off_state_send(void)
{
    static u8  Dc_motor_pwm_sta,water_pwm_sta; //�����������һ��״̬
    //	 u1_printf( "Stepper_motor_run_sta=%d\r\n",Stepper_motor_run_sta);
    //	 u1_printf( "Stepper_motor_run[0]=%d\r\n",Stepper_motor_run[0]);
    if(Dc_motor_pwm != Dc_motor_pwm_sta)
    {
        Dc_motor_pwm_sta = Dc_motor_pwm;
        data_sta(""APP_FAN"", Dc_motor_pwm);
    }
    if(water_pwm != water_pwm_sta)
    {
        water_pwm_sta = water_pwm;
        data_sta(""APP_WATER"", water_pwm);
    }

}
