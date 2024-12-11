/*
 * BlockDriver.c
 *
 *  Created on: Nov 21, 2024
 *      Author: 17202
 */
#include "BlockDriver.h"

static RNG_HandleTypeDef hrng;

static void MX_RNG_Init(void);
static void MX_RNG_Deinit(void);

void map_draw()
{
	//uint16_t y_len = LCD_PIXEL_HEIGHT;
	uint16_t x_len = LCD_PIXEL_WIDTH;
	//edges of 5 pixels in width
	uint16_t x = 0;

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
	uint16_t button_width = LCD_PIXEL_WIDTH/2;

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
	LCD_DisplayChar(76, 154, 'P');
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_DisplayChar(100, 154, 'L');
	LCD_SetTextColor(LCD_COLOR_BLUE2);
	LCD_DisplayChar(124, 154, 'A');
	LCD_SetTextColor(LCD_COLOR_GREEN);
	LCD_DisplayChar(148, 154, 'Y');
}

void end_screen()
{
	LCD_Clear(0,LCD_COLOR_BLACK);
	LCD_SetFont(&Font16x24);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DisplayChar(80, 100, 'G');
	LCD_DisplayChar(102, 100, 'A');
	LCD_DisplayChar(122, 100, 'M');
	LCD_DisplayChar(144, 100, 'E');
	LCD_DisplayChar(80, 130, 'O');
	LCD_DisplayChar(102, 130, 'V');
	LCD_DisplayChar(122, 130, 'E');
	LCD_DisplayChar(144, 130, 'R');
}

void disp_time(uint32_t time)
{
	uint32_t seconds = time / 1000;
	uint32_t min = 0;
	uint32_t sec_tens = 0;
	while(seconds > 60)
	{
		seconds -= 60;
		min++;
	}
	while(seconds > 10)
	{
		seconds -= 10;
		sec_tens++;
	}

	uint32_t sec_char = seconds + '0';
	uint32_t sec_tens_char = sec_tens + '0';
	uint32_t min_char = min + '0';

	LCD_SetFont(&Font12x12);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DisplayChar(100, 160, min_char);
	LCD_DisplayChar(114, 160, ':');
	LCD_DisplayChar(124, 160, sec_tens_char);
	LCD_DisplayChar(138, 160, sec_char);

}

block_t block_drop(block_t *block)
{
	//when timer reaches ARR and enters IRQ handler, call this function
	volatile block_t temp_tetromino = *block;
	for(int i = 0; i < 4; i++)
	{
		temp_tetromino.y[i] += BLOCK_WIDTH;
	}
	//*block = temp_tetromino;
	return temp_tetromino;
}

block_t block_create()
{
	MX_RNG_Init();
	volatile block_t block = {0};
	//evaluate rng -> determine block char
	//determine block matrix corresponding to block char
	//evaluate rng for color
	//set pos_x, pos_y
	//return the block

	uint32_t rand;
	HAL_RNG_GenerateRandomNumber(&hrng, &rand);
	rand %= 7;
	MX_RNG_Deinit();

	uint8_t name = rand;
		block.name = name;

	uint8_t color = rand;

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
			{0,0,1,0},
			{0,0,1,0},
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
			block.x[i] = BLOCK_START_X + i*BLOCK_WIDTH; //BLOCK_START_X is first coordinate in X
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
			{0,0,1,0},
			{0,1,1,1}};
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
			block.y[i] = BLOCK_START_Y + (i-2)*BLOCK_WIDTH; //BLOCK_START_Y is first coordinate in Y
		}
	}
	return block;
}

