/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "lcd.h"
#include "dac.h"
#include "key.h"
#include "ms_delay.h"
#include "usart.h"
#include "exti.h"
#include "pid.h"
#define USART_REC_LEN 25
u16 USART_RX_STA; 
u8 USART_RX_BUF[USART_REC_LEN];
u8 key=0;
char ch_line[3]="\r\n";//???
int j=0;
u8 rec_data;
char rec[4];
char SV[8]="SetV:\r\n";//??????
char AV[8]="ActV:\r\n";//???
char lcdSV[6]="SetV:";//??????
char lcdAV[6]="ActV:";//?????
int flag=0;
/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

void USART2_IRQHandler(void)                	//????2?ж???????
{
	flag=1;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//?????ж?
    {
        rec_data=USART_ReceiveData(USART2);//??????????????
        rec[j]=rec_data;
        j++;
//        if(rec[3]!=0)
//        {
//            j=0;
//            rec[3]='\0';
//        }
    }
		rec[3]='\0';
}
int main(void)
{
    //u16 adcx[21]={0};//adc???????
		u16 adcx = 0;
    double volt;//????
    double pid_volt;
    float dacval=2000;//DAC?????
	  double adcval;
    char cvolt[6];//?????????
    char avolt[6];//??????????
    int t,k,m,n,p,q;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//???????ж?????????? 2
    usart_init(9600);//??????????????9600
    LCD1602_Wait_Ready();//???????????
	  LCD1602_Init();//?????1602???
    Dac1_Init();//dac?????
    KEY_Init();//?????????
    PID_Init();//pid??adc?????
    EXTIX_Init();
    LCD1602_Show_Str(0,0,lcdSV);
    LCD1602_Show_Str(5,0,"0");
    LCD1602_Show_Str(0,1,lcdAV);
    LCD1602_Show_Str(5,1,"0");		
    while(1)
    {
        key=KEY_Scan(0);
        adcx=get_output();
			  //volt=dacval/1000*4096/3.3;
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)==0&&dacval>0)
        {
            ms_delay(20);
            //EXTIX_Init();
            dacval=dacval-35;
            //Dac1_Set_Vol(dacval,cvolt);
            volt=dacval;
						volt/=1000;
						volt=volt*4096/3.3;
            sprintf(cvolt,"%.1lf",volt);
        }
        else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)==0&&dacval<3500)
        {
            ms_delay(20);
            //EXTIX_Init();
            dacval=dacval+35;
            //Dac1_Set_Vol(dacval,cvolt);
            volt=dacval;
						volt/=1000;
						volt=volt*4096/3.3;
            sprintf(cvolt,"%.1lf",volt);
        }


        volt=dacval*0.8/1000*3.55;
        if(flag==1)//PC?????????
        {
					  flag=0;
					j=0;
            volt=atof(rec);
            dacval=volt*350;
				}


        
				
        adcx=get_output();
				//adcx[20]=0;
				adcval=(((float)adcx)*0.0039-0.4101);
				sprintf(avolt,"%.1f",adcval);
				sprintf(cvolt,"%.1f",volt);
//				if(abs(volt-adcval)>0.1)
//				{
					//dacval=PID_Realize(dacval);
//				}
        for(k=0;k<8;k++)
        {
            USART_SendData(USART2,AV[k]);
					  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
        }
        for(p=0;p<6;p++)
        {
            USART_SendData(USART2,avolt[p]);
            while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
        }
				ms_delay(50);
        for(m=0;m<3;m++)
        {
            USART_SendData(USART2,ch_line[m]);
            while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
        }
        //??PC??????????
        DAC_SetChannel1Data(DAC_Align_12b_R, dacval);  //12位右对齐数据格式设置DAC值
				//strcpy(rec,"\0");
        LCD1602_Show_Str(5,0,cvolt);
        LCD1602_Show_Str(5,1,avolt);
        ms_delay(1000);
				//Delay(2000);
    }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


