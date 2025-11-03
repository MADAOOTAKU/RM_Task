// SPDX-License-Identifier: GPL-3.0-only
/*
 * Copyright (c) 2008-2023 100askTeam : Dongshan WEI <weidongshan@qq.com> 
 * Discourse:  https://forums.100ask.net
 */

 
/*  Copyright (C) 2008-2023 深圳百问网科技有限公司
 *  All rights reserved
 *
 *
 * 免责声明: 百问网编写的文档，仅供学员学习使用，可以转发或引用(请保留作者信息)，禁止用于商业用途！
 * 免责声明: 百问网编写的程序，可以用于商业用途，但百问网不承担任何后果！
 * 
 * 
 * 本程序遵循GPL V3协议，使用请遵循协议许可
 * 本程序所用的开发板：	DShanMCU-F103
 * 百问网嵌入式学习平台：https://www.100ask.net
 * 百问网技术交流社区：	https://forums.100ask.net
 * 百问网官方B站：				https://space.bilibili.com/275908810
 * 百问网官方淘宝：			https://100ask.taobao.com
 * 联系我们(E-mail)：	  weidongshan@qq.com
 *
 * 版权所有，盗版必究。
 *  
 * 修改历史     版本号           作者        修改内容
 *-----------------------------------------------------
 * 2023.08.04      v01         百问科技      创建文件
 *-----------------------------------------------------
 */


#include "driver_rotary_encoder.h"
#include "driver_lcd.h"
#include "driver_timer.h"
#include "stm32f1xx_hal.h"
#include "tim.h"

/*
 * PB12 - S1
 * PB0  - S2
 * PB1  - Key
 */
#define ROTARY_ENCODER_S1_GPIO_GROUP GPIOB
#define ROTARY_ENCODER_S1_GPIO_PIN   GPIO_PIN_12

#define ROTARY_ENCODER_S2_GPIO_GROUP GPIOB
#define ROTARY_ENCODER_S2_GPIO_PIN   GPIO_PIN_0

#define ROTARY_ENCODER_KEY_GPIO_GROUP GPIOB
#define ROTARY_ENCODER_KEY_GPIO_PIN   GPIO_PIN_1

static int32_t g_count = 0;






static int RotaryEncoder_Get_S2(void)
{
    if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(ROTARY_ENCODER_S2_GPIO_GROUP, ROTARY_ENCODER_S2_GPIO_PIN))
        return 0;
    else
        return 1;
}


/**
 * @brief GPIO中断输入的方式读取编码器值
 * 
 */
void RotaryEncoder_IRQ_Callback(void)
{
    if (RotaryEncoder_Get_S2())
    {
        g_count++;
    }
    else
    {
        g_count--;
    }

}



void RotaryEncoder_Init(void)
{
    /* PB0,PB1在MX_GPIO_Init中被配置为输入引脚 */
    /* PB12在MX_GPIO_Init中被配置为中断引脚,上升沿触发 */
    //这里保留函数名，为了好看
}

/**
 * @brief 编码器计数在中断回调里更新
 * @note 本函数不再更新计数，只返回当前计数值
 * @param pCnt 
 */
void RotaryEncoder_Read(int32_t *pCnt)
{
    *pCnt = g_count;
}





