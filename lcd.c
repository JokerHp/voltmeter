#include "main.h"
#include "lcd.h"
#include "stm32f4xx_gpio.h"
#include "ms_delay.h"
#include <stdio.h>

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE| RCC_AHB1Periph_GPIOG,ENABLE);//ʹ��PF,PEʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);				//��ʼ��GPIOD0~7 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOF, &GPIO_InitStructure);				//��ʼ��GPIB15,14,13
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);				//��ʼ��GPIB15,14,13
}

/*�ȴ�Һ��׼����*/
void LCD1602_Wait_Ready(void)
{
	u8 sta;
	
	DATAOUT(0xff);
	LCD_RS_Clr();
	LCD_RW_Set();
	do
	{
		LCD_EN_Set();
		ms_delay(5);	//��ʱ5ms
// 		sta = GPIO_ReadInputData(GPIOF);//?????
		sta =GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_7);
		LCD_EN_Clr();
	}while(sta & 0x80);//bit7����1��ʾҺ����æ���ظ����ֱ�������0Ϊֹ
}

/* ��LCD1602Һ��д��һ�ֽ����cmd-��д������ֵ */
void LCD1602_Write_Cmd(u8 cmd)
{
	LCD1602_Wait_Ready();
	ms_delay(1);
	LCD_RS_Clr();
	ms_delay(1);
	LCD_RW_Clr();
	ms_delay(1);
	DATAOUT(cmd);
	ms_delay(1);
	LCD_EN_Set();
	ms_delay(1);
	LCD_EN_Clr();
	ms_delay(1);
    //printf("%d",cmd);
}

/* ��LCD1602Һ��д��һ�ֽ����ݣ�dat-��д������ֵ */
void LCD1602_Write_Dat(u8 dat)
{
	LCD1602_Wait_Ready();
	ms_delay(1);
	LCD_RS_Set();
	ms_delay(1);
	LCD_RW_Clr();
	ms_delay(1);
	DATAOUT(dat);
	ms_delay(1);
	LCD_EN_Set();
	ms_delay(1);
	LCD_EN_Clr();
	ms_delay(1);
}

/* ���� */
void LCD1602_ClearScreen(void)
{
	LCD1602_Write_Cmd(0x01);
}

/* ��Һ������ʾ�ַ�����(x,y)-��Ӧ��Ļ�ϵ���ʼ���꣬str-�ַ���ָ�� */
void LCD1602_Set_Cursor(u8 x, u8 y)
{
	u8 addr;
	
	if (y == 0)
		addr = 0x00 + x;
	else
		addr = 0x40 + x;
	LCD1602_Write_Cmd(addr | 0x80);
}

/* ��Һ������ʾ�ַ�����(x,y)-��Ӧ��Ļ�ϵ���ʼ���꣬str-�ַ���ָ�� */
void LCD1602_Show_Str(u8 x, u8 y, u8*str)
{
	LCD1602_Set_Cursor(x, y);
	while(*str != '\0')
	{
		LCD1602_Write_Dat(*str++);
	}
}

/* ��ʼ��1602Һ��  */
void LCD1602_Init(void)
{
    GPIO_Configuration();
	LCD1602_Write_Cmd(0x38);	//16*2??,5*7??,8????
	LCD1602_Write_Cmd(0x0c);	//???,????
	LCD1602_Write_Cmd(0x06);	//????,????+1
	LCD1602_Write_Cmd(0x01);	//??

//    LCD1602_Write_Cmd(0X02);               
//    LCD1602_Write_Cmd(0X06);             
//	LCD1602_Write_Cmd(0X00);              
//	LCD1602_Write_Cmd(0X0C); 

//	LCD1602_Write_Cmd(0X38);              
//	LCD1602_Write_Cmd(0x01);               
  
}