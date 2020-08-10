/*
 ***********************************************************************************************
 *                                      SHENZHEN QQZM
 *
 * @File    :   simulate_spi.c
 * @Brief   :   IO模拟spi
 * @Author  :   Berecho Xiong
 * @Date    :
 * @Version :
 * @Note    :   已使用SysTick->VAL校准速率：
 *              对于芯片STM32F407,SYSCLK=168Mhz时，SCK≈1Mhz 传输速率约为：119KB/S
 ***********************************************************************************************
 */

/*
 *******************************************************************
 *                          Inc File
 *******************************************************************
 */
#include "simulate_spi.h"

/****************** SPI1 *******************/
#if SIMULATE_SPI_GROUP_SIZE > 0

SimulateSpi sspi1 = {
    SPI1_NSS_GPIO_Port,       //NSSPORT
    SPI1_SCK_GPIO_Port,       //SCKPORT
    SPI1_MISO_GPIO_Port,       //MISOPORT
    SPI1_MOSI_GPIO_Port,       //MOSIPORT
    SPI1_NSS_Pin, //NSSPIN
    SPI1_SCK_Pin, //SCKPIN
    SPI1_MISO_Pin, //MISOPIN
    SPI1_MOSI_Pin, //MOSIPIN
    100          //delayUS
};
#endif

/****************** SPI2 *******************/
#if SIMULATE_SPI_GROUP_SIZE > 1
SimulateSpi sspi2 = {
    SPI2_NSS_GPIO_Port,       //NSSPORT
    SPI1_SCK_GPIO_Port,       //SCKPORT
    SPI1_MISO_GPIO_Port,       //MISOPORT
    SPI1_MOSI_GPIO_Port,       //MOSIPORT
    SPI2_NSS_Pin, //NSSPIN
    SPI1_SCK_Pin, //SCKPIN
    SPI1_MISO_Pin, //MISOPIN
    SPI1_MOSI_Pin, //MOSIPIN
    100          //delayUS
};
#endif

/****************** SPI3 *******************/
#if SIMULATE_SPI_GROUP_SIZE > 2
SimulateSpi sspi3 = {
    GPIOC,       //NSSPORT
    GPIOC,       //SCKPORT
    GPIOC,       //MISOPORT
    GPIOD,       //MOSIPORT
    GPIO_PIN_0,  //NSSPIN
    GPIO_PIN_10, //SCKPIN
    GPIO_PIN_11, //MISOPIN
    GPIO_PIN_12, //MOSIPIN
    100          //delayUS
};
#endif
/****************** SPI4 *******************/
#if SIMULATE_SPI_GROUP_SIZE > 3
SimulateSpi sspi4 = {
    GPIOE,      //NSSPORT
    GPIOE,      //SCKPORT
    GPIOE,      //MISOPORT
    GPIOE,      //MOSIPORT
    GPIO_PIN_2, //NSSPIN
    GPIO_PIN_3, //SCKPIN
    GPIO_PIN_4, //MISOPIN
    GPIO_PIN_5, //MOSIPIN
    100         //delayUS
};
#endif
/****************** SPI5 *******************/
#if SIMULATE_SPI_GROUP_SIZE > 4
SimulateSpi sspi5 = {
    GPIOE,       //NSSPORT
    GPIOE,       //SCKPORT
    GPIOE,       //MISOPORT
    GPIOE,       //MOSIPORT
    GPIO_PIN_8,  //NSSPIN
    GPIO_PIN_9,  //SCKPIN
    GPIO_PIN_10, //MISOPIN
    GPIO_PIN_11, //MOSIPIN
    100          //delayUS
};
#endif
/****************** SPI6 *******************/
#if SIMULATE_SPI_GROUP_SIZE > 5
SimulateSpi sspi6 = {
    GPIOE,       //NSSPORT
    GPIOE,       //SCKPORT
    GPIOE,       //MISOPORT
    GPIOE,       //MOSIPORT
    GPIO_PIN_12, //NSSPIN
    GPIO_PIN_13, //SCKPIN
    GPIO_PIN_14, //MISOPIN
    GPIO_PIN_15, //MOSIPIN
    100          //delayUS
};
#endif
/****************** SPI7 *******************/
#if SIMULATE_SPI_GROUP_SIZE > 6
SimulateSpi sspi7 = {
    GPIOD,       //NSSPORT
    GPIOD,       //SCKPORT
    GPIOD,       //MISOPORT
    GPIOD,       //MOSIPORT
    GPIO_PIN_8,  //NSSPIN
    GPIO_PIN_9,  //SCKPIN
    GPIO_PIN_11, //MISOPIN
    GPIO_PIN_10, //MOSIPIN
    100          //delayUS
};
#endif

