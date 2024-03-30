#include "ctrl.h"
u8 APP_alarm;
u8 one_ctrl[2] = "12"; //用于限制动作，结果：报警后只产生一次动作
/*
alarm_num=1 温度大于上限
alarm_num=2 温度小于下限

alarm_num=3 湿度大于上限
alarm_num=4 湿度小于上限

alarm_num=5 土壤湿度大于上限
alarm_num=6 土壤湿度小于下限

*/
#define LOG  0
u8 ctrl_data(void)
{
    u8 alarm_num = 0;
    if(tem > tem_up) //温度大于上限
    {
#if LOG
        u1_printf( "温度大于上限\r\n");
#endif

        alarm_num = 1; //报警编号
        if(auto_flag && one_ctrl[0]) //自动模式并且第一次进入  
        {
            Dc_motor_pwm = 50;     
            one_ctrl[0] = 0; //清零，下一次进来则需要重新置一后才能进来
        }
    }
    else if(tem < tem_down) //温度小于下限
    {
#if LOG
        u1_printf( "温度小于下限\r\n");
#endif
        alarm_num = 2; //报警编号
        if(auto_flag && one_ctrl[0]) //自动模式
        {
            Dc_motor_pwm = 0;
            one_ctrl[0] = 0; //清零，下一次进来则需要重新置一后才能进来
        }
    }
    else if(tem > tem_down && tem < tem_up) //温度正常
    {
#if LOG
        u1_printf( "温度正常\r\n");
#endif
        one_ctrl[0] = 1;
        if(auto_flag)//自动模式
            Dc_motor_pwm = 0;
    }

    if(humi > humi_up) //湿度大于上限
    {
#if LOG
        u1_printf( "湿度大于上限\r\n");
#endif
        alarm_num = 3; //报警编号
    }
    else if(humi < humi_down) //湿度小于下限
    {
#if LOG
        u1_printf( "湿度小于下限\r\n");
#endif
        alarm_num = 4; //报警编号
    }
    if(soil > soil_up) //土壤湿度大于上限
    {
#if LOG
        u1_printf( "土壤湿度大于上限\r\n");
#endif
        alarm_num = 5; //报警编号
        if(auto_flag && one_ctrl[1]) //自动模式
        {
            water_pwm = 0;//关闭水泵
            one_ctrl[1] = 0; //清零，下一次进来则需要重新置一后才能进来
        }
    }
    else if(soil < soil_down) //土壤湿度小于下限
    {
#if LOG
        u1_printf( "土壤湿度小于下限\r\n");
#endif
        alarm_num = 6; //报警编号
        if(auto_flag && one_ctrl[1]) //自动模式
        {
            water_pwm = 50;//打开水泵
            one_ctrl[1] = 0; //清零，下一次进来则需要重新置一后才能进来
        }
    }
    else if(soil >=soil_down && soil < soil_up) //土壤湿度正常
    {
#if LOG
        u1_printf( "土壤湿度正常\r\n");
#endif
        one_ctrl[1] = 1;
        if(auto_flag)//自动模式
            water_pwm = 0;//关闭水泵
    }
    if(alarm_num == 0) //报警编号为0表示没有报警
    {
#if LOG
        u1_printf( "正常\r\n");
#endif
        APP_alarm = 0; //app报警标志位
        BEEP = 0; //停止报警
    }
		else
		{
        BEEP = !BEEP; 
        screen_num = 5; //报警界面
        APP_alarm = 1; //app报警
        Delay_Ms(100);
		}
    if(wifi_connect && SubcribePack_flag) //标志位置位则连接wifi和服务器
        On_off_state_send();//上传开关的数据
    return alarm_num;
}
void On_off_state_send(void)
{
    static u8  Dc_motor_pwm_sta,water_pwm_sta; //各个输出的上一次状态
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
