#include <stdio.h>

// ö������
typedef enum
{
    GPIO_Speed_2MHz,
    GPIO_Speed_10MHz,
    GPIO_Speed_50MHz
} GPIO_Speed_Typedef;

// �ṹ������
typedef struct
{
    GPIO_Speed_Typedef GPIO_Speed;
} GPIO_InitTypedef;

// ת��������ö��ֵת�ַ���
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

// ��ʼ������
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
