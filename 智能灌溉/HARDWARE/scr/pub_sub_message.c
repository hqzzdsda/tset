#include "pub_sub_message.h"
#include "led.h"        //������Ҫ��ͷ�ļ�

/*//�·����ݵ���ʽ
����:{"method":"thing.service.property.set","id":"1087087436","params":{"TempH":41},"version":"1.0.0"}
star_sp=params":{"TempH":41},"version":"1.0.0"}
star_sp+10=TempH":41},"version":"1.0.0"}  
end_sp=":41},"version":"1.0.0"}
*/
char *star_sp, *end_sp; //������ȡ��ʶ��
char identifier_temp[50];//���ڴ�ű�ʶ��
int identifier_data;//���ڴ���·�������
/*-------------------------------------------------*/
/*���ܣ��õ��������·�������                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/

void  publish_message(void)
{
    u1_printf("����:%s\r\n", &MQTT_CMDOutPtr[2]);                //���������Ϣ
    if(star_sp = strstr((char *)MQTT_CMDOutPtr + 2, (char *)"params")) //app�·�����---��λ�õ���ʶ��ǰ����ַ� �����������ʶ���ж��
    {
		
        end_sp = strstr(star_sp + 10, "\":"); //��λ�õ���ʶ��������ַ� �����������ʶ���ж��--��ʮ����Ϊ params":{" ��һ���ַ�����Ϊʮ
        memcpy(identifier_temp, star_sp + 10, (end_sp) - (star_sp + 10)); //����ʶ����ȡ
            identifier_data = Extract_digit((unsigned char *)end_sp + 2); //atoi�����ַ��������ֱ�����εĺ������õ��·�������   +2������Ϊ ":  ��һ���ַ�����Ϊʮ
            data_sta(identifier_temp, identifier_data);
			
					u1_printf("end_sp + 2:%s\r\n", end_sp + 2);                //���������Ϣ
					u1_printf("identifier_data + 2:%d\r\n", identifier_data );                //���������Ϣ
							
        if(!(strcmp(identifier_temp, ""APP_tem_up""))) //�����¶�����
        {
            tem_up = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_tem_down"")))//�����¶�����
        {
            tem_down = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_humi_up"")))//����ʪ������
        {
            humi_up = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_humi_down"")))//����ʪ������
        {
            humi_down = identifier_data;
        }

        else if(!(strcmp(identifier_temp, ""APP_soil_down"")))//����ʪ������
        {
            soil_down = identifier_data;
        }

        else if(!(strcmp(identifier_temp, ""APP_soil_up"")))//����ʪ������
        {
            soil_up = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_auto_flag"")))//�Զ�ģʽ
        {
            auto_flag = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_FAN"")))//����
        {
						u1_printf("identifier_data + 2:%d\r\n", identifier_data );                //���������Ϣ
            Dc_motor_pwm = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_WATER"")))//ˮ��
        {
            water_pwm = identifier_data;
        }
        memset(identifier_temp, 0, 255);
    }
    MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //ָ������
    if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)           	           //���ָ�뵽������β����
        MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //ָ���λ����������ͷ
}
/*-------------------------------------------------*/
/*����������ʱ�ϴ��������ݵ�����                   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
extern unsigned int SystemTimer ;
void Data_State(void)
{
    char temp[256];             //������

    if((SystemTimer ) >= 100)                                    //ȫ��ʱ��1s������if
    {
        SystemTimer = 0;                                           //�ѵ�ǰ��ȫ��ʱ�䣬��¼����ʪ�ȼ�ʱ����
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\
			\""APP_tem"\":%f,\""APP_humi"\":%f,\""APP_soil"\":%d}\
			,\"version\":\"1.0.0\"}",
                tem, humi, soil); //��������
        MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                //������ݵ����ͻ�����
    }

}
/*-------------------------------------------------*/
/*�������������������ݸ�������                			   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void data_sta(void *str, int cmd)
{
    char temp[TBUFF_UNIT];                   //����һ����ʱ������
	
    if(wifi_connect && SubcribePack_flag) //��־λ��λ������wifi�ͷ�����
    {
        memset(temp, 0, TBUFF_UNIT);
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":%d},\"version\":\"1.0.0\"}", (char *) str, cmd); //���LED2�Ǹߵ�ƽ��˵����Ϩ��״̬����Ҫ�ظ��ر�״̬��������
        MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //������ݣ�������������
        sent_data();
				u1_printf("�ϱ�������----**--%s--**-----",temp);
    }

}
/*-------------------------------------------------*/
/*�������������ַ������ݸ�������           			   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void data_sta_str(void *str, int cmd)
{
    char temp[TBUFF_UNIT];                   //����һ����ʱ������
    if(wifi_connect && SubcribePack_flag) //��־λ��λ������wifi�ͷ�����
    {
        memset(temp, 0, TBUFF_UNIT);
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":\"%d\"},\"version\":\"1.0.0\"}", (char *) str, cmd); //���LED2�Ǹߵ�ƽ��˵����Ϩ��״̬����Ҫ�ظ��ر�״̬��������
        MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //������ݣ�������������
    }

}
/*-------------------------------------------------*/
/*�������������жϿ��ص�״̬��������                   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void judge_data_sta(void *str, uint16_t GPIO_Pin)
{
    char temp[TBUFF_UNIT];                   //����һ����ʱ������
    memset(temp, 0, TBUFF_UNIT);
    if( GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin))//�ж�io�ĵ�ƽ
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":1},\"version\":\"1.0.0\"}", (char *)str); //���LED2�Ǹߵ�ƽ��˵����Ϩ��״̬����Ҫ�ظ��ر�״̬��������
    else
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":0},\"version\":\"1.0.0\"}", (char *)str); //���LED2�Ǹߵ�ƽ��˵����Ϩ��״̬����Ҫ�ظ��ر�״̬��������
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //������ݣ�������������
}
void data_init(void)
{
    char temp[TBUFF_UNIT];                   //����һ����ʱ������
    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\
			\""APP_tem"\":%f,\""APP_humi"\":%f,\""APP_soil"\":%d}\
	,\"version\":\"1.0.0\"}",tem, humi, soil); //��������
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                //������ݵ����ͻ�����
    sent_data();//����

    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\
			\""APP_tem_up"\":%d,\""APP_tem_down"\":%d,\""APP_humi_up"\":%d,\""APP_humi_down"\":%d,\""APP_soil_up"\":%d,\""APP_soil_down"\":%d}\
			,\"version\":\"1.0.0\"}", tem_up, tem_down, humi_up, humi_down, soil_up, soil_down); //��������
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                //������ݵ����ͻ�����
    sent_data();//����
}

/*-------------------------------------------------*/
/*���ܣ������ͻ���������     					          */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void sent_data(void)
{

    /*-------------------------------------------------------------*/
    /*                     �����ͻ���������                      */
    /*-------------------------------------------------------------*/
    if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)                     //if�����Ļ���˵�����ͻ�������������
    {
        //3������ɽ���if
        //��1�֣�0x10 ���ӱ���
        //��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
        //��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
        if((DMA_flag == 0) && ((MQTT_TxDataOutPtr[2] == 0x10) || ((MQTT_TxDataOutPtr[2] == 0x82) && (ConnectPack_flag == 1)) || (SubcribePack_flag == 1)))
        {
            u1_printf("��������:0x%x\r\n", MQTT_TxDataOutPtr[2]); //������ʾ��Ϣ
            MQTT_TxData(MQTT_TxDataOutPtr);                       //��������
            MQTT_TxDataOutPtr += TBUFF_UNIT;                      //ָ������
            if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)            //���ָ�뵽������β����
                MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //ָ���λ����������ͷ
        }
    }//�����ͻ��������ݵ�else if��֧��β
}
/*--------------------------------------------*/
/*���ܣ���ȡ�����ַ��������ַ���             */
/*��  ������Ҫ��ȡ���ַ���                   */
/*����ֵ����ȡ����������                     */
/*-------------------------------------------*/
u32 Extract_digit(u8 *str)
{
	u32 num=0;
	while(*str>='0'&&*str<='9')
	{
		num+=(*str)-48;
		*str++;
		num*=10;

	}
	return num/10;
}
