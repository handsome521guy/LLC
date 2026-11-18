#include "ladrc.h"


extern float Vset_target;
extern float V_out;
struct LADRC_NUM;
//
//函数名：void ADRC_Init(ADRC_NUM *ADRC_TYPE1)
//函数说明：LADRC参数初始化
//
void LADRC_Init(LADRC_NUM *LADRC_TYPE1)
{
LADRC_TYPE1->h=0.005; //定时时间及时间步长
LADRC_TYPE1->r=100; //跟踪速度参数
LADRC_TYPE1->w0=50; //观测器带宽
LADRC_TYPE1->wc=10; //状态误差反馈率带宽
}
 
//
//函数名：void LADRC_TD(ADRC_NUM *ADRC_TYPE1,float Expect)
//函数说明：LADRC跟踪微分部分
//入口参数，期望值Expect(v0)输出值v1,v2
//
void LADRC_TD(LADRC_NUM LADRC_TYPE1,float Expect)
{
float fh= -LADRC_TYPE1.r*LADRC_TYPE1.r*(LADRC_TYPE1.v1-Expect)-2*LADRC_TYPE1.r*LADRC_TYPE1.v2;
LADRC_TYPE1.v1+=LADRC_TYPE1.v2*LADRC_TYPE1.h;
LADRC_TYPE1.v2+=fh*LADRC_TYPE1.h;
}
 //
//函数名：void LADRC_ESO(LADRC_NUM LADRC_TYPE1,float FeedBack)
//函数说明：LADRC扩张状态观测器部分
//入口参数，差值e，观测器的观测值z1,z2,z3
//
void LADRC_ESO(LADRC_NUM LADRC_TYPE1,float FeedBack)
{
float Beita_01=3*LADRC_TYPE1.w0;
float Beita_02=3*LADRC_TYPE1.w0*LADRC_TYPE1.w0;
float Beita_03=LADRC_TYPE1.w0*LADRC_TYPE1.w0*LADRC_TYPE1.w0;
 
float e= LADRC_TYPE1.z1-FeedBack;
LADRC_TYPE1.z1+= (LADRC_TYPE1.z2 - Beita_01*e)*LADRC_TYPE1.h;
LADRC_TYPE1.z2+= (LADRC_TYPE1.z3 - Beita_02*e + LADRC_TYPE1.b0*LADRC_TYPE1.u)*LADRC_TYPE1.h;
LADRC_TYPE1.z3+=-Beita_03*e*LADRC_TYPE1.h;
}
  //
//函数名：void LADRC_LF(LADRC_NUM LADRC_TYPE1)
//函数说明：LADRC控制律部分
//入口参数，输出u
//
void LADRC_LF(LADRC_NUM LADRC_TYPE1)
{
float Kp=LADRC_TYPE1.wc*LADRC_TYPE1.wc;
float Kd=2*LADRC_TYPE1.wc;
float e1=LADRC_TYPE1.v1-LADRC_TYPE1.z1;
float e2=LADRC_TYPE1.v2-LADRC_TYPE1.z2;
float u0=Kp*e1+Kd*e2;
LADRC_TYPE1.u=(u0-LADRC_TYPE1.z3)/LADRC_TYPE1.b0;
}
 
void LADRC_Loop(LADRC_NUM *LADRC_TYPE1)
{
LADRC_TD(LADRC_NUM LADRC_TYPE1,Vset_target);
LADRC_ESO(LADRC_TYPE1,V_out);
LADRC_LF(LADRC_TYPE1);
}