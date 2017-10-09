
#include "iic.h"
#include "common.h"

// I2C�����ٶ� = I2C_SPEED_1K / i2c_speed
uint32_t	i2c_speed = I2C_SPEED_1K/100;



/******************************************************I2C0************************************************************/
/**
  * @brief  ģ��I2C�ӿڳ�ʼ��
  * @param  None
  * @retval None
  * @note
	*		SCL: 	PD11
	*		SDA:	PD12
  */
void I2C0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//ʹ��GPIOB����ʱ��
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	

	
	//I2C��SDA��SCL����Ҫ��Ӳ���������������裬�����������Ϊ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);//�����������ó�ʼ��GPIO
	
		//ʹ��GPIOB����ʱ��
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF , ENABLE );	
	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
		//CE2,3,4,5
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_3|GPIO_Pin_1;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
		//CE6
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	//����SCL��SDA����״̬Ϊ�ߵ�ƽ
	I2C_SCL0 = 1;
	I2C_SDA0 = 1;
	
	I2C_SetSpeed(400);//����I2C�����ٶ�Ϊ400Kbps
}


/**
	* @brief ����I2C��ʼ�ź�
  * @param  None
  * @retval None
  * @note
	*		��ο�I2Cͨ��Э�飬I2C��ʼ�źţ���SCLΪ�ߵ�ƽʱ��SDA�ɸ߱��
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��ʼ�ź�
	*           _____     |
	*        __|__   |    |  ___  ___  ___  ___  ___  ___  ___  ___  
	*   SDA:   |  \__|____|_/   \/   \/   \/   \/   \/   \/   \/   \     /
	*          |     |    | \___/\___/\___/\___/\___/\___/\___/\___/\___/
	*        __|_____|_   |   _    _    _    _    _    _    _    _    _
	*   SCL:   |     | \__|__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \_
	*          |_____|    | 
	*           start         D7   D6   D5   D4   D3   D2   D1   D0   ACK
	*/
void I2C0_Start(void)
{
	uint32_t i2c_delay = i2c_speed;
	
	SDA0_OUT();	//SDA����Ϊ���
	I2C_SDA0 = 1;	//SDA: ��
	I2C_SCL0 = 1;	//SCL: ��
	i2c_delay = i2c_speed;//��ʱ>4.7us
	while(i2c_delay--){}
 	I2C_SDA0 = 0;	//��SCLΪ�ߵ�ƽʱ��SDA�ɸ߱��
	i2c_delay = i2c_speed;//��ʱ>4us
	while(i2c_delay--){}
	I2C_SCL0 = 0;	//SCL��ͣ�ǯסI2C���ߣ�׼�����ͻ�������� 
}

/**
	* @brief ����I2Cֹͣ�ź�
  * @param  None
  * @retval None
  * @note
	*		��ο�I2Cͨ��Э�飬I2Cֹͣ�źţ���SCLΪ�ߵ�ƽʱ��SDA�ɵͱ��
	*		������STOP�źź�SCL��SDA��Ϊ�ߵ�ƽ�����ͷ���I2C����
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��ʼ�ź�
	*                                    _____
	*         ___  ___  ___  ___        |   __|_
	*   SDA: /   \/   \/   \/   \       |  /  |
	*        \___/\___/\___/\___/\______|_/   |
	*         _    _    _    _    _    _|_____|_
	*   SCL: / \__/ \__/ \__/ \__/ \__/ |     |
	*                                   |_____|
	*        D3   D2   D1   D0   ACK     stop
	*/
void I2C0_Stop(void)
{
	uint32_t i2c_delay = i2c_speed;
	
	SDA0_OUT(); 		//SDA����Ϊ���
	I2C_SDA0 = 0;	//SDA�͵�ƽ
	I2C_SCL0 = 1;	//SCL�ߵ�ƽ
 	i2c_delay = i2c_speed;//��ʱ>4us
	while(i2c_delay--){}
	I2C_SDA0 = 1;	//STOP:��SCLΪ�ߵ�ƽʱ��SDA�ɵͱ��
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//��ʱ>4.7us						   	
}

