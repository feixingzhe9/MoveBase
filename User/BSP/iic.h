
#ifndef __I2C_H
#define __I2C_H
#include "stm32f10x.h"
#include "common.h"

#define I2C_SPEED_1K		5000	//根据处理器速度设置，这里处理器速度是72MHz

//I2C端口定义
#define I2C_SCL0    GPIOout(GPIOB, 10)	//SCL--PB10
#define I2C_SDA0    GPIOout(GPIOB, 11)	//SDA--PB11
#define READ_SDA0   GPIOin( GPIOB, 11)	//输入SDA



//设置SDA:PD11输入输出
#define SDA0_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}		//参考stm32寄存器配置
#define SDA0_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//I2C端口定义
#define I2C_SCL1    GPIOout(GPIOE, 11)	//SCL--PE11
#define I2C_SDA1    GPIOout(GPIOE, 12)	//SDA--PE12
#define READ_SDA1   GPIOin( GPIOE, 12)	//输入SDA



//设置SDA:PE12输入输出
#define SDA1_IN()  {GPIOE->CRH&=0XFFF0FFFF;GPIOE->CRH|=8<<16;}		//参考stm32寄存器配置
#define SDA1_OUT() {GPIOE->CRH&=0XFFF0FFFF;GPIOE->CRH|=3<<16;}

typedef enum
{
	I2C_SUCCESS = 0,
	I2C_TIMEOUT,
	I2C_ERROR,
}I2C_StatusTypeDef;

extern uint32_t i2c_speed;	//I2C访问速度 = I2C_SPEED_1K / i2c_speed

/* ---------------------------依照I2C协议编写的时序函数------------------------------*/
void I2C0_Init(void);				//初始化I2C的IO口				 
void I2C0_Start(void);				//发送I2C开始信号
void I2C0_Stop(void);				//发送I2C停止信号
uint8_t I2C0_Wait_ACK(void);	//I2C等待ACK信号
void I2C0_ACK(void);					//I2C发送ACK信号
void I2C0_NACK(void);				//I2C不发送ACK信号
void I2C0_Send_Byte(uint8_t data);		//I2C发送一个字节
uint8_t I2C0_Read_Byte(uint8_t ack);	//I2C读取一个字节


uint16_t I2C_SetSpeed(uint16_t speed);//设置I2C速度(1Kbps~400Kbps,speed单位，Kbps)

/* ---------------------------以下部分是封装好的I2C读写函数--------------------------- */

//具体到某一个器件，请仔细阅读器件规格书关于I2C部分的说明，因为某些器件在I2C的读写操作会
//有一些差异，下面的代码我们在绝大多数的I2C器件中，都是验证OK的！
I2C_StatusTypeDef I2C0_WriteOneByte(uint8_t DevAddr, uint8_t DataAddr, uint8_t Data);//向I2C从设备写入一个字节

I2C_StatusTypeDef I2C0_ReadOneByte(uint8_t DevAddr, uint8_t DataAddr, uint8_t* Data);//从I2C从设备读取一个字节

I2C_StatusTypeDef I2C0_WriteBit(uint8_t DevAddr, uint8_t DataAddr, uint8_t Bitx, uint8_t BitSet);

I2C_StatusTypeDef single_write_I2C0(uint8_t DevAddr, uint16_t DataAddr, uint8_t pData);									//向I2C从设备上写入一个字节

I2C_StatusTypeDef double_write_I2C0(uint8_t DevAddr, uint16_t DataAddr, uint16_t pData);									//向I2C从设备连续写入2个字节

I2C_StatusTypeDef multi_write_I2C0(uint8_t DevAddr, uint16_t DataAddr, uint32_t pData);									//向I2C从设备连续写入4个字节

I2C_StatusTypeDef single_read_I2C0(uint8_t DevAddr, uint16_t DataAddr, uint8_t* pData);									//从I2C设备上读取一个字节

I2C_StatusTypeDef multi_read_I2C0(uint8_t DevAddr, uint16_t DataAddr, uint8_t* pData, uint32_t Num);			//从I2C设备连续读取Num个字节


I2C_StatusTypeDef I2C1_WriteOneByte(uint8_t DevAddr, uint8_t DataAddr, uint8_t Data);//向I2C从设备写入一个字节

I2C_StatusTypeDef I2C1_ReadOneByte(uint8_t DevAddr, uint8_t DataAddr, uint8_t* Data);//从I2C从设备读取一个字节

I2C_StatusTypeDef I2C1_WriteBit(uint8_t DevAddr, uint8_t DataAddr, uint8_t Bitx, uint8_t BitSet);

I2C_StatusTypeDef single_write_I2C1(uint8_t DevAddr, uint16_t DataAddr, uint8_t pData);									//向I2C从设备上写入一个字节

I2C_StatusTypeDef double_write_I2C1(uint8_t DevAddr, uint16_t DataAddr, uint16_t pData);									//向I2C从设备连续写入2个字节

I2C_StatusTypeDef multi_write_I2C1(uint8_t DevAddr, uint16_t DataAddr, uint32_t pData);									//向I2C从设备连续写入4个字节

I2C_StatusTypeDef single_read_I2C1(uint8_t DevAddr, uint16_t DataAddr, uint8_t* pData);									//从I2C设备上读取一个字节

I2C_StatusTypeDef multi_read_I2C1(uint8_t DevAddr, uint16_t DataAddr, uint8_t* pData, uint32_t Num);			//从I2C设备连续读取Num个字节

#endif

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/

