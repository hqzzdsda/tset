#include "total_wifi.h"
/*-------------------------------------------------------mqtt.c�ı���----------------------------------*/
unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];           //���ݵĽ��ջ�����,����----------------���������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
unsigned char *MQTT_RxDataInPtr;                            //ָ����ջ�����������ݵ�λ��
unsigned char *MQTT_RxDataOutPtr;                           //ָ����ջ�������ȡ���ݵ�λ��
unsigned char *MQTT_RxDataEndPtr;                           //ָ����ջ�����������λ��

unsigned char  MQTT_TxDataBuf[T_NUM][TBUFF_UNIT];           //���ݵķ��ͻ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
unsigned char *MQTT_TxDataInPtr;                            //ָ���ͻ�����������ݵ�λ��
unsigned char *MQTT_TxDataOutPtr;                           //ָ���ͻ�������ȡ���ݵ�λ��
unsigned char *MQTT_TxDataEndPtr;                           //ָ���ͻ�����������λ��

unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];              //�������ݵĽ��ջ�����
unsigned char *MQTT_CMDInPtr;                               //ָ���������������ݵ�λ��
unsigned char *MQTT_CMDOutPtr;                              //ָ�����������ȡ���ݵ�λ��
unsigned char *MQTT_CMDEndPtr;                              //ָ���������������λ��

char ClientID[128];                                          //��ſͻ���ID�Ļ�����
int  ClientID_len;                                           //��ſͻ���ID�ĳ���

char Username[128];                                          //����û����Ļ�����
int  Username_len;											 //����û����ĳ���

char Passward[128];                                          //�������Ļ�����
int  Passward_len;											 //�������ĳ���

char ServerIP[128];                                          //��ŷ�����IP��������
int  ServerPort;                                             //��ŷ������Ķ˿ں�

int   Fixed_len;                       					     //�̶���ͷ����
int   Variable_len;                     					 //�ɱ䱨ͷ����
int   Payload_len;                       					 //��Ч���ɳ���
unsigned char  temp_buff[TBUFF_UNIT];						 //��ʱ������������������

char Ping_flag;           //ping����״̬      0������״̬���ȴ���ʱʱ�䵽������Ping����
//ping����״̬      1��Ping�����ѷ��ͣ����յ� �������ظ����ĵĺ� ��1��Ϊ0
char Connect_flag;        //ͬ����������״̬  0����û�����ӷ�����  1�������Ϸ�������
char ConnectPack_flag;    //CONNECT����״̬   1��CONNECT���ĳɹ�
char SubcribePack_flag;   //���ı���״̬      1�����ı��ĳɹ�
/*-------------------------------------------------------mqtt.c�ı���----------------------------------*/
/*----------------------------wifi�ı���--------------------------------------------*/
char wifi_mode = 0;     //����ģʽ 0��SSID������д�ڳ�����   1��Smartconfig��ʽ��APP����
/*----------------------------wifi�ı���--------------------------------------------*/
void  publish_message(void);//�·���Ϣ

