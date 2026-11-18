/*
modbus做主机，
读取频率，功率因数角的设定值
写入输入电压，输入电流，输出电流，当前频率，功率因数角
写操作即是发送的数据 读操作即是读取数据个数
*/
#include "dma.h"
#include "usart.h"
#include "tim.h"
#include "rs485.h"
#include "modbus.h"


////uint8_t    HMI_command[25] 	 ={0};
////uint8_t    Send_to_HMI[10]   ={0};


////__IO DP_FlagStatus    Time_out_flag_485=START;
////uint8_t               RX_count_485=0x00;//接收HMI传来的信号
////uint8_t    RX_data[25] 	 ={0};
////uint8_t    TX_data[10]   ={0};




void USART1_IRQHandler(void)
{
	uint8_t Res;
   if(__HAL_UART_GET_IT(&huart1, UART_IT_RXNE) != RESET)
		 {
			 __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
		   Res =USART1->RDR & 0x01FF;	//读取接收到的数据,两个16位的数
//			 HAL_UART_Receive(&huart1,receiveData,2,HAL_MAX_DELAY);
		   Modbus_zubao(Res);			 
		 }
		 
}





//1ms进一次中断
void TIM3_IRQHandler(void) 																					//TIME3中断服务函数  需要设定中断优先级  即NVIC配置
{
	if(__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_UPDATE)!=RESET)				//判断是否发生了更新(溢出)中断
	{
		__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);											//清除中断源
		__HAL_TIM_CLEAR_FLAG(&htim3, TIM_IT_UPDATE);                   //清除标志位
//		__HAL_TIM_DISABLE(&htim3);                                      //关闭定时器
//		Time_out_flag_485=STOP;                                                 //接收停止，接收完毕，开始分析数据
//		MODS_Poll();
			Modbus_Is_fasong();
	    Modbus_Is_jieshou();
	}
}
