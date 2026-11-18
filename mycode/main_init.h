#ifndef  __MAIN_INIT_H
#define  __MAIN_INIT_H
#include "tim.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
//void HAL_TIM_IC_captureCallback(TIM_HandleTypeDef*htim);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim1);

//void HAL_TIM_IC_capture(void);


//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim15);

void Tsak_200us_Running(void);
void Tsak_5ms_Running(void);
void main_loop(void);

#endif