/*-------------------------------------------------------------*/
/*                     ���ӷ������ĺ���                    */
/*-------------------------------------------------------------*/
void Connect_server(void)
{
    /*--------------------------------------------------------------------*/
    /*   Connect_flag=1ͬ����������������,���ǿ��Է������ݺͽ���������    */
    /*--------------------------------------------------------------------*/
    if(Connect_flag == 1)
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
        /*-------------------------------------------------------------*/
        /*                     ������ջ���������                      */
        /*-------------------------------------------------------------*/
        if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr)                 //if�����Ļ���˵�����ջ�������������
        {
            u1_printf("���յ�����:");                             //������ʾ��Ϣ
            /*-----------------------------------------------------*/
            /*                    ����CONNACK����                  */
            /*-----------------------------------------------------*/
            //if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
            //��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
            if(MQTT_RxDataOutPtr[2] == 0x20)
            {
                switch(MQTT_RxDataOutPtr[5])
                {
                case 0x00 :
                    u1_printf("CONNECT���ĳɹ�\r\n");                            //���������Ϣ
                    ConnectPack_flag = 1;                                        //CONNECT���ĳɹ�
                    MQTT_Subscribe(S_TOPIC_NAME, 0);	                            //���ͻ�������Ӷ���topic���ȼ�0
                    break;                                                       //������֧case 0x00
                case 0x01 :
                    u1_printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");     //���������Ϣ
                    Connect_flag = 0;                                            //Connect_flag���㣬��������
                    break;                                                       //������֧case 0x01
                case 0x02 :
                    u1_printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n"); //���������Ϣ
                    Connect_flag = 0;                                            //Connect_flag���㣬��������
                    break;                                                       //������֧case 0x02
                case 0x03 :
                    u1_printf("�����Ѿܾ�������˲����ã�׼������\r\n");         //���������Ϣ
                    Connect_flag = 0;                                            //Connect_flag���㣬��������
                    break;                                                       //������֧case 0x03
                case 0x04 :
                    u1_printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");   //���������Ϣ
                    Connect_flag = 0;                                            //Connect_flag���㣬��������
                    break;                                                       //������֧case 0x04
                case 0x05 :
                    u1_printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");               //���������Ϣ
                    Connect_flag = 0;                                            //Connect_flag���㣬��������
                    break;                                                       //������֧case 0x05
                default   :
                    u1_printf("�����Ѿܾ���δ֪״̬��׼������\r\n");             //���������Ϣ
                    Connect_flag = 0;                                            //Connect_flag���㣬��������
                    break;                                                       //������֧case default
                }
            }
            /*-----------------------------------------------------*/
            /*                    ����SUBACK����                   */
            /*-----------------------------------------------------*/
            //if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
            //��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
            else if(MQTT_RxDataOutPtr[2] == 0x90)
            {
                switch(MQTT_RxDataOutPtr[6])
                {
                case 0x00 :
                case 0x01 :
                    u1_printf("���ĳɹ�\r\n");             //���������Ϣ
                    SubcribePack_flag = 1;                //SubcribePack_flag��1����ʾ���ı��ĳɹ���
                    TIM3_ENABLE_30S();                    //������ʱ��3 30s��PING���ʱ��
                    TIM_Cmd(TIM2, ENABLE);                          //��TIM2
								data_init();
                    break;                                //������֧
                default   :
                    u1_printf("����ʧ�ܣ�׼������\r\n");   //���������Ϣ
                    Connect_flag = 0;                     //Connect_flag���㣬��������
                    break;                                //������֧
                }
            }
            /*-----------------------------------------------------*/
            /*                  ����PINGRESP����                   */
            /*-----------------------------------------------------*/
            //if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
            else if(MQTT_RxDataOutPtr[2] == 0xD0)
            {
                u1_printf("PING���Ļظ�\r\n"); 		  //���������Ϣ
                if(Ping_flag == 1)                    //���Ping_flag=1����ʾ��һ�η���
                {
                    Ping_flag = 0;    				  //Ҫ���Ping_flag��־
                }
                else if(Ping_flag > 1)  				 //���Ping_flag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
                {
                    Ping_flag = 0;     				  //Ҫ���Ping_flag��־
                    TIM3_ENABLE_30S(); 				  //PING��ʱ���ػ�30s��ʱ��
                }
            }
            /*-----------------------------------------------------*/
            /*                  �����������ͱ���                   */
            /*-----------------------------------------------------*/
            //if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
            //����Ҫ��ȡ��������
            else if((MQTT_RxDataOutPtr[2] == 0x30))
            {
                u1_printf("�������ȼ�0����\r\n"); 		   //���������Ϣ
                MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //����ȼ�0��������
            }

            MQTT_RxDataOutPtr += RBUFF_UNIT;                    //����ָ������
            if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)          //�������ָ�뵽���ջ�����β����
                MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //����ָ���λ�����ջ�������ͷ
        }

        /*-------------------------------------------------------------*/
        /*                     ���������������                      */
        /*-------------------------------------------------------------*/
        if(MQTT_CMDOutPtr != MQTT_CMDInPtr)                              //if�����Ļ���˵�����������������
        {
            publish_message();
        }//��������������ݵ�else if��֧��β

        /*-------------------------------------------------------------*/
        /*                     ����ʱ�ϴ�����                        */
        /*-------------------------------------------------------------*/
        Data_State();

    }
    /*--------------------------------------------------------------------*/
    /*         Connect_flag=0ͬ�������Ͽ�������,����Ҫ���ӷ�����          */
    /*--------------------------------------------------------------------*/
    else
    {
        u1_printf("׼�����ӷ�����\r\n");                  //���������Ϣ
        Usart2_Init(115200);                             //����2���ܳ�ʼ����������115200
        TIM_Cmd(TIM3, DISABLE);                          //�ر�TIM3
        TIM_Cmd(TIM2, DISABLE);                          //�ر�TIM2
				SystemTimer=0;
        WiFi_RxCounter = 0;                              //WiFi������������������
        memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);        //���WiFi���ջ�����
        if(WiFi_Connect_IoTServer() == 0)    			   //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
        {
            u1_printf("���ӷ������ɹ�\r\n");              //���������Ϣ
            Usart2_IDELInit(115200);                     //����2 ����DMA �� �����ж�
            Connect_flag = 1;                            //Connect_flag��1����ʾ���ӳɹ�
            WiFi_RxCounter = 0;                          //WiFi������������������
            memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);    //���WiFi���ջ�����
            MQTT_Buff_Init();                            //��ʼ�����ͽ����������
        }
    }
}

