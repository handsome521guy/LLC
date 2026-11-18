
#include "function.h"
#include "hrtim.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "hrtim.h"
#include "tim.h"
#include "main_init.h"
#include "led_relay.h"
#include "oled.h"
#include "ntc.h"




uint8_t V_in_OVP_Err_flag;	
uint8_t V_in_UVP_Err_flag;	


uint8_t Burst_flag;				        //禁止使用Burst模式
uint8_t Short_Err_flag;						//输入短路故障标志位



//输入过流
uint8_t I_in_OCP_Err_flag;				//输入过流故障标志位
uint8_t I_in_OCPCnt = 0;	
uint16_t I_in_RSCnt = 0;		
uint8_t I_in_RSNum = 0;	

//谐振腔过流
uint8_t I_cs_OCP_Err_flag;				//谐振腔过流故障标志位
uint8_t I_cs_OCPCnt = 0;	
uint16_t I_cs_RSCnt = 0;		
uint8_t I_cs_RSNum = 0;	

//输出过流
uint8_t I_out_OCP_Err_flag;				//输出过流故障标志位
uint16_t I_out_RSCnt = 0;					//过流保护判据保持计数器定义
uint8_t I_out_OCPCnt = 0;					//故障清除保持计数器定义
uint8_t I_out_RSNum = 0;					//保留保护重启计数器

//输入电压
uint8_t V_in_OVP_Err_flag;	
uint8_t V_in_UVP_Err_flag;	
uint16_t V_in_OVPCnt = 0;						  //过压保护判据保持计数器定义
uint16_t V_in_UVPCnt = 0;				 			//欠压保护判据保持计数器定义

//输出电压
uint8_t V_out_OVP_Err_flag;	
uint8_t V_out_UVP_Err_flag;	
uint16_t V_out_OVPCnt = 0;						  //过压保护判据保持计数器定义
uint16_t V_out_UVPCnt = 0;				 			//欠压保护判据保持计数器定义



uint8_t PWM_Falg = 0;							//PWM关闭标志位
uint16_t cnt = 0;									//状态机等待运行标志位
uint16_t Res_Cnt = 0;							//重启时间计数标志位
uint8_t Res_Num = 0;							//重启次数
uint8_t Soft_Start_End = 0;				//软起动结束标志位
uint8_t LLC_SFST_FLAG;




SState_M 	Soft_Wave,Next_Soft;				//软启动状态标志位

#define N 50
uint16_t temp[5][N];
uint32_t ADC2_Value[5];	
uint32_t ADC2_DMA_Value[5];
float V_in;
float I_in;
float V_out;
float I_cs;	
float MOS_T;
float I_out;	
float I_out_VGA;
float LLC_SFST_fs = 11334;//30kHZ
float Freq;

//HMI传输的参数
float V_in_1;
float I_in_1;	
float V_out_1;
float I_cs_1;	
float MOS_T_1;
float I_out_1;
//float PWM_angle_1;               		
//float PF_1;  
float Freq_1;
float power;
float EF;

 
uint16_t PWM_T_Count;             //周期
uint16_t PWM_D_Count;             //电压电流的差值
extern uint32_t PWM_Peroid;
extern float Freq;
extern uint16_t PWM_Peroid_2; 
extern uint16_t PWM_shift_1; 
//float PWM_shift;                  //移相角
float PWM_angle;               		//相位角
float PF;                 				//功率因数

//float Vset_target;
float h;             	   //比例系数
float r;            		 //积分系数
float Wo;           		 	 //微分系数
float Wc; 

extern uint16_t V_in_max; 
extern uint16_t V_in_min; 
extern uint16_t I_in_max; 
//extern uint16_t PWM_angle_min; 
extern uint16_t I_cs_max; 
extern uint16_t I_out_max; 
extern uint16_t V_out_max; 
extern uint16_t V_out_min; 


System_Task Current_State,Next_State; 

