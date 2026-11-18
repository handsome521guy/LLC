//#include "main.h"
//#include "adc.h"
//#include "comp.h"
//#include "dac.h"
//#include "dma.h"
//#include "hrtim.h"
//#include "tim.h"
//#include "usart.h"
//#include "gpio.h"
//#include "function.h"


//uint8_t RUN_flag =0;                  //启停标志位

//uint8_t Time_Base = 0;								//5ms标志位
//uint8_t Time_5ms_Fnish = 0;						//5ms完成标志位
//extern uint32_t ADC2_DMA_Value[4];
//extern uint32_t ADC2_Value[4];			  //
//extern float V_in;					  				//输入电压
//extern float I_in;					  				//输入电流
//extern float MOS_T;                   //温度检测
//extern float I_out;					 	 	  		//输出电流


////启停按键
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if(HAL_GPIO_ReadPin(GPIOC,START_Pin) == GPIO_PIN_SET)
//	{
//		RUN_flag=1;
//	}
//	if(HAL_GPIO_ReadPin(GPIOC,STOP_Pin) == GPIO_PIN_SET)
//	{
//		RUN_flag=0;
//	}
//}





//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)	
//{
//	Time_Base++;
//  ADCSample();	
//	
//}




//void main_loop()
//{
//	HAL_HRTIM_WaveformCounterStart(&hhrtim1, HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B);
//  HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2);
//  HAL_TIM_Base_Start_IT(&htim2);					                            	//打开定时器2
//	HAL_StatusTypeDef HAL_TIM_Base_Start_IT (TIM_HandleTypeDef * htim2);	//使能定时器2中断
//	HAL_TIM_Base_Start(&htim3);																				  	//打开定时器3（85KHZ）
//	HAL_TIM_Base_Start(&htim15);																				  //打开定时器15（50KHz）
//	
//	
//	HAL_COMP_Start(&hcomp2);
//	HAL_DAC_SetValue(&hdac3,DAC1_CHANNEL_1,DAC_ALIGN_12B_R,4095);
//	HAL_DAC_Start(&hdac3,DAC2_CHANNEL_1); //Start DAC
//	HAL_DMA_Start(&hadc2,(uint32_t*)ADC2_DMA_Value,4);
//	
//	
//	while(1)
//	{
//	}
//	
//}
