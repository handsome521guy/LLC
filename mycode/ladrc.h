#ifndef _LADRC_H
#define _LADRC_H
#include "stdint.h"
typedef struct LADRC
{
float v1,v2; //?最速输出值
float r; //速度因子
float h; //步长
float z1,z2,z3; //观测器输出
float w0,wc,b0,u; //
}LADRC_NUM;
extern struct LADRC_NUM LADRC_IN1,LADRC_IN2,LADRC_IN3;

void ADRC_Loop(LADRC_NUM *LADRC_TYPE1);
void LADRC_TD(LADRC_NUM LADRC_TYPE1,float Expect);
void LADRC_Init(LADRC_NUM *LADRC_TYPE1);
void LADRC_ESO(LADRC_NUM LADRC_TYPE1,float FeedBack);
void LADRC_LF(LADRC_NUM LADRC_TYPE1);
#endif