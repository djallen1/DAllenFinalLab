/*
 * game.h
 *
 *  Created on: Nov 26, 2024
 *      Author: 17202
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "BlockDriver.h"

#define STARTED 1
#define NOT_STARTED 0

void game_start_screen();
uint8_t is_start_screen();
void game_init();
void game_over();
void game_drop();
void game_rotate();
void game_move(uint8_t dir);
uint32_t display_time(uint32_t start_time);

#endif /* INC_GAME_H_ */
