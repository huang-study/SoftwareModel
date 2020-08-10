/*
 ***********************************************************************************************
 *                                      SHENZHEN QQZM
 *
 * @File    :   simulateI2C.c
 * @Brief   :   模拟I2C
 * @Author  :   Berecho Xiong
 * @Date    :
 * @Version :
 * @Note    :   未使用SysTick校准通信频率
 ***********************************************************************************************
 */

/*
 ***********************************************************************************************
 *                                      头文件
 ***********************************************************************************************
 */
#include "simulate_i2c.h"

/*
 ***********************************************************************************************
 *                                      全局变量
 ***********************************************************************************************
 */
SimulateI2C i2c={
    I2C1_SDA_GPIO_Port,
    I2C1_SCL_GPIO_Port,
    7,      //SDA
    6,      //SCL
    100
};

/*
 ***********************************************************************************************
 *                                      函数定义
 ***********************************************************************************************
 */

/*
 ****************************************************************
 * @Brief    :  延迟函数
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :  控制模拟I2C的通信频率
                TODO:使用SysTick校准通信频率
 *****************************************************************
 */
static void delay_nus(uint32_t n)
{
	uint16_t j;
	while(n--)
	{
		j=168;
		while(j--);
	}

}

/*
 ****************************************************************
 * @Brief    :  模拟I2C初始化
 * @Param    :  [IN]SimulateI2C     需要初始化的I2C
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */

void SimulateI2C_Init(SimulateI2C i2c)
{
    /*  STM32引脚初始化
     *  GPIO_InitTypeDef initStruct;
     *  __HAL_RCC_GPIOB_CLK_ENABLE();
     *  initStruct.Pin = GETPIN(i2c.SDAPin)|GETPIN(i2c.SCLPin);
     *  initStruct.Mode = GPIO_MODE_OUTPUT_OD;
     *  initStruct.Pull = GPIO_PULLUP;
     *  initStruct.Speed = GPIO_SPEED_FREQ_HIGH;
     *  HAL_GPIO_Init(i2c.SDAPort,&initStruct);
     */
    
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, I2C1_SCL_Pin|I2C1_SDA_Pin, GPIO_PIN_SET);
	
  GPIO_InitStruct.Pin = I2C1_SCL_Pin|I2C1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  HAL_GPIO_WritePin(i2c.SDAPort,GETPIN(i2c.SDAPin),GPIO_PIN_SET);
    
  HAL_GPIO_WritePin(i2c.SCLPort,GETPIN(i2c.SCLPin),GPIO_PIN_SET);
}

/*
 ****************************************************************
 * @Brief    :  切换引脚方向
 * @Param    :
 * @Return   :
 * @Relative :
* @Note     : 可以通过设置为开漏上拉输出模式，要获取输入时设为高电平就能读取输入电平
 *****************************************************************
 */

void switchOut(SimulateI2C i2c)
{ // 引脚切换到输出模式
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  GPIO_InitStruct.Pin = GETPIN(i2c.SDAPin);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(i2c.SDAPort, &GPIO_InitStruct);
}

void switchIn(SimulateI2C i2c)
{ // 引脚切换到输入模式
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  GPIO_InitStruct.Pin = GETPIN(i2c.SDAPin);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(i2c.SDAPort, &GPIO_InitStruct);
}


/*
 ****************************************************************
 * @Brief    :  产生I2C开始信号
 * @Param    :  [IN]SimulateI2C     产生动作的I2C
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */

void SimulateI2C_Start(SimulateI2C i2c)
{
    switchOut(i2c);
    HAL_GPIO_WritePin(i2c.SDAPort, GETPIN(i2c.SDAPin),GPIO_PIN_SET);
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_SET);
    delay_nus(I2C_DELAY_SCL_US);
    HAL_GPIO_WritePin(i2c.SDAPort, GETPIN(i2c.SDAPin),GPIO_PIN_RESET);
    delay_nus(I2C_DELAY_SCL_US);
    HAL_GPIO_WritePin(i2c.SCLPort,GETPIN(i2c.SCLPin),GPIO_PIN_RESET);
}

/*
 ****************************************************************
 * @Brief    :  产生I2C停止信号
 * @Param    :  [IN]SimulateI2C     产生动作的I2C
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */

void SimulateI2C_Stop(SimulateI2C i2c)
{
    switchOut(i2c);
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_RESET);
    HAL_GPIO_WritePin(i2c.SDAPort, GETPIN(i2c.SDAPin),GPIO_PIN_RESET);
    delay_nus(I2C_DELAY_SCL_US);
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_SET);
    HAL_GPIO_WritePin(i2c.SDAPort, GETPIN(i2c.SDAPin),GPIO_PIN_SET);
    delay_nus(I2C_DELAY_SCL_US);
}


/*
 ****************************************************************
 * @Brief    :  等待应答信号到来
 * @Param    :  [IN]SimulateI2C
 * @Return   :  1，接收应答失败
 *              0，接收应答成功
 * @Relative :
 * @Note     :
 *****************************************************************
 */
