#include "RM.h"


int SCL = 0;
int SDA = 0;




static void SDA_High()
{
    SDA = 1;
}

static void SDA_Low()
{
    SDA = 0;
}

static void SCL_High()
{
    SCL = 1;
}

static void SCL_Low()
{
    SCL = 0;
}


/**
 * @brief IIC延时函数
 * 
 */
static void delay_ms(void)
{
    uint8_t ms = 1;
    int a;
    while (ms)
    {
        a = 1800;
        while (a--)
            ;
        ms--;
    }
    return;
}

/**
 * @brief 开始IIC通信
 * 
 */
static void IIC_Start()
{
    SDA_High();
    SCL_High();
    delay_ms();
    SDA_Low();
    delay_ms();
    SCL_Low();
}

/**
 * @brief IIC停止通信
 * 
 */
static void IIC_Stop()
{
    SDA_Low();
    SCL_High();
    delay_ms();
    SDA_High();
}

/**
 * @brief 接收应答
 * 
 */
static void IIC_ReceiveAck()
{
    SDA_High();
    delay_ms();
    SCL_High();
    delay_ms();
    SCL_Low();
    delay_ms();
}

/**
 * @brief IIC发送一字节8位数据
 * 
 * @param dat 
 */
static void Send_Byte(uint8_t dat)
{
    for (int i = 0; i < 8; i++)
    {
        SCL_Low(); // 将时钟信号设置为低电平
        if (dat & 0x80) // 将dat的8位从最高位依次写入
        {
            SDA_High();
        }
        else
        {
            SDA_Low();
        }
        delay_ms();
        SCL_High();
        delay_ms();
        SCL_Low();
        dat <<= 1;
    }
}

/**
 * @brief IIC写一字节数据过程
 * 
 * @param data 
 */
void Send_Data(uint8_t data)
{
    IIC_Start();
    Send_Byte(DEVICE_ADDRESS);
    IIC_ReceiveAck();
    Send_Byte(DEVICE_DATA); // write data
    IIC_ReceiveAck();
    Send_Byte(data);
    IIC_ReceiveAck();
    IIC_Stop();
}


void Send_Cmd(uint8_t cmd)
{
    IIC_Start();
    Send_Byte(DEVICE_ADDRESS);
    IIC_ReceiveAck();
    Send_Byte(DEVICE_CMD); // write command
    IIC_ReceiveAck();
    Send_Byte(cmd);
    IIC_ReceiveAck();
    IIC_Stop();
}