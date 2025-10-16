/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_oled.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.8.18      v01         百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_COLOR_LED_H
#define __DRIVER_COLOR_LED_H

#include <stdint.h>

void ColorLED_Init(void);

void ColorLED_SetColor(uint32_t color);

#endif /* __DRIVER_COLOR_LED_H */