uint8_t SimulateI2C_WaitAck(SimulateI2C i2c)
{
    uint8_t ucErrTime=0;
    HAL_GPIO_WritePin(i2c.SDAPort, GETPIN(i2c.SDAPin),GPIO_PIN_SET);
    switchIn(i2c);
    delay_nus(I2C_DELAY_US);
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_SET);
    delay_nus(I2C_DELAY_SCL_US);
    while(HAL_GPIO_ReadPin(i2c.SDAPort, GETPIN(i2c.SDAPin)))
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            SimulateI2C_Stop(i2c);
            return 1;
        }
        HAL_GPIO_ReadPin(i2c.SDAPort,GETPIN(i2c.SDAPin));
    }
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_RESET);
    return 0;
}




/*
 ****************************************************************
 * @Brief    :  产生ACK应答
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */

void SimulateI2C_Ack(SimulateI2C i2c)
{
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_RESET);
    switchOut(i2c);
    HAL_GPIO_WritePin(i2c.SDAPort, GETPIN(i2c.SDAPin),GPIO_PIN_RESET);
    delay_nus(I2C_DELAY_SCL_US);
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_SET);
    delay_nus(I2C_DELAY_SCL_US);
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_RESET);
}


/*
 ****************************************************************
 * @Brief    :  不产生ACK应答
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */

void SimulateI2C_NAck(SimulateI2C i2c)
{

    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_RESET);
    switchOut(i2c);
    HAL_GPIO_WritePin(i2c.SDAPort, GETPIN(i2c.SDAPin),GPIO_PIN_SET);
    delay_nus(I2C_DELAY_SCL_US);
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_SET);
    delay_nus(I2C_DELAY_SCL_US);
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_RESET);
}



/*
 ****************************************************************
 * @Brief    :  发送单字节
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */

void SimulateI2C_SendByte(SimulateI2C i2c,uint8_t txd)
{
    uint8_t t;
    switchOut(i2c);
    HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_RESET);
    delay_nus(I2C_DELAY_SCL_US);
    for(t=0;t<8;t++)
    {
			HAL_GPIO_WritePin(i2c.SDAPort, GETPIN(i2c.SDAPin),((txd&0x80)>>7)==0?GPIO_PIN_RESET:GPIO_PIN_SET);
        txd<<=1;
        delay_nus(I2C_DELAY_SCL_US);
        HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_SET);
        delay_nus(I2C_DELAY_SCL_US);
        HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_RESET);
        delay_nus(I2C_DELAY_SCL_US);
    }
}


//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
/*
 ****************************************************************
 * @Brief    :  接收单字节
 * @Param    :  [IN]unsigned char   ack=1,发送ACK;ack=0,发送nACK
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */

uint8_t SimulateI2C_RecvByte(SimulateI2C i2c, unsigned char ack)
{
    unsigned char i,receive=0;
    switchIn(i2c);
    for(i=0;i<8;i++ )
    {
        HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_RESET);
        delay_nus(I2C_DELAY_SCL_US);
        HAL_GPIO_WritePin(i2c.SCLPort, GETPIN(i2c.SCLPin),GPIO_PIN_SET);
        receive<<=1;
        if(HAL_GPIO_ReadPin(i2c.SDAPort, GETPIN(i2c.SDAPin)))
            receive++;
        delay_nus(I2C_DELAY_SCL_US);
  }
    if (!ack)
            SimulateI2C_NAck(i2c);//发送nACK
    else
            SimulateI2C_Ack(i2c); //发送ACK
    return receive;
}


/*
 ****************************************************************
 * @Brief    :  I2C写功能
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */
void SimulateI2C_WriteByte(SimulateI2C i2c,uint8_t addr,uint8_t device_addr,uint8_t numToWrite,uint8_t* buf)
{
    SimulateI2C_Start(i2c);
    SimulateI2C_SendByte(i2c,(device_addr<<1));        //发器件地址
    SimulateI2C_WaitAck(i2c);
    SimulateI2C_SendByte(i2c,addr&0xFF);               //发送寄存器地址
    SimulateI2C_WaitAck(i2c);
    int i;
    for(i=0;i<numToWrite;i++)
    {
        SimulateI2C_SendByte(i2c,buf[i]);     //发送字节
        SimulateI2C_WaitAck(i2c);

    }
    SimulateI2C_Stop(i2c);//产生一个停止条件
    delay_nus(2);
}

/*
 ****************************************************************
 * @Brief    :  I2C读功能
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */
void SimulateI2C_ReadByte(SimulateI2C i2c,uint8_t addr,uint8_t device_addr,uint8_t numToRead,uint8_t *buf)  //读寄存器或读数据
{
    SimulateI2C_Start(i2c);
    SimulateI2C_SendByte(i2c,(device_addr<<1));      //发送写指令
    SimulateI2C_WaitAck(i2c);
    SimulateI2C_SendByte(i2c,addr&0xFF);            //写寄存器地址
    SimulateI2C_WaitAck(i2c);
    SimulateI2C_Start(i2c);                         //开始信号
    SimulateI2C_SendByte(i2c,(device_addr<<1)+1);   //发送读指令
    SimulateI2C_WaitAck(i2c);
    int i;
    for(i=0;i<numToRead;i++)
    {
        if(i!=numToRead-1)
            buf[i]=SimulateI2C_RecvByte(i2c,1);
        else
            buf[i]=SimulateI2C_RecvByte(i2c,0);
    }
    SimulateI2C_Stop(i2c);//产生一个停止条件
}
