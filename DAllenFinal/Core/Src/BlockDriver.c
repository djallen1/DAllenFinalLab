/*
 * BlockDriver.c
 *
 *  Created on: Nov 21, 2024
 *      Author: 17202
 */

#include "BlockDriver.h"

static RNG_HandleTypeDef *hrng;

void map_draw()
{
	//uint16_t y_len = LCD_PIXEL_HEIGHT;
	uint16_t x_len = LCD_PIXEL_WIDTH;
	//edges of 5 pixels in width
	//lines at 0-5, 26, 47, 68, 89, 110, 131, 152, 173, 194, 215, 236-240
	uint16_t x = 0;
	//uint16_t y = 0;
	//VERTICAL
	for(int i = 1; i<LCD_PIXEL_WIDTH; i+=BLOCK_WIDTH)
	{
		LCD_Draw_Vertical_Line(EDGE_WIDTH+i, MAP_DIFFERENCE, MAP_HEIGHT, LCD_COLOR_WHITE);
	}
	for(int i = 1; i<=EDGE_WIDTH; i++)
	{
		LCD_Draw_Vertical_Line(i, MAP_DIFFERENCE, MAP_HEIGHT, LCD_COLOR_WHITE);
	}
	for(int i = LCD_PIXEL_WIDTH-EDGE_WIDTH; i<=LCD_PIXEL_WIDTH; i++)
	{
		LCD_Draw_Vertical_Line(i, MAP_DIFFERENCE, MAP_HEIGHT, LCD_COLOR_WHITE);
	}

	//HORIZONTAL
	for(int i = 1; i<MAP_HEIGHT; i+=BLOCK_WIDTH)
	{
		LCD_Draw_Horizontal_Line(x, MAP_DIFFERENCE+EDGE_WIDTH+i, x_len, LCD_COLOR_WHITE);
	}
	for(int i = 0; i<=EDGE_WIDTH; i++)
	{
		LCD_Draw_Horizontal_Line(x, MAP_DIFFERENCE+i, x_len, LCD_COLOR_WHITE);
	}
	for(int i = MAP_HEIGHT-EDGE_WIDTH; i<=MAP_HEIGHT; i++)
	{
		LCD_Draw_Horizontal_Line(x, MAP_DIFFERENCE+i, x_len, LCD_COLOR_WHITE);
	}
}

void start_screen()
{
	//uint16_t play_x = 68; //to 172
	uint16_t play_y = 106; //to 184
	//uint16_t button_height = 48;
	uint16_t button_width = 120;

	for(int i = 1; i<button_width/2; i++)
	{
		LCD_Draw_Horizontal_Line(120-i, play_y+i, 2*i, LCD_COLOR_WHITE);
	}
	for(int i = button_width/2; i>0; i--)
	{
		LCD_Draw_Horizontal_Line(i+button_width/2, play_y + i + (button_width/2) - 1, button_width-(2*i), LCD_COLOR_WHITE);
	}
	for(int i = 1; i<button_width/2 -2; i++)
	{
		LCD_Draw_Horizontal_Line(120-i, play_y+i +2 , 2*i, LCD_COLOR_BLACK);
	}
	for(int i = button_width/2 -2; i>0; i--)
	{
		LCD_Draw_Horizontal_Line(i+button_width/2+1, play_y + i + (button_width/2) - 2, button_width-(2*i)-2, LCD_COLOR_BLACK);
	}

	LCD_SetFont(&Font16x24);
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_DisplayChar(76, 154, 0x50); //0x4c 0x41 0x59
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_DisplayChar(100, 154, 0x4c); //0x4c 0x41 0x59
	LCD_SetTextColor(LCD_COLOR_BLUE2);
	LCD_DisplayChar(124, 154, 0x41); //0x4c 0x41 0x59
	LCD_SetTextColor(LCD_COLOR_GREEN);
	LCD_DisplayChar(148, 154, 0x59); //0x4c 0x41 0x59
}

