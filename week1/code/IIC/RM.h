#ifndef __RM_H__
#define __RM_H__


#include <stdio.h>
#include <stdint.h>

#define DEVICE_ADDRESS 0x78
#define DEVICE_REG 0x40
#define DEVICE_DATA 0x01

extern int SDA;
extern int SCL;


#define SDA_High() (SDA = 1)
#define SDA_Low()  (SDA= 0)
#define SCL_High() (SCL= 1)
#define SCL_Low()  (SCL= 0)

//设置SDA为输出方向，默认是输出方向
#define SDA_Out() \
    {             \
        ;         \
    }
//设置SDA为输入方向
#define SDA_In() \
    {            \
        ;        \
    }

//要在输入模式下才可以用
#define SDA_Read()   (SDA==1)

extern void Send_Data(uint8_t device_addr, uint8_t reg_addr, uint8_t data);

#endif