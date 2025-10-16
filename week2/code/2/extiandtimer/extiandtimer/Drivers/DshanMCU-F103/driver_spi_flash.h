/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_spi_flash.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.8.18      v01         百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_SPI_FLASH_H
#define __DRIVER_SPI_FLASH_H

#include <stdint.h>

int SPIFlash_ReadID(void);
int SPIFlash_EraseSector(uint32_t addr);
int SPIFlash_Write(uint32_t addr, uint8_t *datas, uint32_t len);
int SPIFlash_Read(uint32_t addr, uint8_t *datas, uint32_t len);


#endif /* __DRIVER_SPI_FLASH_H */