/****************** SPI8 *******************/
#if SIMULATE_SPI_GROUP_SIZE > 7
SimulateSpi sspi8 = {
    GPIOD,       //NSSPORT
    GPIOD,       //SCKPORT
    GPIOD,       //MISOPORT
    GPIOD,       //MOSIPORT
    GPIO_PIN_12, //NSSPIN
    GPIO_PIN_13, //SCKPIN
    GPIO_PIN_15, //MISOPIN
    GPIO_PIN_14, //MOSIPIN
    100          //delayUS
};
#endif

/*
 ****************************************************************
 * @Brief    :  延迟，控制SPI通信速率
 * @Param    :  [IN]unsigned int - 延迟倍数
 * @Return   :  无
 * @Relative :
 * @Note     :  延迟亦可通过硬件定时器完成
 *              已使用Systick校准，当前SCK=1Mhz
 *****************************************************************
 */
void delay(unsigned int n)
{
    int j;
    while (n--)
    {
        j = STM32F407_168MHZ_SCK_1MHZ_DELAY;
        while (j--)
            ;
    }
}

void SimulateSpi_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI1_NSS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SPI2_NSS_Pin|SPI1_SCK_Pin|SPI1_MOSI_Pin, GPIO_PIN_SET);
	
  
  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = SPI2_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin 
                           PBPin PBPin */
  GPIO_InitStruct.Pin = SPI1_SCK_Pin|SPI1_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SPI1_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SPI1_MISO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SPI1_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI1_NSS_GPIO_Port, &GPIO_InitStruct);
}
/*
 ****************************************************************
 * @Brief    :  模式零，写数据
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :  未修改，不可用
 *****************************************************************
 */
#if _CPOL == 0 && _CPHA == 0 //MODE   0  0
void SoftSpi_WriteByte(uint8_t data)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        RESET(SCK);
        if (data & 0x80)
            SET(MOSI);
        else
            RESET(MOSI);
        data <<= 1;
        SET(SCK);
    }
    RESET(SCK);
}

/*
 ****************************************************************
 * @Brief    :  模式零，读数据
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :  未修改，不可用
 *****************************************************************
 */

uint8_t SoftSpi_ReadByte()
{
    uint8_t i, data;
    SET(MOSI);
    for (i = 0; i < 8; i++)
    {
        RESET(SCK);
        data <<= 1;
        if (READ(MISO))
            data |= 0x01;
        else
            data &= 0xfe;
        SET(SCK);
    }
    RESET(SCK);
    return data;
}
#endif

/*
 ****************************************************************
 * @Brief    : 模式二，写数据
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     : 未修改，不可用
 *****************************************************************
 */

#if _CPOL == 1 && _CPHA == 0 //MODE   1  0
void SPI_Send_Dat(unsigned char dat)
{
    unsigned char n;
    for (n = 0; n < 8; n++)
    {
        SCK_D(1);
        if (dat & 0x80)
            MOSI_D(1);
        else
            MOSI_D(0);
        dat <<= 1;
        SCK_D(0);
    }
    SCK_D(1);
}

/*
 ****************************************************************
 * @Brief    :  模式二，读数据
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :  未修改，不可用
 *****************************************************************
 */

unsigned char SPI_Receiver_Dat(void)
{
    unsigned char n, dat, bit_t;
    for (n = 0; n < 8; n++)
    {
        SCK_D(1);
        dat <<= 1;
        if (MISO_I())
            dat |= 0x01;
        else
            dat &= 0xfe;
        SCK_D(0);
    }
    SCK_D(1);
    return dat;
}

#endif

/*
 ****************************************************************
 * @Brief    :  模式一，写数据
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :  未修改，不可用
 *****************************************************************
 */

