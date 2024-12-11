/*
 * BlockDriver.h
 *
 *  Created on: Nov 21, 2024
 *      Author: 17202
 */

#ifndef INC_BLOCKDRIVER_H_
#define INC_BLOCKDRIVER_H_

#include "LCD_Driver.h"
#include "stm32f4xx_hal.h"

#define BLOCK_WIDTH 23
#define EDGE_WIDTH 4
#define MAP_HEIGHT 309
#define MAP_DIFFERENCE 11

#define BLOCK_START_X 75
#define BLOCK_START_Y 17
#define RIGHT_EDGE 213
#define LEFT_EDGE 6
#define BOTTOM_ROW 293

#define LEFT 0
#define RIGHT 1
#define DOWN 2

#define ON 1
#define OFF 0

#define O 0
#define L 1
#define J 2
#define S 3
#define Z 4
#define T 5
#define I 6

typedef struct{
	volatile uint8_t name;
	volatile uint16_t color;
	volatile uint16_t mat[4][4];
	volatile uint16_t x[4];
	volatile uint16_t y[4];
}block_t;

typedef struct{
	uint16_t map_color[10][13];
	uint16_t map_mat[10][13];
	uint16_t x[10];
	uint16_t y[13];
}map_t;

void map_draw();
void start_screen();
void draw_shapes();
void draw_block(uint16_t x, uint16_t y, uint16_t color);
void draw_tetromino(block_t block);
void clear_tetromino(block_t block);
void end_screen();
void disp_time(uint32_t time);

block_t block_drop(block_t *block);
block_t block_create();
block_t block_rotate(block_t *block);
block_t block_move(block_t *block, map_t *map, uint8_t dir);

uint8_t collision(block_t *block, map_t* map, uint8_t dir);
uint8_t can_spawn(block_t *block, map_t *map);

map_t map_init();
map_t map_update(block_t *block, map_t* map);

#endif /* INC_BLOCKDRIVER_H_ */
