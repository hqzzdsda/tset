#include "total_wifi.h"
#include "adc.h"
#include "bsp_time2_PWM.h" 
#include "pub_sub_message.h"
#include "dht11.h"
#include "led.h"        //包含需要的头文件
extern char SubcribePack_flag;   //订阅报文状态      1：订阅报文成功
unsigned int adc_trang = 0; //土壤adc  
int main(void) 
{		
	Delay_Init();                   //延时功能初始化 
  Usart1_Init(115200);              //串口1功能初始化，波特率9600
	LED_Init();	                    //LED初始化
	KEY_Init();                     //按键初始化	
	WiFi_ResetIO_Init();            //初始化WiFi的复位IO
	AliIoT_Parameter_Init();	    //初始化连接阿里云IoT平台MQTT服务器的参数	
	OLED_Init();//oled初始化
	KEY_Exti_Init();
	DHT11_Init();
	Adc_Init();//采集土壤湿度
  TIME2_PWM_Init_10MS();//定时器2pwm初始化 水泵风扇的pwm  还有定时上传数据的时基
	while(1)                      //主循环
	{		
		if(!wifi_connect)//断开wifi模式
		{
			SystemTimer=0;//将计时上传数据的变量清零
			Connect_flag = 0;
			USART_Cmd(USART2, DISABLE); //失能串口2  

			Cut_screen();//屏幕显示
			TIM_SetCompare2(TIM2, water_pwm); //水泵 
		/************土壤湿度采集***********************/
			adc_trang = Get_Adc_Average(6, 10); //ADC湿度获取
			soil = (int)((4096 - adc_trang) * 100.0 / (4096 - 1200)); //湿度转换
		/************土壤湿度采集***********************/	
			DHT11_Read_Data(&tem, &humi);                                        //读取温湿度
			ctrl_data();//自动控制
		}
		else//连接wifi模式
		{	
			Connect_server();//连接服务器的函数
			if(SubcribePack_flag)//订阅完主题后才进来
			{		
			Cut_screen();//屏幕显示
			TIM_SetCompare2(TIM2, water_pwm); //水泵
		/************土壤湿度采集***********************/
			adc_trang = Get_Adc_Average(6, 10); //ADC湿度获取
			soil = (int)((4096 - adc_trang) * 100.0 / (4096 - 1200)); //湿度转换
		/************土壤湿度采集***********************/	
			DHT11_Read_Data(&tem, &humi);                                        //读取温湿度
			ctrl_data();//自动控制
			}
		}
	}
}

