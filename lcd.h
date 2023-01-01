#ifndef __LCD1602_H
#define __LCD1602_H	 
#include "system_stm32f4xx.h"
#include "main.h"
//1602液晶指令/数据选择引脚
#define	LCD_RS_Set()	GPIO_SetBits(GPIOF, GPIO_Pin_0)
#define	LCD_RS_Clr()	GPIO_ResetBits(GPIOF, GPIO_Pin_0)

//1602液晶读写引脚
#define	LCD_RW_Set()	GPIO_SetBits(GPIOF, GPIO_Pin_1)
#define	LCD_RW_Clr()	GPIO_ResetBits(GPIOF, GPIO_Pin_1)

//1602液晶使能引脚
#define	LCD_EN_Set()	GPIO_SetBits(GPIOG, GPIO_Pin_15)
#define	LCD_EN_Clr()	GPIO_ResetBits(GPIOG, GPIO_Pin_15)

//1602液晶数据端口	PD0~7
#define	DATAOUT(x)	GPIO_Write(GPIOE, x)

void GPIO_Configuration(void);
void LCD1602_Wait_Ready(void);
void LCD1602_Write_Cmd(uint8_t cmd);
void LCD1602_Write_Dat(uint8_t dat);
void LCD1602_ClearScreen(void);
void LCD1602_Set_Cursor(uint8_t x, uint8_t y);
void LCD1602_Show_Str(uint8_t x, uint8_t y, u8 *str);
void LCD1602_Init(void);

#endif