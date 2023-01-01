#include "dac.h"
#include "lcd.h"
#include "stdio.h"
#include "stm32f4xx_gpio.h"
//DAC通道1输出初始化
void Dac1_Init(void)
{  
    GPIO_InitTypeDef  GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitType;
        
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能DAC时钟
        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

    DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
    DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
    DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

    DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC通道1
    
    DAC_SetChannel1Data(DAC_Align_12b_R, 2000);  //12位右对齐数据格式设置DAC值
}

void Dac1_Set_Vol(float vol,char*cvolt)
{
	double volt=vol;
	volt/=1000;
	volt=volt*4096/3.3;
    sprintf(cvolt,"%.1lf",volt);
    //LCD1602_Show_Str(5,0,cvolt);
	DAC_SetChannel1Data(DAC_Align_12b_R,volt);//12位右对齐数据格式设置DAC值
}