/**
	* @brief  �ȴ�ACKӦ���ź�
  * @param  None
  * @retval 1 - δ���յ�Ӧ���ź�ACK��0 - ���յ�Ӧ���ź�ACK
  * @note
	*		��ο�I2Cͨ��Э�飬���ACKӦ���źţ���SCLΪ�ߵ�ƽʱ����ȡSDAΪ�͵�ƽ
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��ʼ�ź�
	*                             ________     _____
	*         ___  ___  ___  ___ | _      |   |   __|_
	*   SDA: /   \/   \/   \/   \|/ \     |   |  /  |
	*        \___/\___/\___/\___/|   \____|___|_/   |
	*         _    _    _    _   | _____  |  _|_____|
	*   SCL: / \__/ \__/ \__/ \__|/     \_|_/ |     |
	*                            |________|   |_____|
	*        D3   D2   D1   D0      ACK        stop
	*/
uint8_t I2C0_Wait_ACK(void)
{
	uint32_t i2c_delay = i2c_speed;
	uint8_t timeout = 0;
	
	SDA0_IN();			//SDA����Ϊ����
	I2C_SDA0 = 1;	//SDA��������
	I2C_SCL0=1;	//SCL����Ϊ�ߵ�ƽ
	i2c_delay = i2c_speed;
	while(i2c_delay--){}
	while(READ_SDA0 == 1)//�ȴ�ACK
	{
		if(timeout++ > 250)
		{
			I2C0_Stop();
			return 1;
		}
	}
	I2C_SCL0 = 0;//ǯסI2C���ߣ�ʱ���ź���Ϊ�͵�ƽ
	return 0;  
}


/**
	* @brief  ����ACKӦ���ź�
  * @param  None
  * @retval None
  * @note
	*		��ο�I2Cͨ��Э�飬����ACKӦ���ź�: ��SDAΪ�͵�ƽʱ��SCL����һ��������
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��ʼ�ź�
	*                             _____     _____
	*         ___  ___  ___  ___ |     |   |   __|_
	*   SDA: /   \/   \/   \/   \|     |   |  /  |
	*        \___/\___/\___/\___/|\____|___|_/   |
	*         _    _    _    _   |  _  |  _|_____|_
	*   SCL: / \__/ \__/ \__/ \__|_/ \_|_/ |     |
	*                            |_____|   |_____|
	*        D3   D2   D1   D0     ACK      stop
	*/
void I2C0_ACK(void)
{
	uint32_t i2c_delay = i2c_speed;
	
	I2C_SCL0 = 0;	//�͵�ƽ
	SDA0_OUT();		//����SDAΪ���
	I2C_SDA0 = 0;	//ACK�ź�
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//��ʱ>4us
	I2C_SCL0 = 1;	//�ߵ�ƽ
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//��ʱ>4us
	I2C_SCL0 = 0;	//ǯסI2C���ߣ�ʱ���ź���Ϊ�͵�ƽ
}


/**
	* @brief  ������Ӧ���ź�NACK
  * @param  None
  * @retval None
  * @note
	*		��ο�I2Cͨ��Э�飬����ACKӦ���ź�: ��SDAΪ�ߵ�ƽʱ��SCL����һ��������
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��ʼ�ź�
	*                             _____      ______
	*         ___  ___  ___  ___ | ____|_   |    __|_
	*   SDA: /   \/   \/   \/   \|/    | \  |   /  |
	*        \___/\___/\___/\___/|     |  \_|__/   |
	*         _    _    _    _   |  _  |  __|______|_
	*   SCL: / \__/ \__/ \__/ \__|_/ \_|_/  |      |
	*                            |_____|    |______|
	*        D3   D2   D1   D0    NACK        stop
	*/	    
void I2C0_NACK(void)
{
	uint32_t i2c_delay = i2c_speed;
	
	I2C_SCL0 = 0;	//�͵�ƽ
	SDA0_OUT();		//SDA����Ϊ���
	I2C_SDA0 = 1;	//NACK�ź�
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//��ʱ>4us
	I2C_SCL0 = 1;	//�ߵ�ƽ
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//��ʱ>4us
	I2C_SCL0 = 0;	//ǯסI2C���ߣ�ʱ���ź���Ϊ�͵�ƽ
}


/**
	* @brief  I2C����һ���ֽ�
  * @param  None
  * @retval None
  * @note
	*		��ο�I2Cͨ��Э�飬����ACKӦ���ź�: ��SDAΪ�ߵ�ƽʱ��SCL����һ��������
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��ʼ�ź�
	*
	*           _____     |<------------I2C���ݷ�������------------>|
	*        __|__   |    |  ___  ___  ___  ___  ___  ___  ___  ___ | _ 
	*   SDA:   |  \__|____|_/   \/   \/   \/   \/   \/   \/   \/   \|/ 
	*          |     |    | \___/\___/\___/\___/\___/\___/\___/\___/|\_
	*        __|_____|_   |   _    _    _    _    _    _    _    _  |  
	*   SCL:   |     | \__|__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \_|_
	*          |_____|    |                                         |
	*           start     |   D7   D6   D5   D4   D3   D2   D1   D0 |
	*/
