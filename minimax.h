//
// Created by Sebastian Woźniak on 30.04.23.
//

#ifndef CROSSANDCIRCLEMINMAX_MINMAX_H
#define CROSSANDCIRCLEMINMAX_MINMAX_H

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define ROWS 5
#define COLS 5

int board[ROWS][COLS];

int DEPTH = 0 ;
int PLAYER = 0 ;


const int lose[48][3][2] = {
        { {0,0}, {0,1}, {0,2} }, { {0,1}, {0,2}, {0,3} }, { {0,2}, {0,3}, {0,4} },
        { {1,0}, {1,1}, {1,2} }, { {1,1}, {1,2}, {1,3} }, { {1,2}, {1,3}, {1,4} },
        { {2,0}, {2,1}, {2,2} }, { {2,1}, {2,2}, {2,3} }, { {2,2}, {2,3}, {2,4} },
        { {3,0}, {3,1}, {3,2} }, { {3,1}, {3,2}, {3,3} }, { {3,2}, {3,3}, {3,4} },
        { {4,0}, {4,1}, {4,2} }, { {4,1}, {4,2}, {4,3} }, { {4,2}, {4,3}, {4,4} },
        { {0,0}, {1,0}, {2,0} }, { {1,0}, {2,0}, {3,0} }, { {2,0}, {3,0}, {4,0} },
        { {0,1}, {1,1}, {2,1} }, { {1,1}, {2,1}, {3,1} }, { {2,1}, {3,1}, {4,1} },
        { {0,2}, {1,2}, {2,2} }, { {1,2}, {2,2}, {3,2} }, { {2,2}, {3,2}, {4,2} },
        { {0,3}, {1,3}, {2,3} }, { {1,3}, {2,3}, {3,3} }, { {2,3}, {3,3}, {4,3} },
        { {0,4}, {1,4}, {2,4} }, { {1,4}, {2,4}, {3,4} }, { {2,4}, {3,4}, {4,4} },
        { {0,2}, {1,3}, {2,4} }, { {0,1}, {1,2}, {2,3} }, { {1,2}, {2,3}, {3,4} },
        { {0,0}, {1,1}, {2,2} }, { {1,1}, {2,2}, {3,3} }, { {2,2}, {3,3}, {4,4} },
        { {1,0}, {2,1}, {3,2} }, { {2,1}, {3,2}, {4,3} }, { {2,0}, {3,1}, {4,2} },
        { {0,2}, {1,1}, {2,0} }, { {0,3}, {1,2}, {2,1} }, { {1,2}, {2,1}, {3,0} },
        { {0,4}, {1,3}, {2,2} }, { {1,3}, {2,2}, {3,1} }, { {2,2}, {3,1}, {4,0} },
        { {1,4}, {2,3}, {3,2} }, { {2,3}, {3,2}, {4,1} }, { {2,4}, {3,3}, {4,2} }
};

const int win[28][4][2] = {
        { {0,0}, {0,1}, {0,2}, {0,3} },
        { {1,0}, {1,1}, {1,2}, {1,3} },
        { {2,0}, {2,1}, {2,2}, {2,3} },
        { {3,0}, {3,1}, {3,2}, {3,3} },
        { {4,0}, {4,1}, {4,2}, {4,3} },
        { {0,1}, {0,2}, {0,3}, {0,4} },
        { {1,1}, {1,2}, {1,3}, {1,4} },
        { {2,1}, {2,2}, {2,3}, {2,4} },
        { {3,1}, {3,2}, {3,3}, {3,4} },
        { {4,1}, {4,2}, {4,3}, {4,4} },
        { {0,0}, {1,0}, {2,0}, {3,0} },
        { {0,1}, {1,1}, {2,1}, {3,1} },
        { {0,2}, {1,2}, {2,2}, {3,2} },
        { {0,3}, {1,3}, {2,3}, {3,3} },
        { {0,4}, {1,4}, {2,4}, {3,4} },
        { {1,0}, {2,0}, {3,0}, {4,0} },
        { {1,1}, {2,1}, {3,1}, {4,1} },
        { {1,2}, {2,2}, {3,2}, {4,2} },
        { {1,3}, {2,3}, {3,3}, {4,3} },
        { {1,4}, {2,4}, {3,4}, {4,4} },
        { {0,1}, {1,2}, {2,3}, {3,4} },
        { {0,0}, {1,1}, {2,2}, {3,3} },
        { {1,1}, {2,2}, {3,3}, {4,4} },
        { {1,0}, {2,1}, {3,2}, {4,3} },
        { {0,3}, {1,2}, {2,1}, {3,0} },
        { {0,4}, {1,3}, {2,2}, {3,1} },
        { {1,3}, {2,2}, {3,1}, {4,0} },
        { {1,4}, {2,3}, {3,2}, {4,1} }
};

bool loseCheck(int position[ROWS][COLS],int player)
{
    bool l=false;
    for(int i=0; i<48; i++)
        if( (position[lose[i][0][0]][lose[i][0][1]]==player) && (position[lose[i][1][0]][lose[i][1][1]]==player) && (position[lose[i][2][0]][lose[i][2][1]]==player) )
            l=true;
    return l;
}

