

#include "modbus.h"
#include "usart.h"
#include "main_init.h"
uint8_t	Modbus_Addr=1;			//从机地址
uint8_t	Modbus_RX_BUFF[200];	//接收缓冲区2048字节
uint16_t Modbus_RX_CNT=0;		//接收计数器
uint16_t Modbus_RX_EN=0;			//接收倒计时
uint8_t	Modbus_FrameFlag=0;		//帧结束标记
uint8_t	Modbus_TX_FrameFlag=0;	//发送标记
uint8_t	Modbus_TX_BUFF[200];	//发送缓冲区
uint8_t	Modbus_RX_LEN=0;		//接收数据长度
uint16_t	Modbus_TX_EN=0;			//发送倒计时 
extern float PWM_angle; 
extern float V_in;					  				//输入电压
extern float I_in;					  				//输入电流
extern float MOS_T;                   //温度检测
extern float I_out;	
extern float PF;  
extern float PWM_Peroid; 

extern float V_in_1;
extern float I_in_1;	
extern float MOS_T_1;
extern float I_out_1;
extern float V_out_1;
extern float EF;
//extern float PWM_angle_1;               		
//extern float PF_1; 
extern float Freq_1;
extern float power;

//float I_out_1=0.0f;
//float MOS_T_1=0.0f;


//uint16_t Send_to_HMI[6]={0};
//Send_to_HMI[0]=I_out;
//uint16_t I_out_1=I_out;

//extern uint32_t ADC2_DMA_Value[4];
//extern uint32_t ADC2_Value[4];	

unsigned int GetCRC16(unsigned char *ptr,  unsigned char len)
{ 
    uint16_t index;
    uint8_t crcl = 0xFF;  //高CRC字节
    uint8_t crch = 0xFF;  //低CRC字节
    uint8_t  TabH[] = {  //CRC高位字节值表
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,  
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,  
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,  
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,  
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,  
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,  
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,  
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40  
    } ;  
    uint8_t TabL[] = {  //CRC低位字节值表
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,  
        0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,  
        0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,  
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,  
        0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,  
        0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,  
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,  
        0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,  
        0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,  
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,  
        0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,  
        0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,  
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,  
        0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,  
        0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,  
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,  
        0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,  
        0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,  
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,  
        0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,  
        0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,  
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,  
        0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,  
        0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,  
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,  
        0x43, 0x83, 0x41, 0x81, 0x80, 0x40  
    } ;
 
    while (len--)  //计算指定长度的CRC
    {
        index = crcl ^ *ptr++;
        crcl = crch ^ TabH[index];
        crch = TabL[index];
    }
    
    return ((crcl<<8) | crch);  
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Modbus寄存器和单片机寄存器的映射关系
uint16_t	Modbus_IO[100];	//输出开关量寄存器指针(这里使用的是位带操作)

//////////////////////////////////////////////////////////////////////////////
//发送n个字节数据
//buff:发送区首地址
//len：发送的字节数
void Modbus_SendData(uint8_t *buff,uint8_t len)
{
	uint8_t t;
	for(t=0;t<len;t++)	
	{
		while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)==RESET);//传输完成标志位
//		HAL_UART_Transmit(&huart1,(uint8_t*)&buff[t],10,HAL_Delay(100));
//		USART1->RDR & 0x01FF;	
		USART1->TDR=buff[t];
//		HAL_UART_Transmit(&huart1,(uint8_t*)buff,t,100);
	}	 
	while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)==RESET);

			
}