void I2C0_Send_Byte(uint8_t data)
{                        
	uint8_t i = 0;
	uint32_t i2c_delay = i2c_speed;

	SDA0_OUT();									//SDA��Ϊ���
	I2C_SCL0 = 0;								//ǯסI2C���ߣ�SCL��Ϊ�͵�ƽ
	for(i = 0; i < 8; i++)
	{
		if(data&0x80)I2C_SDA0 = 1;	//��λ�ȴ�
		else I2C_SDA0 = 0;
		
		i2c_delay = i2c_speed;
		while(i2c_delay--){}			//��ʱ>4us
	
		I2C_SCL0 = 1;							//��SCL�ϲ���һ��������
		i2c_delay = i2c_speed;
		while(i2c_delay--){}			//��ʱ>4us
			
		I2C_SCL0=0;
		i2c_delay = i2c_speed/3;
		while(i2c_delay--){}			//��ʱ>1us
		data <<= 1;								//����һλ
	}
}


/**
	* @brief  ��I2C��ȡһ���ֽ�
  * @param  ack : 0 - NACK; 1 - ACK
  * @retval ���յ�������
  * @note
	*		��ο�I2Cͨ��Э�飬����ACKӦ���ź�: ��SDAΪ�ߵ�ƽʱ��SCL����һ��������
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��ʼ�ź�
	*
	*           _____     |<------------I2C���ݶ�ȡ����(ACK)------------>|
	*        __|__   |    |  ___  ___  ___  ___  ___  ___  ___  ___      |
	*   SDA:   |  \__|____|_/   \/   \/   \/   \/   \/   \/   \/   \     |
	*          |     |    | \___/\___/\___/\___/\___/\___/\___/\___/\____|_
	*        __|_____|_   |   _    _    _    _    _    _    _    _    _  |
	*   SCL:   |     | \__|__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \_|_
	*          |_____|    |                                              |
	*           start     |   D7   D6   D5   D4   D3   D2   D1   D0   ACK
	*
	*           _____     |<------------I2C���ݶ�ȡ����(NACK)----------->|
	*        __|__   |    |  ___  ___  ___  ___  ___  ___  ___  ___  ____|_
	*   SDA:   |  \__|____|_/   \/   \/   \/   \/   \/   \/   \/   \/    |
	*          |     |    | \___/\___/\___/\___/\___/\___/\___/\___/     |
	*        __|_____|_   |   _    _    _    _    _    _    _    _    _  |
	*   SCL:   |     | \__|__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \_|_
	*          |_____|    |                                              |
	*           start     |   D7   D6   D5   D4   D3   D2   D1   D0  NACK
	*/
uint8_t I2C0_Read_Byte(uint8_t ack)
{
	uint8_t i, receive = 0x00;
	uint32_t i2c_delay = i2c_speed;
	
	I2C_SCL0 = 0;									//SCL�͵�ƽ
	SDA0_IN();											//SDA����Ϊ����
	for(i = 0; i < 8; i++)
	{
		i2c_delay = i2c_speed;
		while(i2c_delay--);
		I2C_SCL0 = 1;								//�ߵ�ƽ
		i2c_delay = i2c_speed;
		while(i2c_delay--);
		receive <<= 1;
		if(READ_SDA0) receive |= 1;	//��λ��ǰ
		I2C_SCL0 = 0;
  }
	if (ack == 0) I2C0_NACK();			//����NACK
	else I2C0_ACK();								//����ACK
	
	return receive;								//���ؽ��յ�������
}

/**
  * @brief  ����I2C�ٶ�
  * @param  speed : I2C�ٶȣ���λKbps
  * @retval ��������ǰ��I2C�ٶ�
	* @note   I2C�ٶ����÷�Χ��: 1Kbps ~ 400Kbps
  */
uint16_t I2C_SetSpeed(uint16_t speed)
{
	uint16_t temp;
	
	//I2C�ٶȱ���С��400Kbps������ 1Kbps
	if((speed > 400)|| (speed < 1)) return 0;
	
	temp = I2C_SPEED_1K / i2c_speed;	//����ԭ����i2c�ٶ�
	i2c_speed = I2C_SPEED_1K / speed;	//�����µ�i2c�ٶ�

	return temp;	//��������ǰ��i2c�ٶ�
}