void rng_init()
{
	hrng->Instance = RNG;
	if(HAL_RNG_Init(hrng) != HAL_OK)
	{
		APPLICATION_ASSERT(1);
	}
}

void block_drop(block_t *block, map_t *map)
{
	//when timer reaches ARR and enters IRQ handler, call this function
	//erase current and draw new tetromino

	if(!rest(block, map))
	{
		clear_tetromino(*block);
		volatile block_t temp_tetromino = *block;
		for(int i = 0; i < 4; i++)
		{
			temp_tetromino.y[i] += BLOCK_WIDTH;
		}
		*block = temp_tetromino;
		draw_tetromino(*block);
	}
	else
	{
		block_rest(block, map);
	}
}

block_t block_create(uint8_t num)
{
	volatile block_t block = {0};
	//evaluate rng -> determine block char
	//determine block matrix corresponding to block char
	//evaluate rng for color
	//set pos_x, pos_y
	//return the block

	//uint32_t rand = HAL_RNG_GetRandomNumber(hrng);
	uint32_t rand = num;
	uint8_t name = rand & 0b111;
		block.name = name;

	//uint32_t rando = HAL_RNG_GetRandomNumber(hrng);
	uint32_t rando = rand;
	uint8_t color = rando & 0b111;

	//COLOR
	if(color == BLUE)
	{
		block.color = LCD_COLOR_BLUE;
	}
	if(color == RED)
	{
		block.color = LCD_COLOR_RED;
	}
	if(color == MAGENTA)
	{
		block.color = LCD_COLOR_MAGENTA;
	}
	if(color == GREEN)
	{
		block.color = LCD_COLOR_GREEN;
	}
	if(color == CYAN)
	{
		block.color = LCD_COLOR_CYAN;
	}
	if(color == YELLOW)
	{
		block.color = LCD_COLOR_YELLOW;
	}
	if(color == ORANGE)
	{
		block.color = LCD_COLOR_ORANGE;
	}
	if(color == PURPLE)
	{
		block.color = LCD_COLOR_PURPLE;
	}
	//COORDINATES
	for(int i = 0; i < 4; i++)
	{
		block.x[i] = BLOCK_START_X + i*BLOCK_WIDTH; //BLOCK_START_X is first coordinate in X
	}

	//BLOCK MATRICES
	if(block.name == O)
	{
		uint16_t temp[4][4] =
			{{0,0,0,0},
			{0,0,0,0},
			{0,0,1,1},
			{0,0,1,1}};

		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				block.mat[i][j] = temp[j][i];
			}
		}
		for(int i = 0; i < 4; i++)
		{
			block.x[i] = BLOCK_START_X + (i-1)*BLOCK_WIDTH; //BLOCK_START_X is first coordinate in X
			block.y[i] = BLOCK_START_Y + (i-2)*BLOCK_WIDTH; //BLOCK_START_Y is first coordinate in Y
		}
	}
	if(block.name == L)
	{
		uint16_t temp[4][4] =
			{{0,0,0,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,1}};
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				block.mat[i][j] = temp[j][i];
			}
		}
		for(int i = 0; i < 4; i++)
		{
			block.x[i] = BLOCK_START_X + (i-1)*BLOCK_WIDTH; //BLOCK_START_X is first coordinate in X
			block.y[i] = BLOCK_START_Y + (i-1)*BLOCK_WIDTH; //BLOCK_START_Y is first coordinate in Y
		}
	}
	if(block.name == J)
	{
		uint16_t temp[4][4] =
			{{0,0,0,0},
			{0,0,0,1},
			{0,0,0,1},
			{0,0,1,1}};
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				block.mat[i][j] = temp[j][i];
			}
		}
		for(int i = 0; i < 4; i++)
		{
			block.x[i] = BLOCK_START_X + (i-1)*BLOCK_WIDTH; //BLOCK_START_X is first coordinate in X
			block.y[i] = BLOCK_START_Y + (i-1)*BLOCK_WIDTH; //BLOCK_START_Y is first coordinate in Y
		}
	}
	if(block.name == S)
	{
		uint16_t temp[4][4] =
			{{0,0,0,0},
			{0,0,0,0},
			{0,0,1,1},
			{0,1,1,0}};
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				block.mat[i][j] = temp[j][i];
			}
		}
		for(int i = 0; i < 4; i++)
		{
			block.y[i] = BLOCK_START_Y + (i-2)*BLOCK_WIDTH; //BLOCK_START_Y is first coordinate in Y
		}
	}
	if(block.name == Z)
	{
		uint16_t temp[4][4] =
			{{0,0,0,0},
			{0,0,0,0},
			{0,1,1,0},
			{0,0,1,1}};
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				block.mat[i][j] = temp[j][i];
			}
		}
		for(int i = 0; i < 4; i++)
		{
			block.y[i] = BLOCK_START_Y + (i-2)*BLOCK_WIDTH; //BLOCK_START_Y is first coordinate in Y
		}
	}
	if(block.name == T)
	{
		uint16_t temp[4][4] =
			{{0,0,0,0},
			{0,0,0,0},
			{0,1,1,1},
			{0,0,1,0}};
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				block.mat[i][j] = temp[j][i];
			}
		}
		for(int i = 0; i < 4; i++)
		{
			block.y[i] = BLOCK_START_Y + (i-2)*BLOCK_WIDTH; //BLOCK_START_Y is first coordinate in Y
		}
	}
	if(block.name == I)
	{
		uint16_t temp[4][4] =
			{{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1}};
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				block.mat[i][j] = temp[j][i];
			}
		}
		for(int i = 0; i < 4; i++)
		{
			block.y[i] = BLOCK_START_Y + (i-3)*BLOCK_WIDTH; //BLOCK_START_Y is first coordinate in Y
		}
	}
	return block;
}

