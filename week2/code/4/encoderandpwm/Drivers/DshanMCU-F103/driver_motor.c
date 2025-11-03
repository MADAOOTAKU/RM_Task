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
#include "driver_oled.h"
#include "driver_motor.h"
#include "driver_lcd.h"
#include "driver_timer.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "driver_key.h"

int nowspeed,tagertspeed;//当前速度
int nowencoder;
float speed = 0;

extern float speed;

#define MOTOR_A_IN1_GPIO_GROUP GPIOA
#define MOTOR_A_IN1_GPIO_PIN   GPIO_PIN_4

#define MOTOR_A_IN2_GPIO_GROUP GPIOA
#define MOTOR_A_IN2_GPIO_PIN   GPIO_PIN_0

#define MOTOR_B_IN1_GPIO_GROUP GPIOB
#define MOTOR_B_IN1_GPIO_PIN   GPIO_PIN_4

#define MOTOR_B_IN2_GPIO_GROUP GPIOB
#define MOTOR_B_IN2_GPIO_PIN   GPIO_PIN_15

#define MOTOR_B_IN1_CHANNEL    TIM_CHANNEL_1
#define MOTOR_B_IN2_CHANNEL    TIM_CHANNEL_3

extern TIM_HandleTypeDef htim3;
static TIM_HandleTypeDef *g_HPWM_BIN1 = &htim3;

//extern TIM_HandleTypeDef htim1;
//static TIM_HandleTypeDef *g_HPWM_BIN2 = &htim1;

//pid计算函数
float pid(float err,float kp,float ki,float kd)
{
	static float out_speed;
	
	static float last_err,last_last_err;
	
	out_speed+=(err-last_err)*kp+err*ki+kd*(err-2*last_err+last_last_err);		
	
	if(out_speed>99)
	{
		out_speed=99;
	}
	else if(out_speed<-99)
	{
		out_speed=-99;
	}
		
	last_last_err=last_err;
	
	last_err=err;
	
	return out_speed;
}


/**********************************************************************
 * 函数名：Motor_PWM_SetDuty
 * 功能描述：初始化Motor
 * 输入参数：channel - MOTOR_A 或 MOTOR_B
 * 输出参数：无
 * 返回值：无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2023/08/05        V1.0     韦东山       创建
 ***********************************************************************/
static void Motor_PWM_SetSpeed(int32_t speed)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    TIM_OC_InitTypeDef sConfig = {0};
    
    if (speed == 0)
    {
        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(MOTOR_B_IN1_GPIO_GROUP, MOTOR_B_IN1_GPIO_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_B_IN2_GPIO_GROUP, MOTOR_B_IN2_GPIO_PIN, GPIO_PIN_SET);

        /*Configure GPIO pins*/
        GPIO_InitStruct.Pin = MOTOR_B_IN1_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(MOTOR_B_IN1_GPIO_GROUP, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = MOTOR_B_IN2_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(MOTOR_B_IN2_GPIO_GROUP, &GPIO_InitStruct);        
    }
    else if (speed > 0)
    {
        /* B_IN1 设置为PWM,  B_IN2设置为输出并且输出0 */
        /* B_IN1 */

        /* 配置B_IN1为 PWM引脚 */
        GPIO_InitStruct.Pin = MOTOR_B_IN1_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(MOTOR_B_IN1_GPIO_GROUP, &GPIO_InitStruct);
        
        /* 配置B_IN1的PWM占空比 */
        
        sConfig.OCMode = TIM_OCMODE_PWM1;         // PWM 输出的两种模式:PWM1 当极性为低,CCR<CNT,输出低电平,反之高电平
        sConfig.OCPolarity = TIM_OCPOLARITY_HIGH; // 设置极性为高(硬件上BIN1为高,BIN2为低电平让motor正转)
        sConfig.OCFastMode = TIM_OCFAST_DISABLE;  // 输出比较快速使能禁止(仅在 PWM1 和 PWM2 可设置)
        sConfig.Pulse = speed*10;                 // 周期是1000, speed取值0~99

        HAL_TIM_PWM_Stop(g_HPWM_BIN1, MOTOR_B_IN1_CHANNEL);        
        HAL_TIM_PWM_ConfigChannel(g_HPWM_BIN1, &sConfig, MOTOR_B_IN1_CHANNEL);
        HAL_TIM_PWM_Start(g_HPWM_BIN1, MOTOR_B_IN1_CHANNEL);

        /* B_IN2 */
        HAL_GPIO_WritePin(MOTOR_B_IN2_GPIO_GROUP, MOTOR_B_IN2_GPIO_PIN, GPIO_PIN_RESET);
        GPIO_InitStruct.Pin = MOTOR_B_IN2_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(MOTOR_B_IN2_GPIO_GROUP, &GPIO_InitStruct);        
    }
    else
    {
        speed = 0 - speed;
        /* B_IN1 设置为PWM,  B_IN2设置为输出并且输出1 */
        /* B_IN1 */

        /* 配置B_IN1为 PWM引脚 */
        GPIO_InitStruct.Pin = MOTOR_B_IN1_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(MOTOR_B_IN1_GPIO_GROUP, &GPIO_InitStruct);
        
        /* 配置B_IN1的PWM占空比 */
        
        sConfig.OCMode = TIM_OCMODE_PWM1;         // PWM 输出的两种模式:PWM1 当极性为低,CCR<CNT,输出低电平,反之高电平
        sConfig.OCPolarity = TIM_OCPOLARITY_HIGH; // 设置极性为高(硬件上BIN2为高,BIN1为低电平让motor反转)
        sConfig.OCFastMode = TIM_OCFAST_DISABLE;  // 输出比较快速使能禁止(仅在 PWM1 和 PWM2 可设置)
        sConfig.Pulse = (99 - speed)*10;          // 周期是1000, speed取值0~99

        HAL_TIM_PWM_Stop(g_HPWM_BIN1, MOTOR_B_IN1_CHANNEL);        
        HAL_TIM_PWM_ConfigChannel(g_HPWM_BIN1, &sConfig, MOTOR_B_IN1_CHANNEL);
        HAL_TIM_PWM_Start(g_HPWM_BIN1, MOTOR_B_IN1_CHANNEL);

        /* B_IN2 */
        HAL_GPIO_WritePin(MOTOR_B_IN2_GPIO_GROUP, MOTOR_B_IN2_GPIO_PIN, GPIO_PIN_SET);
        GPIO_InitStruct.Pin = MOTOR_B_IN2_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(MOTOR_B_IN2_GPIO_GROUP, &GPIO_InitStruct);        
    }
}


