#ifndef __PUN_SUB_MESSAGE_H
#define __PUN_SUB_MESSAGE_H
#include "stm32f10x.h"  //包含需要的头文件
#include "main.h"       //包含需要的头文件
#include "delay.h"      //包含需要的头文件
#include "usart1.h"     //包含需要的头文件
#include "usart2.h"     //包含需要的头文件
#include "timer3_ping.h"     //包含需要的头文件
#include "led.h"        //包含需要的头文件
#include "key.h"        //包含需要的头文件
#include "total_wifi.h"
#include "dht11.h"
#include "show.h"
#include "ctrl.h"
#define APP_tem_up   "tem_up"//空气温度上限 
#define APP_tem_down   "tem_down"//空气温度下限
#define APP_humi_up   "humi_up"//空气湿度上限
#define APP_humi_down   "humi_down"//空气湿度下限
#define APP_soil_down   "soil_down"//土壤湿度下限
#define APP_soil_up   "soil_up"//土壤湿度上限
/*-  bool型值*/
#define APP_tem_alarm   "tem_alarm"//空气温度报警
#define APP_hum_alarm   "hum_alarm"//空气湿度报警
#define APP_soil_alarm  	"soil_alarm"//土壤湿度报警 bool型
//																					开 关
#define APP_FAN  	"FAN"//风扇
#define APP_WATER  	"WATER"//水泵
#define APP_tem  	"tem"//空气温度
#define APP_humi  	"humi"//空气湿度
#define APP_soil  	"soil"//土壤湿度
#define APP_auto_flag  	"auto_flag"//自动模式
void Data_State(void);//定时上传任务
void data_sta(void *str, int cmd);//发布开关的状态给服务器 
void data_init(void);
void sent_data(void);
u32 Extract_digit(u8 *str);
void data_sta_str(void *str, int cmd);
void judge_data_sta(void *str, uint16_t GPIO_Pin);//发布判断开关的状态给服务器  
#endif