uint8_t biaozhiwei[20];
////,biaozhiwei_jiaobiao=0;
void shunxuzhixing(void)
{
	if(biaozhiwei[0])
	{
//		biaozhiwei_jiaobiao=0;
		is_fasong_06=1;
		jicunqi_06=2;
		neirong_06=I_out*1000.0f;
		return;
	}
	
	if(biaozhiwei[1])
	{
//		biaozhiwei_jiaobiao=1;
		is_fasong_06=1;
		jicunqi_06=3;
		neirong_06=MOS_T*1000.0f;
		return;
	}
	
	if(biaozhiwei[2])
	{
//		biaozhiwei_jiaobiao=2;
		is_fasong_06=1;
		jicunqi_06=4;
		neirong_06=I_in*1000.0f;
		return;
	}
	
		if(biaozhiwei[3])
	{
//		biaozhiwei_jiaobiao=3;
		is_fasong_06=1;
		jicunqi_06=5;
		neirong_06=V_in*1000.0f;
		return;
	}
	
		if(biaozhiwei[4])
	{
//		biaozhiwei_jiaobiao=4;
		is_fasong_16=1;
		jicunqi_06=6;
		neirong_06=PWM_angle*1000.f;
		return;
	}
	
		if(biaozhiwei[5])
	{
//		biaozhiwei_jiaobiao=5;
		is_fasong_06=1;
		jicunqi_06=7;
		neirong_06=PF*1000.0f;
		return;
	}
}
void shunxuquxiao(void)
{
		int i=0;
		for(i=0;i<20;i++)
		{
			if(biaozhiwei[i])
			{
			biaozhiwei[i]=0;
				return;
			}
		}
}

/////////////////////////////////////////////////////////////////////////////////////
//Modbus服务程序，用于处理接收到的数据(请在主函数中循环调用)
uint16_t startRegAddr;
uint16_t  RegNum;
uint16_t  calCRC;
void Modbus_Service(void)
{
	uint16_t  recCRC;
	if(Modbus_FrameFlag==1)
	{
		calCRC = GetCRC16(Modbus_RX_BUFF,Modbus_RX_LEN-2);//计算CRC,除去CRC校验位的那些位
		recCRC=Modbus_RX_BUFF[Modbus_RX_LEN-1]|(((uint16_t )Modbus_RX_BUFF[Modbus_RX_LEN-2])<<8);//接收的
		if(calCRC==recCRC)//CRC校验正确
		{
			if(Modbus_RX_BUFF[0]==Modbus_Addr)//地址正确
			{
				startRegAddr=(((uint16_t )Modbus_RX_BUFF[2])<<8)|Modbus_RX_BUFF[3];//获取寄存器起始地址
				switch(Modbus_RX_BUFF[1])//根据不同的功能码进行处理
				{   //回复可关
					case 03: //读多个寄存器
					{
						Modbus_03_Solve();
						break;
					}
					case 06: //写单个寄存器
					{
						is_fasong_06=0;
						shunxuquxiao();
//						biaozhiwei[biaozhiwei_jiaobiao]=0;
//						Modbus_06_Solve();
						break;
					}
					case 16: //写多个寄存器
					{
						is_fasong_16=0;
						Modbus_16_Solve();
						break;
					}
					default:break;
            //回复可关
				}
			}
		}
		Modbus_FrameFlag=0;//复位帧结束标志
		Modbus_RX_CNT=0;//接收计数器清零
		Modbus_RX_LEN = 0;
	}
}
//这是处理要发送的数据
#define qingqiudizhi 1
#define jicunqidizhi 0
#define jicunqichangdu 30

uint16_t is_fasong_06=0;//是否发送06
uint16_t jicunqi_06;
uint16_t neirong_06;
uint16_t is_fasong_16=0;//是否发送16
uint16_t is_fasong_3=0;//是否发送16

void fasong03(void)
{
	Modbus_TX_FrameFlag=0;
	Modbus_TX_BUFF[0]=qingqiudizhi;
	Modbus_TX_BUFF[1]=0x03;
	Modbus_TX_BUFF[2]=jicunqidizhi/256;
	Modbus_TX_BUFF[3]=jicunqidizhi%256;
	Modbus_TX_BUFF[4]=jicunqichangdu/256;
	Modbus_TX_BUFF[5]=jicunqichangdu%256;
	calCRC = GetCRC16(Modbus_TX_BUFF,6);
	Modbus_TX_BUFF[6]=calCRC/256;
	Modbus_TX_BUFF[7]=calCRC%256;
	Modbus_SendData(Modbus_TX_BUFF,8);
}

