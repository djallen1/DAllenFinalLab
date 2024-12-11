/*
 * game.c
 *
 *  Created on: Nov 24, 2024
 *      Author: 17202
 */
#include "game.h"
static block_t block;
static map_t map;
volatile uint8_t game_started;

void game_init()
{
	game_started = STARTED;
	//uint32_t start_time = HAL_GetTick();

	//STMPE811_t touch_data = {0};

	//while(STMPE811_ReadTouch(touch_data) != STMPE811_State_Pressed)
	//{
		//map_draw();
	//}

	//HAL_Delay(2000);
	LCD_Clear(0,LCD_COLOR_BLACK);
	map_draw();


	block = block_create();
	draw_tetromino(block);
	map = map_init();
}

void game_start_screen()
{
	game_started = NOT_STARTED;
	draw_shapes();
	start_screen();
}

uint8_t is_start_screen()
{
	return game_started;
}

void game_over()
{
	end_screen();
	uint32_t time = HAL_GetTick();
	disp_time(time);
	while(1){}
}

void game_drop()
{
	if(!rest(&block, &map))
	{
		//HAL_Delay(500);
		clear_tetromino(block);
		block = block_drop(&block);
		draw_tetromino(block);
	}
	else
	{
		map = map_update(&block, &map);
		block = block_create();
		if(can_spawn(&block, &map))
		{
			draw_tetromino(block);
		}
	}
}

void game_rotate()
{
	//HAL_Delay(500);
	clear_tetromino(block);
	block = block_rotate(&block);
	draw_tetromino(block);
}

void game_move(uint8_t dir)
{
	if(can_move(&block, &map, dir))
	{
		clear_tetromino(block);
		block = block_move(&block, &map, dir);
		draw_tetromino(block);
	}
}

uint8_t game_finished()
{
	if(can_spawn(&block, &map))
	{
		return 0;
	}
	return 1;
}
