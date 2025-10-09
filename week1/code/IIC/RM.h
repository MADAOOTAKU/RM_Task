#ifndef __RM_H__
#define __RM_H__


#include <stdio.h>
#include <stdint.h>

#define DEVICE_ADDRESS 0x78
#define DEVICE_CMD 0x00
#define DEVICE_DATA 0x01

extern void Send_Cmd(uint8_t cmd);
extern void Send_Data(uint8_t data);

#endif