#include "minimax.h"

#include <stdio.h>
#include <limits.h>

// 5x5 game board
static int** board;
// depth of minimax algorithm
static int DESTINATED_DEPTH;
// player 1 for X and 2 for O
static int PLAYER;


// all loosing combinations (3 in a row)
static const int lose[48][3][2] = {
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

// all winning combinations (4 in a row)
static const int win[28][4][2] = {
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


// returns true if any of the loosing combinations consists of "player" symbols
static bool loseCheck(int** position,const int player){
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


// returns true if any of winning combinations consists of "player" symbols
static bool winCheck(int** position, const int player) {
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

static int min(int a, int b){
    return a > b ? b : a;
}

static int max(int a, int b){
    return a > b ? a : b;
}

/*
 * Player 4 symbols +20000
 * Player 3 symbols -20000
 * Opponent 4 symbols -10000
 * Opponent 3 symbols +10000
 *
 * for each winning combination [playerSymbols : opponentSymbols] :
 *  - [3:0] +9
 *  - [0:3] -9
 *  - [2:0] +5
 *  - [0:2] -4
 *  - [1:0] +2
 *   -[0:1] -2
 *
 *   2x [3:0] && 0x [0:3] +200
 *   2x [0:3] && 0x [3:0] -200
 * for each loosing combination [playerSymbols : opponentSymbols] :
 * - [2:0] -3
 * - [0:2] +4
 * - [1:0] -1
 * - [0:1] +2
 *
 * */
static int getEval(int** position){
    char p1three = 0;
    char oppthree = 0;
    int eval = 0;
    // winning combinations
    for (int i = 0; i < 28; ++i) {
        char players = 0;
        char opponents = 0;

        for(size_t j = 0; j<4; j++){
            if(position[win[i][j][0]][win[i][j][1]] == PLAYER) players++;
            else if(position[win[i][j][0]][win[i][j][1]] == 3 - PLAYER) opponents++;
        }

        if(opponents == 4) return -10000;
        if(players == 4) return 20000;

        if(players == 1 && opponents == 0) eval += 2;
        else if(players == 0 && opponents == 1) eval -= 2;
        else if(players == 2 && opponents == 0) eval += 5;
        else if(players == 0 && opponents == 2) eval -= 4;
        else if(players == 3 && opponents == 0) {
            eval += 9;
            p1three++;
        }
        else if(players == 0 && opponents == 3) {
            eval -= 9;
            oppthree++;
        }
    }
    if(p1three>1 && oppthree==0)eval+=200;
    else if(oppthree>1 && p1three==0)eval-=200;

    // loosing combinations
    for (int i = 0; i < 48; ++i) {
        char players = 0;
        char opponents = 0;

        for(size_t j = 0; j<3;j++){
            if(position[lose[i][j][0]][lose[i][j][1]] == PLAYER) players++;
            else if(position[lose[i][j][0]][lose[i][j][1]] == 3 - PLAYER) opponents++;
        }

        if(players == 3) return -20000;
        if(opponents == 3) return 10000;

        if(players == 2 && opponents == 0) eval -= 3;
        else if(players == 0 && opponents == 2) eval += 4;
        else if(players == 1 && opponents == 0) eval -= 1;
        else if(players == 0 && opponents == 1) eval += 2;
    }
    return eval;
}

/* Possible moves
11 12 13 14 15
21 22 23 24 25
31 32 33 34 35
41 42 43 44 45
51 52 53 54 55
*/
void makeMove(int move, int player){
    size_t row,col;
    row = (move/10)-1;
    col = (move%10)-1;
    if(row<5 && col<5){
    board[row][col] = player;
    }
    else printf("Wrong parameter\n");
}

// Displays the current state of the board
void printBoard(void){
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

// Sets every square on the board to empty
void initializeBoard(void){
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            board[i][j] = 0;
        }
    }
}

// First call parameters:
// Depth = 1
// alpha = -infinity
// beta = +infinity
// maxplayer = true
int minmax(int** position, int depth, bool maxplayer, int alpha, int beta){
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

int** getCurBoard(void){
    return board;
}

void setDestinatedDepth(int depth){
    DESTINATED_DEPTH = depth;
}

void setPlayerNum(int pNum){
    PLAYER = pNum;
}

