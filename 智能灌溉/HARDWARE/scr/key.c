#include "key.h"
#include "stm32f10x_exti.h"
/*-------------------------------------------------*/
/*函数名：初始化KEY按键函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                       //定义一个设置IO的结构体

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);     //使能PA端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//使能PORTB口时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //关闭JTAG，SWD可以调试的
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;           //准备设置PB5,PB6,PB7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   		   //下拉输入，默认状态是低电平
    GPIO_Init(GPIOB, &GPIO_InitStructure);            		   //设置PA0
}

/*-------------------------------------------------*/
/*函数名：按键外部中断初始化程序.                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void KEY_Exti_Init(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure; //定义一个设置IO的结构体
    EXTI_InitTypeDef   EXTI_InitStructure; //定义一个设置中断的结构体

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	         //使能GPIO复用功能时钟
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);   //配置KEY1-PB55中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);   //配置KEY1-PB55中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);   //配置KEY2-PB6中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);   //配置KEY3-PB7中断线

    EXTI_InitStructure.EXTI_Line = EXTI_Line3;	                  //准备设置外部5中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
    EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断5线

    EXTI_InitStructure.EXTI_Line = EXTI_Line4;	                  //准备设置外部5中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
    EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断5线

    EXTI_InitStructure.EXTI_Line = EXTI_Line5;	                  //准备设置外部6中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
    EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断6线

    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
    EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断7线

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			  //准备设置外部中断5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		      //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //使能外部中断1通道
    NVIC_Init(&NVIC_InitStructure);                               //设置外部中断4

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			  //准备设置外部中断5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		      //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //使能外部中断1通道
    NVIC_Init(&NVIC_InitStructure);                               //设置外部中断4

    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			  //准备设置外部中断5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		      //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //使能外部中断1通道
    NVIC_Init(&NVIC_InitStructure);                               //设置外部中断4
}
void EXTI3_IRQHandler(void)//选择按键
{
    Delay_Ms(10);//消抖
    if(!KEY0_IN_STA)
    {
        switch(screen_num)
        {
        case 0://主界面
            screen_choose++;
            screen_choose %= 3;
            break;
        case 2://限值选择界面
            switch(admin_screen_page)
            {
            case 1://第一个界面
                if(admin_choose < 4)
                    admin_choose++;
                else admin_choose = 0;
                break;
            case 2://第二个界面
                if(admin_choose < 8)
                    admin_choose++;
                else admin_choose = 5;
                break;
            }
            break;
        case 3://开关控制界面
            pwr_choose++;
            pwr_choose %= 3;
            break;
        case 5://报警界面
            choose_fan_admin = !choose_fan_admin;
            break;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE5上的中断标志位
}
u8 DC_MOTOR_flag, water_pwm_flag;
//外部中断4服务程序
void EXTI4_IRQHandler(void)//确认按键
{
    Delay_Ms(10);//消抖
    if(!KEY1_IN_STA)
    {
        switch(screen_num)
        {
        case 0://在主界面
            if(screen_choose == 0)screen_num = 1;//0表示箭头指向 查看数据 界面
            else 	if(screen_choose == 1)screen_num = 2;//1表示箭头指向 限值修改界面
            else 	if(screen_choose == 2)screen_num = 3;//2表示箭头指向 开关控制界面
            break;
        case 2://在限值修改界面
            switch(admin_screen_page)
            {
            case 1 ://限值第一界面
                if(admin_choose == 3)
                {
                    screen_num = 0;//选中返回 则返回主界面
                    admin_choose = 0;
                }
                else 	if(admin_choose == 4)
                {
                    admin_screen_page = 2;//选择下一页 则进入下一页
                    admin_choose = 5;
                }
                break;
            case 2://限值第二界面
                if(admin_choose == 8)
                {
                    admin_screen_page = 1;//选中返回 则返回第一界面
                    admin_choose = 0;
                }

                break;
                break;
            }
            break;
        case 1://再数据显示界面
            screen_num = 0;//选中返回 则返回主界面
            break;
        case 3://开关控制界面
            switch(pwr_choose)
            {
            case 0:
                water_pwm_flag = !water_pwm_flag;
                if(water_pwm_flag)
                {
                    water_pwm = 50;
                }
                else water_pwm = 0;
                data_sta(""APP_WATER"", water_pwm);//上报该数据到服务器
                break;
            case 1:
                DC_MOTOR_flag = !DC_MOTOR_flag;
                if(DC_MOTOR_flag)
                {
                    Dc_motor_pwm = 50;                    data_sta(""APP_FAN"", Dc_motor_pwm);//上报该数据到服务器
                }
                else 
								{
Dc_motor_pwm = 0;                    data_sta(""APP_FAN"", Dc_motor_pwm);//上报该数据到服务器
								}
                data_sta(""APP_FAN"", Dc_motor_pwm);//上报该数据到服务器
                break;
            case 2:
                screen_num = 0;//选中返回 则返回主界面
                break;
            }
            break;
        }

    }
    EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE5上的中断标志位
}

//外部中断5-6-7服务程序
void EXTI9_5_IRQHandler(void)  // ++ --
{
    Delay_Ms(10);//消抖

    if(KEY2_IN_STA == 0) //消除抖动的影响后，再次判断控制KEY1的IO输入电平，是否是低电平，如果是进入if，说明按键按下了
    {
        if(screen_num == 3 && pwr_choose == 1)   //在开关控制界面选择到风扇
        {
            if(Dc_motor_pwm < 99)
                Dc_motor_pwm++;                    data_sta(""APP_FAN"", Dc_motor_pwm);//上报该数据到服务器
        }
        if(screen_num == 3 && pwr_choose == 0)//在开关控制界面选择到水泵
        {
            if(water_pwm < 99)
                water_pwm++;  data_sta(""APP_WATER"", water_pwm);//上报该数据到服务器
        }
        if(screen_num == 0)  //在主界面切换模式
        {
            auto_flag = !auto_flag;   data_sta(""APP_auto_flag"", auto_flag);
        }
        if(screen_num == 2)
            switch(admin_choose)
            {
            case 0:
                tem_up++;
                data_sta(""APP_tem_up"", tem_up);//上报该数据到服务器
                break;
            case 1:
                tem_down++;
                data_sta(""APP_tem_down"", tem_down);
                break;
            case 5:
                humi_up++;
                data_sta(""APP_humi_up"", humi_up);//上报该数据到服务器
                break;
            case 6:
                humi_down++;
                data_sta(""APP_humi_down"", humi_down);//上报该数据到服务器
                break;
            case 7:
                soil_down++;
                data_sta(""APP_soil_down"", soil_down);//上报该数据到服务器
                break;
            case 2:
                soil_up++;
                data_sta(""APP_soil_up"", soil_up);//上报该数据到服务器
                break;

            }
        if(screen_num == 5)//在报警界面
        {
					
            switch(ctrl_data())
            {
            case 1:
                if(choose_fan_admin == 1)
                {
                    tem_up++;
                    data_sta(""APP_tem_up"", tem_up);//上报该数据到服务器
                }
                else
                {
                    Dc_motor_pwm++;
                    data_sta(""APP_FAN"", Dc_motor_pwm);//上报该数据到服务器
                }
                break;
            case 2:
                tem_down++;
                data_sta(""APP_tem_down"", tem_down);//上报该数据到服务器
                break;
            case 3:
                humi_up++;
                data_sta(""APP_humi_up"", humi_up);//上报该数据到服务器
                break;
            case 4:
                humi_down++;
                data_sta(""APP_humi_down"", humi_down);//上报该数据到服务器
                break;
            case 6:
                if(choose_fan_admin == 1)
                {
                    if(soil_down <99)
                        soil_down++;
                        data_sta(""APP_soil_down"", soil_down);//上报该数据到服务器
                }
                else
                {
									if(water_pwm <99)
											water_pwm++;			
                        data_sta(""APP_WATER"", water_pwm);//上报该数据到服务器
                }
                break;
            case 5:
                soil_up++;
                data_sta(""APP_soil_up"", soil_up);//上报该数据到服务器
                break;

            }
        }

    }
    if(KEY3_IN_STA == 0) //消除抖动的影响后，再次判断控制 KEY2 的IO输入电平，是否是低电平，如果是进入if，说明按键按下了
    {
        if(screen_num == 3 && pwr_choose == 1)//在开关控制界面选择到风扇
        {
            if(Dc_motor_pwm != 0)
                Dc_motor_pwm--;     data_sta(""APP_FAN"", Dc_motor_pwm);//上报该数据到服务器
        }
        if(screen_num == 3 && pwr_choose == 0)//在开关控制界面选择到水泵
        {
            if(water_pwm != 0)
                water_pwm--;  data_sta(""APP_WATER"", water_pwm);//上报该数据到服务器
        }
        if(screen_num == 0)//主屏幕
        {
            wifi_connect = !wifi_connect;//断开或连接wifi
        }

        if(screen_num == 2)//在限值调节界面
            switch(admin_choose)
            {
            case 0:
                if(tem_up != 0)
                    tem_up--;
                data_sta(""APP_tem_up"", tem_up);//上报该数据到服务器
                break;
            case 1:
                if(tem_down != 0)
                    tem_down--;
                data_sta(""APP_tem_down"", tem_down);
                break;
            case 5:
                if(humi_up != 0)
                    humi_up--;
                data_sta(""APP_humi_up"", humi_up);//上报该数据到服务器
                break;
            case 6:
                if(humi_down != 0)
                    humi_down--;
                data_sta(""APP_humi_down"", humi_down);//上报该数据到服务器
                break;
            case 7:
                if(soil_down != 0)
                    soil_down--;
                data_sta(""APP_soil_down"", soil_down);//上报该数据到服务器
                break;
            case 2:
                if(soil_up != 0)
                    soil_up--;
                data_sta(""APP_soil_up"", soil_up);//上报该数据到服务器
                break;


            }

        if(screen_num == 5)//在报警界面
        {
            switch(ctrl_data())
            {
            case 1:
                if(choose_fan_admin == 1)
                {
                    if(tem_up != 0)
                        tem_up--;

                    data_sta(""APP_tem_up"", tem_up);//上报该数据到服务器
                }
                else
                {
                    if(Dc_motor_pwm != 0)
                        Dc_motor_pwm--;

                    data_sta(""APP_FAN"", Dc_motor_pwm);//上报该数据到服务器
                }
                break;
            case 2:
                if(tem_down != 0)
                    tem_down--;

                data_sta(""APP_tem_down"", tem_down);//上报该数据到服务器
                break;
            case 3:
                if(humi_up != 0)
                    humi_up--;

                data_sta(""APP_humi_up"", humi_up);//上报该数据到服务器
                break;
            case 4:
                if(humi_down != 0)
                    humi_down--;

                data_sta(""APP_humi_down"", humi_down);//上报该数据到服务器
                break;

            case 6:
                if(choose_fan_admin == 1)
                {
                    if(soil_down != 0)
                        soil_down--;
                        data_sta(""APP_soil_down"", soil_down);//上报该数据到服务器
                }
                else
                {
									if(water_pwm != 0)
											water_pwm--;			
                        data_sta(""APP_WATER"", water_pwm);//上报该数据到服务器
                }
                break;
            case 5:
                if(soil_up != 0)
                    soil_up--;
                data_sta(""APP_soil_up"", soil_up);//上报该数据到服务器
                break;

            }
        }

    }
    EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE5上的中断标志位
    EXTI_ClearITPendingBit(EXTI_Line7);  //清除LINE7上的中断标志位
}

