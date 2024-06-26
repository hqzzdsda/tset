#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
//IO方向设置
#define DHT11_IO_IN()  {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=8<<24;}
#define DHT11_IO_OUT() {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=3<<24;}
////IO操作函数											   
#define	DHT11_DQ_OUT PBout(6) //数据端口	PB12
#define	DHT11_DQ_IN  PBin(6)  //数据端口	PB12 

extern float tem,humi;
u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(float *humi,float *temp)    ;//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    
#endif
