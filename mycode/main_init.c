#include "main.h"
#include "adc.h"
#include "comp.h"
#include "dac.h"
#include "dma.h"
#include "hrtim.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "function.h"
#include "main_init.h"
#include "led_relay.h"
#include "oled.h"
#include "rs485.h"
#include "modbus.h"
#include "ntc.h"
//#include "stdlib.h"
//#include "oledfont.h"
//#include "stdio.h"

//uint8_t RUN_flag=0;              êÖ¾Î»

extern uint32_t ADC2_DMA_Value[5];
extern uint32_t ADC2_Value[5];	


extern uint8_t Soft_Start_End;	

uint8_t Time_Base = 0;								//5ms±êÖ¾Î»
uint8_t Time_5ms_Fnish = 0;						//5msÍê³É±êÖ¾Î»

uint16_t I_in_max; 
uint16_t I_cs_max; 
uint16_t I_out_max; 
uint16_t V_in_max; 
uint16_t V_in_min; 
uint16_t V_out_max; 
uint16_t V_out_min; 
		  //

extern float Kp;             	   //±ÈÀýÏµÊý
extern float Ki;            		 //»ý·ÖÏµÊý
extern float Kd;           		 	 //Î¢·ÖÏµÊý

extern float h;             	   //±ÈÀýÏµÊý
extern float r;            		 //»ý·ÖÏµÊý
extern float Wo;           		 	 //Î¢·ÖÏµÊý
extern float Wc; 

extern float Vset_target;
extern float Iset_target;
extern float V_out;					  				//ÊäÈëµçÑ¹
extern float I_cs;					  				//ÊäÈëµçÁ÷
extern float MOS_T;                   //ÎÂ¶È¼ì²â
extern float I_out;					 	 	  		//Êä³öµçÁ÷

extern float MOS_T_1; 
//uint16_t PWM_T_Count;             //ÖÜÆÚ
//uint16_t PWM_D_Count;             //µçÑ¹µçÁ÷µÄ²îÖµ
extern float PWM_Peroid;            //ÆµÂÊ
uint16_t PWM_Peroid_2; 
uint16_t PWM_shift_1;
//extern uint32_t PWM_Peroid;
extern float Freq;
extern float PWM_angle;               //ÏàÎ»½Ç
extern float PF;                 //¹¦ÂÊÒòÊý
//uint16_t Iout_actual;
//extern uint32_t ADC2_Value[4];


///***
// *			ÆôÍ£°´¼ü
// *
// */
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if(HAL_GPIO_ReadPin(STOP_GPIO_Port,STOP_Pin) == GPIO_PIN_SET)
//	{
//		RUN_flag=0;
//		StateMErr();
//		RELAY1_OFF();
//	}
//	if(HAL_GPIO_ReadPin(START_GPIO_Port,START_Pin) == GPIO_PIN_SET)
//	{
//		HAL_Delay(1);
//		RELAY2_OFF();
//		HAL_Delay(100);
//		RELAY1_ON();
//		RUN_flag=1;
//	}
//}











/***
 *		¶¨Ê±Æ÷4ÖÐ¶Ï»Øµ÷º¯Êý(200us½øÈëÒ»´Î)
 *		
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)	
{
  if(htim !=&htim4)return;
	Time_Base++;												//5ms±êÖ¾Î»++
//	HAL_TIM_IC_capture();op
	
	ADCSample();											  //AD¶ÁÈ¡µçÑ¹µçÁ÷Öµ		
	PID_Incremental(V_out);			  //PIDÔËËã¸üÐÂÖÜÆÚ	
  Burst();
//	potentiometer();
//  HMI_peroid();
//	TIM_Capture();
	FastProtection();										//Êä³ö¶ÌÂ·±£»¤

	
	
//	State_Machine();									//×´Ì¬»úº¯Êý
//	SlowProtection();									//ÂýËÙ±£»¤º¯Êý		
	if(Time_Base == 250)									//¶¨Ê±5msÍê³É
	{
		
		Time_Base = 0;										//±êÖ¾Î»¹éÁã
		Time_5ms_Fnish = 1;								//5msÍê³É±êÖ¾Î»ÖÃ1
	}	
	if(Time_5ms_Fnish == 1)							//5ms¸ÃÔËÐÐµÄ³ÌÐò
	{
		Time_5ms_Fnish = 0;								//±êÖ¾Î»ÇåÁã
//		if(RUN_flag==1)
//		{
//    is_fasong_16=1;	
		MOS_T=temp_Cal(ADC2_Value[1])-100;
    MOS_T_1 =MOS_T*10.0f;
//    Iout_VGA();
		State_Machine();									//×´Ì¬»úº¯Êý
		SlowProtection();									//ÂýËÙ±£»¤º¯Êý
//			Modbus_Service_SEND();
//			Modbus_Service();		
//	}
		
	}
	
	
}



/***
 *			³õÊ¼»¯
 *
 */
