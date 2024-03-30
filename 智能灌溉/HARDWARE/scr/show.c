#include "show.h"
extern char SubcribePack_flag;   //���ı���״̬      1�����ı��ĳɹ�
u8 screen_num = 0; //��Ļ��
char screen_num_sta = 0; //��Ļ��״̬
u8 date_screen_page = 1, admin_screen_page = 1,pwr_screen_page; //��ת��һҳ�ı�־λ
unsigned char screen_choose, admin_choose,pwr_choose,choose_fan_admin; //ѡ��;
/*-----------------����������--------------------------*/
u8 tem_up = 50, humi_up = 80;
u8 tem_down = 10, humi_down = 10;
u8 soil_down = 0, soil_up = 50;
/*------------------����������-------------------------*/
u8 soil;
u8 wifi_connect;
u8 auto_flag;
u8 Dc_motor_pwm=0, water_pwm;
/*********************************************************************************************************
* �� �� �� : void	Cut_screen()
* ����˵�� : ��Ļ����
* ��    �� : ��
* �� �� ֵ : ��
* ��    ע : 2023-03-04
*********************************************************************************************************/
void	Cut_screen(void)
{
    OLED_Refresh();//�����Դ�--����ʾ�����ݷ����ı�ʱ�����
		
    if(screen_num != screen_num_sta) ////��ֵ�����ı�--���ڼ��ֵ�Ƿ����ı�
    {
        screen_num_sta = screen_num; //���浱ǰֵ
        OLED_Clear();//����
    }

    switch (screen_num)
    {
    case 0:
        if(auto_flag)//�Զ�
            OLED_show_str(120, 48, "A");
        else OLED_show_str(120, 48, "w");
        if( wifi_connect && SubcribePack_flag) //��־λ��λ������wifi�ͷ�����
            OLED_show_str (98, 32, "WIFI");//���ӷ�����ģʽ
        else OLED_show_str (98, 32, "           "); //������ģʽ
        main_screen();//����Ļ
        choose_screen();
        break;//ѡ����Ļ
    case 1:
        data_screen();//��ʾ���ݽ���
        OLED_show_str_turn(0, 48, "����");
        break;
    case 2:
        admin_screen();//�������޵Ľ���
        choose_admin();
        break;
		case 3:
        pwr_choose_screen();//���ؿ���
        choose_pwr();
        break;
    case 5:
        beep_screen();//��������
        break;
    }
}
void main_screen(void)//������
{
    OLED_show_str(20, 0, "���ܹ��");
    OLED_show_str(16, 16, "�鿴����");
    OLED_show_str(16, 32, "��ֵ�޸�");
    OLED_show_str(16, 48, "���ؿ���");
}

