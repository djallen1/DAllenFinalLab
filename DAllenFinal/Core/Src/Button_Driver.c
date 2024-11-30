/*
 * Button_Driver.c
 *
 *  Created on: Sep 24, 2024
 *      Author: 17202
 */
#include "Button_Driver.h"

void ButtonInit()
{
	GPIO_InitTypeDef config = {0};

	config.Pin = GPIO_PIN_0;
	config.Mode = GPIO_MODE_IT_RISING;
	config.Speed = GPIO_SPEED_FREQ_HIGH;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &config);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	//GPIO_Init(BUTTON_PORT_VALUE, config, BUTTON_PIN_NUMBER);
}

uint8_t ButtonPress()
{
	uint8_t temp = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

	if(temp)
	{
		return BUTTON_ASSERT;
	}
	else
	{
		return BUTTON_NOT_ASSERT;
	}
}

