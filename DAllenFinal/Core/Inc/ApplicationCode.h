/*
 * ApplicationCode.h
 *
 *  Created on:
 *      Author:
 */

//#include "LCD_Driver.h"
#include "game.h"
#include "Button_Driver.h"

#include <stdio.h>

#define ARR_VAL 59999
#define PRESCALAR_VAL 3999 // ~3s
//#define PRESCALAR_VAL 999 // ~0.75s

#ifndef INC_APPLICATIONCODE_H_
#define INC_APPLICATIONCODE_H_

void ApplicationInit(void);
void Application_game_inst(void);
void applicationButtonInit();
void start_timer();
uint8_t game_finished();

#if (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)
void LCD_Touch_Polling_Demo(void);
#endif // (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)

#endif /* INC_APPLICATIONCODE_H_ */
