#ifndef __SHOW_H
#define __SHOW_H
#include "sys.h"
#include "oled.h"
#include "key.h"
#include "led.h"
#include "pub_sub_message.h"
extern u8 soil;
extern float humi, tem;
extern u8  tem_up, tem_down; //�洢�¶�������ֵ
extern u8  soil_down, soil_up; //�洢����ʪ��������ֵ
extern u8  humi_up, humi_down; //�洢ʪ��������ֵ
extern u8 auto_flag;
extern u8 Dc_motor_pwm, water_pwm;
extern u8 admin_choose, pwr_choose;
extern u8 wifi_connect;
extern u8 date_screen_page, admin_screen_page, pwr_screen_page; //��ת��һҳ�ı�־λ
extern u8  screen_choose, screen_num;
extern u8 APP_alarm;//app����
extern u8 choose_fan_admin;//�ڱ������� ѡ����ڷ��Ȼ�����ֵ
void pwr_choose_screen(void);//����ѡ�����

void Cut_screen(void);
void admin_screen(void);//������ֵ���ý���
void data_screen(void);
void beep_screen(void); //������ʾ��Ļ
void main_screen(void);//������
void pwr_choose_screen(void);//����ѡ�����
void choose_screen(void); //ѡ����Ļ
void choose_parameters(void); //ѡ����Ҫ���õĲ���
void choose_admin(void);//ѡ���޸ĵ���ֵ
void choose_pwr(void);
void alarm_choose(void);
#endif