void State_Machine()
{
	Current_State = Next_State;
	
	switch(Current_State)
	{
		case Init:									//初始化状态
			StateMInit();							//初始化		
		break;
		
		case Wait:									//等待状态
			StateMWait();							//等待
					
		break;
		
		case Rise:									//软启动状态
			StateMRise();							//软起动
		  LED_BLUE_ON();			        //绿灯亮

		
		break;
		
		case Run:										//运行状态
			StateMRun();							//正常运行
			LED_BLUE_OFF();			        //绿灯亮	
			LED_GREEN_ON();			        //绿灯亮
		break;
		
		case Err:										//故障状态
			StateMErr();							//故障运行
		  LED_RED_ON();				     		//绿灯灭
		  LED_GREEN_OFF();
		  LED_G_ON(); 
		break;	
		
		default:
			break;		
	}
}

/***
 *			延时函数
 *
 */
void Delay_ms(__IO uint32_t nCount)
{
  while (nCount != 0)
  {
		nCount--;
	  Delay_us(1000);
  }
}
void Delay_us(__IO uint16_t time)
{
		 TIM15->CNT=0;			
	   while((TIM15->CNT)<time);   //TIME15,时钟频率为1MHz，计一个数为1us  


}


///***
// *			余弦运算
// *
// */

//void COS_CORDIC_INT	(void)
//{
//	CORDIC_HandleTypeDef hcordic;                             //三角函数描述结构体
//	CORDIC_ConfigTypeDef sCordicConfig;                       //参数配置结构体
//  __HAL_RCC_CORDIC_CLK_ENABLE();                            //开启时钟
//	__IO float result=0.0f;                                  //保存计算结果
//	
//	hcordic.Instance = CORDIC;                                //选择三角函数计算单元
//  HAL_CORDIC_Init(&hcordic);                                //初始化
//	
//	sCordicConfig.Function         = CORDIC_FUNCTION_COSINE;     //选择计算余弦
//  sCordicConfig.Precision        = CORDIC_PRECISION_6CYCLES; //选择计算精度等级,1个周期对应4次算法迭代
//  sCordicConfig.Scale            = CORDIC_SCALE_0;           //选择计算系数,比例系数为1
//  sCordicConfig.NbWrite          = CORDIC_NBWRITE_1;         //选择计算结果个数
//  sCordicConfig.NbRead           = CORDIC_NBREAD_1;          //选择输出余弦
//  sCordicConfig.InSize           = CORDIC_INSIZE_32BITS;     //选择输入数据格式Q1.31，
//  sCordicConfig.OutSize          = CORDIC_OUTSIZE_32BITS;    //选择数据输出格式Q1.31
//  HAL_CORDIC_Configure(&hcordic, &sCordicConfig);            //初始化
//	
//}


//float cos_f(float angles)
//{
//		/*
//	*输入参数0~360°，输出参数-1~1
//	*主频170MHz时，本函数执行时间330nS
//	*/
// MODIFY_REG(CORDIC->CSR,CORDIC_CSR_FUNC|CORDIC_CSR_SCALE,CORDIC_FUNCTION_COSINE|CORDIC_SCALE_0); //选择计算类型
// WRITE_REG(CORDIC->WDATA, (int32_t)(angles*11930464.7f));//(angles/180)*2147483648.0f;//输入参数
// return (int32_t)READ_REG(CORDIC->RDATA)/2147483648.0f;//返回值
//}






/***
 *			ADC采样
 *
 */
