#include "usart.h"
#include "stm32f4xx_gpio.h"
void usart_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能 GPIOD 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能 USART2 时钟

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //复用为 USART1 tx
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //复用为USART1 rx

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //GPIOA9 与 GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化 PA9，PA10

	USART_InitStructure.USART_BaudRate = bound;//一般设置为 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为 8
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; 
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(USART2, &USART_InitStructure); //初始化串口

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接收中断

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级 2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =2; //响应优先级 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
    NVIC_Init(&NVIC_InitStructure); //初始化 NVIC 寄存器

    USART_Cmd(USART2, ENABLE); //使能串口
} 