bool winCheck(int position[ROWS][COLS], int player)
{
    bool w=false;
    for(int i=0; i<28; i++)
        if( (position[win[i][0][0]][win[i][0][1]]==player) && (position[win[i][1][0]][win[i][1][1]]==player) && (position[win[i][2][0]][win[i][2][1]]==player) && (position[win[i][3][0]][win[i][3][1]]==player) )
            w=true;
    return w;
}

void makeMove(int move, int player){
    size_t row,col;
    row = (move/10)-1;
    col = (move%10)-1;
    board[row][col] = player;
}

void printBoard(){
    printf("  1 2 3 4 5\n");
    for(int i=0; i<5; i++) {
        printf("%d",i+1);
        for(int j=0; j<5; j++ )
            switch(board[i][j]) {
                case 0: printf(" -"); break;
                case 1: printf(" X"); break;
                case 2: printf(" O"); break;
            }
        printf("\n");
    }
    printf("\n");
}

void initializeBoard(){
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            board[i][j] = 0;
        }
    }
}


// 4 in a row +10000
// opposite player 4 in a row -10000
// opposite player 3 in a row  +5000
// 3 in a row -5000

//for every field check free fields around - free field +10, player symbol +40, opposite player symbol -20
int getEval(int position[ROWS][COLS]){
    int eval = 0;
    if(winCheck(position, PLAYER)) eval += 10000;
    else if(loseCheck(position, PLAYER)) eval -= 5000;
    if(winCheck(position, 3 - PLAYER))eval-=10000;
    else if(loseCheck(position, 3- PLAYER)) eval+=5000;

    if(eval == 0) {
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; ++col) {
                // check left
                for (int l = col - 1; l >= 0; l--) {
                    if (position[row][l] == 0) eval += 10;
                    else if(position[row][l] == PLAYER) eval += 40;
                    else{
                        eval -= 20;
                        break;
                    }
                }
                // check right
                for (int l = col + 1; l < COLS; l++) {
                    if (position[row][l] == 0) eval += 10;
                    else if(position[row][l] == PLAYER) eval += 40;
                    else{
                        eval -= 20;
                        break;
                    }
                }
                // check up
                for (int l = row - 1; l >= 0; l--) {
                    if (position[l][col] == 0) eval += 10;
                    else if(position[l][col] == PLAYER) eval += 40;
                    else{
                        eval -= 20;
                        break;
                    }
                }
                // check down
                for (int l = row + 1; l < ROWS; l++) {
                    if (position[l][col] == 0) eval += 10;
                    else if(position[l][col] == PLAYER) eval += 40;
                    else{
                        eval -= 20;
                        break;
                    }
                }
                // check NW
                for (int l = 1; row - l >=0 && col - l >=0; l++) {
                    if (position[row-l][col-l] == 0) eval += 10;
                    else if(position[row-l][col-l] == PLAYER) eval += 40;
                    else{
                        eval -= 20;
                        break;
                    }
                }
                // check NE
                for (int l = 1; row - l >= 0 && col + l < COLS; l++) {
                    if (position[row-l][col+l] == 0) eval += 10;
                    else if(position[row-l][col+l] == PLAYER) eval += 40;
                    else{
                        eval -= 20;
                        break;
                    }
                }
                // check SE
                for (int l = 1; row + l < ROWS && col + l < COLS; l++) {
                    if (position[row+l][col+l] == 0) eval += 10;
                    else if(position[row+l][col+l] == PLAYER) eval += 40;
                    else{
                        eval -= 20;
                        break;
                    }
                }
                // check SW
                for (int l = 1; row + l < ROWS && col - l >=0; l++) {
                    if (position[row+l][col-l] == 0) eval += 10;
                    else if(position[row+l][col-l] == PLAYER) eval += 40;
                    else{
                        eval -= 20;
                        break;
                    }
                }
            }
        }
    }

    return eval;
}

int minmax(int position[ROWS][COLS], int depth, bool maximizingPlayer, int player){
    int besteval;
    int bestmove;
    if(maximizingPlayer) {
        besteval = -999999;
    } else {besteval = 999999;}
    for (size_t i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if(position[i][j] == 0) {
                position[i][j] = player;
                // the deepest node
                if(depth == 0){
                    int eval = getEval(board);
                    if(maximizingPlayer){
                        if(eval > besteval){
                            besteval = eval;
                        }
                    }
                    else if(eval < besteval) besteval = eval;
                }
                else{
                    // if someone won/lost stop recursion on this node
                    int eval = loseCheck(position, PLAYER) || loseCheck(position, 3 - PLAYER) || winCheck(position, PLAYER) ||
                                       winCheck(position, 3 - PLAYER) ? getEval(position) :
                                       minmax(position, depth - 1, !maximizingPlayer, 3 - player);
                    if(maximizingPlayer){
                        if(eval > besteval){
                            besteval = eval;
                            if(DEPTH - 1 == depth){
                                bestmove = 10*(i+1) + (j+1);
                            }
                        }
                    }
                    else if(eval < besteval){
                        besteval = eval;
                    }
                }
                //undo move
                position[i][j] = 0;
            }
        }
    }
    if(DEPTH-1 != depth)return besteval;
    else return bestmove;

}

#endif //CROSSANDCIRCLEMINMAX_MINMAX_H
