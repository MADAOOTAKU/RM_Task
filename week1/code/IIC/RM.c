#include "RM.h"


int SCL = 1;
int SDA = 0;


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
 * @brief 接收应答信号
 * @return 0:ACK, 1:NACK
 */
static int IIC_ReceiveAck(void)
{
    SCL_Low();
    SDA_In();
    SDA_High();
    delay_ms();
    SCL_High();
    delay_ms();
    int ack = SDA_Read() ? 1 : 0;
    SCL_Low();
    SDA_Out();
    return ack;
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
void Send_Data(uint8_t device_addr,uint8_t reg_addr,uint8_t data)
{
    IIC_Start();
    Send_Byte((device_addr << 1) | 0);
    if (IIC_ReceiveAck())
    {
        IIC_Stop();
        return;
    }
    Send_Byte(reg_addr);
    if (IIC_ReceiveAck())
    {
        IIC_Stop();
        return;
    }
    Send_Byte(data);
    if (IIC_ReceiveAck())
    {
        IIC_Stop();
        return;
    }
    IIC_Stop();
}

