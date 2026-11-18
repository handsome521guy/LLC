#ifndef __RS485_H
#define __RS485_H

#include "main.h"
//#include "main.h"
//typedef enum {STOP = 0, START = !STOP} DP_FlagStatus;

//void USART_TEST(void);
//void MODS_Poll(void);
//void MODS_AnalyzeApp(void);
//void Get_crc1(void);
//uint16_t CRC16 ( uint8_t *arr_buff, uint8_t len);
//void MODS_01H(void);
//void MODS_06H(void);
void USART1_IRQHandler(void);
void TIM3_IRQHandler(void);
/******************************************************


寄存器定义

读取频率，功率因数角的设定值
写入输入电压，输入电流，输出电流，当前频率，功率因数角

*/

///******************************************************                                 设定输入电压
//# AA 电源板地址
//# 06写:单字节指令格式
//# 指令示例：写入输入电压值
//# AA 06 01 02 2E E0 CRC1 CRC2   设定输出电压12000mV(0x2EE0)
//# 从机返回
//# AA 06 01 02 00 01 CRC1 CRC2   返回指令
//*/
//#define Reg_Vin_Hbyte         			0x01   		 //设定输入电压    高字节
//#define Reg_Vin_Lbyte         			0x02  		 //设定输入电压   低字节
//#define Reg_vin                    0x0102




///******************************************************                                 设定输入电流
//# AA 电源板地址
//# 06写:单字节指令格式
//# 指令示例：写入输入电流值
//# AA 06 03 04 13 88 CRC1 CRC2   设定输出电流5000mA(0x1388)
//# AA 06 03 04 00 01 CRC1 CRC2   返回指令
//*/
//#define Reg_Iin_Hbyte         			0x03   		 //设定输出电流    高字节
//#define Reg_Iin_Lbyte         			0x04       //设定输出电流    低字节
//#define Reg_Iin                    0x0304



///******************************************************                                 设定输出电流
//# AA 电源板地址
//# 06写:单字节指令格式
//# 指令示例：写入输出电流值
//# AA 06 05 06 13 88 CRC1 CRC2   设定输出电流5000mA(0x1388)
//# AA 06 05 06 00 01 CRC1 CRC2   返回指令
//*/
//#define Reg_Iout_Hbyte         			0x05   		 //设定输出电流    高字节
//#define Reg_Iout_Lbyte         			0x06       //设定输出电流    低字节
//#define Reg_Iout                    0x0506



///******************************************************                                  读输出电压
//# AA 电源板地址
//# 01读:单字节指令格式
//# 指令示例：读取频率值
//# AA 01 09 A0 00 02 CRC1 CRC2   读取频率值
//# 从机返回
//# AA 01 02 2E E0 CRC1 CRC2   当前频率值：
//*/
//#define Reg_Peroid_Hbyte  						0x09       //读取频率值    高字节
//#define Reg_Peroid_Lbyte  						0xA0       //读取频率值   低字节  
//#define Reg_Peroid                    0x09A0




///******************************************************                                  读输出电流
//# AA 电源板地址
//# 01读:单字节指令格式
//# 指令示例：读功率因数角
//# AA 01 A1 A2 00 02 CRC1 CRC2   读取功率因数角
//# 从机返回
//# AA 01 02 13 88 CRC1 CRC2    当前功率因数角
//*/
//#define Reg_angle_Hbyte  						0xA1       //读取功率因数角    高字节
//#define Reg_angle_Lbyte  						0xA2       //读取功率因数角   低字节
//#define Reg_angle                    0xA1A2






#endif
