#include "motor.h"

uint8_t MOTOR1_ENABLE_FLAG = 0; // 电机使能标志位
uint8_t MOTOR2_ENABLE_FLAG = 0;




/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------PID计算部分部分---------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
// 速度环参数修改
pid_t pid_Motor1_Speed = {
    .Kp = 100.0f,
    .Ki = 20.0f,
    .Kd = 10.0f,
    .Target = 0,
    .Measure = 0,
    {0, 0, 0},
    .KpOut = 0,
    .KiOut = 0,
    .KdOut = 0,
    .PID_Out = 0,
    .PID_Limit_MAX = 9999,
    .Ki_Limit_MAX = 9999,
};
pid_t pid_Motor2_Speed = {
    .Kp = 100.0f,
    .Ki = 20.0f,
    .Kd = 10.0f,
    .Target = 0,
    .Measure = 0,
    {0, 0, 0},
    .KpOut = 0,
    .KiOut = 0,
    .KdOut = 0,
    .PID_Out = 0,
    .PID_Limit_MAX = 9999,
    .Ki_Limit_MAX = 9999,
};
// 转向环参数修改			60cm/s	6	40
pid_t pid_Turn = {
    .Kp = 37.0,
    //.Kp = 40.0,
    .Ki = 0,
    .Kd = 3,
    .Target = 0,
    .Measure = 0,
    {0, 0, 0},
    .KpOut = 0,
    .KiOut = 0,
    .KdOut = 0,
    .PID_Out = 0,
    .PID_Limit_MAX = 9999,
    .Ki_Limit_MAX = 9999,
};

// PWM限幅函数 *a传入要限幅的参数  ABS_MAX限幅大小
void PID_Limit(float *a, float ABS_MAX)
{
    if (*a > ABS_MAX)
        *a = ABS_MAX;
    if (*a < -ABS_MAX)
        *a = -ABS_MAX;
}
/*************************************************************************************************
 *	函 数 名:	PID_Calculate
 *
 *	函数功能:	PID计算公式
 *
 *   参    数：  PID结构体，当前值，目标值
 *************************************************************************************************/
float pid_calculate(pid_t *pid, float Measure, float Target)
{
    pid->Target = Target;             // 赋值目标值
    pid->Measure = Measure;           // 赋值测量值
    pid->Error[0] = Target - Measure; // 计算当前误差

    pid->KpOut = pid->Kp * (pid->Error[0]);                 // 比例计算，Kp*当前误差
    pid->KiOut += pid->Ki * pid->Error[0];                  // 积分计算，Ki*误差累加值
    PID_Limit(&(pid->KiOut), pid->Ki_Limit_MAX);            // 积分限幅
    pid->KdOut = pid->Kd * (pid->Error[0] - pid->Error[1]); // 微分计算，Kd*(当前误差-上一次误差)

    pid->PID_Out = pid->KpOut + pid->KiOut + pid->KdOut; // PID计算
    PID_Limit(&(pid->PID_Out), pid->PID_Limit_MAX);      // PID输出限幅

    pid->Error[2] = pid->Error[1];
    pid->Error[1] = pid->Error[0];

    return pid->PID_Out;
}

// PWM限幅函数 *a传入要限幅的参数  ABS_MAX限幅大小
void PWM_Limit(int *a, int ABS_MAX)
{
    if (*a > ABS_MAX)
        *a = ABS_MAX;
    if (*a < -ABS_MAX)
        *a = -ABS_MAX;
}


/**
 * @brief 电机初始化
 * @note 包括两部分，标志位置1和PWM输出使能
 */
void motor_init(void)
{
    MOTORS_ENABLE();

    //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_ALL); // 电机PWM输出使能
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}


/**
 * @brief 设置左电机PWM值
 * 
 * @param Target_PWM 期望PWM值
 */
void Set_Motor1_PWM(int Target_PWM)
{
    PWM_Limit(&Target_PWM, 9999);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, Target_PWM);
}


/**
 * @brief 设置右电机PWM值
 * 
 * @param Target_PWM 期望PWM值
 */
void Set_Motor2_PWM(int Target_PWM)
{
    PWM_Limit(&Target_PWM, 9999);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Target_PWM);
}

// 设置电机1的速度
void Set_Motor1_Speed(int Target_Speed)
{
    if (MOTOR1_ENABLE_FLAG == 1)
    {
        if (Target_Speed >= 0) // 正转
        {
            Set_Motor1_PWM(Target_Speed);
            Motor1_Forward();
        }
        else if (Target_Speed < 0) // 反转
        {
            Set_Motor1_PWM(-Target_Speed);
            Motor1_Backward();
        }
    }
    else
    {
        Motor1_Stop();
        pid_Motor1_Speed.KpOut = 0;
        pid_Motor1_Speed.KiOut = 0;
        pid_Motor1_Speed.KdOut = 0;
        pid_Motor1_Speed.PID_Out = 0;
    } // 输出清零
}
// 设置电机2的速度
void Set_Motor2_Speed(int Target_Speed)
{
    if (MOTOR2_ENABLE_FLAG == 1)
    {
        if (Target_Speed >= 0) // 正转
        {
            Set_Motor2_PWM(Target_Speed);
            Motor2_Forward();
        }
        else if (Target_Speed < 0) // 反转
        {
            Set_Motor2_PWM(-Target_Speed);
            Motor2_Backward();
        }
    }
    else
    {
        Motor2_Stop();
        pid_Motor2_Speed.KpOut = 0;
        pid_Motor2_Speed.KiOut = 0;
        pid_Motor2_Speed.KdOut = 0;
        pid_Motor2_Speed.PID_Out = 0;
    } // 输出清零
}



// 设置所有电机速度
void SET_MOTORS_SPEED(int Target_Motor1_Speed, int Target_Motor2_Speed)
{
    Set_Motor1_Speed(Target_Motor1_Speed);
    Set_Motor2_Speed(Target_Motor2_Speed);
}