void ADCSample(void)
{
	uint8_t i;
	static float sum[5];
	static uint16_t m[5];
	for(i=0;i<5;i++)
	{
		sum[i]-=temp[i][m[i]]/(float)N;
		temp[i][m[i]]=ADC2_DMA_Value[i];
		
		sum[i]+=temp[i][m[i]]/(float)N;
		m[i]++;if(m[i]>=N)m[i]=0;
		ADC2_Value[i]=sum[i];

		I_out_VGA=(float)ADC2_Value[0]/4095*3.0f*10.0f;//输出电流，iout=0.1*
//		MOS_T=(float)ADC2_Value[1]/4095*3.0f*2.0f;//mos温度(电位器调节)
		I_in=(float)ADC2_Value[2]/4095*3.0f*1.626f;
		V_in=(float)ADC2_Value[3]/4095*3.0f*201.0f+3.0f;
//		I_cs=(float)ADC2_Value[2]/4095*3.0f*6.0f;//谐振腔电流
		V_out=(float)ADC2_Value[4]/4095*3.0f*40.0f+2.0f;//输出电压，vout=adc/(0.1*0.125*2)
//    MOS_T=temp_Cal(ADC2_Value[1])-100;
//		VGA_1();//增益1倍
		I_out=I_out_VGA;
		
//		if(PFC_OK_flag==1)
//		{

		
		V_in_1 =V_in*100.0f;
		I_in_1 =I_in*1000.0f;
		
//    MOS_T=temp_Cal(ADC2_Value[1])-100;
//    MOS_T_1 =MOS_T*10.0f;
		I_cs_1 =I_cs*1000.0f;
		V_out_1 =V_out*1000.0f;


		power=V_out*I_out*10.0f;
		EF=V_out*I_out/V_in/I_in*1000.0f;
		I_out_1 =I_out*1000.0f;
		
//	}		
		
//		if(I_out<0.5f)
//		{
//			HAL_GPIO_WritePin(A_VGA_GPIO_Port, A_VGA_Pin, GPIO_PIN_SET);//输出电流变增益
//			I_out=I_out_VGA/2;
//			
//		}
//		else
//		{
//			HAL_GPIO_WritePin(A_VGA_GPIO_Port, A_VGA_Pin, GPIO_PIN_RESET);
//			I_out=I_out_VGA;
//			
//		}
		

	}
}

///***
// *			输出电流可变增益
// *
// */
//void Iout_VGA(void)
//{
//		if(I_out_VGA>10.0f)   //
//		{
//			VGA_1();//增益1倍
//			I_out = I_out_VGA;
//			
//		}
//		
//		if((I_out_VGA<10.0f)&&(I_out_VGA>5.0f))   //
//		{
//			VGA_2();//增益2倍
//			I_out = I_out_VGA*0.5f;
//			
//		}
//		
//		if((I_out_VGA<5.0f)&&(I_out_VGA>2.5f))   //
//		{
//			VGA_4();//增益4倍
//			I_out = I_out_VGA*0.25f;
//			
//		}
//		
//		if(I_out_VGA<2.5f)   //
//		{
//			VGA_10();//增益10倍
//			I_out = I_out_VGA*0.1f;
//			
//		}
//		
//		power=V_out*I_out*10.0f;
//		EF=V_out*I_out/V_in/I_in*1000.0f;
//		I_out_1 =I_out*1000.0f;
//}


/***
 *			电压环路计算
 *
 */
#define Min_Inte 34000			     //最小频率对应最大周期	(40KHz)
#define Max_Inte 6800				 	   //最大频率对应最小周期 (200KHz)

//float Kp = 0.1f;             	   //比例系数
//float Ki = 0.01f;            		 //积分系数
//float Kd = 0.0f;           		 	 //微分系数

float Kp;             	   //比例系数
float Ki;            		   //积分系数
float Kd;           		 	 //微分系数

float error1 = 0.0f;        		 //当前输出和设定值误差
float error2 = 0.0f;        		 //当前输出和设定值误差
float e1 = 0.0f;            		 //上次输出和设定值误差
float e2 = 0.0f;            		 //上上次输出和设定值误差

double ERROR_Increase_V = 0.0f;	 //误差增加量
uint32_t PWM_Peroid = 0.0f;		   //PWM周期值
uint32_t PWM_Peroid_1 = 0.0f;

float Vset_target = 0.0f;
float Iset_target = 0.0f;

