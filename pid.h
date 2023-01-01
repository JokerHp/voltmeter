#ifndef __pid_H_ // shift+U转换为大写
#define __pid_H_

#include "stm32f4xx.h"

typedef struct _pid
{
	float expecte_value;	   //定义期望值
	unsigned int actual_value; //定义实际值
	float d_value;			   //定义偏差值
	float d_value_l;		   //定义上一个偏差值
	float kp, ki, kd;		   //定义比例、积分、微分系数
	float output;			   //输出反馈控制值
	float integral_value;	   //定义积分值

	// float umax;
	// float umin;
} Pid;

void PID_Init(void);
float PID_realize(float exc_v, unsigned int output);
double get_output(void);
float PID_Realize(float exc_v);

static float makeoutput(float input_);
static void PIDPin_init(void);

#endif