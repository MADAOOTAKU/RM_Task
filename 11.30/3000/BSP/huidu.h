#ifndef __HUIDU_H
#define __HUIDU_H


#include "main.h"

// extern void Huidu_init();
// extern void Huidu_Proc(uint8_t huidu_data);
// extern uint8_t gw_gray_serial_read(void);


// extern uint8_t Huidu_Datas;
// extern int Huidu_Error;

extern uint8_t gw_gray_serial_read(void);
extern int Huidu_Proc(uint8_t huidu_data);

extern float Huidu_Target;
extern uint8_t Huidu_Datas;
extern int Huidu_Error;
extern int Huidu_Sum;


#endif