/* ---------------------------���²����Ƿ�װ�õ�I2C��д����--------------------------- */

//���嵽ĳһ������������ϸ�Ķ�������������I2C���ֵ�˵������ΪĳЩ����I2C�Ķ�д������
//��һЩ���죬����Ĵ��������ھ��������I2C�����У�������֤OK�ģ�

/**
  * @brief  ���豸ָ����ַд�뵥һByte����
  * @param  DevAddr : I2C���豸��ַ
  * @param  DataAddr: ��Ҫ���ʵ��豸�ڵ�ַ(��Ĵ�����ַ��EEPROM��ַ��)
  * @param  Data    : д�������
  * @retval I2C���ʵĽ��: I2C_SUCCESS / I2C_TIMEOUT / I2C_ERROR
	* @note   
	*   1 - �豸��ַDevAddr��7bit�ǹ̶��ģ����Ϊ�Ƕ�/д(R/W)λ��1Ϊ����0Ϊд
	*		2 - ʱ��
	*           _______________________________________
	*          | |         |   |        |   |    |   | |
	*   Master:|S|DevAddr+W|   |DataAddr|   |Data|   |P|
	*          |_|_________|___|________|___|____|___|_|
	*           _______________________________________
	*          | |         |   |        |   |    |   | |
	*   Slave: | |         |ACK|        |ACK|    |ACK| |
	*          |_|_________|___|________|___|____|___|_|
  */
I2C_StatusTypeDef I2C0_WriteOneByte(uint8_t DevAddr, uint8_t DataAddr, uint8_t Data)
{
	I2C0_Start();													//Master������ʼ�ź�
	I2C0_Send_Byte(DevAddr);								//Master���ʹ��豸��ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	I2C0_Send_Byte(DataAddr);							//�������ݵ�ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	I2C0_Send_Byte(Data);									//��������
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	I2C0_Stop();														//����ֹͣ�ź�
	return I2C_SUCCESS;
}

/**
  * @brief  ���豸ָ����ַ����д������(Burstдģʽ)
  * @param  DevAddr : I2C���豸��ַ
  * @param  DataAddr: ��Ҫ���ʵ��豸�ڵ�ַ(��Ĵ�����ַ��EEPROM��ַ��)
	*                   ����Burstģʽ��DataAddrһ�����豸��FIFO,���棬��洢�豸�����ݵ�ַ
  * @param  *pData  : д��������׵�ַ
  * @param     Num  : ����д������ݸ���
  * @retval I2C���ʵĽ��: I2C_SUCCESS / I2C_TIMEOUT / I2C_ERROR
	* @note   
	*   1 - �豸��ַDevAddr��7bit�ǹ̶��ģ����Ϊ�Ƕ�/д(R/W)λ��1Ϊ����0Ϊд
	*		2 - ʱ��
	*           ____________________________________________________
	*          | |         |   |        |   |    |   |   |    |   | |
	*   Master:|S|DevAddr+W|   |DataAddr|   |Data|   |...|Data|   |P|
	*          |_|_________|___|________|___|____|___|___|____|___|_|
	*           ____________________________________________________
	*          | |         |   |        |   |    |   |   |    |   | |
	*   Slave: | |         |ACK|        |ACK|    |ACK|...|    |ACK| |
	*          |_|_________|___|________|___|____|___|___|____|___|_|
  */
I2C_StatusTypeDef single_write_I2C0(uint8_t DevAddr, uint16_t DataAddr, uint8_t pData)
{	
	I2C0_Start();													//Master������ʼ�ź�
	I2C0_Send_Byte(DevAddr);								//Master���ʹ��豸��ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	/*
	I2C0_Send_Byte(DataAddr);							//�������ݵ�ַ
	*/
	I2C0_Send_Byte((uint8_t)(DataAddr>>8));
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����	
	I2C0_Send_Byte((uint8_t)DataAddr);
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	
	I2C0_Send_Byte(pData);						//��������
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	
	I2C0_Stop();	//����ֹͣ�ź�
	return I2C_SUCCESS;
}