#if _CPOL == 0 && _CPHA == 1 //MODE  0  1
void SPI_Send_Dat(unsigned char dat)
{
    unsigned char n;
    SCK_D(0);
    for (n = 0; n < 8; n++)
    {
        SCK_D(1);
        if (dat & 0x80)
            MOSI_D(1);
        else
            MOSI_D(0);
        dat <<= 1;
        SCK_D(0);
    }
}

/*
 ****************************************************************
 * @Brief    :  模式一，读数据
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :  未修改，不可用
 *****************************************************************
 */

unsigned char SPI_Receiver_Dat(void)
{
    unsigned char n, dat, bit_t;
    for (n = 0; n < 8; n++)
    {
        SCK_D(1);
        dat <<= 1;
        if (MISO_I())
            dat |= 0x01;
        else
            dat &= 0xfe;
        SCK_D(0);
    }
    SCK_D(0);
    return dat;
}
#endif

/*
 ****************************************************************
 * @Brief    :  模式三，片选使能
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */
#if _CPOL == 1 && _CPHA == 1 //MODE  1  1

void SimulateSpi_Enable(SimulateSpi spi)
{
    HAL_GPIO_WritePin(spi.SCKPort, spi.SCKPin, GPIO_PIN_SET);
    //SET(SPI1_SCK_GPIO);
    // delay(spi.delayUS);
    HAL_GPIO_WritePin(spi.NSSPort, spi.NSSPin, GPIO_PIN_RESET);
    //RESET(SPI1_nSCS_GPIO);
    //delay(spi.delayUS);
}

/*
 ****************************************************************
 * @Brief    :  模式三，片选失能
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */

void SimulateSpi_Disable(SimulateSpi spi)
{
    HAL_GPIO_WritePin(spi.SCKPort, spi.SCKPin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(spi.NSSPort, spi.NSSPin, GPIO_PIN_SET);
    // delay(spi.delayUS);
}

/*
 ****************************************************************
 * @Brief    :  模式三，写数据
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */
void SimulateSpi_WriteByte(SimulateSpi spi, uint8_t data)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (data & 0x80)
            HAL_GPIO_WritePin(spi.MOSIPort, spi.MOSIPin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(spi.MOSIPort, spi.MOSIPin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(spi.SCKPort, spi.SCKPin, GPIO_PIN_RESET);
        delay(spi.delayUS);
        data <<= 1;
        HAL_GPIO_WritePin(spi.SCKPort, spi.SCKPin, GPIO_PIN_SET);
        delay(spi.delayUS);
    }
}

/*
 ****************************************************************
 * @Brief    :  模式三，读数据
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */

uint8_t SimluateSpi_ReadByte(SimulateSpi spi)
{
    uint8_t i, data;

    for (i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(spi.SCKPort, spi.SCKPin, GPIO_PIN_RESET);
        delay(spi.delayUS);
        data <<= 1;
        if (HAL_GPIO_ReadPin(spi.MISOPort, spi.MISOPin))
            data |= 0x01;
        else
            data &= 0xfe;
        HAL_GPIO_WritePin(spi.SCKPort, spi.SCKPin, GPIO_PIN_SET);
        delay(spi.delayUS);
    }
    return data;
}

/*
 ****************************************************************
 * @Brief    :  模式三，读写数据
 * @Param    :
 * @Return   :
 * @Relative :
 * @Note     :
 *****************************************************************
 */

uint8_t SimulateSpi_ReadWriteByte(SimulateSpi spi, uint8_t data)
{
    uint8_t i, rxdata;
    //start
    for (i = 0; i < 8; i++)
    {

        HAL_GPIO_WritePin(spi.SCKPort, spi.SCKPin, GPIO_PIN_RESET);
        //start sck
        delay(1);
        rxdata <<= 1;
        if (data & 0x80)
            HAL_GPIO_WritePin(spi.MOSIPort, spi.MOSIPin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(spi.MOSIPort, spi.MOSIPin, GPIO_PIN_RESET);
        data <<= 1;
        if (HAL_GPIO_ReadPin(spi.MISOPort, spi.MISOPin))
            rxdata |= 0x01;
        else
            rxdata &= 0xfe;
        HAL_GPIO_WritePin(spi.SCKPort, spi.SCKPin, GPIO_PIN_SET);
        delay(1);
        //end sck
    }
    //end
    return rxdata;
}

#endif
