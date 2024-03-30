#include "pub_sub_message.h"
#include "led.h"        //包含需要的头文件

/*//下发数据的样式
命令:{"method":"thing.service.property.set","id":"1087087436","params":{"TempH":41},"version":"1.0.0"}
star_sp=params":{"TempH":41},"version":"1.0.0"}
star_sp+10=TempH":41},"version":"1.0.0"}  
end_sp=":41},"version":"1.0.0"}
*/
char *star_sp, *end_sp; //用于提取标识符
char identifier_temp[50];//用于存放标识符
int identifier_data;//用于存放下发的数据
/*-------------------------------------------------*/
/*功能：得到并处理下发的数据                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/

void  publish_message(void)
{
    u1_printf("命令:%s\r\n", &MQTT_CMDOutPtr[2]);                //串口输出信息
    if(star_sp = strstr((char *)MQTT_CMDOutPtr + 2, (char *)"params")) //app下发数据---定位得到标识符前面的字符 ，用于求出标识符有多大
    {
		
        end_sp = strstr(star_sp + 10, "\":"); //定位得到标识符后面的字符 ，用于求出标识符有多大--加十是因为 params":{" 这一串字符长度为十
        memcpy(identifier_temp, star_sp + 10, (end_sp) - (star_sp + 10)); //将标识符提取
            identifier_data = Extract_digit((unsigned char *)end_sp + 2); //atoi（将字符串的数字变成整形的函数）得到下发的数据   +2是是因为 ":  这一串字符长度为十
            data_sta(identifier_temp, identifier_data);
			
					u1_printf("end_sp + 2:%s\r\n", end_sp + 2);                //串口输出信息
					u1_printf("identifier_data + 2:%d\r\n", identifier_data );                //串口输出信息
							
        if(!(strcmp(identifier_temp, ""APP_tem_up""))) //空气温度上限
        {
            tem_up = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_tem_down"")))//空气温度下限
        {
            tem_down = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_humi_up"")))//空气湿度上限
        {
            humi_up = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_humi_down"")))//空气湿度下限
        {
            humi_down = identifier_data;
        }

        else if(!(strcmp(identifier_temp, ""APP_soil_down"")))//土壤湿度下限
        {
            soil_down = identifier_data;
        }

        else if(!(strcmp(identifier_temp, ""APP_soil_up"")))//土壤湿度上限
        {
            soil_up = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_auto_flag"")))//自动模式
        {
            auto_flag = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_FAN"")))//风扇
        {
						u1_printf("identifier_data + 2:%d\r\n", identifier_data );                //串口输出信息
            Dc_motor_pwm = identifier_data;
        }
        else if(!(strcmp(identifier_temp, ""APP_WATER"")))//水泵
        {
            water_pwm = identifier_data;
        }
        memset(identifier_temp, 0, 255);
    }
    MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //指针下移
    if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)           	           //如果指针到缓冲区尾部了
        MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //指针归位到缓冲区开头
}
/*-------------------------------------------------*/
/*函数名：定时上传各种数据的任务                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
extern unsigned int SystemTimer ;
void Data_State(void)
{
    char temp[256];             //缓冲区

    if((SystemTimer ) >= 100)                                    //全局时间1s，进入if
    {
        SystemTimer = 0;                                           //把当前的全局时间，记录到温湿度计时变量
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\
			\""APP_tem"\":%f,\""APP_humi"\":%f,\""APP_soil"\":%d}\
			,\"version\":\"1.0.0\"}",
                tem, humi, soil); //构建数据
        MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                //添加数据到发送缓冲区
    }

}
/*-------------------------------------------------*/
/*函数名：发布数字数据给服务器                			   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void data_sta(void *str, int cmd)
{
    char temp[TBUFF_UNIT];                   //定义一个临时缓冲区
	
    if(wifi_connect && SubcribePack_flag) //标志位置位则连接wifi和服务器
    {
        memset(temp, 0, TBUFF_UNIT);
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":%d},\"version\":\"1.0.0\"}", (char *) str, cmd); //如果LED2是高电平，说明是熄灭状态，需要回复关闭状态给服务器
        MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //添加数据，发布给服务器
        sent_data();
				u1_printf("上报的数据----**--%s--**-----",temp);
    }

}
/*-------------------------------------------------*/
/*函数名：发布字符串数据给服务器           			   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void data_sta_str(void *str, int cmd)
{
    char temp[TBUFF_UNIT];                   //定义一个临时缓冲区
    if(wifi_connect && SubcribePack_flag) //标志位置位则连接wifi和服务器
    {
        memset(temp, 0, TBUFF_UNIT);
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":\"%d\"},\"version\":\"1.0.0\"}", (char *) str, cmd); //如果LED2是高电平，说明是熄灭状态，需要回复关闭状态给服务器
        MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //添加数据，发布给服务器
    }

}
/*-------------------------------------------------*/
/*函数名：发布判断开关的状态给服务器                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void judge_data_sta(void *str, uint16_t GPIO_Pin)
{
    char temp[TBUFF_UNIT];                   //定义一个临时缓冲区
    memset(temp, 0, TBUFF_UNIT);
    if( GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin))//判断io的电平
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":1},\"version\":\"1.0.0\"}", (char *)str); //如果LED2是高电平，说明是熄灭状态，需要回复关闭状态给服务器
    else
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":0},\"version\":\"1.0.0\"}", (char *)str); //如果LED2是高电平，说明是熄灭状态，需要回复关闭状态给服务器
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //添加数据，发布给服务器
}
void data_init(void)
{
    char temp[TBUFF_UNIT];                   //定义一个临时缓冲区
    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\
			\""APP_tem"\":%f,\""APP_humi"\":%f,\""APP_soil"\":%d}\
	,\"version\":\"1.0.0\"}",tem, humi, soil); //构建数据
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                //添加数据到发送缓冲区
    sent_data();//发送

    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\
			\""APP_tem_up"\":%d,\""APP_tem_down"\":%d,\""APP_humi_up"\":%d,\""APP_humi_down"\":%d,\""APP_soil_up"\":%d,\""APP_soil_down"\":%d}\
			,\"version\":\"1.0.0\"}", tem_up, tem_down, humi_up, humi_down, soil_up, soil_down); //构建数据
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                //添加数据到发送缓冲区
    sent_data();//发送
}

/*-------------------------------------------------*/
/*功能：处理发送缓冲区数据     					          */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void sent_data(void)
{

    /*-------------------------------------------------------------*/
    /*                     处理发送缓冲区数据                      */
    /*-------------------------------------------------------------*/
    if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)                     //if成立的话，说明发送缓冲区有数据了
    {
        //3种情况可进入if
        //第1种：0x10 连接报文
        //第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
        //第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
        if((DMA_flag == 0) && ((MQTT_TxDataOutPtr[2] == 0x10) || ((MQTT_TxDataOutPtr[2] == 0x82) && (ConnectPack_flag == 1)) || (SubcribePack_flag == 1)))
        {
            u1_printf("发送数据:0x%x\r\n", MQTT_TxDataOutPtr[2]); //串口提示信息
            MQTT_TxData(MQTT_TxDataOutPtr);                       //发送数据
            MQTT_TxDataOutPtr += TBUFF_UNIT;                      //指针下移
            if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)            //如果指针到缓冲区尾部了
                MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //指针归位到缓冲区开头
        }
    }//处理发送缓冲区数据的else if分支结尾
}
/*--------------------------------------------*/
/*功能：提取传入字符串的数字返回             */
/*参  数：需要提取的字符串                   */
/*返回值：提取出来的数字                     */
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
