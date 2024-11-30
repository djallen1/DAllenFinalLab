/*
 * Button_Driver.h
 *
 *  Created on: Sep 24, 2024
 *      Author: 17202
 */

#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include "stm32f4xx_hal.h"

#define BUTTON_PORT_VALUE GPIOA
#define BUTTON_PIN_NUMBER 0
#define BUTTON_ASSERT 1
#define BUTTON_NOT_ASSERT 0

void ButtonInit();
void ButtonClockEnable();
uint8_t ButtonPress();
void ButtonInitWithInterrupts();

#endif /* BUTTON_DRIVER_H_ */
