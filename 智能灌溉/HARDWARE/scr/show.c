#include "show.h"
extern char SubcribePack_flag;   //订阅报文状态      1：订阅报文成功
u8 screen_num = 0; //屏幕号
char screen_num_sta = 0; //屏幕号状态
u8 date_screen_page = 1, admin_screen_page = 1,pwr_screen_page; //跳转下一页的标志位
unsigned char screen_choose, admin_choose,pwr_choose,choose_fan_admin; //选择;
/*-----------------数据上下限--------------------------*/
u8 tem_up = 50, humi_up = 80;
u8 tem_down = 10, humi_down = 10;
u8 soil_down = 0, soil_up = 50;
/*------------------数据上下限-------------------------*/
u8 soil;
u8 wifi_connect;
u8 auto_flag;
u8 Dc_motor_pwm=0, water_pwm;
/*********************************************************************************************************
* 函 数 名 : void	Cut_screen()
* 功能说明 : 屏幕切屏
* 形    参 : 无
* 返 回 值 : 无
* 备    注 : 2023-03-04
*********************************************************************************************************/
void	Cut_screen(void)
{
    OLED_Refresh();//更新显存--当显示的数据发生改变时需更新
		
    if(screen_num != screen_num_sta) ////当值发生改变--用于检测值是否发生改变
    {
        screen_num_sta = screen_num; //保存当前值
        OLED_Clear();//清屏
    }

    switch (screen_num)
    {
    case 0:
        if(auto_flag)//自动
            OLED_show_str(120, 48, "A");
        else OLED_show_str(120, 48, "w");
        if( wifi_connect && SubcribePack_flag) //标志位置位则连接wifi和服务器
            OLED_show_str (98, 32, "WIFI");//连接服务器模式
        else OLED_show_str (98, 32, "           "); //不连接模式
        main_screen();//主屏幕
        choose_screen();
        break;//选择屏幕
    case 1:
        data_screen();//显示数据界面
        OLED_show_str_turn(0, 48, "返回");
        break;
    case 2:
        admin_screen();//设置上限的界面
        choose_admin();
        break;
		case 3:
        pwr_choose_screen();//开关控制
        choose_pwr();
        break;
    case 5:
        beep_screen();//报警界面
        break;
    }
}
void main_screen(void)//主界面
{
    OLED_show_str(20, 0, "智能灌溉");
    OLED_show_str(16, 16, "查看数据");
    OLED_show_str(16, 32, "阈值修改");
    OLED_show_str(16, 48, "开关控制");
}

