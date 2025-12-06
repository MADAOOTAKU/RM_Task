#ifndef __MOTOR_H
#define __MOTOR_H


#include  "stdint.h"
#include "main.h"
#include "tim.h"

typedef struct 
{
    float Kp;
    float Ki;
    float Kd;

    float Target;
    float Measure;
    float Error[3];

    float KpOut;
    float KiOut;
    float KdOut;
    float PID_Out;

    uint32_t PID_Limit_MAX;  //最大输出限幅
    uint32_t Ki_Limit_MAX; //最大积分限幅
} pid_t;

/**
 * @brief 电机使能宏
 * 
 */
#define MOTORS_ENABLE()         \
    {                           \
        MOTOR1_ENABLE_FLAG = 1; \
        MOTOR2_ENABLE_FLAG = 1; \
    }
#define MOTORS_DISABLE()        \
    {                           \
        MOTOR1_ENABLE_FLAG = 0; \
        MOTOR2_ENABLE_FLAG = 0; \
    }


/**
 * @brief 电机信号控制宏
 * 
 */
#define AIN1(x) x?HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET)

#define AIN2(x) x?HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET)

#define BIN1(x) x?HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET)

#define BIN2(x) x?HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET)



#define Motor1_Forward() \
    {                    \
        AIN1(1);         \
        AIN2(0);         \
    }
#define Motor1_Backward() \
    {                     \
        AIN1(0);          \
        AIN2(1);          \
    }
#define Motor1_Stop() \
    {                 \
        AIN1(0);      \
        AIN2(0);      \
    }

#define Motor2_Forward() \
    {                    \
        BIN1(1);         \
        BIN2(0);         \
    }
#define Motor2_Backward() \
    {                     \
        BIN1(0);          \
        BIN2(1);          \
    }
#define Motor2_Stop() \
    {                 \
        BIN1(0);      \
        BIN2(0);      \
    }

extern uint8_t MOTOR1_ENABLE_FLAG;
extern uint8_t MOTOR2_ENABLE_FLAG;
// PID参数
extern pid_t pid_Motor1_Speed;
extern pid_t pid_Motor2_Speed;
extern pid_t pid_Turn;

extern uint8_t Turn_PID_Flag;


void motor_init(void);
void SET_MOTORS_SPEED(int Left_Speed, int Right_Speed);
extern float pid_calculate(pid_t *pid, float Measure, float Target);

#endif