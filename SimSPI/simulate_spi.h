#ifndef _SSPI_H_
#define _SSPI_H_

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
#define STM32F407_168MHZ_SCK_1MHZ_DELAY 6
#define SIMULATE_SPI_GROUP_SIZE 2

#define _CPOL   1
#define _CPHA   1

#define SPI2_NSS_Pin GPIO_PIN_12
#define SPI2_NSS_GPIO_Port GPIOB
#define SPI1_SCK_Pin GPIO_PIN_13
#define SPI1_SCK_GPIO_Port GPIOB
#define SPI1_MISO_Pin GPIO_PIN_14
#define SPI1_MISO_GPIO_Port GPIOB
#define SPI1_MOSI_Pin GPIO_PIN_15
#define SPI1_MOSI_GPIO_Port GPIOB
#define SPI1_NSS_Pin GPIO_PIN_8
#define SPI1_NSS_GPIO_Port GPIOA

/*
 ********************************************************************************
 *                               Type Definition
 ********************************************************************************
*/
typedef struct{
     GPIO_TypeDef* const NSSPort;
     GPIO_TypeDef* const SCKPort;
     GPIO_TypeDef* const MISOPort;
     GPIO_TypeDef* const MOSIPort;
     const uint16_t NSSPin;
     const uint16_t SCKPin;
     const uint16_t MISOPin;
     const uint16_t MOSIPin;
     const uint32_t delayUS;
}SimulateSpi;

extern SimulateSpi sspi1;
extern SimulateSpi sspi2;

/*
 ********************************************************************************
 *                               Func Declare
 ********************************************************************************
*/
void SimulateSpi_init(void);
void SimulateSpi_Enable(SimulateSpi spi);
void SimulateSpi_Disable(SimulateSpi spi);
void delay_nus(uint32_t n);
void SimulateSpiSpi_WriteByte(SimulateSpi spi, uint8_t data);
uint8_t SimulateSpi_ReadByte(SimulateSpi spi);
uint8_t SimulateSpi_ReadWriteByte(SimulateSpi spi, uint8_t data);

#endif
