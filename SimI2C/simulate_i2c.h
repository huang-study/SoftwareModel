#ifndef _SIMULATE_I2C_H_
#define _SIMULATE_I2C_H_


/*
 ********************************************************************************
 *                                  Inc File
 ********************************************************************************
 */
 
#include "stm32f1xx_hal.h"
#include "stdint.h"

/*
 ********************************************************************************
 *                               Macro Definition
 ********************************************************************************
 */
 

#define I2C1_SCL_Pin GPIO_PIN_6
#define I2C1_SCL_GPIO_Port GPIOB
#define I2C1_SDA_Pin GPIO_PIN_7
#define I2C1_SDA_GPIO_Port GPIOB
 
#define I2C_DELAY_SCL_US 0
#define I2C_DELAY_US     0
#define GETPIN(arg) (1<<arg)


/*
 ********************************************************************************
 *                               Type Definition
 ********************************************************************************
*/

typedef struct
{
    GPIO_TypeDef* const SDAPort;
    GPIO_TypeDef* const SCLPort;
    const uint8_t SDAPin;
    const uint8_t SCLPin;
    uint32_t delayUS;
}SimulateI2C;


/*
 ********************************************************************************
 *                               Func Declare
 ********************************************************************************
*/
void SimulateI2C_Init(SimulateI2C i2c);                             //��ʼ��IIC��IO��				 
void SimulateI2C_Start(SimulateI2C i2c);				            //����IIC��ʼ�ź�
void SimulateI2C_Stop(SimulateI2C i2c);	  			                //����IICֹͣ�ź�
void SimulateI2C_SendByte(SimulateI2C i2c, uint8_t txd);			        //IIC����һ���ֽ�
uint8_t SimulateI2C_RecvByte(SimulateI2C i2c,unsigned char ack);    //IIC��ȡһ���ֽ�
uint8_t SimulateI2C_WaitAck(SimulateI2C i2c); 				        //IIC�ȴ�ACK�ź�
void SimulateI2C_Ack(SimulateI2C i2c);					            //IIC����ACK�ź�
void SimulateI2C_NAck(SimulateI2C i2c);				                //IIC������ACK�ź�

 
void SimulateI2C_WriteByte(SimulateI2C i2c, uint8_t addr, uint8_t device_addr, uint8_t numToWrite, uint8_t *buf);
void SimulateI2C_ReadByte(SimulateI2C i2c, uint8_t addr, uint8_t device_addr,uint8_t numToRead, uint8_t *buf);  


extern SimulateI2C i2c;

#endif