/**********************************************************************
 * 函数名：Motor_Init
 * 功能描述：初始化Motor
 * 输入参数：channel - MOTOR_A 或 MOTOR_B
 * 输出参数：无
 * 返回值：无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2023/08/05        V1.0     韦东山       创建
 ***********************************************************************/
void Motor_Init(int32_t channel)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    if (MOTOR_A == channel)
    {
        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(MOTOR_A_IN1_GPIO_GROUP, MOTOR_A_IN1_GPIO_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_A_IN2_GPIO_GROUP, MOTOR_A_IN2_GPIO_PIN, GPIO_PIN_SET);

        /*Configure GPIO pins*/
        GPIO_InitStruct.Pin = MOTOR_A_IN1_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(MOTOR_A_IN1_GPIO_GROUP, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = MOTOR_A_IN2_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(MOTOR_A_IN2_GPIO_GROUP, &GPIO_InitStruct);        
    }

    if (MOTOR_B == channel)
    {
        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(MOTOR_B_IN1_GPIO_GROUP, MOTOR_B_IN1_GPIO_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_B_IN2_GPIO_GROUP, MOTOR_B_IN2_GPIO_PIN, GPIO_PIN_SET);

        /*Configure GPIO pins*/
        GPIO_InitStruct.Pin = MOTOR_B_IN1_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(MOTOR_B_IN1_GPIO_GROUP, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = MOTOR_B_IN2_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(MOTOR_B_IN2_GPIO_GROUP, &GPIO_InitStruct);        
    }
    
}

/**********************************************************************
 * 函数名：Motor_SetPpeed
 * 功能描述：设置马达速度
 * 输入参数：channel - MOTOR_A 或 MOTOR_B
 *           speed - 取值(-100~100),
 *                   0表示停止, 
 *                   正数表示顺时针旋转速度, 
 *                   负数表示逆时钟旋转速度
 * 输出参数：无
 * 返回值：无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2023/08/05        V1.0     韦东山       创建
 ***********************************************************************/
void Motor_SetSpeed(int32_t channel, int32_t speed)
{
    if (speed > 99)
        speed = 99;
    else if (speed < -99)
        speed = -99;
    
    if (MOTOR_A == channel)
    {
        if (0 == speed)
        {
            HAL_GPIO_WritePin(MOTOR_A_IN1_GPIO_GROUP, MOTOR_A_IN1_GPIO_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MOTOR_A_IN2_GPIO_GROUP, MOTOR_A_IN2_GPIO_PIN, GPIO_PIN_SET);
        }
        else if (0 < speed)
        {
            HAL_GPIO_WritePin(MOTOR_A_IN1_GPIO_GROUP, MOTOR_A_IN1_GPIO_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MOTOR_A_IN2_GPIO_GROUP, MOTOR_A_IN2_GPIO_PIN, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(MOTOR_A_IN1_GPIO_GROUP, MOTOR_A_IN1_GPIO_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_A_IN2_GPIO_GROUP, MOTOR_A_IN2_GPIO_PIN, GPIO_PIN_SET);
        }
    }

    if (MOTOR_B == channel)
    {
        Motor_PWM_SetSpeed(speed);
    }
}


/**
 * @brief RM任务4-速度环
 * 
 */
void Motor_Test(void)
{

    //测速变量
    int a = 0;
    int lasta;

    int len = 0;
    int key=0;
	int mode=1;
	int outspeed=0;
	float err=0;

    mdelay(50);//每50ms执行一次
    
    key=Key_Read();
    
    if(key==1)
    {			
        while(Key_Read());
        mode++;
        if(mode>3)
        {
            mode=1;
        }
    }

    RotaryEncoder_Read(&a);
    
    //speed即为编码器量的增量——单位为count的"速度"
    speed=a-lasta;
    lasta=a;
    
    speed=speed*20.0/270.0;//转每秒
    //20.0因为上面延时50ms，所以1秒执行20次
    //270是编码器的每转脉冲数，问客服得到的

    len +=LCD_PrintSignedVal(0,0,speed);
    LCD_ClearLine(len, 0);

    len +=LCD_PrintSignedVal(0,2,a);
    LCD_ClearLine(len, 2);
                            
    
    if(mode==1)
    {
        err=1.0-speed;

    }
    else if(mode==2)
    {
        err=2.0-speed;

    }
    else if(mode==3)
    {
        err=3.0-speed;

    }

    outspeed=pid(err,5,0.5,2.2);	
    
    Motor_SetSpeed(MOTOR_B, -outspeed);//编码器方向和电机运动方向相反
    //即，电机往前走，编码器计数值为负，所以这里要给负的速度，这才是期望的向前速度
    //不然的话，编码器参与的每一次反馈都变成”负反馈“都变成正反馈，电机就会疯转
}

