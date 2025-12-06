#include "huidu.h"

#define READ_IN1() ((HAL_GPIO_ReadPin(IN1_GPIO_Port, IN1_Pin) == GPIO_PIN_SET) ? 1 : 0)


#define READ_IN2() ((HAL_GPIO_ReadPin(IN2_GPIO_Port, IN2_Pin) == GPIO_PIN_SET) ? 1 : 0)

#define READ_IN3() ((HAL_GPIO_ReadPin(IN3_GPIO_Port, IN3_Pin) == GPIO_PIN_SET) ? 1 : 0)

#define READ_IN4() ((HAL_GPIO_ReadPin(IN4_GPIO_Port, IN4_Pin) == GPIO_PIN_SET) ? 1 : 0)

#define READ_IN5() ((HAL_GPIO_ReadPin(IN5_GPIO_Port, IN5_Pin) == GPIO_PIN_SET) ? 1 : 0)

#define READ_IN6() ((HAL_GPIO_ReadPin(IN6_GPIO_Port, IN6_Pin) == GPIO_PIN_SET) ? 1 : 0)

#define READ_IN7() ((HAL_GPIO_ReadPin(IN7_GPIO_Port, IN7_Pin) == GPIO_PIN_SET) ? 1 : 0)

#define READ_IN8() ((HAL_GPIO_ReadPin(IN8_GPIO_Port, IN8_Pin) == GPIO_PIN_SET) ? 1 : 0)

#define GW_GRAY_DELAY_TICK 360

/* 8MHz 下5us大概是27 */
// static void gw_gray_delay(uint32_t delay)
// {
//     volatile uint32_t delay_tick = delay;

//     while (delay_tick > 0)
//     {
//         delay_tick--;
//     }
// }

int get_gray_value(uint8_t num)
{
    switch(num)
    {
        case 1:
            return READ_IN1();

        case 2:
            return READ_IN2();

        case 3:
            return READ_IN3();

        case 4:
            return READ_IN4();

        case 5:
            return READ_IN5();

        case 6:
            return READ_IN6();

        case 7:
            return READ_IN7();

        case 8:
            return READ_IN8();
        
        default:
            return 0;
    }
}



uint8_t gw_gray_serial_read(void)
{
    uint8_t huidu_datas = 0;
    for (int i = 1; i <= 8;i++)
    {
        huidu_datas |= (get_gray_value(i) << (8 - i));
    }

    return huidu_datas;
}

float Huidu_Target = 0;
int Huidu_Error;
int Huidu_Sum;

int Huidu_Proc(uint8_t huidu_data)
{   
    //黑线=0.白线等于1
    switch (huidu_data)
    {   //1111 0111
        case 247:
            return -10;

        case 251:
            return -20;

         // 1111 1101
        case 253:
            return -30;

        // 1111 1110
        case 254:
            return -40;

        // 1111 0011
        case 243:
            return -20;

        // 1111 1001
        case 249:
            return -30;

        // 1111 1100
        case 252:
            return -40;

        // 三个传感器检测到白线
        // 1111 0001
        case 241:
            return -30;

        // 1111 1000
        case 248:
            return -40;

        // 理想状态（中间两个传感器检测到黑线）
        // 1110 0111
        case 231:
            return 0;

        // 车偏右

        // 1110 1111
        case 239:
            return 10;

        // 1101 1111
        case 223:
            return 20;

        // 1011 1111
        case 191:
            return 30;

        // 0111 1111
        case 127:
            return 40;

        // 两个传感器检测到白线
        // 1100 1111
        case 207:
            return 20;

        // 1001 1111
        case 159:
            return 30;

        // 0011 1111
        case 63:
            return 40;

        // 1000 1111
        case 143:
            return 30;

        // 0001 1111
        case 31:
            return 40;

        default : return 0;
    }
}