/*----------------------------------------------------------*/
/*����������ʼ������,����,�������ݵ� ������ �Լ���״̬����  */
/*��  ������                                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void MQTT_Buff_Init(void)
{
    MQTT_RxDataInPtr = MQTT_RxDataBuf[0];             //ָ���ͻ�����������ݵ�ָ���λ
    MQTT_RxDataOutPtr = MQTT_RxDataInPtr;             //ָ���ͻ�������ȡ���ݵ�ָ���λ
    MQTT_RxDataEndPtr = MQTT_RxDataBuf[R_NUM - 1];    //ָ���ͻ�����������ָ���λ

    MQTT_TxDataInPtr = MQTT_TxDataBuf[0];             //ָ���ͻ�����������ݵ�ָ���λ
    MQTT_TxDataOutPtr = MQTT_TxDataInPtr;             //ָ���ͻ�������ȡ���ݵ�ָ���λ
    MQTT_TxDataEndPtr = MQTT_TxDataBuf[T_NUM - 1];    //ָ���ͻ�����������ָ���λ

    MQTT_CMDInPtr = MQTT_CMDBuf[0];                   //ָ���������������ݵ�ָ���λ
    MQTT_CMDOutPtr = MQTT_CMDInPtr;                   //ָ�����������ȡ���ݵ�ָ���λ
    MQTT_CMDEndPtr = MQTT_CMDBuf[C_NUM - 1];          //ָ���������������ָ���λ

    MQTT_ConectPack();                                //���ͻ�����������ӱ���
    Ping_flag = ConnectPack_flag = SubcribePack_flag = 0;  //������������
}
/*----------------------------------------------------------*/
/*�������������Ƴ�ʼ���������õ��ͻ���ID���û���������      */
/*��  ������                                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void AliIoT_Parameter_Init(void)
{
    char temp[128];                                                       //������ܵ�ʱ����ʱʹ�õĻ�����

    memset(ClientID, 0, 128);                                             //�ͻ���ID�Ļ�����ȫ������
    sprintf(ClientID, "%s|securemode=3,signmethod=hmacsha1|", DEVICENAME); //�����ͻ���ID�������뻺����
    ClientID_len = strlen(ClientID);                                      //����ͻ���ID�ĳ���

    memset(Username, 0, 128);                                             //�û����Ļ�����ȫ������
    sprintf(Username, "%s&%s", DEVICENAME, PRODUCTKEY);                   //�����û����������뻺����
    Username_len = strlen(Username);                                      //�����û����ĳ���

    memset(temp, 0, 128);                                                                    //��ʱ������ȫ������
    sprintf(temp, "clientId%sdeviceName%sproductKey%s", DEVICENAME, DEVICENAME, PRODUCTKEY); //��������ʱ������
    utils_hmac_sha1(temp, strlen(temp), Passward, DEVICESECRE, DEVICESECRE_LEN);             //��DeviceSecretΪ��Կ��temp�е����ģ�����hmacsha1���ܣ�����������룬�����浽��������
    Passward_len = strlen(Passward);                                                         //�����û����ĳ���

    memset(ServerIP, 0, 128);
    sprintf(ServerIP, "%s.iot-as-mqtt.cn-shanghai.aliyuncs.com", PRODUCTKEY);                //��������������
    ServerPort = 1883;                                                                       //�������˿ں�1883

    u1_printf("�� �� ����%s:%d\r\n", ServerIP, ServerPort); //�������������Ϣ
    u1_printf("�ͻ���ID��%s\r\n", ClientID);              //�������������Ϣ
    u1_printf("�� �� ����%s\r\n", Username);              //�������������Ϣ
    u1_printf("��    �룺%s\r\n", Passward);              //�������������Ϣ
}

/*----------------------------------------------------------*/
/*�����������ӷ���������                                    */
/*��  ������                                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void MQTT_ConectPack(void)
{
    int temp, Remaining_len;

    Fixed_len = 1;                                                        //���ӱ����У��̶���ͷ������ʱ��=1
    Variable_len = 10;                                                    //���ӱ����У��ɱ䱨ͷ����=10
    Payload_len = 2 + ClientID_len + 2 + Username_len + 2 + Passward_len; //���ӱ����У����س���
    Remaining_len = Variable_len + Payload_len;                           //ʣ�೤��=�ɱ䱨ͷ����+���س���

    temp_buff[0] = 0x10;                     //�̶���ͷ��1���ֽ� ���̶�0x01
    do                                       //ѭ������̶���ͷ�е�ʣ�೤���ֽڣ��ֽ�������ʣ���ֽڵ���ʵ���ȱ仯
    {
        temp = Remaining_len % 128;          //ʣ�೤��ȡ��128
        Remaining_len = Remaining_len / 128; //ʣ�೤��ȡ��128
        if(Remaining_len > 0)
            temp |= 0x80;                    //��Э��Ҫ��λ7��λ
        temp_buff[Fixed_len] = temp;         //ʣ�೤���ֽڼ�¼һ������
        Fixed_len++;	                     //�̶���ͷ�ܳ���+1
    }
    while(Remaining_len > 0);                //���Remaining_len>0�Ļ����ٴν���ѭ��

    temp_buff[Fixed_len + 0] = 0x00; //�ɱ䱨ͷ��1���ֽ� ���̶�0x00
    temp_buff[Fixed_len + 1] = 0x04; //�ɱ䱨ͷ��2���ֽ� ���̶�0x04
    temp_buff[Fixed_len + 2] = 0x4D;	//�ɱ䱨ͷ��3���ֽ� ���̶�0x4D
    temp_buff[Fixed_len + 3] = 0x51;	//�ɱ䱨ͷ��4���ֽ� ���̶�0x51
    temp_buff[Fixed_len + 4] = 0x54;	//�ɱ䱨ͷ��5���ֽ� ���̶�0x54
    temp_buff[Fixed_len + 5] = 0x54;	//�ɱ䱨ͷ��6���ֽ� ���̶�0x54
    temp_buff[Fixed_len + 6] = 0x04;	//�ɱ䱨ͷ��7���ֽ� ���̶�0x04
    temp_buff[Fixed_len + 7] = 0xC2;	//�ɱ䱨ͷ��8���ֽ� ��ʹ���û���������У�飬��ʹ���������������Ự
    temp_buff[Fixed_len + 8] = 0x00; 	//�ɱ䱨ͷ��9���ֽ� ������ʱ����ֽ� 0x00
    temp_buff[Fixed_len + 9] = 0x64;	//�ɱ䱨ͷ��10���ֽڣ�����ʱ����ֽ� 0x64   100s

    /*     CLIENT_ID      */
    temp_buff[Fixed_len + 10] = ClientID_len / 256;                			  			 //�ͻ���ID���ȸ��ֽ�
    temp_buff[Fixed_len + 11] = ClientID_len % 256;               			  			 //�ͻ���ID���ȵ��ֽ�
    memcpy(&temp_buff[Fixed_len + 12], ClientID, ClientID_len);                 			//���ƹ����ͻ���ID�ִ�
    /*     �û���        */
    temp_buff[Fixed_len + 12 + ClientID_len] = Username_len / 256; 				  		 //�û������ȸ��ֽ�
    temp_buff[Fixed_len + 13 + ClientID_len] = Username_len % 256; 				 		 //�û������ȵ��ֽ�
    memcpy(&temp_buff[Fixed_len + 14 + ClientID_len], Username, Username_len);          //���ƹ����û����ִ�
    /*      ����        */
    temp_buff[Fixed_len + 14 + ClientID_len + Username_len] = Passward_len / 256;			 //���볤�ȸ��ֽ�
    temp_buff[Fixed_len + 15 + ClientID_len + Username_len] = Passward_len % 256;			 //���볤�ȵ��ֽ�
    memcpy(&temp_buff[Fixed_len + 16 + ClientID_len + Username_len], Passward, Passward_len); //���ƹ��������ִ�

    TxDataBuf_Deal(temp_buff, Fixed_len + Variable_len + Payload_len);                  //���뷢�����ݻ�����
}
/*----------------------------------------------------------*/
/*��������SUBSCRIBE����topic����                            */
/*��  ����QoS�����ĵȼ�                                     */
/*��  ����topic_name������topic��������                     */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void MQTT_Subscribe(char *topic_name, int QoS)
{
    Fixed_len = 2;                              //SUBSCRIBE�����У��̶���ͷ����=2
    Variable_len = 2;                           //SUBSCRIBE�����У��ɱ䱨ͷ����=2
    Payload_len = 2 + strlen(topic_name) + 1;   //������Ч���ɳ��� = 2�ֽ�(topic_name����)+ topic_name�ַ����ĳ��� + 1�ֽڷ���ȼ�

    temp_buff[0] = 0x82;                                  //��1���ֽ� ���̶�0x82
    temp_buff[1] = Variable_len + Payload_len;            //��2���ֽ� ���ɱ䱨ͷ+��Ч���ɵĳ���
    temp_buff[2] = 0x00;                                  //��3���ֽ� �����ı�ʶ�����ֽڣ��̶�ʹ��0x00
    temp_buff[3] = 0x01;		                            //��4���ֽ� �����ı�ʶ�����ֽڣ��̶�ʹ��0x01
    temp_buff[4] = strlen(topic_name) / 256;              //��5���ֽ� ��topic_name���ȸ��ֽ�
    temp_buff[5] = strlen(topic_name) % 256;		      //��6���ֽ� ��topic_name���ȵ��ֽ�
    memcpy(&temp_buff[6], topic_name, strlen(topic_name)); //��7���ֽڿ�ʼ �����ƹ���topic_name�ִ�
    temp_buff[6 + strlen(topic_name)] = QoS;              //���1���ֽڣ����ĵȼ�

    TxDataBuf_Deal(temp_buff, Fixed_len + Variable_len + Payload_len);  //���뷢�����ݻ�����
}
/*----------------------------------------------------------*/
/*��������PING���ģ�������                                  */
/*��  ������                                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void MQTT_PingREQ(void)
{
    temp_buff[0] = 0xC0;            //��1���ֽ� ���̶�0xC0
    temp_buff[1] = 0x00;            //��2���ֽ� ���̶�0x00

    TxDataBuf_Deal(temp_buff, 2);   //�������ݵ�������
}
/*----------------------------------------------------------*/
/*���������ȼ�0 ������Ϣ����                                */
/*��  ����topic_name��topic����                             */
/*��  ����data������                                        */
/*��  ����data_len�����ݳ���                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void MQTT_PublishQs0(char *topic, char *data, int data_len)
{
    int temp, Remaining_len;

    Fixed_len = 1;                              //�̶���ͷ������ʱ�ȵ��ڣ�1�ֽ�
    Variable_len = 2 + strlen(topic);           //�ɱ䱨ͷ���ȣ�2�ֽ�(topic����)+ topic�ַ����ĳ���
    Payload_len = data_len;                     //��Ч���ɳ��ȣ�����data_len
    Remaining_len = Variable_len + Payload_len; //ʣ�೤��=�ɱ䱨ͷ����+���س���

    temp_buff[0] = 0x30;                     //�̶���ͷ��1���ֽ� ���̶�0x30
    do                                       //ѭ������̶���ͷ�е�ʣ�೤���ֽڣ��ֽ�������ʣ���ֽڵ���ʵ���ȱ仯
    {
        temp = Remaining_len % 128;          //ʣ�೤��ȡ��128
        Remaining_len = Remaining_len / 128; //ʣ�೤��ȡ��128
        if(Remaining_len > 0)
            temp |= 0x80;                    //��Э��Ҫ��λ7��λ
        temp_buff[Fixed_len] = temp;         //ʣ�೤���ֽڼ�¼һ������
        Fixed_len++;	                     //�̶���ͷ�ܳ���+1
    }
    while(Remaining_len > 0);                //���Remaining_len>0�Ļ����ٴν���ѭ��

    temp_buff[Fixed_len + 0] = strlen(topic) / 256;                //�ɱ䱨ͷ��1���ֽ�     ��topic���ȸ��ֽ�
    temp_buff[Fixed_len + 1] = strlen(topic) % 256;		         //�ɱ䱨ͷ��2���ֽ�     ��topic���ȵ��ֽ�
    memcpy(&temp_buff[Fixed_len + 2], topic, strlen(topic));       //�ɱ䱨ͷ��3���ֽڿ�ʼ ������topic�ַ���
    memcpy(&temp_buff[Fixed_len + 2 + strlen(topic)], data, data_len); //��Ч���ɣ�����data����

    TxDataBuf_Deal(temp_buff, Fixed_len + Variable_len + Payload_len);  //���뷢�����ݻ�����
}
/*----------------------------------------------------------*/
/*����������������������ĵȼ�0������                       */
/*��  ����redata�����յ�����                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void MQTT_DealPushdata_Qs0(unsigned char *redata)
{
    int  re_len;               	           //����һ����������Ž��յ������ܳ���
    int  pack_num;                         //����һ�����������������һ�����ʱ���������͵ĸ���
    int  temp, temp_len;                   //����һ���������ݴ�����
    int  totle_len;                        //����һ������������Ѿ�ͳ�Ƶ����͵���������
    int  topic_len;              	       //����һ���������������������ĳ���
    int  cmd_len;                          //����һ����������������а������������ݵĳ���
    int  cmd_loca;                         //����һ����������������а������������ʼλ��
    int  i;                                //����һ������������forѭ��
    int  local, multiplier;
    unsigned char tempbuff[RBUFF_UNIT];	   //��ʱ������
    unsigned char *data;                   //redata������ʱ�򣬵�һ���ֽ�������������data����ָ��redata�ĵ�2���ֽڣ����������ݿ�ʼ�ĵط�

    re_len = redata[0] * 256 + redata[1];                           //��ȡ���յ������ܳ���
    data = &redata[2];                                              //dataָ��redata�ĵ�2���ֽڣ����������ݿ�ʼ��
    pack_num = temp_len = totle_len = temp = 0;                	    //������������
    local = 1;
    multiplier = 1;
    do
    {
        pack_num++;                                     			//��ʼѭ��ͳ�����͵ĸ�����ÿ��ѭ�����͵ĸ���+1
        do
        {
            temp = data[totle_len + local];
            temp_len += (temp & 127) * multiplier;
            multiplier *= 128;
            local++;
        }
        while ((temp & 128) != 0);
        totle_len += (temp_len + local);                          	//�ۼ�ͳ�Ƶ��ܵ����͵����ݳ���
        re_len -= (temp_len + local) ;                              //���յ������ܳ��� ��ȥ ����ͳ�Ƶ����͵��ܳ���
        local = 1;
        multiplier = 1;
        temp_len = 0;
    }
    while(re_len != 0);                                  			//������յ������ܳ��ȵ���0�ˣ�˵��ͳ�������
    u1_printf("���ν�����%d����������\r\n", pack_num); //���������Ϣ
    temp_len = totle_len = 0;                		            	//������������
    local = 1;
    multiplier = 1;
    for(i = 0; i < pack_num; i++)                                   //�Ѿ�ͳ�Ƶ��˽��յ����͸�������ʼforѭ����ȡ��ÿ�����͵�����
    {
        do
        {
            temp = data[totle_len + local];
            temp_len += (temp & 127) * multiplier;
            multiplier *= 128;
            local++;
        }
        while ((temp & 128) != 0);
        topic_len = data[local + totle_len] * 256 + data[local + 1 + totle_len] + 2; //���㱾����������������ռ�õ�������
        cmd_len = temp_len - topic_len;                             //���㱾��������������������ռ�õ�������
        cmd_loca = totle_len + local +  topic_len;                  //���㱾�������������������ݿ�ʼ��λ��
        memcpy(tempbuff, &data[cmd_loca], cmd_len);                 //�������ݿ�������
        CMDBuf_Deal(tempbuff, cmd_len);                             //�������������
        totle_len += (temp_len + local);                            //�ۼ��Ѿ�ͳ�Ƶ����͵����ݳ���
        local = 1;
        multiplier = 1;
        temp_len = 0;
    }
}
/*----------------------------------------------------------*/
/*�������������ͻ�����                                    */
/*��  ����data������                                        */
/*��  ����size�����ݳ���                                    */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void TxDataBuf_Deal(unsigned char *data, int size)
{
    memcpy(&MQTT_TxDataInPtr[2], data, size);    //�������ݵ����ͻ�����
    MQTT_TxDataInPtr[0] = size / 256;            //��¼���ݳ���
    MQTT_TxDataInPtr[1] = size % 256;            //��¼���ݳ���
    MQTT_TxDataInPtr += TBUFF_UNIT;               //ָ������
    if(MQTT_TxDataInPtr == MQTT_TxDataEndPtr)    //���ָ�뵽������β����
        MQTT_TxDataInPtr = MQTT_TxDataBuf[0];    //ָ���λ����������ͷ
}
/*----------------------------------------------------------*/
/*�������������������                                    */
/*��  ����data������                                        */
/*��  ����size�����ݳ���                                    */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void CMDBuf_Deal(unsigned char *data, int size)
{
    memcpy(&MQTT_CMDInPtr[2], data, size);    //�������ݵ��������
    MQTT_CMDInPtr[0] = size / 256;            //��¼���ݳ���
    MQTT_CMDInPtr[1] = size % 256;            //��¼���ݳ���
    MQTT_CMDInPtr[size + 2] = '\0';           //�����ַ���������
    MQTT_CMDInPtr += CBUFF_UNIT;               //ָ������
    if(MQTT_CMDInPtr == MQTT_CMDEndPtr)       //���ָ�뵽������β����
        MQTT_CMDInPtr = MQTT_CMDBuf[0];       //ָ���λ����������ͷ
}

