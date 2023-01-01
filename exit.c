#include "exti.h"
#include "key.h"
#include "stm32f4xx_exti.h"
void EXTIX_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    KEY_Init();
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource9|EXTI_PinSource10);
    EXTI_InitStruct.EXTI_Line=EXTI_Line0;
    EXTI_InitStruct.EXTI_LineCmd=ENABLE;
    EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
}
