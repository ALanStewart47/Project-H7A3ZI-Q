#include "Led_NuH7A3Q.h"

#include <stdlib.h>

extern int rand (void);

extern void srand (unsigned int seed);

/**
 * @brief  Create a random number,range 
 * @note   
 * @retval 
 */
int create_random_number()
{
  uint32_t number = 0;
	uint32_t temp = 0;
	uint32_t time = 0;
	
	time=HAL_GetTick();
	srand((unsigned)time);
	temp = rand();
  number=rand()%3000+200;
	
	
	
  return number;
}


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

void LED_Toggle(GPIO_TypeDef *GPIO,uint16_t pin,uint32_t delay)
{
		
    HAL_GPIO_TogglePin(GPIO,pin);
	
		HAL_Delay(delay);
}

void Random_LED_Toggle(GPIO_TypeDef *GPIO,uint16_t pin)
{
		
    HAL_GPIO_TogglePin(GPIO,pin);
	
		HAL_Delay(create_random_number());
}