void fasong06(uint16_t  jicunqi,uint16_t  neirong)
{
	Modbus_TX_FrameFlag=0;
	Modbus_TX_BUFF[0]=qingqiudizhi;
	Modbus_TX_BUFF[1]=0x06;
	Modbus_TX_BUFF[2]=jicunqi/256;
	Modbus_TX_BUFF[3]=jicunqi%256;
	Modbus_TX_BUFF[4]=neirong/256;
	Modbus_TX_BUFF[5]=neirong%256;
	calCRC = GetCRC16(Modbus_TX_BUFF,6);
	Modbus_TX_BUFF[6]=calCRC/256;
	Modbus_TX_BUFF[7]=calCRC%256;
	Modbus_SendData(Modbus_TX_BUFF,8);
}

//void fasong16(uint16_t  jicunqi,uint16_t  changdu)
//{
//	Modbus_TX_FrameFlag=0;
//	Modbus_TX_BUFF[0]=qingqiudizhi;
//	Modbus_TX_BUFF[1]=0x10;
//	Modbus_TX_BUFF[2]=jicunqi/256;
//	Modbus_TX_BUFF[3]=jicunqi%256;
//	Modbus_TX_BUFF[4]=changdu/256;
//	Modbus_TX_BUFF[5]=changdu%256;
//	calCRC = GetCRC16(Modbus_TX_BUFF,6);
//	Modbus_TX_BUFF[6]=calCRC/256;
//	Modbus_TX_BUFF[7]=calCRC%256;
//	Modbus_SendData(Modbus_TX_BUFF,8);
//}

//void fasong16(uint16_t  jicunqi,uint16_t  changdu,uint16_t zijieshu)
//{
//	Modbus_TX_FrameFlag=0;
//	Modbus_TX_BUFF[0]=qingqiudizhi;
//	Modbus_TX_BUFF[1]=0x10;
//	Modbus_TX_BUFF[2]=jicunqi/256;
//	Modbus_TX_BUFF[3]=jicunqi%256;
//	Modbus_TX_BUFF[4]=changdu/256;
//	Modbus_TX_BUFF[5]=changdu%256;
//	Modbus_TX_BUFF[6]=zijieshu/256;
//	Modbus_TX_BUFF[7]=(uint16_t)I_out_1/256;
//	Modbus_TX_BUFF[8]=(uint16_t)I_out_1%256;
//	calCRC = GetCRC16(Modbus_TX_BUFF,9);
//	Modbus_TX_BUFF[9]=calCRC/256;
//	Modbus_TX_BUFF[10]=calCRC%256;
//	
//	Modbus_SendData(Modbus_TX_BUFF,11);
//}


void fasong16(uint16_t  jicunqi,uint16_t  changdu,uint16_t zijieshu)
{
	Modbus_TX_FrameFlag=0;
	Modbus_TX_BUFF[0]=qingqiudizhi;
	Modbus_TX_BUFF[1]=0x10;
	Modbus_TX_BUFF[2]=jicunqi/256;
	Modbus_TX_BUFF[3]=jicunqi%256;
	Modbus_TX_BUFF[4]=changdu/256;
	Modbus_TX_BUFF[5]=changdu%256;
	Modbus_TX_BUFF[6]=zijieshu%256;
	Modbus_TX_BUFF[7]=(uint16_t)I_out_1/256;
	Modbus_TX_BUFF[8]=(uint16_t)I_out_1%256;
	
	Modbus_TX_BUFF[9]=(uint16_t)MOS_T_1/256;
	Modbus_TX_BUFF[10]=(uint16_t)MOS_T_1%256;
	
	Modbus_TX_BUFF[11]=(uint16_t)I_in_1/256;
	Modbus_TX_BUFF[12]=(uint16_t)I_in_1%256;
	
	Modbus_TX_BUFF[13]=(uint16_t)V_in_1/256;
	Modbus_TX_BUFF[14]=(uint16_t)V_in_1%256;
	
	Modbus_TX_BUFF[15]=(uint16_t)V_out_1/256;
	Modbus_TX_BUFF[16]=(uint16_t)V_out_1%256;
	
	Modbus_TX_BUFF[17]=(uint16_t)EF/256;
	Modbus_TX_BUFF[18]=(uint16_t)EF%256;
	
	Modbus_TX_BUFF[19]=(uint16_t)Freq_1/256;
	Modbus_TX_BUFF[20]=(uint16_t)Freq_1%256;
	
	Modbus_TX_BUFF[21]=(uint16_t)power/256;
	Modbus_TX_BUFF[22]=(uint16_t)power%256;
	
	
	calCRC = GetCRC16(Modbus_TX_BUFF,23);
	Modbus_TX_BUFF[23]=calCRC/256;
	Modbus_TX_BUFF[24]=calCRC%256;
	
	Modbus_SendData(Modbus_TX_BUFF,25);
}