void PID_Incremental(float V_out)//设置电压和当前电压
{
	error1 = Vset_target - V_out;						  		 	 					//当前误差   
	error1 = error1 * 0.01f;
	e1 = error1 - error2;                                         //两次误差变化量
	ERROR_Increase_V += Kp * e1 + Ki * error1 + Kd * (e1 - e2);  	//优化之后的PID算法(Kp*(ek-ek_1)+Ki*ek+Kd*(ek-2ek_1+ek_2))
	error2 = error1;																						 	//移位
	e2 = e1;                                 							       	//移位
	
	if(ERROR_Increase_V >= 3.0f) 					  
	{
		ERROR_Increase_V = 3.0f;           
	}
	else if(ERROR_Increase_V <= 0.1f)		  
	{ 
		ERROR_Increase_V =  0.1f;   
	}
		
	PWM_Peroid = 5862.068966+ 9379.310345 * ERROR_Increase_V;		//计算PWM周期值	
	
	Freq=	1.36e6f/PWM_Peroid;
	Freq_1 =	Freq*100.0f;
	
	hhrtim1.Instance->sMasterRegs.MPER = PWM_Peroid;
	hhrtim1.Instance->sTimerxRegs[0].PERxR = PWM_Peroid;					//更新周期值
	hhrtim1.Instance->sTimerxRegs[1].PERxR = PWM_Peroid;
	hhrtim1.Instance->sTimerxRegs[0].CMP3xR = PWM_Peroid >> 2;		//占空比的一半
	hhrtim1.Instance->sTimerxRegs[0].CMP1xR = PWM_Peroid >> 1;		//PWM占空比为周期的一半	
}


///***
// *			电位器调频
// *
// */
//void potentiometer(void)
//{
//			PWM_Peroid_1 = 34000 - 7271 * MOS_T;		//计算PWM周期值
////			Open_PWM_LLC();
//	
//					if(Soft_Start_End==0)                    
//				{
//			if(LLC_SFST_fs <= PWM_Peroid_1)//软起动中
//				{
//					PWM_Peroid_1 = LLC_SFST_fs;
//					LLC_SFST_fs += 0.01;
//				}
//			else					//完成软起动
//				{
//					LLC_SFST_fs=11334;//30KHz
//					Soft_Start_End = 1;
//				}
//				}
//			PWM_Peroid=PWM_Peroid_1;
//					
//			Freq=	34000.0f/PWM_Peroid*10.0f;
//			Freq_1 =	Freq*1000.0f;
//			hhrtim1.Instance->sMasterRegs.MPER = PWM_Peroid;
//			hhrtim1.Instance->sTimerxRegs[0].PERxR = PWM_Peroid;					//更新周期值
//			hhrtim1.Instance->sTimerxRegs[1].PERxR = PWM_Peroid;					//更新周期值
//			
//			hhrtim1.Instance->sTimerxRegs[0].CMP3xR = PWM_Peroid >> 2;		//占空比的一半
//			hhrtim1.Instance->sTimerxRegs[0].CMP1xR = PWM_Peroid >> 1;		//PWM占空比为周期的一半	
//}

/////***
//// *			屏幕调频加移相
//// *
//// 
//uint16_t HMI_PWM_peroid;
////uint16_t HMI_PWM_shift_comp1;
////uint16_t HMI_PWM_shift_comp2;

//void HMI_peroid(void)
//{
//	
//			if(PWM_Peroid_2>30000.0f)
//			{
//			PWM_Peroid_2=30000.0f;
//			}
//			else if(PWM_Peroid_2<10000.0f)
//			{
//			 PWM_Peroid_2=10000.0f;
//			}
//			
////      if(PWM_shift_1>180.0f)
////			{
////			PWM_shift_1 = 180.0f;
////			
////			}
////			else if(PWM_shift_1<10.0f)
////			{
////			PWM_shift_1 =  10.0f;
////			}
//			
//	    
//	    HMI_PWM_peroid = 340000000.0f/PWM_Peroid_2;
//			
//      if(Soft_Start_End==0)                    
//				{
//					
//			if(LLC_SFST_fs <= HMI_PWM_peroid)//软起动中
//				{
//					HMI_PWM_peroid = LLC_SFST_fs;
//					LLC_SFST_fs += 0.01;
//				}
//			else					//完成软起动
//				{
//					LLC_SFST_fs=11334;//30KHz
//					Soft_Start_End = 1;
//				}
//				}
//				
//			PWM_Peroid=HMI_PWM_peroid;
//				
////			HMI_PWM_shift_comp1 = PWM_shift_1*PWM_Peroid/360.0f;	//移相角度,改HRTIMB的comp1
////			HMI_PWM_shift_comp2 = HMI_PWM_shift_comp1+(PWM_Peroid>>1);//移相角度,改HRTIMB的comp2
//				
//			Freq=	34000.0f/PWM_Peroid*10.0f;
//			Freq_1 =	Freq*1000.0f;
//			hhrtim1.Instance->sMasterRegs.MPER = PWM_Peroid;              //主定时器周期更新
//			hhrtim1.Instance->sTimerxRegs[0].PERxR = PWM_Peroid;					//更新周期值HRTIMEA
//			hhrtim1.Instance->sTimerxRegs[1].PERxR = PWM_Peroid;					//更新周期值HRTIMEB
//			
//			hhrtim1.Instance->sTimerxRegs[0].CMP3xR = PWM_Peroid >> 2;		//占空比的一半
//			hhrtim1.Instance->sTimerxRegs[0].CMP1xR = PWM_Peroid >> 1;		//PWM占空比为周期的一半	
//				
//				
////			hhrtim1.Instance->sTimerxRegs[1].CMP1xR = HMI_PWM_shift_comp1;		//最小移相角度
////			hhrtim1.Instance->sTimerxRegs[1].CMP2xR = HMI_PWM_shift_comp2;		//PWM占空比为周期的一半	
//			}			



