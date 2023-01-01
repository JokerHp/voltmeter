#include "stm32f4xx_gpio.h"
#include "key.h"

/*控制按键初始化IO口*/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//使能GPIOA时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//浮空输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//注意此函数有响应优先级,KEY0>KEY1>!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0))
	{
		ms_delay(10);//去抖动 
		key_up=0;
		if(KEY0==0)
        return 1;
		else if(KEY1==0)
        return 2;
		//else if(KEY2==0)return 3;
		//else if(WK_UP==1)return 4;
	}
    else if(KEY0==1&&KEY1==1)
    key_up=1; 	    
 	return 0;// 无按键按下
}