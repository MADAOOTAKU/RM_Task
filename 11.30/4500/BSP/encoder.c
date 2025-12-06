#include "encoder.h"


//左电机真实速度(mm/s)
volatile int32_t motor1_speed = 0;
//右电机真实速度(mm/s)
volatile int32_t motor2_speed = 0;

volatile int32_t distance = 0;
/**
 * @brief 编码器初始化
 * 
 */
void encoder_init(void)
{
    // 左编码器
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);

    //右编码器
    HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);

}


/**
 * @brief 获取左电机速度
 * @note 更新全局变量motor1_speed
 */
void Motor1_Get_Speed(void)
{
    int32_t motor1_count = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
    __HAL_TIM_SET_COUNTER(&htim4,0);

    //motor1_speed = (motor1_count * PI * RR) / CC * 0.0875;
    motor1_speed = motor1_count;
}

/**
 * @brief 获取右电机速度
 * @note 更新全局变量motor2_speed
 */
void Motor2_Get_Speed(void)
{
    int32_t motor2_count = (int16_t)__HAL_TIM_GET_COUNTER(&htim3);
    __HAL_TIM_SET_COUNTER(&htim3, 0);

    //motor2_speed = (motor2_count * PI * RR) / CC * 0.0875;
    motor2_speed = motor2_count;

}

/**
 * @brief 测量全部电机速度
 * 
 */
void MEASURE_MOTORS_SPEED(void)
{
    Motor1_Get_Speed();
    Motor2_Get_Speed();
    distance += (motor1_speed + motor2_speed) / 2;
}