/***
 *			输入欠压保护
 *
 */
void Vin_uvp_Protect()
{	
//		if(V_in < 0.05f)						 //输入电压小于200V
//	{
//		V_in_UVPCnt++;									 //过压保护计数标志位++
//		if(V_in_UVPCnt > 20)						 //持续100ms认为故障
//		{
//			V_in_UVPCnt = 0;							 //标志位清零
//		  Burst_flag = 0;		//禁止使用Burst模式
//			Close_PWM_LLC();				 //关闭原边PWM
//			V_in_UVP_Err_flag = 1;	 //输入欠压故障标志位
//			Next_State = Err;		 		 //状态机跳转到故障
//		}		
//	}
//	else												
//	{
//		V_in_UVPCnt = 0;									//标志位清零
//	}
}


/***
 *			输入过压保护
 *
 */
void Vin_ovp_Protect()
{
	if(V_in >V_in_max)						 //输入电压大于360V V_in_max
	{
		V_in_OVPCnt++;									 //过压保护计数标志位++
		if(V_in_OVPCnt > 200)						 //持续100ms认为故障
		{
			V_in_OVPCnt = 0;							 //标志位清零
		  Burst_flag = 0;		//禁止使用Burst模式
			Close_PWM_LLC();				 //关闭原边PWM
			V_in_OVP_Err_flag = 1;	 //输入过压故障标志位
			Next_State = Err;		 		 //状态机跳转到故障
		}		
	}
	else												
	{
		V_in_OVPCnt = 0;									//标志位清零
	}
}




/***
 *			输出欠压保护
 *
 */
void Vout_uvp_Protect()
{	
//		if(V_out < 0.05f)						 //输入电压小于200V
//	{
//		V_out_UVPCnt++;									 //过压保护计数标志位++
//		if(V_out_UVPCnt > 20)						 //持续100ms认为故障
//		{
//			V_out_UVPCnt = 0;							 //标志位清零
//		
//			Close_PWM_LLC();				 //关闭原边PWM
//			V_out_UVP_Err_flag = 1;	 //输入欠压故障标志位
//			Next_State = Err;		 		 //状态机跳转到故障
//		}		
//	}
//	else												
//	{
//		V_out_UVPCnt = 0;									//标志位清零
//	}
}


/***
 *			输出过压保护
 *
 */
void Vout_ovp_Protect()
{
	if(V_out > V_out_max)						 //输出电压大于60V   V_out_max
	{
		V_out_OVPCnt++;									 //过压保护计数标志位++
		if(V_out_OVPCnt > 200)						 //持续100ms认为故障
		{
			V_out_OVPCnt = 0;							 //标志位清零
		  Burst_flag = 0;		//禁止使用Burst模式
			Close_PWM_LLC();				 //关闭原边PWM
			V_out_OVP_Err_flag = 1;	 //输出过压故障标志位
			Next_State = Err;		 		 //状态机跳转到故障
		}		
	}
	else												
	{
		V_out_OVPCnt = 0;									//标志位清零
	}
}


