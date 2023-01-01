/**
 * 主函数时刻读取输出，并与期望值进行比较，相差绝对值大于给定最低误差时开启PID调控（或时刻开启PID调控）　
 */

/* include ------------------------------------------------------------------*/
#include "pid.h"
#include "main.h"
#include <math.h>

/* private define -----------------------------------------------------------*/
#define COE 1                         //线性系数：控制量/采集输出量
#define UP_LIM 200                    //变积分上阈值
#define DOWN_LIM 180                  //变积分下阈值
#define MAX 3500                      //上限
#define MIN 0                        //下限
#define INTE_COE 3500                 //积分分离范围
#define PID_Pin GPIO_Pin_5            // PID引脚, PA5 通道 5
#define ADC_Channel_PID ADC_Channel_5 // PID通道
#define RCC_APB2Periph_ADC_PID RCC_APB2Periph_ADC1
#define RCC_AHB1Periph_GPIO_PID RCC_AHB1Periph_GPIOA
#define GPIO_PID GPIOA
#define ADC_PID ADC1

/* private variables --------------------------------------------------------*/
Pid pid; // pid结构体

/* public function prototype ------------------------------------------------*/

/**
 * @brief		初始化 PID 结构体 和 PID 配置
 * @note	   	None
 */
void PID_Init(void)
{
    pid.expecte_value = 0;
    pid.actual_value = 0;
    pid.d_value = 0;
    pid.d_value_l = 0;
    pid.output = 0;
    pid.integral_value = 0;
    pid.kp = 0.1;
    pid.ki = 0;
    pid.kd = 0;

    PIDPin_init();
}

/**
 * @brief		PID 实现
 * @param  		exc_v: 期望值
 * @param       output: 实际输出值
 * @retval 		经过一次PID反馈运算后的控制量
 * @note
 */
float PID_realize(float exc_v, unsigned int output_)
{
    float index = 1; //变积分系数
    unsigned int output;
    output = output_ * 1.3639-153.54;
    pid.expecte_value = exc_v ;
    pid.d_value = pid.expecte_value - output;

    if (output > MAX)
    {
        if (fabs((double)pid.d_value) > INTE_COE)
            index = 0;
        else
        {
            index = 1;
            if (pid.d_value < 0)
            {
                pid.integral_value += pid.d_value;
            }
        }
    }
    else if (output < MIN)
    {
        if (fabs((double)pid.d_value) > INTE_COE)
            index = 0;
        else
        {
            index = 1;
            if (pid.d_value > 0)
            {
                pid.integral_value += pid.d_value;
            }
        }
    }
    else
    {
        if (fabs((double)pid.d_value) > INTE_COE)
            index = 0;
        else
        {
            index = 1;
            pid.integral_value += pid.d_value;
        }
    }

    // if (fabs((double)pid.d_value) > UP_LIM) //超过上阈值，不考虑积分
    // {
    //     index = 0.0;
    // }
    // else if (fabs((double)pid.d_value) < DOWN_LIM) //低于下阈值，全积分，全考虑积分
    // {
    //     index = 1.0;
    //     pid.integral_value += pid.d_value;
    // }
    // else //介于上下之间，全积分，线性考虑积分
    // {
    //     index = (UP_LIM - fabs((double)pid.d_value)) / (UP_LIM - DOWN_LIM);
    // pid.integral_value += pid.d_value;
    // }
    pid.output = (pid.kp * pid.d_value + index * pid.ki * pid.integral_value + pid.kd * (pid.d_value - pid.d_value_l));
    // pid.output *= COE;

    pid.d_value_l = pid.d_value;
    //pid.actual_value = (unsigned int)((pid.output + 0.4101) / 0.0039);
    return pid.output;
}

/**
 * @brief		读取输出值
 * @retval 		实际输出值
 * @note	   	无
 */
double get_output(void)
{
    static double V_out[3] = {0};
    /* 设置指定 ADC 的规则组通道，一个序列，采样时间 */
    ADC_RegularChannelConfig(ADC_PID, ADC_Channel_PID, 1, ADC_SampleTime_480Cycles);
    ADC_SoftwareStartConv(ADC_PID);                   //使能指定的 ADC 的软件转换启动功能
    while (!ADC_GetFlagStatus(ADC_PID, ADC_FLAG_EOC)) //等待转换结束
        ;
    V_out[0] = (double)ADC_GetConversionValue(ADC_PID); //最近一次 ADC1 规则组的转换结果
    ms_delay(20);
    V_out[1] = (double)ADC_GetConversionValue(ADC_PID);
    // DAC 最大输出  3570  2.8V->10V  最小输出 75 0V
    // ADC 最大输入  3723  3V->10V
    makeoutput(V_out[0]);
    V_out[2] = makeoutput(V_out[1]);
    return (V_out[2]);
}

/**
 * @brief		PID 实现
 * @param  		exc_v: 期望值
 * @retval 		经过一次PID反馈运算后的控制量
 * @note	   	变积分的PID控制算法
 */
float PID_Realize(float exc_v)
{
    return PID_realize(exc_v, get_output());
}

/* private function prototype -----------------------------------------------*/
/**
 * @brief		滤波函数
 * @param  		input_: 输入值
 * @retval 		滤波后的值
 * @note	   	线性递推平均滤波
 */
static float makeoutput(float input_)
{
    static float input[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    static float output;
    int i = 0, j = 0;
    for (i = 8; i >= 0; i--)
    {
        input[i + 1] = input[i];
    }
    input[0] = input_;
    output = 0;
    for (i = 0; i < 10; i++)
    {
        if (input[i] >= 0)
        {
            j++;
            output += input[i];
        }
    }
    return (output / j);
}

/**
 * @brief		PID 配置
 * @note	   	None
 */
static void PIDPin_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC_PID, ENABLE);  //使能 ADC1 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIO_PID, ENABLE); //使能 GPIO 时钟

    GPIO_InitStructure.GPIO_Pin = PID_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不带上下拉
    GPIO_Init(GPIO_PID, &GPIO_InitStructure);        //初始化

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC_PID, ENABLE);  // ADC1 复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC_PID, DISABLE); //复位结束

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样阶段之间的延迟 5 个时钟
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;      // DMA 失能
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                   //预分频 4 分频。
    // ADCCLK=PCLK2/4=84/4=21Mhz,ADC 时钟最好不要超过 36Mhz
    ADC_CommonInit(&ADC_CommonInitStructure); //初始化

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                      // 12 位模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                               //非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                         //关闭连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;                                  // 1 个转换在规则序列中
    ADC_Init(ADC_PID, &ADC_InitStructure);                                      // ADC 初始化

    ADC_Cmd(ADC_PID, ENABLE); //开启 AD 转换器

//    // ADC校准
//    /* Enable ADC1 */
//    ADC_Cmd(ADC1, ENABLE);

//    /* Enable ADC1 reset calibration register */
//    ADC_ResetCalibration(ADC1);
//    /* CheADC1 Software Conversion */
//    ADC_SoftwareSck the end of ADC1 reset calibration register */
//    while (ADC_GetResetCalibrationStatus(ADC1))
//        ;

//    /* Start ADC1 calibration */
//    ADC_StartCalibration(ADC1);
//    /* Check the end of ADC1 calibration */
//    while (ADC_GetCalibrationStatus(ADC1))
//        ;

//    /* Start tartConvCmd(ADC1, ENABLE);
}