void main_loop()
{
	HAL_HRTIM_WaveformCounterStart(&hhrtim1, HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B);

  

	
	HAL_TIM_Base_Start_IT(&htim3);																				  	//´ò¿ª¶¨Ê±Æ÷3(200K)
	HAL_TIM_Base_Start_IT(&htim4);	                                      //´ò¿ª¶¨Ê±Æ÷4(50k)
//	HAL_StatusTypeDef HAL_TIM_Base_Start_IT (TIM_HandleTypeDef * htim4);	//Ê¹ÄÜ¶¨Ê±Æ÷4ÖÐ¶Ï
	HAL_TIM_Base_Start(&htim15);																				  //´ò¿ª¶¨Ê±Æ÷15(1MHz)
	
//	HAL_COMP_Start(&hcomp2);
//	HAL_DAC_SetValue(&hdac3,DAC1_CHANNEL_1,DAC_ALIGN_12B_R,4095);
//	HAL_DAC_Start(&hdac3,DAC2_CHANNEL_1); //Start DAC
	
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);//ADC2µÄ¾«¶ÈÐ£×¼º¯Êý
	
	HAL_ADC_Start_DMA(&hadc2,(uint32_t *)ADC2_DMA_Value,5);
//	HAL_UART_Init(&huart1);//Ê¹ÄÜusart1

__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);//Ê¹ÄÜ½ÓÊÕ¼Ä´æÆ÷·Ç¿ÕÖÐ¶Ï
	
//	OLED_Init();
	
	while(1)
	{

//			is_fasong_16=1;
			Modbus_Service_SEND();
			Modbus_Service();
		
				 if(Soft_Start_End == 1)
		 {
		  	Vset_target=Modbus_IO[0]/1000.0f;//ÆµÂÊµÄÉè¶¨Öµ 
				if(Vset_target<40.0f)
				{
					Vset_target=40.0f;
				}
				Iset_target=Modbus_IO[1]/1000.0f;//½Ç¶ÈµÄÉè¶¨Öµ		
				if(Iset_target<1.0f)
				{
				Iset_target=1.0f;
				}
		 }
		
//     	Vset_target=Modbus_IO[0]/1000.0f;//ÆµÂÊµÄÉè¶¨Öµ
//			if(Vset_target<40.0f)
//			{
//			Vset_target=40.0f;
//			}
//			Iset_target=Modbus_IO[1]/1000.0f;//½Ç¶ÈµÄÉè¶¨Öµ		
//      if(Iset_target<1.0f)
//			{
//			Iset_target=1.0f;
//			}
		   
			V_out_max=Modbus_IO[10]/1000.0f; 
			I_in_max=Modbus_IO[11]/1000.0f; 
			I_out_max=Modbus_IO[12]/1000.0f; 
			
			V_in_max=Modbus_IO[13]/100.0f; 
			V_in_min=Modbus_IO[14]/100.0f; 
			V_out_min=Modbus_IO[15]/1000.0f; 
			I_cs_max=Modbus_IO[16]/1000.0f; 
			
			h=Modbus_IO[17]/1000.0f;             	   //±ÈÀýÏµÊý
			r=Modbus_IO[18]/100.0f;             		 //»ý·ÖÏµÊý
			Wo=Modbus_IO[19]/1000.0f;            		 	 //Î¢·ÖÏµÊý
			Wc=Modbus_IO[20]/1000.0f;  
			
			Kp=Modbus_IO[21]/1000.0f; 
      Ki=Modbus_IO[22]/1000.0f;
			Kd=Modbus_IO[23]/1000.0f;
			
			
//		  DISPLAY_Voltage(Freq,V_out,Vset_target);
//	    HAL_Delay(500);
//			DISPLAY_Voltage(Freq,V_out,Vset_target);
	}
	
}
