#ifndef __ENCODER_H
#define __ENCODER_H

#include "main.h"
#include "tim.h"

#define ENCODE_13X 13    // 编码器线数
#define JIANSUBI 20      // 减速比
#define BEIPIN 4         // 倍频
#define SAMPLE_TIME 0.01 // 采样时间10ms
#define CC (ENCODE_13X * JIANSUBI * BEIPIN * SAMPLE_TIME)

#define PI 3.1415f
#define RR 48.0f // 车轮直径单位mm

void encoder_init(void);
void MEASURE_MOTORS_SPEED(void);

extern volatile int32_t motor1_speed;
extern volatile int32_t motor2_speed;

#endif