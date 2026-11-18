#ifndef  __FUNCTION_H
#define  __FUNCTION_H
#include "stdint.h"

typedef enum
{
	Init,			//初始化
  Wait,			//空闲等待
  Rise,			//软启
  Run,			//正常运行
  Err				//故障
}System_Task;


typedef enum			//软启动枚举变量
{
	SSInit,//软启初始化
	SSWait,//软启等待
	SSRun//开始软启
} SState_M;


void Open_PWM_LLC(void);					//开启原边mos管PWM
void Close_PWM_LLC(void);					//关闭原边mos管PWM
//void PID_Incremental(uint16_t Vout_temp);//电压环路计算
void potentiometer(void);         //电位器调频
void HMI_peroid(void);            //屏幕调频
void TIM_Capture(void);           //输入捕获
void Cap_protection(void);        //容性区保护
void Vout_uvp_Protect(void);				//输入欠压保护
void Vout_ovp_Protect(void);				//输入过压保护
void Vin_uvp_Protect(void);				//输入欠压保护
void Vin_ovp_Protect(void);				//输入过压保护
void I_in_Short_Protect(void);		//输入短路保护
void State_Machine(void);					//状态机函数
void FastProtection(void);				//快速保护函数
void SlowProtection(void);				//慢速保护函数
void ADCSample(void);							//AD采样函数
void MOS_OTP(void);
void I_out_OCP(void);							//输出过流保护
void I_cs_OCP(void);							//输出过流保护
void I_in_OCP(void);							//输出过流保护
void StateMInit(void);						//初始化状态函数，参数函数初始化
void StateMWait(void);						//等待状态机，等待*S后无故障则软启
void StateMRise(void);						//软启阶段
void StateMRun(void);							//运行状态
void StateMErr(void);							//故障状态
void ValInit(void);								//相关参数初始化函数

void PID_Incremental(float V_out);//设置电压和当前电压
void Iout_VGA(void);
void Burst(void);//轻载函数

void Delay_us(uint16_t time);
void Delay_ms(uint32_t nCount);

void COS_CORDIC_INT	(void);       //余弦计算
float cos_f(float angles);

#endif