/***
 *			输入过流保护
 *
 */
void I_in_OCP()
{
	if((I_in > I_in_max)&&(Next_State == Run))		//当输出电流大于30A，且保持*毫秒  I_in_max
//	  if(I_in > I_in_max)		//当输出电流大于30A，且保持*毫秒  I_in_max
	{		
		I_in_OCPCnt++;						//条件保持计时
		if(I_in_OCPCnt > 20)			//条件保持100ms，则认为过流
		{
			I_in_OCPCnt = 0;				//标志位清零
			Burst_flag = 0;		//禁止使用Burst模式
			Close_PWM_LLC();	      //关闭原边PWM
			I_in_OCP_Err_flag = 1;	//错误标志位置1
			Next_State = Err;				//状态机跳转到故障状态
		}		
	}
	else
	{
		I_in_OCPCnt = 0;				  //标志位清零
	}
	
	if(I_in_OCP_Err_flag == 1)	//输出过流恢复
	{
		I_in_RSCnt++;						//计数标志++
		if(I_in_RSCnt > 80)			//等待4s钟
		{
			I_in_RSCnt= 0;					//技术标志位清零
			I_in_RSNum++;					//重启次数标志位++
			if(I_in_RSNum > 10)		//重启次数大于10次
			{
				I_in_RSNum = 11;			//确保不清楚故障，不再重启
				Burst_flag = 0;		//禁止使用Burst模式
				Close_PWM_LLC();			//关闭原边PWM
			}
			else
			{
				I_in_OCP_Err_flag = 0;		//输出过流故障标志位清零
			}
		}
	}
}



/***
 *			输出过流保护
 *
 */
void I_out_OCP()
{
	if((I_out > I_out_max)&&(Next_State == Run))		//当输出电流大于30A，且保持*毫秒 I_out_max
//	if(I_out > I_out_max)		//当输出电流大于30A，且保持*毫秒 I_out_max
	{		
		I_out_OCPCnt++;						//条件保持计时
		if(I_out_OCPCnt > 20)			//条件保持100ms，则认为过流
		{
			I_out_OCPCnt = 0;				//标志位清零
			Burst_flag = 0;		//禁止使用Burst模式
			Close_PWM_LLC();	      //关闭原边PWM
			I_out_OCP_Err_flag = 1;	//错误标志位置1
			Next_State = Err;				//状态机跳转到故障状态
		}		
	}
	else
	{
		I_out_OCPCnt = 0;				  //标志位清零
	}
	
	if(I_out_OCP_Err_flag == 1)	//输出过流恢复
	{
		I_out_RSCnt++;						//计数标志++
		if(I_out_RSCnt > 80)			//等待4s钟
		{
			I_out_RSCnt= 0;					//技术标志位清零
			I_out_RSNum++;					//重启次数标志位++
			if(I_out_RSNum > 10)		//重启次数大于10次
			{
				I_out_RSNum = 11;			//确保不清楚故障，不再重启
				Burst_flag = 0;		//禁止使用Burst模式
				Close_PWM_LLC();			//关闭原边PWM
			}
			else
			{
				I_out_OCP_Err_flag = 0;		//输出过流故障标志位清零
			}
		}
	}
}

///***
// *			谐振过流保护
// *
// */
//void I_cs_OCP()
//{
//	if((I_cs > I_cs_max)&&(Next_State == Run))		//当输入电流大于30A，且保持*毫秒
//	{		
//		I_cs_OCPCnt++;						//条件保持计时
//		if(I_cs_OCPCnt > 20)			//条件保持100ms，则认为过流
//		{
//			I_cs_OCPCnt = 0;				//标志位清零
//			Burst_flag = 0;		//禁止使用Burst模式
//			Close_PWM_LLC();	      //关闭原边PWM
//			I_cs_OCP_Err_flag = 1;	//错误标志位置1
//			Next_State = Err;				//状态机跳转到故障状态
//		}		
//	}
//	else
//	{
//		I_cs_OCPCnt = 0;				//标志位清零
//	}
//	
//	if(I_cs_OCP_Err_flag == 1)					//输入过流恢复
//	{
//		I_cs_RSCnt++;							//计数标志++
//		if(I_cs_RSCnt > 80)				//等待4s钟
//		{
//			I_cs_RSCnt = 0;					//技术标志位清零
//      I_cs_RSNum++;						//重启次数标志位++
//			if(I_cs_RSNum > 10)			//重启次数大于10次
//			{
//        I_cs_RSNum = 11;				//确保不清楚故障，不再重启
//				Burst_flag = 0;		//禁止使用Burst模式
//				Close_PWM_LLC();	//关闭原边PWM
//			}
//			else
//			{
//				I_cs_OCP_Err_flag = 0;		//输入过流故障标志位清零
//			}
//		}
//	}
//}


