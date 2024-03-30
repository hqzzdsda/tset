#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
//IO��������
#define DHT11_IO_IN()  {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=8<<24;}
#define DHT11_IO_OUT() {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=3<<24;}
////IO��������											   
#define	DHT11_DQ_OUT PBout(6) //���ݶ˿�	PB12
#define	DHT11_DQ_IN  PBin(6)  //���ݶ˿�	PB12 

extern float tem,humi;
u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(float *humi,float *temp)    ;//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
#endif
