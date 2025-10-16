/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_spi_flash.c
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2023.8.03      v01         百问科技      创建文件
 *--------------------------------------------------
*/

#include "driver_spi_flash.h"

#include "stm32f1xx_hal.h"

#define SPI_FLASH_TIMEOUT 1000

void Wait_SPI1_TxCplt(int timeout);
void Wait_SPI1_TxRxCplt(int timeout);
void Wait_SPI1_RxCplt(int timeout);


extern SPI_HandleTypeDef hspi1;

/* 内部函数  */
static void SPIFlash_Select(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
}

static void SPIFlash_DeSelect(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
}

static int SPIFlash_WriteEnable(void)
{
	uint8_t buf[1] = {0x06};
	SPIFlash_Select();
	HAL_SPI_Transmit_IT(&hspi1, buf, 1);
	Wait_SPI1_TxCplt(SPI_FLASH_TIMEOUT);
	SPIFlash_DeSelect();
}

static int SPIFlash_ReadStatus(void)
{
	uint8_t txbuf[2] = {0x05, 0xff};
	uint8_t rxbuf[2] = {0, 0};
	
	SPIFlash_Select();
	HAL_SPI_TransmitReceive_IT(&hspi1, txbuf, rxbuf, 2);
	Wait_SPI1_TxRxCplt(SPI_FLASH_TIMEOUT);
	SPIFlash_DeSelect();
	return rxbuf[1];
}

static int SPIFlash_WaitReady(void)
{
	while (SPIFlash_ReadStatus() & 1 == 1);
}

/* 对外的函数 */

int SPIFlash_ReadID(void)
{
	uint8_t txbuf[2] = {0x9F, 0xff};
	uint8_t rxbuf[2] = {0, 0};
	
	SPIFlash_Select();
	HAL_SPI_TransmitReceive_IT(&hspi1, txbuf, rxbuf, 2);
	Wait_SPI1_TxRxCplt(SPI_FLASH_TIMEOUT);
	SPIFlash_DeSelect();
	return rxbuf[1];
}

int SPIFlash_EraseSector(uint32_t addr)
{
	uint8_t txbuf[4] = {0x20};
	
	/* write enable */
	SPIFlash_WriteEnable();
	
	/* erase */
	txbuf[1]= (addr>>16) & 0xff;
	txbuf[2]= (addr>>8) & 0xff;
	txbuf[3]= (addr>>0) & 0xff;

	SPIFlash_Select();
	HAL_SPI_Transmit_IT(&hspi1, txbuf, 4);
	Wait_SPI1_TxCplt(SPI_FLASH_TIMEOUT);
	SPIFlash_DeSelect();
	
	/* wait ready */
	SPIFlash_WaitReady();
	
	return 0;
}

int SPIFlash_Write(uint32_t addr, uint8_t *datas, uint32_t len)
{
	uint8_t txbuf[4] = {0x02};
	
	/* write enable */
	SPIFlash_WriteEnable();
	
	/* program */
	txbuf[1]= (addr>>16) & 0xff;
	txbuf[2]= (addr>>8) & 0xff;
	txbuf[3]= (addr>>0) & 0xff;

	SPIFlash_Select();
	
	/* 发送命令和地址 */
	HAL_SPI_Transmit_IT(&hspi1, txbuf, 4);
	Wait_SPI1_TxCplt(SPI_FLASH_TIMEOUT);
	
	/* 发送数据 */
	HAL_SPI_Transmit_IT(&hspi1, datas, len);
	Wait_SPI1_TxCplt(SPI_FLASH_TIMEOUT);
	
	SPIFlash_DeSelect();
	
	/* wait ready */
	SPIFlash_WaitReady();
	
	return 0;
}

int SPIFlash_Read(uint32_t addr, uint8_t *datas, uint32_t len)
{
	uint8_t txbuf[4] = {0x03};
		
	/* read */
	txbuf[1]= (addr>>16) & 0xff;
	txbuf[2]= (addr>>8) & 0xff;
	txbuf[3]= (addr>>0) & 0xff;

	SPIFlash_Select();
	
	/* 发送命令和地址 */
	HAL_SPI_Transmit_IT(&hspi1, txbuf, 4);
	Wait_SPI1_TxCplt(SPI_FLASH_TIMEOUT);
	
	/* 读取数据 */
	HAL_SPI_Receive_IT(&hspi1, datas, len);
	Wait_SPI1_RxCplt(SPI_FLASH_TIMEOUT);
	
	SPIFlash_DeSelect();
		
	return 0;
}