map_t map_init()
{
	volatile map_t map = {0};
	for(int i = 0; i < 10; i++)
	{
		map.x[i] = LEFT_EDGE + i*BLOCK_WIDTH;
	}
	for(int i = 0; i < 13; i++)
	{
		map.y[i] = MAP_DIFFERENCE + EDGE_WIDTH + i*BLOCK_WIDTH + 2;
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

	uint8_t map_y_min_index[10] = {UINT8_MAX};
	uint8_t map_x_min_index[10] = {UINT8_MAX};
	uint16_t map_y_min[10];
	uint16_t map_x_min[10];
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 13; j++)
		{
			if(temp_map.map_mat[i][j])
			{
				if(map_y_min_index[i] > j)
				{
					map_y_min_index[i] = j;
				}
			}
			else
			{
				map_y_min_index[i] = NO_INDEX;
			}
		}
		map_x_min_index[i] = i;
	}
	for(int i = 0; i < 10; i++)
	{
		if(map_y_min_index[i] == NO_INDEX)
		{
			map_y_min[i] = LCD_PIXEL_HEIGHT;
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

//	for(int i = 0; i < 4; i++)
//	{
		if(map_y_min[overlap_map_x_index[0]] == y_max[0] + BLOCK_WIDTH || y_max[0] == BOTTOM_ROW)
		{
			return 1;
		}
		else if(map_y_min[overlap_map_x_index[1]] == y_max[1] + BLOCK_WIDTH || y_max[1] == BOTTOM_ROW)
		{
			return 1;
		}
		else if(map_y_min[overlap_map_x_index[2]] == y_max[2] + BLOCK_WIDTH || y_max[2] == BOTTOM_ROW)
		{
			return 1;
		}
		else if(map_y_min[overlap_map_x_index[3]] == y_max[3] + BLOCK_WIDTH || y_max[3] == BOTTOM_ROW)
		{
			return 1;
		}
//	}
	return 0;
}

map_t map_update(block_t *block, map_t* map)
{
	//append the map array with the new blocks in their respective coordinates
	//create new block
	volatile block_t temp_tetromino = *block;
	volatile map_t temp_map = *map;

	uint8_t y_index[4];
	uint8_t x_index[4];
	uint16_t y_coor[4];
	uint16_t x_coor[4];
	uint8_t ind = 0;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(temp_tetromino.mat[i][j])
			{
				x_index[ind] = i;
				y_index[ind] = j;
				ind++;
			}
		}
	}
	for(int i = 0; i < 4; i++)
	{
		if(x_index[i]>NO_INDEX && y_index[i]>NO_INDEX)
		{
			x_coor[i] = temp_tetromino.x[x_index[i]];
			y_coor[i] = temp_tetromino.y[y_index[i]];
		}
	}

	//get indices for the map coordinates corresponding to the block coordinates
	//set the map logical matrix, and the color
	uint8_t index = 0;
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 13; j++)
		{
			if(temp_map.x[i] == x_coor[index] && temp_map.y[j] == y_coor[index])
			{
				temp_map.map_mat[i][j] = ON;
				temp_map.map_color[i][j] = temp_tetromino.color;
				index++;
			}
		}
	}

	return temp_map;
}

block_t block_rotate(block_t *block)
{
	block_t temp_block = *block;
	if(temp_block.name == O)
	{
		return temp_block;
	}

	if(temp_block.name == I)
	{
		uint8_t transpose[4][4];
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				transpose[i][j] = temp_block.mat[j][i];
			}
		}
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				temp_block.mat[i][j] = transpose[i][j];
			}
		}
	}


	else if(temp_block.name != O)
	{
		uint8_t transpose_3x3[3][3];
		uint8_t transform[3][3];
		uint8_t mirror_identity[3][3] =
		{{0, 0, 1},
		{0, 1, 0},
		{1, 0, 0}};

		for(int i = 1; i < 4; i++)
		{
			for(int j = 1; j < 4; j++)
			{
				transpose_3x3[i-1][j-1] = temp_block.mat[j][i];
			}
		}

		for(int k = 0; k < 3; k++)
		{
			for(int i = 0; i < 3; i++)
			{
				transform[k][i] = 0;
				for(int j = 0; j < 3; j++)
				{
					transform[k][i] += (transpose_3x3[k][j] * mirror_identity[j][i]);
				}
			}
		}

		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				if(i == 0 || j == 0)
				{
					temp_block.mat[i][j] = 0;
				}
				else
				{
					temp_block.mat[i][j] = transform[i-1][j-1];
				}
			}
		}
	}
	return temp_block;
}

