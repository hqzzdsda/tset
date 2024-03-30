#ifndef __SHOW_H
#define __SHOW_H
#include "sys.h"
#include "oled.h"
#include "key.h"
#include "led.h"
#include "pub_sub_message.h"
extern u8 soil;
extern float humi, tem;
extern u8  tem_up, tem_down; //存储温度上下限值
extern u8  soil_down, soil_up; //存储土壤湿度上下限值
extern u8  humi_up, humi_down; //存储湿度上下限值
extern u8 auto_flag;
extern u8 Dc_motor_pwm, water_pwm;
extern u8 admin_choose, pwr_choose;
extern u8 wifi_connect;
extern u8 date_screen_page, admin_screen_page, pwr_screen_page; //跳转下一页的标志位
extern u8  screen_choose, screen_num;
extern u8 APP_alarm;//app报警
extern u8 choose_fan_admin;//在报警界面 选择调节风扇还是限值
void pwr_choose_screen(void);//开关选择界面

void Cut_screen(void);
void admin_screen(void);//光照限值设置界面
void data_screen(void);
void beep_screen(void); //报警显示屏幕
void main_screen(void);//主界面
void pwr_choose_screen(void);//开关选择界面
void choose_screen(void); //选择屏幕
void choose_parameters(void); //选择需要设置的参数
void choose_admin(void);//选择修改的限值
void choose_pwr(void);
void alarm_choose(void);
#endif
