#ifndef __TOTAL_WIFI_H
#define __TOTAL_WIFI_H
#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "main.h"       //������Ҫ��ͷ�ļ�
#include "delay.h"      //������Ҫ��ͷ�ļ�
#include "usart1.h"     //������Ҫ��ͷ�ļ�
#include "usart2.h"     //������Ҫ��ͷ�ļ�
#include "timer3_ping.h"     //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�
#include "key.h"        //������Ҫ��ͷ�ļ�
#include "string.h"       //������Ҫ��ͷ�ļ�
#include "stdio.h"        //������Ҫ��ͷ�ļ�
#include "utils_hmac.h"   //������Ҫ��ͷ�ļ�
#include "pub_sub_message.h"
void Connect_server(void);
void  publish_message(void);
extern unsigned int SystemTimer,SystemTimer2,SystemTimer3,SystemTimer4,SystemTimer5,SystemTimer6;   //����ȫ�ּ�ʱ�ı���           ��λ��
extern unsigned int TEHUTimer,TEHUTimer2,TEHUTimer3,TEHUTimer4,TEHUTimer5,TEHUTimer6 ;     //������ʪ�Ȳɼ��ļ�ʱ�ı���   ��λ��


/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/
/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/
/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/
#define  R_NUM               8     //���ջ���������
#define  RBUFF_UNIT          300   //���ջ���������

#define  T_NUM               8     //���ͻ���������  
#define  TBUFF_UNIT          300   //���ͻ���������

#define  C_NUM               8     //�����������
#define  CBUFF_UNIT          300   //�����������

#define SSID   "WIFIhot"                     //��ʼ����·����SSID����
#define PASS   "1234567890"                 //��ʼ����·��������
#define  MQTT_TxData(x)       u2_TxData(x)                                         //����2�������ݷ���



#define  PRODUCTKEY           "a1vDzGn2QXE"                                        //��ƷID
#define  PRODUCTKEY_LEN       strlen(PRODUCTKEY)                                   //��ƷID����

#define  DEVICENAME           "ABCDEFG"                                               //�豸��  
#define  DEVICENAME_LEN       strlen(DEVICENAME)                                   //�豸������

#define  DEVICESECRE          "2d08f59b4fafe813dcf4590419bff00a"                   //�豸��Կ   
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)                                  //�豸��Կ����

#define  S_TOPIC_NAME         "/sys/a1aCQosVcrx/ABCD/thing/service/property/set"   //��Ҫ���ĵ����� 

#define  P_TOPIC_NAME         "/sys/a1aCQosVcrx/ABCD/thing/event/property/post"    //��Ҫ����������  

extern unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];       //�ⲿ�������������ݵĽ��ջ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
extern unsigned char *MQTT_RxDataInPtr;                        //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_RxDataOutPtr;                       //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_RxDataEndPtr;                       //�ⲿ����������ָ�򻺳���������λ��
extern unsigned char  MQTT_TxDataBuf[T_NUM][TBUFF_UNIT];       //�ⲿ�������������ݵķ��ͻ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
extern unsigned char *MQTT_TxDataInPtr;                        //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_TxDataOutPtr;                       //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_TxDataEndPtr;                       //�ⲿ����������ָ�򻺳���������λ��
extern unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];          //�ⲿ�����������������ݵĽ��ջ�����
extern unsigned char *MQTT_CMDInPtr;                           //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_CMDOutPtr;                          //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_CMDEndPtr;                          //�ⲿ����������ָ�򻺳���������λ��

extern char ClientID[128];     //�ⲿ������������ſͻ���ID�Ļ�����
extern int  ClientID_len;      //�ⲿ������������ſͻ���ID�ĳ���
extern char Username[128];     //�ⲿ��������������û����Ļ�����
extern int  Username_len;	   //�ⲿ��������������û����ĳ���
extern char Passward[128];     //�ⲿ�����������������Ļ�����
extern int  Passward_len;	   //�ⲿ�����������������ĳ���
extern char ServerIP[128];     //�ⲿ������������ŷ�����IP��������
extern int  ServerPort;        //�ⲿ������������ŷ������Ķ˿ں�

extern char Ping_flag;         //�ⲿ����������ping����״̬      0������״̬���ȴ���ʱʱ�䵽������Ping����
                               //�ⲿ����������ping����״̬      1��Ping�����ѷ��ͣ����յ� �������ظ����ĵĺ� ��1��Ϊ0
extern char Connect_flag;      //�ⲿ����������ͬ����������״̬  0����û�����ӷ�����  1�������Ϸ�������
extern char ReConnect_flag;    //�ⲿ��������������������״̬    0�����ӻ�����  1�����ӶϿ�������
extern char ConnectPack_flag;  //�ⲿ����������CONNECT����״̬   1��CONNECT���ĳɹ�
extern char SubcribePack_flag; //�ⲿ�������������ı���״̬      1�����ı��ĳɹ�

void MQTT_Buff_Init(void);
void AliIoT_Parameter_Init(void);
void MQTT_ConectPack(void);
void MQTT_Subscribe(char *, int);
void MQTT_PingREQ(void);
void MQTT_PublishQs0(char *, char *, int);
void MQTT_DealPushdata_Qs0(unsigned char *);	
void TxDataBuf_Deal(unsigned char *, int);
void CMDBuf_Deal(unsigned char *, int);
/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/
/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/
/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/


/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)x)  //PA4����WiFi�ĸ�λ

#define WiFi_printf       u2_printf           //����2���� WiFi
#define WiFi_RxCounter    Usart2_RxCounter    //����2���� WiFi
#define WiFi_RX_BUF       Usart2_RxBuff       //����2���� WiFi
#define WiFi_RXBUFF_SIZE  USART2_RXBUFF_SIZE  //����2���� WiFi



void WiFi_ResetIO_Init(void);
char WiFi_SendCmd(char *cmd, int timeout);
char WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Smartconfig(int timeout);
char WiFi_WaitAP(int timeout);
char WiFi_Connect_IoTServer(void);
/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
#endif
