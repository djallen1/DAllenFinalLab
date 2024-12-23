/*
 * game.h
 *
 *  Created on: Nov 26, 2024
 *      Author: 17202
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "BlockDriver.h"

#define STARTED 0
#define NOT_STARTED 1

uint8_t is_start_screen();

void game_start_screen();
void game_init();
void game_drop();
void game_rotate();
void game_move(uint8_t dir);
void game_over();

#endif /* INC_GAME_H_ */
