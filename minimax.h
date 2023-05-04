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
#include <limits.h>

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
    for(int i=0; i<48; i++) {
        if ((position[lose[i][0][0]][lose[i][0][1]] == player) && (position[lose[i][1][0]][lose[i][1][1]] == player) &&
            (position[lose[i][2][0]][lose[i][2][1]] == player)) {
            l = true;
            break;
        }
    }
    return l;
}

bool winCheck(int position[ROWS][COLS], int player) {
    bool w = false;
    for (int i = 0; i < 28; i++) {
        if ((position[win[i][0][0]][win[i][0][1]] == player) && (position[win[i][1][0]][win[i][1][1]] == player) &&
            (position[win[i][2][0]][win[i][2][1]] == player) && (position[win[i][3][0]][win[i][3][1]] == player)) {
            w = true;
            break;
        }
    }
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
// opposite player 3 in a row  +20000
// 3 in a row -20000

// for player symbols check fields around - player symbol +30 (2 : +90, 3 : +180), opposite player symbol - break
// for opposite player symbols - opp symbol close -30 (2 : -90, 3 : -180), player symbol  +90 break
int getEval(int position[ROWS][COLS]){
    int eval = 0;
    if(winCheck(position, PLAYER)) eval += 10000;
    else if(loseCheck(position, PLAYER)) eval -= 20000;
    if(winCheck(position, 3 - PLAYER))eval -= 10000;
    else if(loseCheck(position, 3 - PLAYER)) eval += 20000;

    if(eval == 0) {
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; ++col) {
                if(position[row][col] == 0) continue;
                bool opplayer = (position[row][col] == 3 - PLAYER);
                int found = 0;
                // check left
                for (int l = col - 1; l >= 0; l--) {
                    if(!opplayer) {
                        if (position[row][l] == PLAYER) eval += ++found*30;
                        else if(position[row][l] == 3- PLAYER) break;

                    } else{
                        if(position[row][l] == 3 - PLAYER) eval -= ++found*30;
                        else if(position[row][l] == PLAYER) {
                            eval += 90;
                            break;
                        }
                    }
                }
                found = 0;
                // check right
                for (int l = col + 1; l < COLS; l++) {
                    if(!opplayer) {
                        if (position[row][l] == PLAYER) eval += ++found*30;
                        else if(position[row][l] == 3- PLAYER) break;

                    } else{
                        if(position[row][l] == 3 - PLAYER) eval -= ++found*30;
                        else if(position[row][l] == PLAYER) {
                            eval += 90;
                            break;
                        }
                    }
                }
                found = 0;
                // check up
                for (int l = row - 1; l >= 0; l--) {
                    if(!opplayer) {
                        if (position[l][col] == PLAYER) eval += ++found*30;
                        else if(position[l][col] == 3- PLAYER) break;

                    } else{
                        if(position[l][col] == 3 - PLAYER) eval -= ++found*30;
                        else if(position[l][col] == PLAYER) {
                            eval += 90;
                            break;
                        }
                    }
                }
                found = 0;
                // check down
                for (int l = row + 1; l < ROWS; l++) {
                    if(!opplayer) {
                        if (position[l][col] == PLAYER) eval += ++found*30;
                        else if(position[l][col] == 3- PLAYER) break;

                    } else{
                        if(position[l][col] == 3 - PLAYER) eval -= ++found*30;
                        else if(position[l][col] == PLAYER) {
                            eval += 90;
                            break;
                        }
                    }
                }
                found = 0;
                // check NW
                for (int l = 1; row - l >= 0 && col - l >= 0; l++) {
                    if(!opplayer) {
                        if (position[row-l][col-l] == PLAYER) eval += ++found*30;
                        else if(position[row-l][col-l] == 3- PLAYER) break;

                    } else{
                        if(position[row-l][col-l] == 3 - PLAYER) eval -= ++found*30;
                        else if(position[row-l][col-l] == PLAYER) {
                            eval += 90;
                            break;
                        }
                    }

                }
                found = 0;
                // check NE
                for (int l = 1; row - l >= 0 && col + l < COLS; l++) {
                    if(!opplayer) {
                        if (position[row-l][col+l] == PLAYER) eval += ++found*30;
                        else if(position[row-l][col+l] == 3- PLAYER) break;

                    } else{
                        if(position[row-l][col+l] == 3 - PLAYER) eval -= ++found*30;
                        else if(position[row-l][col+l] == PLAYER) {
                            eval += 90;
                            break;
                        }
                    }
                }
                found = 0;
                // check SE
                for (int l = 1; row + l < ROWS && col + l < COLS; l++) {
                    if(!opplayer) {
                        if (position[row+l][col+l] == PLAYER) eval += ++found*30;
                        else if(position[row+l][col+l] == 3- PLAYER) break;

                    } else{
                        if(position[row+l][col+l] == 3 - PLAYER) eval -= ++found*30;
                        else if(position[row+l][col+l] == PLAYER) {
                            eval += 90;
                            break;
                        }
                    }
                }
                found = 0;
                // check SW
                for (int l = 1; row + l < ROWS && col - l >= 0; l++) {
                    if(!opplayer) {
                        if (position[row+l][col-l] == PLAYER) eval += ++found*30;
                        else if(position[row+l][col-l] == 3- PLAYER) break;

                    } else{
                        if(position[row+l][col-l] == 3 - PLAYER) eval -= ++found*30;
                        else if(position[row+l][col-l] == PLAYER) {
                            eval += 90;
                            break;
                        }
                    }
                }
            }
        }
    }

    return eval;
}

