/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_color_led.c
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2023.8.03      v01         百问科技      创建文件
 *--------------------------------------------------
*/

#include "driver_color_led.h"

#include "stm32f1xx_hal.h"
#include "cmsis_version.h"

extern TIM_HandleTypeDef htim2;

#define COLOR_LED_R TIM_CHANNEL_3
#define COLOR_LED_G TIM_CHANNEL_1
#define COLOR_LED_B TIM_CHANNEL_2

void ColorLED_Init(void)
{
	//MX_TIM2_Init();
	HAL_TIM_PWM_Start(&htim2, COLOR_LED_R);
	HAL_TIM_PWM_Start(&htim2, COLOR_LED_G);
	HAL_TIM_PWM_Start(&htim2, COLOR_LED_B);
}

/* 0x00RRGGBB */
void ColorLED_SetColor(uint32_t color)
{
  TIM_OC_InitTypeDef sConfigOC_R = {0};
	TIM_OC_InitTypeDef sConfigOC_G = {0};
	TIM_OC_InitTypeDef sConfigOC_B = {0};

  sConfigOC_R.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_R.Pulse = ((color >> 16) & 0xff)*1999/255;  /* 最大值1999 */
  sConfigOC_R.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC_R.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_R, COLOR_LED_R) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC_G.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_G.Pulse = ((color >> 8) & 0xff)*1999/255;  /* 最大值1999 */
  sConfigOC_G.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC_G.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_G, COLOR_LED_G) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC_B.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_B.Pulse = ((color >> 0) & 0xff)*1999/255;  /* 最大值1999 */
  sConfigOC_B.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC_B.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_B, COLOR_LED_B) != HAL_OK)
  {
    Error_Handler();
  }
	
}