void admin_screen(void)
{
    static	u8 admin_screen_page_sta;

    if(admin_screen_page_sta != admin_screen_page) ////��ֵ�����ı�--���ڼ��ֵ�Ƿ����ı�
    {
        admin_screen_page_sta = admin_screen_page; //���浱ǰֵ
        OLED_Clear();//����
    }
    switch(admin_screen_page)
    {
    case 1://�ڵ�һҳ
        OLED_show_str(0, 0, "�����¶�����:");
        OLED_ShowNum(16 * 7, 0, tem_up, 2, 16);

        OLED_show_str(0, 16, "�����¶�����:");
        OLED_ShowNum(16 * 7, 16, tem_down, 2, 16);

        OLED_show_str(0, 32, "����ʪ������:");
        OLED_ShowNum(16 * 7, 32, soil_up, 2, 16);
        OLED_show_str(0, 48, "����");
        OLED_show_str(128 - 16 * 3, 48, "��һҳ");
        break;
    case 2://�ڵڶ�ҳ
        OLED_show_str(0, 0, "����ʪ������:");
        OLED_ShowNum(16 * 7, 0, humi_up, 2, 16);

        OLED_show_str(0, 16, "����ʪ������:");
        OLED_ShowNum(16 * 7, 16, humi_down, 2, 16);

        OLED_show_str(0, 32, "����ʪ������:");
        OLED_ShowNum(16 * 7, 32, soil_down, 2, 16);

        OLED_show_str(0, 48, "����");
        break;
    }
}
void data_screen(void)
{
    OLED_show_str(0, 0, "�����¶�:");
    OLED_ShowFloat(16 * 4 + 8, 0 + 0, tem, 1, 16);
    OLED_show_str(16 * 6 + 8, 0, "��");
    OLED_show_str(0, 16, "����ʪ��:");
    OLED_ShowFloat(16 * 4 + 8, 0 + 16, humi, 1, 16);
    OLED_show_str(16 * 6 + 8, 16, "%RH");
    OLED_show_str(0, 32, "����ʪ��:");
    OLED_ShowNum(16 * 4 + 8, 0 + 32, soil, 2, 16);
    OLED_show_str(16 * 5 + 8, 32, "%RH");
    OLED_show_str(0, 48, "����");
}
void choose_screen(void) //ѡ����Ļ
{
    switch (screen_choose)
    {
    case 0:
        OLED_show_str (0, 16, "->");
        OLED_show_str (0, 32, "  ");
        OLED_show_str (0, 48, "  ");
        break;
    case 1:
        OLED_show_str (0, 16, "  ");
        OLED_show_str (0, 32, "->");
        OLED_show_str (0, 48, "  ");
        break;
    case 2:
        OLED_show_str (0, 16, "  ");
        OLED_show_str (0, 32, "  ");
        OLED_show_str (0, 48, "->");
        break;
    }
}
void choose_admin(void)
{
    switch(admin_choose)
    {
    case 0:
        OLED_show_str_turn(0, 0, "�����¶�����:");
        break;
    case 1:
        OLED_show_str_turn(0, 16, "�����¶�����:");
        break;
    case 2:
        OLED_show_str_turn(0, 32, "����ʪ������:");
        break;
    case 3:
        OLED_show_str_turn(0, 48, "����");
        break;
    case 4:
        OLED_show_str_turn(128 - 16 * 3, 48, "��һҳ");
        break;
    /*-------------------������---------------------------------------*/
    case 5:
        OLED_show_str_turn(0, 0, "����ʪ������:");
        break;
    case 6:
        OLED_show_str_turn(0, 16, "����ʪ������:");
        break;
    case 7:
        OLED_show_str_turn(0, 32, "����ʪ������:");
        break;
    case 8:
        OLED_show_str_turn(0, 48, "����");
        break;
    }
}
void pwr_choose_screen(void)//����ѡ�����
{
    static	u8 pwr_screen_page_sta;
    if(pwr_screen_page_sta != pwr_screen_page) ////��ֵ�����ı�--���ڼ��ֵ�Ƿ����ı�
    {
        pwr_screen_page_sta = pwr_screen_page; //���浱ǰֵ
        OLED_Clear();//����
    }
    switch(pwr_screen_page)
    {
    case 0:
        OLED_show_str(0, 0, "ˮ��:");
        if(water_pwm)//����1 ���ʾ��
            OLED_show_str(16 * 2 + 8, 0, "��");
        else 	OLED_show_str(16 * 2 + 8, 0, "��");
        OLED_show_str(16 * 4 + 8, 0, "�ٶ�:");
        OLED_ShowNum(16 * 7, 0, water_pwm, 2, 16);

        OLED_show_str(0, 16, "����:");
        if(Dc_motor_pwm)//����1 ���ʾ��
            OLED_show_str(16 * 2 + 8, 16, "��");
        else 	OLED_show_str(16 * 2 + 8, 16, "��");
        OLED_show_str(16 * 4 + 8, 16, "�ٶ�:");
        OLED_ShowNum(16 * 7, 16, Dc_motor_pwm, 2, 16);


        OLED_show_str(0, 48, "����");
        break;
    }
    OLED_show_str(0, 48, "����");
}
void choose_pwr(void)
{
    switch(pwr_choose)
    {
    case 0:
        OLED_show_str_turn(0, 0, "ˮ��:");
        break;
    case 1:
        OLED_show_str_turn(0, 16, "����:");
        break;
    case 2:
        OLED_show_str_turn(0, 48, "����");
        break;
    }
}
extern u8 APP_alarm;
void alarm_choose(void)
{
    if(ctrl_data() == 1)
    {
        if(choose_fan_admin == 1)
        {
            OLED_show_str_turn(0, 32, "�����¶�����:");
        }
        else
            OLED_show_str_turn(16 * 4 + 8, 48, "�ٶ�:");
    }
    else   if(ctrl_data() == 6)
    {
        if(choose_fan_admin == 1)
        {
            OLED_show_str_turn(0, 32, "����ʪ�ȹ���:");
        }
        else
            OLED_show_str_turn(16 * 4 + 8, 48, "�ٶ�:");
    }
}
u8 tem_APP_alarm = 1, soil_APP_alarm = 1, hum_APP_alarm = 1; //���������ϱ�app�����ı�־λ
void beep_screen(void) //������ʾ��Ļ
{
    static u8 alarm_numsta = 0; //���������һ�εĺ���
    u8 alarm_num = 0; //�������
    alarm_num = ctrl_data();
    /*
    alarm_num=1 �¶ȴ�������
    alarm_num=2 �¶�С������

    alarm_num=3 ʪ�ȴ�������
    alarm_num=4 ʪ��С������


    alarm_num=5 ����ʪ�ȴ�������
    alarm_num=6 ����ʪ��С������
    */

    if(APP_alarm)
    {
        if(alarm_numsta != alarm_num) //������ŷ����仯������������ʾ
        {
            alarm_numsta = alarm_num;
            OLED_Clear();//����
        }
        switch(alarm_num)
        {
        case 1:
            OLED_show_str_turn(0, 0, "�����¶ȹ���");
            OLED_show_str(0, 16, "�����¶�:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, tem, 2, 16);
            OLED_show_str(0, 32, "�����¶�����:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, tem_up, 2, 16);
            OLED_show_str(16 * 4 + 8, 48, "�ٶ�:");
            alarm_choose();

            OLED_ShowNum(16 * 7, 48, Dc_motor_pwm, 2, 16);
            if(tem_APP_alarm)
            {
                tem_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_tem_alarm"", 1);//��app����������Ϣ
            }
            break;
        case 2:
            OLED_show_str_turn(0, 0, "�����¶ȹ���");
            OLED_show_str(0, 16, "�����¶�:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, tem, 2, 16);
            OLED_show_str(0, 32, "�����¶�����:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, tem_down, 2, 16);
            if(tem_APP_alarm)
            {
                tem_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_tem_alarm"", 0);//��app����������Ϣ
            }
            break;
        case 3:
            OLED_show_str_turn(0, 0, "����ʪ�ȹ���");
            OLED_show_str(0, 16, "����ʪ��:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, humi, 2, 16);
            OLED_show_str(0, 32, "����ʪ������:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, humi_up, 2, 16);
            if(hum_APP_alarm)
            {
                hum_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_hum_alarm"", 1);//��app����������Ϣ
            }
            break;
        case 4:
            OLED_show_str_turn(0, 0, "����ʪ�ȹ���");
            OLED_show_str(0, 16, "����ʪ��:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, humi, 2, 16);
            OLED_show_str(0, 32, "����ʪ������:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, humi_down, 2, 16);
            if(hum_APP_alarm)
            {
                hum_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_hum_alarm"", 0);//��app����������Ϣ
            }
            break;
        case 5:
            OLED_show_str_turn(0, 0, "����ʪ�ȹ���");
            OLED_show_str(0, 16, "����ʪ��:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, soil, 2, 16);
            OLED_show_str(0, 32, "����ʪ������:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, soil_up, 2, 16);
            if(soil_APP_alarm)
            {
                soil_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_soil_alarm"", 1);//��app����������Ϣ
            }
            break;
        case 6:
            OLED_show_str_turn(0, 0, "����ʪ�ȹ���");
            OLED_show_str(0, 16, "����ʪ��:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, soil, 2, 16);
            OLED_show_str(0, 32, "����ʪ������:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, soil_down, 2, 16);
				            OLED_show_str(16 * 4 + 8, 48, "�ٶ�:");
            alarm_choose();
				            OLED_ShowNum(16 * 7, 48, water_pwm, 2, 16);
            if(soil_APP_alarm)
            {
                soil_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_soil_alarm"", 0);//��app����������Ϣ
            }
            break;
        }
    }
    else
    {
        tem_APP_alarm = 1, soil_APP_alarm = 1, hum_APP_alarm = 1;
        screen_num = 0;
    }
}
