//
// Created by Sebastian Woźniak on 30.04.23.
//

#ifndef CROSSANDCIRCLEMINMAX_MINMAX_H
#define CROSSANDCIRCLEMINMAX_MINMAX_H

#include <stdbool.h>

#define ROWS 5
#define COLS 5

int** getCurBoard(void);

void setDestinatedDepth(int depth);

void setPlayerNum(int pNum);

void makeMove(int move, int player);

void printBoard(void);

void initializeBoard(void);

int minmax(int** position, int depth, bool maxplayer, int alpha, int beta);

#endif //CROSSANDCIRCLEMINMAX_MINMAX_H