/*

地址码     功能码     寄存器起始地址     寄存器个数     字节数     数据N      CRC 
0            1          2      3           4     5      6   7         8
*/
//void fasong16(uint16_t  jicunqi,uint16_t  changdu,uint16_t  zijieshu)
//{
//	
//	Modbus_TX_FrameFlag=0;
//	Modbus_TX_BUFF[0]=qingqiudizhi;
//	Modbus_TX_BUFF[1]=0x10;
//	Modbus_TX_BUFF[2]=jicunqi/256;
//	Modbus_TX_BUFF[3]=jicunqi%256;
//	Modbus_TX_BUFF[4]=changdu/256;
//	Modbus_TX_BUFF[5]=changdu%256;
//	calCRC = GetCRC16(Modbus_TX_BUFF,6);
//	Modbus_TX_BUFF[6]=zijieshu/256;
//	Modbus_TX_BUFF[7]=zijieshu%256;
//	Modbus_TX_BUFF[8]=((int)I_out)*1000/256;
//	Modbus_TX_BUFF[9]=((int)I_out)*1000%256;
//	Modbus_TX_BUFF[10]=((int)MOS_T)/256;
//	Modbus_TX_BUFF[11]=((int)MOS_T)%256;
//	Modbus_TX_BUFF[12]=calCRC/256;
//	Modbus_TX_BUFF[13]=calCRC%256;
//	Modbus_SendData(Modbus_TX_BUFF,14);
//}



//发送
void Modbus_Service_SEND(void)
{

	if(Modbus_TX_FrameFlag==1)
		{
			
			
		
//			HAL_Delay(1000);
      
      	
		  if(is_fasong_16)
			{ 
				is_fasong_16=0;
				fasong16(2,8,16);
				
				
				
			
			}
			else
			{
        fasong03(); 
				is_fasong_16=1;

			}
			
		}	
	
	
//	{
//		shunxuzhixing();
//		
//		if(is_fasong_06)
//		{
//			
//			fasong06(jicunqi_06,neirong_06);
////			is_fasong_06=0;
//		}
//		else if(is_fasong_16)
//		{
//			
//      fasong16(2,7,14);
//			is_fasong_16=0;
//			
//		}
//		else
//		{
//				fasong03();		
//		}
//	}
	
}

////Modbus功能码03处理程序///////////////////////////////////////////////////////////////////////////////////////已验证程序OK
////读保持寄存器,读HMI屏幕上的频率设定
void Modbus_03_Solve(void)
{
	uint8_t i;	

	RegNum= (uint16_t)Modbus_RX_BUFF[2];//获取寄存器数量
	
	for(i=0;i<RegNum;i+=2)
	{
		Modbus_IO[jicunqidizhi+i/2]=Modbus_RX_BUFF[3+i]<<8|Modbus_RX_BUFF[4+i];//这是读取数据，在主函数中调用
	}
}

////Modbus功能码06处理程序   //////////////////////////////////////////////////////////////////////////////////已验证程序OK
////写单个保持寄存器
void Modbus_06_Solve(void)
{
	Modbus_IO[startRegAddr]=Modbus_RX_BUFF[4]<<8;//高字节在前                    ////////修改为高字节在前，低字节在后
	Modbus_IO[startRegAddr]|=((uint16_t)Modbus_RX_BUFF[5]);//低字节在后

	Modbus_TX_BUFF[0]=Modbus_RX_BUFF[0];
	Modbus_TX_BUFF[1]=Modbus_RX_BUFF[1];
	Modbus_TX_BUFF[2]=Modbus_RX_BUFF[2];
	Modbus_TX_BUFF[3]=Modbus_RX_BUFF[3];
	Modbus_TX_BUFF[4]=Modbus_RX_BUFF[4];
	Modbus_TX_BUFF[5]=Modbus_RX_BUFF[5];
	

	calCRC=GetCRC16(Modbus_TX_BUFF,6);
	Modbus_TX_BUFF[6]=(calCRC>>8)&0xFF;
	Modbus_TX_BUFF[7]=(calCRC)&0xFF;
	Modbus_SendData(Modbus_TX_BUFF,8);
}