uint8_t can_move(block_t *block, map_t *map, uint8_t dir)
{
	//dir is 1 if R, 0 if L

	//evaluate the L/R touchpad input
	//erase current and draw new

	volatile block_t temp_tetromino = *block;
	volatile map_t temp_map = *map;

	//evaluate the logical AND between the true blocks in block_t and
	//the next row of map_t (block y_pos + block size)
	uint16_t y_block[4] = {0};
	uint16_t x_block[4] = {0};
	uint8_t ind = 0;
	for(int j = 0; j < 4; j++)
	{
		for(int i = 0; i < 4; i++)
		{
			if(temp_tetromino.mat[i][j])
			{
				y_block[ind] = temp_tetromino.y[j];
				x_block[ind] = temp_tetromino.x[i];
				ind++;
			}
		}
	}

	uint16_t map_points = 0;
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 13; j++)
		{
			if(temp_map.map_mat[i][j])
			{
				map_points++;
			}
		}
	}
	uint8_t map_y_index[map_points];
	uint8_t map_x_index[map_points];
	uint16_t map_y[map_points];
	uint16_t map_x[map_points];
	uint8_t index = 0;
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 13; j++)
		{
			if(temp_map.map_mat[i][j])
			{
				map_x_index[index] = i;
				map_y_index[index] = j;
				index++;
			}
		}
	}
	for(int i = 0; i < map_points; i++)
	{
		map_y[i] = temp_map.y[map_y_index[i]];
		map_x[i] = temp_map.x[map_x_index[i]];
	}

	uint8_t overlap_map_y_index[4];
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 13; j++)
		{
			if(y_block[i] == map_y[j])
			{
				overlap_map_y_index[i] = j;
			}
		}
	}

	for(int i = 0; i < 4; i++)
	{
		if(dir)
		{
			if(map_x[overlap_map_y_index[i]] == x_block[i] + BLOCK_WIDTH || x_block[i] == RIGHT_EDGE)
			{
				return 0;
			}
		}
		else if(!dir)
		{
			if(map_x[overlap_map_y_index[i]] == x_block[i] - BLOCK_WIDTH || x_block[i] == LEFT_EDGE)
			{
				return 0;
			}
		}
	}
	return 1;
}

