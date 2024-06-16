#include "bsp_timer_ic.h"


uint8_t		Tim_Ch1_Capture_H_Flag = 0;			/*	捕获到下降沿标志位	*/
uint8_t		Tim_Ch1_Capture_Complete_Flag = 0;	/*	捕获完成标志位	*/
uint8_t		Tim_Ch1_Capture_Counter = 0;		/*	溢出次数标志位	*/
uint32_t	Tim_Ch1_Capture_Value = 0;			/*	输入捕获值	*/



void bsp_TIM_Hard_Init(TIM_HandleTypeDef tim_handle,	TIM_TypeDef * _tim,	uint32_t Prescaler)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
 
  tim_handle.Instance = _tim;
  tim_handle.Init.Prescaler = Prescaler;		//200-1
  tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  tim_handle.Init.Period = 4294967295;
  tim_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&tim_handle) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&tim_handle, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&tim_handle) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&tim_handle, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&tim_handle, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

	HAL_TIM_IC_Start_IT(&tim_handle,TIM_CHANNEL_1);   //开始捕获TIM5的通道1
  __HAL_TIM_ENABLE_IT(&tim_handle,TIM_IT_UPDATE);   //使能更新中断

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(Tim_Ch1_Capture_Complete_Flag	==	0)
	{
		if(Tim_Ch1_Capture_H_Flag	==	1)
		{
			if(Tim_Ch1_Capture_Counter ==	0xFF)
			{
				Tim_Ch1_Capture_Complete_Flag	=	1;
				Tim_Ch1_Capture_Value	=	0XFFFFFFFF	;
			}
			else
			{
				Tim_Ch1_Capture_Counter++;
			}
		}
	}
}

/**
*	Input Capture callback
**/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(Tim_Ch1_Capture_Complete_Flag	==	0)
	{
		if(Tim_Ch1_Capture_H_Flag	==	1)	
		{
			Tim_Ch1_Capture_H_Flag	=	0;
			Tim_Ch1_Capture_Complete_Flag	=	1;
			Tim_Ch1_Capture_Value	=	HAL_TIM_ReadCapturedValue(&htim5,TIM_CHANNEL_1);
			TIM_RESET_CAPTUREPOLARITY(&htim5,TIM_CHANNEL_1);
			TIM_SET_CAPTUREPOLARITY(&htim5,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);
		}
		else			/*	捕获到上升沿	*/
		{
			Tim_Ch1_Capture_Value	=	0;
			Tim_Ch1_Capture_H_Flag	=	1;	

			__HAL_TIM_DISABLE(&htim5);
			__HAL_TIM_SET_COUNTER(&htim5,0);
			TIM_RESET_CAPTUREPOLARITY(&htim5,TIM_CHANNEL_1);
			TIM_SET_CAPTUREPOLARITY(&htim5,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);
			__HAL_TIM_ENABLE(&htim5);
		}
	}
}


void bsp_TIM5_CH1_Capture_Hight_Time(void)
{
	long	long	temp	=	0;
	long	double	temp_sec	=	0;
	
	if(Tim_Ch1_Capture_Complete_Flag	==	1)
	{
		temp	=	Tim_Ch1_Capture_Counter;
		temp	*=	0XFFFFFFFF;
		temp	+=	Tim_Ch1_Capture_Value;
		temp_sec	=	(long	double)	temp	/	1000000	;
			
		printf("%lld	\r\n",temp);
		printf("Second: %.3Lf \r\n",temp_sec);
		Tim_Ch1_Capture_Complete_Flag	=	0;
		Tim_Ch1_Capture_Counter	=	0;
		Tim_Ch1_Capture_Value	=	0;
	}
}



/*******************************	end file	*************************************/