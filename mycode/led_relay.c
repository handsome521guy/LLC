#include "LED_RELAY.h"
#include "gpio.h"

//系统正常运行指示灯
void LED_G_ON(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);  //输出高电平
}
void LED_G_OFF(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);  //输出高电平
}

void LED_GREEN_ON(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //输出高电平
}
void LED_GREEN_OFF(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);  //输出高电平	
}


void LED_RED_ON(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);  //输出高电平
}
void LED_RED_OFF(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);  //输出高电平
}


void LED_BLUE_ON(void)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);  //输出高电平
}
void LED_BLUE_OFF(void)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);  //输出高电平
}

void VGA_1(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);  //A_VGA

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET); //B_VGA
}

void VGA_2(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);  //A_VGA

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET); //B_VGA
}

void VGA_4(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);  //A_VGA

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET); //B_VGA
}

void VGA_10(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);  //A_VGA

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET); //B_VGA
}


	


////继电器1，控制主电路接触器
//void RELAY1_ON(void)
//{
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);  //高高低吸
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //吸合
//	
//	
//}
//void RELAY1_OFF(void)
//{
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);  //吸合
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);  //断开
//	
//}


////继电器2，控制副接触器（母线电容预充电）
//void RELAY2_ON(void)
//{
//	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);  //断开
//	
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);  //吸合
//	
//}
//void RELAY2_OFF(void)
//{
//	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);  //吸合
//	
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);  //断开
//}



