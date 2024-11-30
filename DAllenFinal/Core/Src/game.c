/*
 * game.c
 *
 *  Created on: Nov 24, 2024
 *      Author: 17202
 */
#include "game.h"
volatile block_t block1, block2;
volatile map_t map;
void game_init()
{
	//uint32_t start_time = HAL_GetTick();

	//STMPE811_t touch_data = {0};

	//while(STMPE811_ReadTouch(touch_data) != STMPE811_State_Pressed)
	//{
		//map_draw();
		draw_shapes();
		start_screen();
	//}

	HAL_Delay(2000);
	LCD_Clear(0,LCD_COLOR_BLACK);
	map_draw();
	//rng_init();
	block1 = block_create(6);
	draw_tetromino(block1);

	map = map_init();
	//game_over(start_time);
	for(int i=0;i<12;i++)
	{
		HAL_Delay(1000);
		block_drop(&block1, &map);
	}
	block2 = block_create(3);
	draw_tetromino(block2);
	for(int i=0;i<11;i++)
	{
		HAL_Delay(1000);
		block_drop(&block2, 0);
	}
}

void game_over(uint32_t start_time)
{
	//uint32_t total_time = display_time(start_time);
}

uint32_t display_time(uint32_t start_time)
{
	uint32_t end_time = HAL_GetTick();
	uint32_t time = end_time - start_time;
	return time;
}

void I2C3_EV_IRQHandler()
{
	//disable IRQ
	//lower flag
	//stuff
	//enable IRQ
}
