#ifndef __PUN_SUB_MESSAGE_H
#define __PUN_SUB_MESSAGE_H
#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "main.h"       //������Ҫ��ͷ�ļ�
#include "delay.h"      //������Ҫ��ͷ�ļ�
#include "usart1.h"     //������Ҫ��ͷ�ļ�
#include "usart2.h"     //������Ҫ��ͷ�ļ�
#include "timer3_ping.h"     //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�
#include "key.h"        //������Ҫ��ͷ�ļ�
#include "total_wifi.h"
#include "dht11.h"
#include "show.h"
#include "ctrl.h"
#define APP_tem_up   "tem_up"//�����¶����� 
#define APP_tem_down   "tem_down"//�����¶�����
#define APP_humi_up   "humi_up"//����ʪ������
#define APP_humi_down   "humi_down"//����ʪ������
#define APP_soil_down   "soil_down"//����ʪ������
#define APP_soil_up   "soil_up"//����ʪ������
/*-  bool��ֵ*/
#define APP_tem_alarm   "tem_alarm"//�����¶ȱ���
#define APP_hum_alarm   "hum_alarm"//����ʪ�ȱ���
#define APP_soil_alarm  	"soil_alarm"//����ʪ�ȱ��� bool��
//																					�� ��
#define APP_FAN  	"FAN"//����
#define APP_WATER  	"WATER"//ˮ��
#define APP_tem  	"tem"//�����¶�
#define APP_humi  	"humi"//����ʪ��
#define APP_soil  	"soil"//����ʪ��
#define APP_auto_flag  	"auto_flag"//�Զ�ģʽ
void Data_State(void);//��ʱ�ϴ�����
void data_sta(void *str, int cmd);//�������ص�״̬�������� 
void data_init(void);
void sent_data(void);
u32 Extract_digit(u8 *str);
void data_sta_str(void *str, int cmd);
void judge_data_sta(void *str, uint16_t GPIO_Pin);//�����жϿ��ص�״̬��������  
#endif
