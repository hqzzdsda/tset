#include "total_wifi.h"
#include "adc.h"
#include "bsp_time2_PWM.h" 
#include "pub_sub_message.h"
#include "dht11.h"
#include "led.h"        //������Ҫ��ͷ�ļ�
extern char SubcribePack_flag;   //���ı���״̬      1�����ı��ĳɹ�
unsigned int adc_trang = 0; //����adc  
int main(void) 
{		
	Delay_Init();                   //��ʱ���ܳ�ʼ�� 
  Usart1_Init(115200);              //����1���ܳ�ʼ����������9600
	LED_Init();	                    //LED��ʼ��
	KEY_Init();                     //������ʼ��	
	WiFi_ResetIO_Init();            //��ʼ��WiFi�ĸ�λIO
	AliIoT_Parameter_Init();	    //��ʼ�����Ӱ�����IoTƽ̨MQTT�������Ĳ���	
	OLED_Init();//oled��ʼ��
	KEY_Exti_Init();
	DHT11_Init();
	Adc_Init();//�ɼ�����ʪ��
  TIME2_PWM_Init_10MS();//��ʱ��2pwm��ʼ�� ˮ�÷��ȵ�pwm  ���ж�ʱ�ϴ����ݵ�ʱ��
	while(1)                      //��ѭ��
	{		
		if(!wifi_connect)//�Ͽ�wifiģʽ
		{
			SystemTimer=0;//����ʱ�ϴ����ݵı�������
			Connect_flag = 0;
			USART_Cmd(USART2, DISABLE); //ʧ�ܴ���2  

			Cut_screen();//��Ļ��ʾ
			TIM_SetCompare2(TIM2, water_pwm); //ˮ�� 
		/************����ʪ�Ȳɼ�***********************/
			adc_trang = Get_Adc_Average(6, 10); //ADCʪ�Ȼ�ȡ
			soil = (int)((4096 - adc_trang) * 100.0 / (4096 - 1200)); //ʪ��ת��
		/************����ʪ�Ȳɼ�***********************/	
			DHT11_Read_Data(&tem, &humi);                                        //��ȡ��ʪ��
			ctrl_data();//�Զ�����
		}
		else//����wifiģʽ
		{	
			Connect_server();//���ӷ������ĺ���
			if(SubcribePack_flag)//�����������Ž���
			{		
			Cut_screen();//��Ļ��ʾ
			TIM_SetCompare2(TIM2, water_pwm); //ˮ��
		/************����ʪ�Ȳɼ�***********************/
			adc_trang = Get_Adc_Average(6, 10); //ADCʪ�Ȼ�ȡ
			soil = (int)((4096 - adc_trang) * 100.0 / (4096 - 1200)); //ʪ��ת��
		/************����ʪ�Ȳɼ�***********************/	
			DHT11_Read_Data(&tem, &humi);                                        //��ȡ��ʪ��
			ctrl_data();//�Զ�����
			}
		}
	}
}

