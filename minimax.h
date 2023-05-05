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

int DESTINATED_DEPTH = 0 ;
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


bool loseCheck(const int position[ROWS][COLS],const int player){
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

bool winCheck(const int position[ROWS][COLS], const int player) {
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


/*
 * Player 4 symbols +10000
 * Player 3 symbols -20000
 * Opponent 4 symbols -10000
 * Opponent 3 symbols +20000
 *
 * for each winning combination [playerSymbols : opponentSymbols] :
 *  - [3:0] +9
 *  - [0:3] -9
 *  - [2:0] +5
 *  - [0:2] -4
 *  - [1:0] +2
 *   -[0:1] -2
 * for each loosing combination [playerSymbols : opponentSymbols] :
 * - [2:0] -3
 * - [0:2] +4
 * - [1:0] -1
 * - [0:1] +2
 * */
int getEval(int position[ROWS][COLS]){
    int eval = 0;
    // winning combinations
    for (int i = 0; i < 28; ++i) {
        char players = 0;
        char opponents = 0;
        if(position[win[i][0][0]][win[i][0][1]] == PLAYER) players++;
        else if(position[win[i][0][0]][win[i][0][1]] == 3 - PLAYER) opponents++;
        if(position[win[i][1][0]][win[i][1][1]] == PLAYER) players++;
        else if(position[win[i][1][0]][win[i][1][1]] == 3 - PLAYER) opponents++;
        if(position[win[i][2][0]][win[i][2][1]] == PLAYER) players++;
        else if(position[win[i][2][0]][win[i][2][1]] == 3 - PLAYER) opponents++;
        if(position[win[i][3][0]][win[i][3][1]] == PLAYER) players++;
        else if(position[win[i][3][0]][win[i][3][1]] == 3 - PLAYER) opponents++;

        if(opponents == 4) return -10000;
        if(players == 4) return 10000;

        if(players == 1 && opponents == 0) eval += 2;
        else if(players == 0 && opponents == 1) eval -= 2;
        else if(players == 2 && opponents == 0) eval += 5;
        else if(players == 0 && opponents == 2) eval -= 4;
        else if(players == 3 && opponents == 0) eval += 9;
        else if(players == 0 && opponents == 3) eval -= 9;
    }

    // loosing combinations
    for (int i = 0; i < 48; ++i) {
        char players = 0;
        char opponents = 0;
        if(position[lose[i][0][0]][lose[i][0][1]] == PLAYER) players++;
        else if(position[lose[i][0][0]][lose[i][0][1]] == 3 - PLAYER) opponents++;
        if(position[lose[i][1][0]][lose[i][1][1]] == PLAYER) players++;
        else if(position[lose[i][1][0]][lose[i][1][1]] == 3 - PLAYER) opponents++;
        if(position[lose[i][2][0]][lose[i][2][1]] == PLAYER) players++;
        else if(position[lose[i][2][0]][lose[i][2][1]] == 3 - PLAYER) opponents++;

        if(players == 3) return -20000;
        if(opponents == 3) return 20000;

        if(players == 2 && opponents == 0) eval -= 3;
        else if(players == 0 && opponents == 2) eval += 4;
        else if(players == 1 && opponents == 0) eval -= 1;
        else if(players == 0 && opponents == 1) eval += 2;
    }
    return eval;
}

int min(int a, int b){
    return a > b ? b : a;
}

int max(int a, int b){
    return a > b ? a : b;
}

// First call parameters:
// Depth = 1
// alpha = -infinity
// beta = +infinity
// maxplayer = true
int minmax(int position[ROWS][COLS], int depth, bool maxplayer, int alpha, int beta){
    if(winCheck(position, PLAYER) || winCheck(position, 3 - PLAYER)
    || loseCheck(position, 3 - PLAYER) || loseCheck(position, PLAYER)){
        return getEval(position);
    }

    int eval;
    int bestmove;
    if(maxplayer) eval = INT_MIN;
    else eval = INT_MAX;
    // for each node
    for(size_t i = 0; i < ROWS; i++){
        if(beta <= alpha) break;    // alpha-beta cutoff
        for (size_t j = 0; j < COLS; ++j) {
            if(position[i][j] == 0){
                // simulate move
                if(maxplayer) position[i][j] = PLAYER;
                else position[i][j] = 3 - PLAYER;

                if(depth ==  1){    // go deeper if DESTINATED_DEPTH != 1 , else find best move
                    int e = (depth == DESTINATED_DEPTH) ? getEval(position) : minmax(position, depth + 1, !maxplayer, alpha, beta);
                    if(e > eval){
                        eval = e;
                        alpha = max(alpha, eval);
                        bestmove = (i+1) * 10 + j+1;
                    }
                } else if(depth != DESTINATED_DEPTH){  // go deeper
                    if(maxplayer) {
                        eval = max(eval, minmax(position, depth + 1, !maxplayer, alpha, beta));
                        alpha = max(alpha, eval);
                    }
                    else {
                        eval = min(eval, minmax(position, depth + 1, !maxplayer, alpha, beta));
                        beta = min(beta, eval);

                    }
                }
                else{   // getEvaluation - destinated depth reached
                    if(maxplayer) {
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
                if(beta <= alpha) break;    // alpha-beta cutoff
            }
        }
    }
    if(depth == 1) {
        if(eval <= -10000) printf("I already lost... :C \n");
        else if(eval >= 10000) printf("I already won :) \n");
        else printf("Move eval = %d\n", eval);
        return bestmove;
    }
    return eval;
}

#endif //CROSSANDCIRCLEMINMAX_MINMAX_H
