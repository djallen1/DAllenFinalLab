/*
 * game.c
 *
 *  Created on: Nov 24, 2024
 *      Author: 17202
 */
#include "game.h"

static block_t block;
static map_t map;
static uint8_t game_started;
static uint8_t singles = 0;
static uint8_t doubles = 0;
static uint8_t triples = 0;
static uint8_t tetris = 0;

uint8_t is_start_screen()
{
	return game_started;
}

void game_start_screen()
{
	game_started = NOT_STARTED;
	draw_shapes();
	start_screen();
}

void game_init()
{
	game_started = STARTED;
	LCD_Clear(0,LCD_COLOR_BLACK);

	map_draw();
	block = block_create();
	draw_tetromino(block);
	map = map_init();
}

void game_drop()
{
	if(!collision(&block, &map, DOWN))
	{
		//HAL_Delay(500);
		clear_tetromino(block);
		block = block_drop(&block);
		draw_tetromino(block);
	}
	else
	{
		map = map_update(&block, &map);

		if(num_levels_cleared(&map) != 0)
		{
			if(num_levels_cleared(&map) == 1)
			{
				singles++;
			}
			else if(num_levels_cleared(&map) == 2)
			{
				doubles++;
			}
			else if(num_levels_cleared(&map) == 3)
			{
				triples++;
			}
			else if(num_levels_cleared(&map) == 4)
			{
				tetris++;
			}

			clear_map(map);
			map = level_clear(&map);
			draw_updated_map(map);
		}

		block = block_create();
		if(can_spawn(&block, &map))
		{
			draw_tetromino(block);
		}
	}
}

void game_rotate()
{
	clear_tetromino(block);
	block = block_rotate(&block, &map);
	draw_tetromino(block);
}

void game_move(uint8_t dir)
{
	if(!collision(&block, &map, dir))
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

void game_over()
{
	end_screen(singles, doubles, triples, tetris);
	uint32_t time = HAL_GetTick();
	disp_time(time);
	while(1){}
}