map_t map_init()
{
	volatile map_t map = {0};
	for(int i = 0; i < 10; i++)
	{
		map.x[i] = EDGE_WIDTH + i*BLOCK_WIDTH;
	}
	for(int i = 0; i < 13; i++)
	{
		map.y[i] = MAP_DIFFERENCE + EDGE_WIDTH + i*BLOCK_WIDTH;
	}
	return map;
}

uint8_t rest(block_t *block, map_t *map)
{
	volatile block_t temp_tetromino = *block;
	volatile map_t temp_map = *map;

	//evaluate the logical AND between the true blocks in block_t and
	//the next row of map_t (block y_pos + block size)
	uint8_t y_max_index[4] = {0};
	uint8_t x_max_index[4] = {0};
	uint16_t y_max[4] = {0};
	uint16_t x_max[4] = {0};
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(temp_tetromino.mat[i][j])
			{
				if(j > y_max_index[i])
				{
					y_max_index[i] = j;
				}
			}
			else
			{
				y_max_index[i] = NO_BLOCK;
			}
		}
		x_max_index[i] = i;
	}
	for(int i = 0; i < 4; i++)
	{
		if(y_max_index[i] == NO_BLOCK)
		{
			y_max[i] = NO_BLOCK;
		}
		else
		{
			y_max[i] = temp_tetromino.y[y_max_index[i]];
			x_max[i] = temp_tetromino.x[x_max_index[i]];
		}
	}

	uint8_t map_y_min_index[4] = {UINT8_MAX};
	uint8_t map_x_min_index[4] = {UINT8_MAX};
	uint16_t map_y_min[4];
	uint16_t map_x_min[4];
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 13; j++)
		{
			if(temp_map.map_mat[i][j])
			{
				if(j < y_max_index[i])
				{
					map_y_min_index[i] = j;
				}
				else
				{
					map_y_min_index[i] = NO_BLOCK;
				}
			}
		}
		map_x_min_index[i] = i;
	}
	for(int i = 0; i < 10; i++)
	{
		if(map_y_min[i] == NO_BLOCK)
		{
			y_max[i] = LCD_PIXEL_HEIGHT;
		}
		else
		{
			map_y_min[i] = temp_map.y[map_y_min_index[i]];
			map_x_min[i] = temp_map.x[map_x_min_index[i]];
		}
	}

	uint8_t overlap_map_x_index[4];
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			if(x_max[i] == map_x_min[j])
			{
				overlap_map_x_index[i] = j;
			}
		}
	}

	for(int i = 0; i < 4; i++)
	{
		if(map_y_min[overlap_map_x_index[i]] == y_max[i] + BLOCK_WIDTH || y_max[i] == BOTTOM_ROW)
		{
			return 1;
		}
	}
	return 0;
}