/***
 *			mos过温保护
 *
 */
void MOS_OTP()
{
	if((MOS_T > 800)&&(Next_State == Run))		//当输入电流大于30A，且保持*毫秒
	{		
		I_cs_OCPCnt++;						//条件保持计时
		if(I_cs_OCPCnt > 20)			//条件保持100ms，则认为过流
		{
			I_cs_OCPCnt = 0;				//标志位清零
			Burst_flag = 0;		//禁止使用Burst模式
			Close_PWM_LLC();	      //关闭原边PWM
			I_cs_OCP_Err_flag = 1;	//错误标志位置1
			Next_State = Err;				//状态机跳转到故障状态
		}		
	}
	else
	{
		I_cs_OCPCnt = 0;				//标志位清零
	}
	
	if(I_cs_OCP_Err_flag == 1)					//输入过流恢复
	{
		I_cs_RSCnt++;							//计数标志++
		if(I_cs_RSCnt > 80)				//等待4s钟
		{
			I_cs_RSCnt = 0;					//技术标志位清零
      I_cs_RSNum++;						//重启次数标志位++
			if(I_cs_RSNum > 10)			//重启次数大于10次
			{
        I_cs_RSNum = 11;				//确保不清楚故障，不再重启
				Burst_flag = 0;		//禁止使用Burst模式
				Close_PWM_LLC();	//关闭原边PWM
			}
			else
			{
				I_cs_OCP_Err_flag = 0;		//输入过流故障标志位清零
			}
		}
	}
}


/***
 *	空载或轻载情况,调频不能满足输出电压需求，Burst控制，关闭PWM
 *
 */
void Burst()
{	
	if(Burst_flag == 1)								//进入Burst模式
	{
		if(V_out > 60.0f)//?????????????????????
		{
			if(PWM_Falg == 0)
			{
				PWM_Falg = 1;									//标志位置1
				Close_PWM_LLC();							//关闭PWM
			}
		}
		else
		{
			if(PWM_Falg == 1)
			{
				PWM_Falg = 0;									//清除标志位
				Open_PWM_LLC();								//打开PWM
			}
		}
	}
}


/***
 * 		快速保护函数
 *		输入短路保护和原边硬件过流封波，主回路切断
 */
void FastProtection()
{
	if(Next_State != Init)			//状态机不是初始化状态
	{
		I_in_Short_Protect();			//输入短路保护
		
	}
}

/***
 * 		慢速保护函数
 *		执行输出过流保护
 */
void SlowProtection()
{
	if(Next_State != Init)			//状态机不是初始化状态
	{
//		if(I_out>1.0f)
//		{
//				Cap_protection();         //容性区保护
//		}
    MOS_OTP();
//		Vout_uvp_Protect();					//输入欠压保护
//		I_cs_OCP();					        //输出过流保护		
		I_out_OCP();					      //输出过流保护
		I_in_OCP();	
		Vout_ovp_Protect();					//输出过压保护
		Vout_uvp_Protect();
		Vin_ovp_Protect();					//输出过压保护
		Vin_uvp_Protect();
	}
}

/***
 *			输入短路保护
 *
 */	
void I_in_Short_Protect()
{	
	if((I_in > 10.0f) && (V_in < 20.0f))	//当输电流大于50A(2.5v)和输入电压小于20V（0.2v）即判定为输入短路
	{
		
		Close_PWM_LLC();				//关闭原边PWM
//		RELAY1_OFF();           //主接触器断开
		Short_Err_flag = 1;			//故障标志位置1
		Next_State = Err;		    //跳转至故障状态		
	}
}