I2C_StatusTypeDef double_write_I2C0(uint8_t DevAddr, uint16_t DataAddr, uint16_t pData)
{
  uint8_t buffer[2];
	
	I2C0_Start();													//Master������ʼ�ź�
	I2C0_Send_Byte(DevAddr);								//Master���ʹ��豸��ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	/*
	I2C0_Send_Byte(DataAddr);							//�������ݵ�ַ
	*/
	I2C0_Send_Byte((uint8_t)(DataAddr>>8));
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����	
	I2C0_Send_Byte((uint8_t)DataAddr);
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	
	
  buffer[0]=pData>>8;
  buffer[1]=pData&0xFF;
	I2C0_Send_Byte(buffer[0]);						//��������
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����	
	I2C0_Send_Byte(buffer[1]);						//��������
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	
	I2C0_Stop();	//����ֹͣ�ź�
	return I2C_SUCCESS;
}

I2C_StatusTypeDef multi_write_I2C0(uint8_t DevAddr, uint16_t DataAddr, uint32_t pData)
{
  uint8_t buffer[4];
	
	I2C0_Start();													//Master������ʼ�ź�
	I2C0_Send_Byte(DevAddr);								//Master���ʹ��豸��ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	/*
	I2C0_Send_Byte(DataAddr);							//�������ݵ�ַ
	*/
	I2C0_Send_Byte((uint8_t)(DataAddr>>8));
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����	
	I2C0_Send_Byte((uint8_t)DataAddr);
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	
	
  buffer[0]=pData>>24;
  buffer[1]=(pData>>16)&0xFF;
  buffer[2]=(pData>>8)&0xFF;
  buffer[3]=pData&0xFF;
	I2C0_Send_Byte(buffer[0]);						//��������
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����	
	I2C0_Send_Byte(buffer[1]);						//��������
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����	
	I2C0_Send_Byte(buffer[2]);						//��������
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����	
	I2C0_Send_Byte(buffer[3]);						//��������
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	
	I2C0_Stop();	//����ֹͣ�ź�
	return I2C_SUCCESS;
}

/**
  * @brief  ��ָ���豸��ȡ1Byte����
  * @param  DevAddr : I2C���豸��ַ
  * @param  DataAddr: ��Ҫ���ʵ��豸�ڵ�ַ(��Ĵ�����ַ��EEPROM��ַ��)
  * @param  *Data   : ���ݵĴ�ŵ�ַ
  * @retval I2C���ʵĽ��: I2C_SUCCESS / I2C_TIMEOUT / I2C_ERROR
	* @note   
	*   1 - �豸��ַDevAddr��7bit�ǹ̶��ģ����Ϊ�Ƕ�/д(R/W)λ��1Ϊ����0Ϊд
	*		2 - ʱ��
	*           _________________________________________________________
	*          | |         |   |        |    | |         |   |    |    | |
	*   Master:|S|DevAddr+W|   |DataAddr|    |S|DevAddr+R|   |    |NACK|P|
	*          |_|_________|___|________|____|_|_________|___|____|____|_|
	*           _________________________________________________________
	*          | |         |   |        |    | |         |   |    |    | |
	*   Slave: | |         |ACK|        |ACK | |         |ACK|Data|    | |
	*          |_|_________|___|________|____|_|_________|___|____|____|_|
  */
I2C_StatusTypeDef I2C0_ReadOneByte(uint8_t DevAddr, uint8_t DataAddr, uint8_t* Data)
{
	I2C0_Start();													//Master������ʼ�ź�
	I2C0_Send_Byte(DevAddr);								//Master���ʹ��豸��ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	I2C0_Send_Byte(DataAddr);							//�������ݵ�ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	
	I2C0_Start();													//Master������ʼ�ź�
	I2C0_Send_Byte(DevAddr+1);							//Master���ʹ��豸����ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	*Data = I2C0_Read_Byte(0);							//�����ݣ�NACK
	I2C0_Stop();														//����ֹͣ�ź�
	return I2C_SUCCESS;
}