void block_rest(block_t *block, map_t *map)
{
	//append the map array with the new blocks in their respective coordinates
	//create new block
	volatile block_t temp_tetromino = *block;
	volatile map_t temp_map = *map;

	uint8_t y_index[4];
	uint8_t x_index[4];
	uint16_t y_coor[4];
	uint16_t x_coor[4];
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(temp_tetromino.mat[i][j])
			{
				x_index[i] = i;
				y_index[j] = j;
			}
		}
	}
	for(int i = 0; i < 4; i++)
	{
		x_coor[i] = temp_tetromino.x[x_index[i]];
		y_coor[i] = temp_tetromino.y[y_index[i]];
	}

	//get indices for the map coordinates corresponding to the block coordinates
	//set the map logical matrix, and the color
	for(int k = 0; k < 4; k++)
	{
		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < 13; j++)
			{
				if(temp_map.x[i] == x_coor[k] && temp_map.y[j] == y_coor[k])
				{
					temp_map.map_mat[j][i] = ON;
					temp_map.map_color[j][i] = temp_tetromino.color;

				}
				else
				{
					temp_map.map_mat[j][i] = OFF;
					temp_map.map_color[j][i] = LCD_COLOR_BLACK;
				}
			}
		}
	}
	//create new block
	*map = temp_map;

	//block_t new = block_create();
	//draw_tetromino(new);
}

void block_rotate(block_t block)
{

}

void block_move(block_t *block, uint8_t dir)
{
	clear_tetromino(*block);
	//dir is 1 if R, 0 if L

	//evaluate the L/R touchpad input
	//erase current and draw new

	volatile block_t temp_tetromino = *block;

	//FIND LEFT/RIGHT x COORDINATE OF BLOCK
	volatile uint8_t max_x_index = 0;
	volatile uint8_t max_x_val = 0;
	volatile uint8_t min_x_index = UINT8_MAX;
	volatile uint8_t min_x_val = 0;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(temp_tetromino.mat[i][j])
			{
				if(i > max_x_index)
				{
					max_x_index = i;
				}
				if(i < min_x_index)
				{
					min_x_index = i;
				}
			}
		}
	}
	max_x_val = temp_tetromino.x[max_x_index];
	min_x_val = temp_tetromino.x[min_x_index];
	if(dir)
	{
		if(max_x_val != RIGHT_EDGE)
		{
			for(int i = 0; i < 4; i++)
			{
				temp_tetromino.x[i] += BLOCK_WIDTH;
			}
		}
	}
	if(!dir)
	{
		if(min_x_val != LEFT_EDGE)
		{
			for(int i = 0; i < 4; i++)
			{
				temp_tetromino.x[i] -= BLOCK_WIDTH;
			}
		}
	}
	*block = temp_tetromino;
	draw_tetromino(*block);
}

void draw_block(uint16_t x, uint16_t y, uint16_t color)
{
	for(int i = 0; i < BLOCK_WIDTH-1; i++)
	{
		LCD_Draw_Horizontal_Line(x, y+i, BLOCK_WIDTH-1, color);
	}
}