/***
 *		初始化状态函数，参数函数初始化
 *
 */
void StateMInit()
{
	uint8_t FirstFlag = 0;			//初始化标志位(只初始化一次)
	
	if(FirstFlag == 0)
	{
		FirstFlag = 1;						//上电只初始化一次
		ValInit();								//相关参数初始化
	}
	Next_State = Wait;					//状态机跳转置等待状态
}

/***
 *		等待状态机，等待*S后无故障则软启
 *
 */
void StateMWait()
{	

	cnt = cnt + 1;									//每次进加一
	
	if(cnt > 40)										//等待200ms
	{
		cnt = 0;
		//在无故障的情况下启动
		if((I_out_OCP_Err_flag == 0) && (V_out_OVP_Err_flag == 0))
		{
			Next_State = Rise;					//状态机跳转到等待状态
			Soft_Wave = SSInit;					//软启动子状态跳转至初始化状态
		}
	}
}

/***
 *		软启阶段
 *
 */
void StateMRise()
{
	Soft_Wave = Next_Soft;
	
	switch(Soft_Wave)					//判断软启状态
	{
		case SSInit:						//初始化状态
			Burst_flag = 0;				//禁止使用Burst模式
			Close_PWM_LLC();			//关闭原边PWM	
			Next_Soft = SSWait;		//跳转至软启等待状态
		break;
		
		case SSWait:						//等待软启动状态			
			Burst_flag = 1;				//使能Burst模式
			Open_PWM_LLC();				//打开原边PWM
			Next_Soft = SSRun;		//跳转至软启状态
		break;
		
		case SSRun:							//软启动状态					
			if(Soft_Start_End == 0)									//软起动未结束
			{
				if(Vset_target < 48.0f)									//电压参考值小于48V
				{
					Vset_target = Vset_target + 0.05f;			//电压参考值递加0.01V
				}
				if(Vset_target >= 48.0f)								//电压参考值大于48V
				{
					Vset_target = 48.0f;									//电压参考值等于48V
					Soft_Start_End = 1;									//软起动结束
					Next_Soft = SSInit;									//软起动跳转至初始化
					Next_State = Run;										//状态机跳转到正常运行状态
				}
			}			
		break;
		
		default:
			break;
	}
}







/***
 *		运行状态
 *
 */
void StateMRun()
{
	OLED_ShowString(50,48,"Normal",16,1);
//	OLED_Refresh();
}

/***
 *		故障状态
 *
 */
void StateMErr()
{

	Close_PWM_LLC();							//关闭原边PWM
//	OLED_ShowString(50,48,"Error!",16,1);
//	OLED_Refresh();
//	RELAY1_OFF();                 //断开主接触器

}

/***
 * 		相关参数初始化函数
 *
 */
void ValInit()
{
	Close_PWM_LLC();							//关闭原边PWM
  
//	PFC_OK_flag = 0;              //pfc电压OK
	Burst_flag = 0;								//禁止Burst模式
  Short_Err_flag = 0;						//初始化输入短路故障标志位
	I_cs_OCP_Err_flag = 0;				//初始化输出过流故障标志位
	I_out_OCP_Err_flag = 0;				//初始化输出过流故障标志位
  V_out_OVP_Err_flag = 0;				//初始化输入过压故障标志位
  V_out_UVP_Err_flag = 0;				//初始化输入过压故障标志位
	I_in_OCP_Err_flag = 0;				//初始化输出过流故障标志位
  V_in_OVP_Err_flag = 0;				//初始化输入过压故障标志位
  V_in_UVP_Err_flag = 0;				//初始化输入过压故障标志位
	
}
/***
 *		开启原边mos管PWM(PA8,PA9,PA10,PA11)
 *
 */
void Open_PWM_LLC()
{
	 HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2);
}

/***
 *		关闭原边mos管PWM(PA8,PA9,PA10,PA11)
 *
 */		
void Close_PWM_LLC()
{
	HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2);
}