void admin_screen(void)
{
    static	u8 admin_screen_page_sta;

    if(admin_screen_page_sta != admin_screen_page) ////当值发生改变--用于检测值是否发生改变
    {
        admin_screen_page_sta = admin_screen_page; //保存当前值
        OLED_Clear();//清屏
    }
    switch(admin_screen_page)
    {
    case 1://在第一页
        OLED_show_str(0, 0, "空气温度上限:");
        OLED_ShowNum(16 * 7, 0, tem_up, 2, 16);

        OLED_show_str(0, 16, "空气温度下限:");
        OLED_ShowNum(16 * 7, 16, tem_down, 2, 16);

        OLED_show_str(0, 32, "土壤湿度上限:");
        OLED_ShowNum(16 * 7, 32, soil_up, 2, 16);
        OLED_show_str(0, 48, "返回");
        OLED_show_str(128 - 16 * 3, 48, "下一页");
        break;
    case 2://在第二页
        OLED_show_str(0, 0, "空气湿度上限:");
        OLED_ShowNum(16 * 7, 0, humi_up, 2, 16);

        OLED_show_str(0, 16, "空气湿度下限:");
        OLED_ShowNum(16 * 7, 16, humi_down, 2, 16);

        OLED_show_str(0, 32, "土壤湿度下限:");
        OLED_ShowNum(16 * 7, 32, soil_down, 2, 16);

        OLED_show_str(0, 48, "返回");
        break;
    }
}
void data_screen(void)
{
    OLED_show_str(0, 0, "空气温度:");
    OLED_ShowFloat(16 * 4 + 8, 0 + 0, tem, 1, 16);
    OLED_show_str(16 * 6 + 8, 0, "℃");
    OLED_show_str(0, 16, "空气湿度:");
    OLED_ShowFloat(16 * 4 + 8, 0 + 16, humi, 1, 16);
    OLED_show_str(16 * 6 + 8, 16, "%RH");
    OLED_show_str(0, 32, "土壤湿度:");
    OLED_ShowNum(16 * 4 + 8, 0 + 32, soil, 2, 16);
    OLED_show_str(16 * 5 + 8, 32, "%RH");
    OLED_show_str(0, 48, "返回");
}
void choose_screen(void) //选择屏幕
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
        OLED_show_str_turn(0, 0, "空气温度上限:");
        break;
    case 1:
        OLED_show_str_turn(0, 16, "空气温度下限:");
        break;
    case 2:
        OLED_show_str_turn(0, 32, "土壤湿度上限:");
        break;
    case 3:
        OLED_show_str_turn(0, 48, "返回");
        break;
    case 4:
        OLED_show_str_turn(128 - 16 * 3, 48, "下一页");
        break;
    /*-------------------二界面---------------------------------------*/
    case 5:
        OLED_show_str_turn(0, 0, "空气湿度上限:");
        break;
    case 6:
        OLED_show_str_turn(0, 16, "空气湿度下限:");
        break;
    case 7:
        OLED_show_str_turn(0, 32, "土壤湿度下限:");
        break;
    case 8:
        OLED_show_str_turn(0, 48, "返回");
        break;
    }
}
void pwr_choose_screen(void)//开关选择界面
{
    static	u8 pwr_screen_page_sta;
    if(pwr_screen_page_sta != pwr_screen_page) ////当值发生改变--用于检测值是否发生改变
    {
        pwr_screen_page_sta = pwr_screen_page; //保存当前值
        OLED_Clear();//清屏
    }
    switch(pwr_screen_page)
    {
    case 0:
        OLED_show_str(0, 0, "水泵:");
        if(water_pwm)//大于1 则表示打开
            OLED_show_str(16 * 2 + 8, 0, "开");
        else 	OLED_show_str(16 * 2 + 8, 0, "关");
        OLED_show_str(16 * 4 + 8, 0, "速度:");
        OLED_ShowNum(16 * 7, 0, water_pwm, 2, 16);

        OLED_show_str(0, 16, "风扇:");
        if(Dc_motor_pwm)//大于1 则表示打开
            OLED_show_str(16 * 2 + 8, 16, "开");
        else 	OLED_show_str(16 * 2 + 8, 16, "关");
        OLED_show_str(16 * 4 + 8, 16, "速度:");
        OLED_ShowNum(16 * 7, 16, Dc_motor_pwm, 2, 16);


        OLED_show_str(0, 48, "返回");
        break;
    }
    OLED_show_str(0, 48, "返回");
}
void choose_pwr(void)
{
    switch(pwr_choose)
    {
    case 0:
        OLED_show_str_turn(0, 0, "水泵:");
        break;
    case 1:
        OLED_show_str_turn(0, 16, "风扇:");
        break;
    case 2:
        OLED_show_str_turn(0, 48, "返回");
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
            OLED_show_str_turn(0, 32, "空气温度上限:");
        }
        else
            OLED_show_str_turn(16 * 4 + 8, 48, "速度:");
    }
    else   if(ctrl_data() == 6)
    {
        if(choose_fan_admin == 1)
        {
            OLED_show_str_turn(0, 32, "土壤湿度过低:");
        }
        else
            OLED_show_str_turn(16 * 4 + 8, 48, "速度:");
    }
}
u8 tem_APP_alarm = 1, soil_APP_alarm = 1, hum_APP_alarm = 1; //各个数据上报app报警的标志位
void beep_screen(void) //报警显示屏幕
{
    static u8 alarm_numsta = 0; //报警编号上一次的号码
    u8 alarm_num = 0; //报警编号
    alarm_num = ctrl_data();
    /*
    alarm_num=1 温度大于上限
    alarm_num=2 温度小于下限

    alarm_num=3 湿度大于上限
    alarm_num=4 湿度小于上限


    alarm_num=5 土壤湿度大于上限
    alarm_num=6 土壤湿度小于下限
    */

    if(APP_alarm)
    {
        if(alarm_numsta != alarm_num) //报警编号发生变化则清屏重新显示
        {
            alarm_numsta = alarm_num;
            OLED_Clear();//清屏
        }
        switch(alarm_num)
        {
        case 1:
            OLED_show_str_turn(0, 0, "空气温度过高");
            OLED_show_str(0, 16, "空气温度:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, tem, 2, 16);
            OLED_show_str(0, 32, "空气温度上限:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, tem_up, 2, 16);
            OLED_show_str(16 * 4 + 8, 48, "速度:");
            alarm_choose();

            OLED_ShowNum(16 * 7, 48, Dc_motor_pwm, 2, 16);
            if(tem_APP_alarm)
            {
                tem_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_tem_alarm"", 1);//给app发布报警信息
            }
            break;
        case 2:
            OLED_show_str_turn(0, 0, "空气温度过低");
            OLED_show_str(0, 16, "空气温度:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, tem, 2, 16);
            OLED_show_str(0, 32, "空气温度下限:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, tem_down, 2, 16);
            if(tem_APP_alarm)
            {
                tem_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_tem_alarm"", 0);//给app发布报警信息
            }
            break;
        case 3:
            OLED_show_str_turn(0, 0, "空气湿度过高");
            OLED_show_str(0, 16, "空气湿度:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, humi, 2, 16);
            OLED_show_str(0, 32, "空气湿度上限:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, humi_up, 2, 16);
            if(hum_APP_alarm)
            {
                hum_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_hum_alarm"", 1);//给app发布报警信息
            }
            break;
        case 4:
            OLED_show_str_turn(0, 0, "空气湿度过低");
            OLED_show_str(0, 16, "空气湿度:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, humi, 2, 16);
            OLED_show_str(0, 32, "空气湿度下限:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, humi_down, 2, 16);
            if(hum_APP_alarm)
            {
                hum_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_hum_alarm"", 0);//给app发布报警信息
            }
            break;
        case 5:
            OLED_show_str_turn(0, 0, "土壤湿度过高");
            OLED_show_str(0, 16, "土壤湿度:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, soil, 2, 16);
            OLED_show_str(0, 32, "土壤湿度上限:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, soil_up, 2, 16);
            if(soil_APP_alarm)
            {
                soil_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_soil_alarm"", 1);//给app发布报警信息
            }
            break;
        case 6:
            OLED_show_str_turn(0, 0, "土壤湿度过低");
            OLED_show_str(0, 16, "土壤湿度:");
            OLED_ShowNum(16 * 4 + 8, 0 + 16, soil, 2, 16);
            OLED_show_str(0, 32, "土壤湿度下限:");
            OLED_ShowNum(16 * 6 + 8, 0 + 32, soil_down, 2, 16);
				            OLED_show_str(16 * 4 + 8, 48, "速度:");
            alarm_choose();
				            OLED_ShowNum(16 * 7, 48, water_pwm, 2, 16);
            if(soil_APP_alarm)
            {
                soil_APP_alarm = 0;
                Delay_Ms(500);
                data_sta(""APP_soil_alarm"", 0);//给app发布报警信息
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