////Modbus功能码16处理程序 /////////////////////////////////////////////////////////////////////////////////////////////////已验证程序OK
////写多个保持寄存器
void Modbus_16_Solve(void)
{
	uint8_t i;
	RegNum= (((uint16_t)Modbus_RX_BUFF[4])<<8)|((Modbus_RX_BUFF[5]));//获取寄存器数量
	if((startRegAddr+RegNum)<1000)//寄存器地址+数量在范围内
	{
		for(i=0;i<RegNum;i++)
		{
			Modbus_IO[startRegAddr+i]=Modbus_RX_BUFF[7+i*2]<<8; //低字节在前                 /////// 低字节在前，高字节在后正常
			Modbus_IO[startRegAddr+i]|=((uint16_t)Modbus_RX_BUFF[8+i*2]); //高字节在后
		}

		Modbus_TX_BUFF[0]=Modbus_RX_BUFF[0];
		Modbus_TX_BUFF[1]=Modbus_RX_BUFF[1];
		Modbus_TX_BUFF[2]=Modbus_RX_BUFF[2];
		Modbus_TX_BUFF[3]=Modbus_RX_BUFF[3];
		Modbus_TX_BUFF[4]=Modbus_RX_BUFF[4];
		Modbus_TX_BUFF[5]=Modbus_RX_BUFF[5];

		calCRC=GetCRC16(Modbus_TX_BUFF,6);
		Modbus_TX_BUFF[6]=(calCRC>>8)&0xFF;
		Modbus_TX_BUFF[7]=(calCRC)&0xFF;
		Modbus_SendData(Modbus_TX_BUFF,8);
	}
	else//寄存器地址+数量超出范围
	{
		Modbus_TX_BUFF[0]=Modbus_RX_BUFF[0];
		Modbus_TX_BUFF[1]=Modbus_RX_BUFF[1]|0x80;
		Modbus_TX_BUFF[2]=0x02; //异常码
		Modbus_SendData(Modbus_TX_BUFF,3);
	}
}

void Rec_Buf_clean(uint8_t *point,uint8_t len ) //清空数组
{
	int p;
	for (p=0;p<len;p++)
	{
		point[p]=0;
	}
}

//用于串口接收中断，进行组包
void Modbus_zubao(uint8_t Res)
{
	if(Modbus_RX_CNT<200)
	{
		Modbus_RX_BUFF[Modbus_RX_CNT++]=Res;
		Modbus_RX_EN = 0;
	}
	else
	{
		Rec_Buf_clean(Modbus_RX_BUFF,200);
		Modbus_RX_CNT = 0;
	}
}

//用于定时器，判断是否接收完成,注意数据长度
#define Modbus_RX_EN_MAX 30
void Modbus_Is_jieshou(void)
{
	Modbus_RX_EN++;
	if((Modbus_RX_EN>Modbus_RX_EN_MAX) &&(Modbus_RX_CNT!=0))
	{
		Modbus_RX_LEN = Modbus_RX_CNT;
		Modbus_FrameFlag=1;//置位帧结束标记

		Modbus_RX_EN = 0;
	}
}
//0.1s发一次
#define Modbus_TX_EN_MAX 200
void Modbus_Is_fasong(void) 
{
	Modbus_TX_EN++;//累加定时时间计数器
	if(Modbus_TX_EN>Modbus_TX_EN_MAX)//判断定时时间是否超过了设定的最大值
	{
		Modbus_TX_EN=0;//清除定时计数器 处理数据 清除buffer（）
		Modbus_TX_FrameFlag=1;//置位帧结束标记
		
	}
}



