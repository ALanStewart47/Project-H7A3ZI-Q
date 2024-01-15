#include "Led_NuH7A3Q.h"

#include <stdlib.h>

extern int rand (void);

extern void srand (unsigned int seed);


/**
  * @brief LED GPIO Initialization Function
  * @param None
  * @retval None
  */
void LED_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();


  /*Configure GPIO pins : LD1_Pin LD3_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);



  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}


/**
 * @brief  Create a random number,range 
 * @note   generate random number
 * @retval random number
 */
int create_random_number(void)
{
  uint32_t number = 0;
	//uint32_t time = 0;
	
	//time=HAL_GetTick();
	srand((unsigned)HAL_GetTick());
  number=rand();
	
  return number;
}

/**
 * @brief  Create a random number of LED Port
 * @note   generate 1~3 number
 * @retval random number
 */
int Get_Random_LED_Port(void)
{
		uint8_t mode;
		srand(HAL_GetTick());
		mode=rand()%3+1;		//rand()%3-->0~2 £¬ 0~2+1-->1~3
		mode=create_random_number();
		
	if((mode>0)&&(mode<4))
	{
		return mode;
	}
	else
	{
		return 0;
	}
}

/**
 * @brief  Create a random number of LED Port
 * @note   generate 1~3 number
 * @retval random number
 */
void Random_Delay_LED_Toggle(GPIO_TypeDef *GPIO,uint16_t pin)
{
		
    HAL_GPIO_TogglePin(GPIO,pin);
	
		HAL_Delay(create_random_number()%401+150);
	
		HAL_GPIO_TogglePin(GPIO,pin);
}


