/*
 * game.c
 *
 *  Created on: Nov 24, 2024
 *      Author: 17202
 */
#include "game.h"
static block_t block;
static map_t map;
void game_init()
{
	uint32_t start_time = HAL_GetTick();
	uint8_t rand = S;

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
	block = block_create(rand);
	draw_tetromino(block);

	map = map_init();
	//game_over(start_time);
//	for(int i=0;i<5;i++)
//	{
//
//	}
//	for(int i=0;i<10;i++)
//	{
//
//	}
	game_over(start_time);
}

void game_over(uint32_t start_time)
{
	uint32_t end_time = HAL_GetTick();
	uint32_t total_time = end_time - start_time;
	end_screen();
	disp_time(total_time);
}

uint32_t display_time(uint32_t start_time)
{
	uint32_t end_time = HAL_GetTick();
	uint32_t time = end_time - start_time;
	return time;
}

void game_drop()
{
	if(!rest(&block, &map))
		{
			HAL_Delay(500);
			clear_tetromino(block);
			block = block_drop(&block);
			draw_tetromino(block);
		}
		else
		{
			map = map_update(&block, &map);
			block = block_create(S);
			draw_tetromino(block);
		}
}

void game_rotate()
{
	HAL_Delay(500);
	clear_tetromino(block);
	block = block_rotate(&block);
	draw_tetromino(block);
}

void game_move(uint8_t dir)
{
	clear_tetromino(block);
	block = block_move(&block, &map, dir);
	draw_tetromino(block);
}

void I2C3_EV_IRQHandler()
{
	//disable IRQ
	//lower flag
	//stuff
	//enable IRQ
}