int min(int a, int b){
    return a > b ? b : a;
}

int max(int a, int b){
    return a > b ? a : b;
}

int minmax(int position[ROWS][COLS], int depth, int player, int alpha, int beta){
    if(winCheck(position, PLAYER) || winCheck(position, 3 - PLAYER)
    || loseCheck(position, 3 - PLAYER) || loseCheck(position, PLAYER)){
        return getEval(position);
    }

    int eval;
    int bestmove;
    if(player == PLAYER) eval = INT_MIN;
    else eval = INT_MAX;
    // for each node
    for(size_t i = 0; i < ROWS; i++){
        for (size_t j = 0; j < COLS; ++j) {
            if(position[i][j] == 0){
                position[i][j] = player;
                if(depth ==  1){    // go deeper and find best move
                    int e = (depth == DESTINATED_DEPTH) ? getEval(position) : minmax(position, depth + 1, 3 - player, alpha, beta);
                    if(e > eval){
                        eval = e;
                        alpha = max(alpha, eval);
                        bestmove = (i+1) * 10 + j+1;
                    }
                } else if(depth != DESTINATED_DEPTH){  // go deeper
                    if(player == PLAYER) {
                        eval = max(eval, minmax(position, depth + 1, 3 - player, alpha, beta));
                        alpha = max(alpha, eval);
                    }
                    else {
                        eval = min(eval, minmax(position, depth + 1, 3 - player, alpha, beta));
                        beta = min(beta, eval);

                    }

                }
                else{   // getEvaluation - destinated depth reached
                    if(player == PLAYER) {
                        eval = max(eval, getEval(position));
                        alpha = max(alpha, eval);
                    }
                    else {
                        eval = min(eval, getEval(position));
                        beta = min(beta, eval);
                    }
                }
                // undo move
                position[i][j] = 0;
                if(beta <= alpha) break;
            }
        }
    }
    if(depth == 1) {

        if(eval <= -10000) printf("I already lost...☠\uFE0F \n");
        else if(eval >= 10000) printf("I already won :) \n");
        else printf("Move eval = %d\n", eval);
        return bestmove;
    }
    return eval;
}

#endif //CROSSANDCIRCLEMINMAX_MINMAX_H
