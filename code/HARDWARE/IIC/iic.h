#ifndef __I2C_H
#define __I2C_H
#include "sys.h"

#define SDA_IN()  {GPIOB->MODER&=0XFFFF0FFF;GPIOB->MODER|=0X00001000;}
#define SDA_OUT() {GPIOB->MODER&=0X0FFF0FFF;GPIOB->MODER|=0X00005000;}
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA


void i2c_init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(void );

#endif