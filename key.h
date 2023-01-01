#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#define KEY0 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9) //PA9
#define KEY1 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)//PA10 
void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);

#endif