void draw_shapes()
{
	uint16_t block_size = 23;
	uint16_t spacing = 15;
	uint16_t x_offset = (LCD_PIXEL_WIDTH - (7 * (block_size * 4 + spacing))) / 2;
	//uint16_t y_offset = 50;

	// "I" shape (cyan) //fb3
	for (int i = 0; i < 4; i++)
	{
	   	draw_block(6, 193 + 31 + i*BLOCK_WIDTH, LCD_COLOR_CYAN);
	}


	// "L" shape (blue)
	x_offset += block_size * 4 + spacing;
	for (int i = 0; i < 3; i++)
	{
	   	draw_block(80 + i * BLOCK_WIDTH + 88 + 4 - 28, 193 + 31 + block_size, LCD_COLOR_GREEN);
	}
	draw_block(216 - 26, 193 + 31, LCD_COLOR_GREEN);

	// "J" shape (yellow)
	x_offset += block_size * 4 + spacing;
	for (int i = 0; i < 3; i++)
	{
	   	draw_block(145 + i * BLOCK_WIDTH - 39 - block_size*2 - 18 -13, 193 + 31 + block_size, LCD_COLOR_YELLOW);
	}
	draw_block(123 + block_size - 39 - 18 -14, 193 + 31 + 2* block_size, LCD_COLOR_YELLOW);

	// "O" shape (grn)
	x_offset += block_size * 4 + spacing;
	for (int i = 0; i < 2; i++) {
	    for (int j = 0; j < 2; j++) {
	       	draw_block(x_offset + i * BLOCK_WIDTH - 7 - 18 -13, 195+75 + j * block_size, LCD_COLOR_ORANGE);
	    }
	}

	// "S" shape (green)
	x_offset += block_size * 4 + spacing;
	draw_block(x_offset - 30, 195 + 75 + block_size, LCD_COLOR_MAGENTA);
	draw_block(x_offset - 30 + block_size, 195 + 75 + block_size, LCD_COLOR_MAGENTA);
	draw_block(x_offset - 30 + block_size, 195 + 75, LCD_COLOR_MAGENTA);
	draw_block(x_offset - 30 + block_size * 2, 195 + 75, LCD_COLOR_MAGENTA);

	// "T" shape (purple)
	x_offset += block_size * 4 + spacing;
	for (int i = 0; i < 3; i++) {
	   	draw_block(98 + 8 + i * BLOCK_WIDTH - 18 -13, 196 + 97, LCD_COLOR_BLUE2);
	}
	draw_block(98 - 18 + 8 + block_size -13, 194 + 53 + block_size, LCD_COLOR_BLUE2);

	// "Z" shape (red)
	x_offset += block_size * 4 + spacing;
	draw_block(x_offset - 50, 194 + 52, LCD_COLOR_STRAWBERRY);
	draw_block(x_offset - 50 + block_size, 194 + 52, LCD_COLOR_STRAWBERRY);
	draw_block(x_offset - 50 + block_size, 194 + 52 + block_size, LCD_COLOR_STRAWBERRY);
	draw_block(x_offset - 50 + block_size * 2, 194 + 52 + block_size, LCD_COLOR_STRAWBERRY);

	// "O" shape (magenta)
	draw_block(x_offset - 50 + block_size, 194 + 52 -9*block_size, LCD_COLOR_MAGENTA);
	draw_block(x_offset - 50 , 194 + 52 - 9*block_size, LCD_COLOR_MAGENTA);
	draw_block(x_offset - 50 , 194 + 52 + block_size - 9*block_size, LCD_COLOR_MAGENTA);
	draw_block(x_offset - 50 + block_size, 194 + 52 + block_size - 9*block_size, LCD_COLOR_MAGENTA);
}

void draw_tetromino(block_t block)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(block.mat[i][j])
			{
				draw_block(block.x[i], block.y[j], block.color);
			}
		}
	}
}

void clear_tetromino(block_t block)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(block.mat[i][j])
			{
				draw_block(block.x[i], block.y[j], LCD_COLOR_BLACK);
			}
		}
	}
}