/*-------------------------------------------------*/
/*����������ʼ��WiFi�ĸ�λIO                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/

void WiFi_ResetIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                      //����һ������IO�˿ڲ����Ľṹ��
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);    //ʹ��PA�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //׼������PA4
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //����50Mhz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		  //���������ʽ
    GPIO_Init(GPIOA, &GPIO_InitStructure);            		  //����PA4
    RESET_IO(1);                                              //��λIO���ߵ�ƽ
}
/*-------------------------------------------------*/
/*��������WiFi��������ָ��                         */
/*��  ����cmd��ָ��                                */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_SendCmd(char *cmd, int timeout)
{
    WiFi_RxCounter = 0;                         //WiFi������������������
    memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);   //���WiFi���ջ�����
    WiFi_printf("%s\r\n", cmd);                 //����ָ��
    while(timeout--)                            //�ȴ���ʱʱ�䵽0
    {
        Delay_Ms(100);                          //��ʱ100ms
        if(strstr(WiFi_RX_BUF, "OK"))           //������յ�OK��ʾָ��ɹ�
            break;       						//��������whileѭ��
        u1_printf("%d ", timeout);              //����������ڵĳ�ʱʱ��
    }
    u1_printf("\r\n");                          //���������Ϣ
    if(timeout <= 0)return 1;                   //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
    else return 0;		         				//��֮����ʾ��ȷ��˵���յ�OK��ͨ��break��������while
}
/*-------------------------------------------------*/
/*��������WiFi��λ                                 */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
    RESET_IO(0);                                    //��λIO���͵�ƽ
    Delay_Ms(500);                                  //��ʱ500ms
    RESET_IO(1);                                    //��λIO���ߵ�ƽ
    while(timeout--)                                //�ȴ���ʱʱ�䵽0
    {
        OLED_ShowString(0, 0, "RSTING", 16);
        Delay_Ms(100);                              //��ʱ100ms
        if(strstr(WiFi_RX_BUF, "ready")||strstr(WiFi_RX_BUF, "invalid"))            //������յ�  ready invalid ��ʾ��λ�ɹ�
            break;       						    //��������whileѭ��

        u1_printf("%d ", timeout);                  //����������ڵĳ�ʱʱ��
        OLED_ShowNum(0, 32, timeout, 2, 16);
        OLED_Refresh();
    }
    u1_printf("\r\n");                              //���������Ϣ
    OLED_Clear();
    if(timeout <= 0)return 1;                       //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�ready������1
    else return 0;		         				    //��֮����ʾ��ȷ��˵���յ�ready��ͨ��break��������while
}
/*-------------------------------------------------*/
/*��������WiFi����·����ָ��                       */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_JoinAP(int timeout)
{
    WiFi_RxCounter = 0;                             //WiFi������������������
    memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);       //���WiFi���ջ�����
    WiFi_printf("AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASS); //����ָ��
    u1_printf("AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASS); //����ָ��
    while(timeout--)                                //�ȴ���ʱʱ�䵽0
    {
				OLED_ShowString(0, 0, "CONNECT TO", 16);
				OLED_ShowString(0, 16, SSID, 16);
        Delay_Ms(1000);                             //��ʱ1s
				if(timeout%10==0)  
				{
					WiFi_RxCounter = 0;                             //WiFi������������������
					memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);       //���WiFi���ջ�����
					WiFi_printf("AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASS); //����ָ��
				}
        if(strstr(WiFi_RX_BUF, "WIFI GOT IP\r\n\r\nOK")) //������յ�WIFI GOT IP��ʾ�ɹ�
            break;       						    //��������whileѭ��
        u1_printf("%d ", timeout);                  //����������ڵĳ�ʱʱ��
        OLED_ShowNum(0, 32, timeout, 2, 16);
        OLED_Refresh();
				
    }
    u1_printf("\r\n");                              //���������Ϣ
		    OLED_Clear();
    if(timeout <= 0)return 1;                       //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�WIFI GOT IP������1
    return 0;                                       //��ȷ������0


}
/*-------------------------------------------------*/
/*������������TCP��������������͸��ģʽ            */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
char WiFi_Connect_Server(int timeout)
{
    WiFi_RxCounter = 0;                             //WiFi������������������
    memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);       //���WiFi���ջ�����
    WiFi_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ServerIP, ServerPort); //�������ӷ�����ָ��
    while(timeout--)                                //�ȴ���ʱ���
    {
				OLED_ShowString(0, 0, "CONNECT TO", 16);
				OLED_ShowString(0, 16, "TCP", 16);
				if(timeout%10==0)  
				{
					WiFi_RxCounter = 0;                             //WiFi������������������
					memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);       //���WiFi���ջ�����
					WiFi_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ServerIP, ServerPort); //�������ӷ�����ָ��
				}				
        Delay_Ms(100);                              //��ʱ100ms
        if(strstr(WiFi_RX_BUF, "CONNECT"))          //������ܵ�CONNECT��ʾ���ӳɹ�
            break;                                  //����whileѭ��
        if(strstr(WiFi_RX_BUF, "CLOSED"))           //������ܵ�CLOSED��ʾ������δ����
            return 1;                               //������δ��������1
        if(strstr(WiFi_RX_BUF, "ALREADY CONNECTED"))//������ܵ�ALREADY CONNECTED�Ѿ���������
            return 2;                               //�Ѿ��������ӷ���2
        u1_printf("%d ", timeout);                  //����������ڵĳ�ʱʱ��
        OLED_ShowNum(0, 32, timeout, 2, 16);
        OLED_Refresh();
    }
    u1_printf("\r\n");                        //���������Ϣ
    if(timeout <= 0)return 3;                 //��ʱ���󣬷���3
    else                                      //���ӳɹ���׼������͸��
    {
        u1_printf("���ӷ������ɹ���׼������͸��\r\n");  //������ʾ��Ϣ
        WiFi_RxCounter = 0;                             //WiFi������������������
        memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);       //���WiFi���ջ�����
        WiFi_printf("AT+CIPSEND\r\n");                  //���ͽ���͸��ָ��
        while(timeout--)                                //�ȴ���ʱ���
        {
            Delay_Ms(100);                              //��ʱ100ms
            if(strstr(WiFi_RX_BUF, "\r\nOK\r\n\r\n>"))  //���������ʾ����͸���ɹ�
                break;                          //����whileѭ��
            u1_printf("%d ", timeout);          //����������ڵĳ�ʱʱ��
        }
        if(timeout <= 0)return 4;               //͸����ʱ���󣬷���4
    }
		    OLED_Clear();
    return 0;	                                //�ɹ�����0
}
/*-------------------------------------------------*/
/*��������WiFi_Smartconfig                         */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Smartconfig(int timeout)
{

    WiFi_RxCounter = 0;                         //WiFi������������������
    memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);   //���WiFi���ջ�����
    while(timeout--)                            //�ȴ���ʱʱ�䵽0
    {
        Delay_Ms(1000);                         //��ʱ1s
        if(strstr(WiFi_RX_BUF, "connected"))    //������ڽ��ܵ�connected��ʾ�ɹ�
            break;                              //����whileѭ��
        u1_printf("%d ", timeout);              //����������ڵĳ�ʱʱ��
    }
    u1_printf("\r\n");                          //���������Ϣ
    if(timeout <= 0)return 1;                   //��ʱ���󣬷���1
    return 0;                                   //��ȷ����0
}
/*-------------------------------------------------*/
/*���������ȴ�����·����                           */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_WaitAP(int timeout)
{
    while(timeout--)                                //�ȴ���ʱʱ�䵽0
    {
        Delay_Ms(1000);                             //��ʱ1s
        if(strstr(WiFi_RX_BUF, "WIFI GOT IP"))      //������յ�WIFI GOT IP��ʾ�ɹ�
            break;       						    //��������whileѭ��
        u1_printf("%d ", timeout);                  //����������ڵĳ�ʱʱ��
    }
    u1_printf("\r\n");                              //���������Ϣ
    if(timeout <= 0)return 1;                       //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�WIFI GOT IP������1
    return 0;                                       //��ȷ������0
}
/*-------------------------------------------------*/
/*��������WiFi���ӷ�����                           */
/*��  ������                                       */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Connect_IoTServer(void)
{
    OLED_Clear();

    u1_printf("׼����λģ��\r\n");                     //������ʾ����
    if(WiFi_Reset(50))                                 //��λ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
    {
        u1_printf("��λʧ�ܣ�׼������\r\n");           //���ط�0ֵ������if��������ʾ����
        return 1;                                      //����1
    }
    else u1_printf("��λ�ɹ�\r\n");                    //������ʾ����

    u1_printf("׼������STAģʽ\r\n");                  //������ʾ����
    if(WiFi_SendCmd("AT+CWMODE=1", 50))                //����STAģʽ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
    {
        u1_printf("����STAģʽʧ�ܣ�׼������\r\n");    //���ط�0ֵ������if��������ʾ����
        return 2;                                      //����2
    }
    else u1_printf("����STAģʽ�ɹ�\r\n");             //������ʾ����

    if(wifi_mode == 0)                                     //�������ģʽ=0��SSID������д�ڳ�����
    {
        u1_printf("׼��ȡ���Զ�����\r\n");                 //������ʾ����
        if(WiFi_SendCmd("AT+CWAUTOCONN=0", 50))            //ȡ���Զ����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
        {
            u1_printf("ȡ���Զ�����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
            return 3;                                      //����3
        }
        else u1_printf("ȡ���Զ����ӳɹ�\r\n");            //������ʾ����

        u1_printf("׼������·����\r\n");                   //������ʾ����
        if(WiFi_JoinAP(30))                                //����·����,1s��ʱ��λ���ܼ�30s��ʱʱ��
        {
            u1_printf("����·����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
            return 4;                                      //����4
        }
        else u1_printf("����·�����ɹ�\r\n");              //������ʾ����
    }
    else                                                   //�������ģʽ=1��Smartconfig��ʽ,��APP����
    {
        if(KEY2_IN_STA == 0)                                   //�����ʱK2�ǰ��µ�
        {
            u1_printf("׼�������Զ�����\r\n");                 //������ʾ����
            if(WiFi_SendCmd("AT+CWAUTOCONN=1", 50))            //�����Զ����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
            {
                u1_printf("�����Զ�����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
                return 3;                                      //����3
            }
            else u1_printf("�����Զ����ӳɹ�\r\n");            //������ʾ����

            u1_printf("׼������Smartconfig\r\n");              //������ʾ����
            if(WiFi_SendCmd("AT+CWSTARTSMART", 50))            //����Smartconfig��100ms��ʱ��λ���ܼ�5s��ʱʱ��
            {
                u1_printf("����Smartconfigʧ�ܣ�׼������\r\n");//���ط�0ֵ������if��������ʾ����
                return 4;                                      //����4
            }
            else u1_printf("����Smartconfig�ɹ�\r\n");         //������ʾ����

            u1_printf("��ʹ��APP�����������\r\n");            //������ʾ����
            if(WiFi_Smartconfig(60))                           //APP����������룬1s��ʱ��λ���ܼ�60s��ʱʱ��
            {
                u1_printf("��������ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
                return 5;                                      //����5
            }
            else u1_printf("��������ɹ�\r\n");                //������ʾ����

            u1_printf("׼���ر�Smartconfig\r\n");              //������ʾ����
            if(WiFi_SendCmd("AT+CWSTOPSMART", 50))             //�ر�Smartconfig��100ms��ʱ��λ���ܼ�5s��ʱʱ��
            {
                u1_printf("�ر�Smartconfigʧ�ܣ�׼������\r\n");//���ط�0ֵ������if��������ʾ����
                return 6;                                      //����6
            }
            else u1_printf("�ر�Smartconfig�ɹ�\r\n");         //������ʾ����
        }
        else                                                   //��֮����ʱK2��û�а���
        {
            u1_printf("�ȴ�����·����\r\n");                   //������ʾ����
            if(WiFi_WaitAP(30))                                //�ȴ�����·����,1s��ʱ��λ���ܼ�30s��ʱʱ��
            {
                u1_printf("����·����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
                return 7;                                      //����7
            }
            else u1_printf("����·�����ɹ�\r\n");              //������ʾ����
        }
    }

    u1_printf("׼������͸��\r\n");                     //������ʾ����
    if(WiFi_SendCmd("AT+CIPMODE=1", 50))               //����͸����100ms��ʱ��λ���ܼ�5s��ʱʱ��
    {
        u1_printf("����͸��ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
        return 8;                                      //����8
    }
    else u1_printf("����͸���ɹ�\r\n");                //������ʾ����

    u1_printf("׼���رն�·����\r\n");                 //������ʾ����
    if(WiFi_SendCmd("AT+CIPMUX=0", 50))                //�رն�·���ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
    {
        u1_printf("�رն�·����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
        return 9;                                      //����9
    }
    else u1_printf("�رն�·���ӳɹ�\r\n");            //������ʾ����

    u1_printf("׼�����ӷ�����\r\n");                   //������ʾ����
    if(WiFi_Connect_Server(100))                       //���ӷ�������100ms��ʱ��λ���ܼ�10s��ʱʱ��
    {
        u1_printf("���ӷ�����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
        return 10;                                     //����10
    }
    else u1_printf("���ӷ������ɹ�\r\n");              //������ʾ����

    return 0;                                          //��ȷ����0

}