/**
  * @brief  ���豸ָ����ַ����д������(Burstдģʽ)
  * @param  DevAddr : I2C���豸��ַ
  * @param  DataAddr: ��Ҫ���ʵ��豸�ڵ�ַ(��Ĵ�����ַ��EEPROM��ַ��)
	*                   ����Burstģʽ��DataAddrһ�����豸��FIFO,���棬��洢�豸�����ݵ�ַ
  * @param  *pData  : д��������׵�ַ
  * @param     Num  : ����д������ݸ���
  * @retval	I2C���ʵĽ��: I2C_SUCCESS / I2C_TIMEOUT / I2C_ERROR
	* @note   
	*   1 - �豸��ַDevAddr��7bit�ǹ̶��ģ����Ϊ�Ƕ�/д(R/W)λ��1Ϊ����0Ϊд
	*		2 - ʱ��
	*           _____________________________________________________________________
	*          | |         |   |        |   | |         |   |    |   |   |    |    | |
	*   Master:|S|DevAddr+W|   |DataAddr|   |S|DevAddr+R|   |    |ACK|...|    |NACK|P|
	*          |_|_________|___|________|___|_|_________|___|____|___|___|____|____|_|
	*           _____________________________________________________________________
	*          | |         |   |        |   | |         |   |    |   |   |    |    | |
	*   Slave: | |         |ACK|        |ACK| |         |ACK|Data|   |...|Data|    | |
	*          |_|_________|___|________|___|_|_________|___|____|___|___|____|____|_|
  */
I2C_StatusTypeDef single_read_I2C0(uint8_t DevAddr, uint16_t DataAddr, uint8_t* pData)
{
	I2C0_Start();													//Master������ʼ�ź�
	I2C0_Send_Byte(DevAddr);								//Master���ʹ��豸��ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	/*
	I2C0_Send_Byte(DataAddr);							//�������ݵ�ַ
	*/
	I2C0_Send_Byte((uint8_t)(DataAddr>>8));
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����	
	I2C0_Send_Byte((uint8_t)DataAddr);
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	
	I2C0_Stop();														//����ֹͣ�ź�

	I2C0_Start();													//Master������ʼ�ź�
	I2C0_Send_Byte(DevAddr+1);							//Master���ʹ��豸����ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����

	*pData = I2C0_Read_Byte(0);				//�����ݣ�NACK
	
	I2C0_Stop();														//����ֹͣ�ź�
	return I2C_SUCCESS;
}

I2C_StatusTypeDef multi_read_I2C0(uint8_t DevAddr, uint16_t DataAddr, uint8_t* pData, uint32_t Num)
{
	uint32_t i = 0;
	
	I2C0_Start();													//Master������ʼ�ź�
	I2C0_Send_Byte(DevAddr);								//Master���ʹ��豸��ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	/*
	I2C0_Send_Byte(DataAddr);							//�������ݵ�ַ
	*/
	I2C0_Send_Byte((uint8_t)(DataAddr>>8));
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����	
	I2C0_Send_Byte((uint8_t)DataAddr);
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����
	
	I2C0_Stop();														//����ֹͣ�ź�

	I2C0_Start();													//Master������ʼ�ź�
	I2C0_Send_Byte(DevAddr+1);							//Master���ʹ��豸����ַ
	if(I2C0_Wait_ACK()) return I2C_TIMEOUT;//�ȴ�ACK��ʱ����

	for(i = 0; i < (Num-1); i++)
	{
		*(pData+i) = I2C0_Read_Byte(0);				//�����ݣ�NACK
	}
	*(pData+i) = I2C0_Read_Byte(0);				//�����ݣ�NACK
	
	I2C0_Stop();														//����ֹͣ�ź�
	return I2C_SUCCESS;
}
/**
  * @brief  �������ݵ�ĳһλ
  * @param  DevAddr : I2C���豸��ַ
  * @param  DataAddr: ��Ҫ���ʵ��豸�ڵ�ַ(��Ĵ�����ַ��EEPROM��ַ��)
  * @param  Bitx  : �ڼ�λ
  * @param  BitSet: ��Ҫ���õ�ֵ
  * @retval I2C���ʵĽ��: I2C_SUCCESS / I2C_TIMEOUT / I2C_ERROR
	* @note  
	*/
I2C_StatusTypeDef I2C0_WriteBit(uint8_t DevAddr, uint8_t DataAddr, uint8_t Bitx, uint8_t BitSet)
{
	I2C_StatusTypeDef status = I2C_ERROR;
	uint8_t tempdata = 0;
	
	status = I2C0_ReadOneByte(DevAddr, DataAddr, &tempdata);	//��ȡԭ������
	if(status != I2C_SUCCESS) return status;								//I2C�����򷵻�
	
	tempdata &= ~(1<<Bitx);																	//��Ҫ�趨��λ����
	tempdata |= (BitSet<<Bitx);															//����ָ����bit
	status = I2C0_WriteOneByte(DevAddr, DataAddr, tempdata);	//д������
	
	return status;	//����״̬
}