block_t block_move(block_t *block, map_t *map, uint8_t dir)
{
	//when timer reaches ARR and enters IRQ handler, call this function
	//erase current and draw new tetromino
	volatile block_t temp_tetromino = *block;
	if(dir)
	{
		for(int i = 0; i < 4; i++)
		{
			temp_tetromino.x[i] += BLOCK_WIDTH;
		}
	}
	else
	{
		for(int i = 0; i < 4; i++)
		{
			temp_tetromino.x[i] -= BLOCK_WIDTH;
		}
	}
	return temp_tetromino;
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
	uint16_t x_offset = (LCD_PIXEL_WIDTH - (7 * (BLOCK_WIDTH * 4 + 15))) / 2;

	// I tetromino (cyan)
	for (int i = 0; i < 4; i++)
	{
	   	draw_block(6, 224 + i*BLOCK_WIDTH, LCD_COLOR_CYAN);
	}

	// L tetromino (green)
	x_offset += BLOCK_WIDTH * 4 + 15;
	for (int i = 0; i < 3; i++)
	{
	   	draw_block(i*BLOCK_WIDTH + 144, 224 + BLOCK_WIDTH, LCD_COLOR_GREEN);
	}
	draw_block(190, 224, LCD_COLOR_GREEN);

	// J tetromino (yellow)
	x_offset += BLOCK_WIDTH * 4 + 15;
	for (int i = 0; i < 3; i++)
	{
	   	draw_block(75 + (i-2)*BLOCK_WIDTH, 224 + BLOCK_WIDTH, LCD_COLOR_YELLOW);
	}
	draw_block(52 + BLOCK_WIDTH, 224 + 2*BLOCK_WIDTH, LCD_COLOR_YELLOW);

	// O tetromino (orange)
	x_offset += BLOCK_WIDTH * 4 + 15;
	for (int i = 0; i < 2; i++) {
	    for (int j = 0; j < 2; j++) {
	       	draw_block(67 + i*BLOCK_WIDTH - 38, 270 + j*BLOCK_WIDTH, LCD_COLOR_ORANGE);
	    }
	}

	// S tetromino (magenta)
	x_offset += BLOCK_WIDTH * 4 + 15;
	draw_block(144, 270 + BLOCK_WIDTH, LCD_COLOR_MAGENTA);
	draw_block(144 + BLOCK_WIDTH, 270 + BLOCK_WIDTH, LCD_COLOR_MAGENTA);
	draw_block(144 + BLOCK_WIDTH, 270, LCD_COLOR_MAGENTA);
	draw_block(144 + BLOCK_WIDTH * 2, 270, LCD_COLOR_MAGENTA);

	// T tetromino (blue2)
	x_offset += BLOCK_WIDTH * 4 + 15;
	for (int i = 0; i < 3; i++) {
	   	draw_block(LEFT_EDGE + (i+3) * BLOCK_WIDTH, 293, LCD_COLOR_BLUE2);
	}
	draw_block(LEFT_EDGE + 4*BLOCK_WIDTH, 293 - BLOCK_WIDTH, LCD_COLOR_BLUE2);

	// Z tetromino (strawberry)
	x_offset += BLOCK_WIDTH * 4 + 15;
	draw_block(LEFT_EDGE + 4*BLOCK_WIDTH, 247, LCD_COLOR_STRAWBERRY);
	draw_block(LEFT_EDGE + 5*BLOCK_WIDTH, 247, LCD_COLOR_STRAWBERRY);
	draw_block(LEFT_EDGE + 5*BLOCK_WIDTH, 247 + BLOCK_WIDTH, LCD_COLOR_STRAWBERRY);
	draw_block(LEFT_EDGE + 6*BLOCK_WIDTH, 247 + BLOCK_WIDTH, LCD_COLOR_STRAWBERRY);

	// "O" shape (magenta)
	draw_block(x_offset - 50 + BLOCK_WIDTH, 246 - 9*BLOCK_WIDTH, LCD_COLOR_MAGENTA);
	draw_block(x_offset - 50 , 246 - 9*BLOCK_WIDTH, LCD_COLOR_MAGENTA);
	draw_block(x_offset - 50 , 246 - 8*BLOCK_WIDTH, LCD_COLOR_MAGENTA);
	draw_block(x_offset - 50 + BLOCK_WIDTH, 246 - 8*BLOCK_WIDTH, LCD_COLOR_MAGENTA);
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

uint8_t can_spawn(block_t *block, map_t *map)
{
	volatile block_t temp_tetromino = *block;
	volatile map_t temp_map = *map;

	uint16_t block_x[4];
	uint16_t block_y[4];
	uint8_t ind = 0;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(temp_tetromino.mat[i][j])
			{
				block_x[ind] = temp_tetromino.x[i];
				block_y[ind] = temp_tetromino.y[j];
				ind++;
			}
		}
	}

	//iterate through the columns in the first 3 rows of the map, and if overlap, return 0
	for(int i = 3; i < 7; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(temp_map.map_mat[i][j])
			{
				if(temp_map.x[i] == block_x[0] && temp_map.y[j] == block_y[0])
				{
					return 0;
				}
				else if(temp_map.x[i] == block_x[1] && temp_map.y[j] == block_y[1])
				{
					return 0;
				}
				else if(temp_map.x[i] == block_x[2] && temp_map.y[j] == block_y[2])
				{
					return 0;
				}
				else if(temp_map.x[i] == block_x[3] && temp_map.y[j] == block_y[3])
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

static void MX_RNG_Init(void)
{
  __HAL_RCC_RNG_CLK_ENABLE();
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
	  APPLICATION_ASSERT(1);
  }
}

static void MX_RNG_Deinit(void)
{
  __HAL_RCC_RNG_CLK_DISABLE();
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
	  APPLICATION_ASSERT(1);
  }
}
