#include <stdio.h>

// 枚举类型
typedef enum
{
    GPIO_Speed_2MHz,
    GPIO_Speed_10MHz,
    GPIO_Speed_50MHz
} GPIO_Speed_Typedef;

// 结构体类型
typedef struct
{
    GPIO_Speed_Typedef GPIO_Speed;
} GPIO_InitTypedef;

// 转换函数：枚举值转字符串
const char *GPIO_SpeedToString(GPIO_Speed_Typedef speed)
{
    switch (speed)
    {
    case GPIO_Speed_2MHz:
        return "GPIO_Speed_2MHz";
    case GPIO_Speed_10MHz:
        return "GPIO_Speed_10MHz";
    case GPIO_Speed_50MHz:
        return "GPIO_Speed_50MHz";
    default:
        return "Unknown";
    }
}

// 初始化函数
void GPIO_StructureInit(GPIO_InitTypedef *GPIO_InitStruct)
{
    if (GPIO_InitStruct == NULL)
        return;
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
}

int main()
{
    GPIO_InitTypedef gpio_init;
    GPIO_StructureInit(&gpio_init);
    printf("GPIO_Speed = %s\n", GPIO_SpeedToString(gpio_init.GPIO_Speed));
    return 0;
}
