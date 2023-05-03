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

bool loseCheck(int position[ROWS][COLS],int player)
{
    bool l=false;
    for(int i=0; i<48; i++)
        if( (position[lose[i][0][0]][lose[i][0][1]]==player) && (position[lose[i][1][0]][lose[i][1][1]]==player) && (position[lose[i][2][0]][lose[i][2][1]]==player) )
            l=true;
    return l;
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


// 4 in a row + 20000
// 3 in a row - 10000
// opposite player 4 in a row - 20000
// opposite player 3 in a row + 10000

// 3 in same line (that can form wining line) + 500
// 2 in same line (that can form wining line) + 100
// opposite 3 in same line (that can form wining line) - 500
// opposite 2 in same line (that can form wining line) - 100

// +20 for every empty field that connects with players symbol
int getEval(int position[ROWS][COLS], int player){
    int eval = 0;
    // HORIZONTAL check
    for(size_t row = 0; row < ROWS; row++){
        int free_fields = 0;
        // player stats
        int in_row = 1;
        int line_count = 0;
        // opposite player stats
        int op_in_row = 1;
        int prev = -1;
        int op_line_count = 0;
        for(size_t j = 0; j < COLS; j++){
            if(position[row][j] == player){
                line_count++;
                if(prev == player) in_row++;
                prev = player;
                if(op_in_row == 3) eval += 10000;
                else if(op_in_row == 4) eval -= 20000;
                if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
                else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
                op_line_count = 0;
                op_in_row = 1;
            }
            else{
                if(position[row][j] == 3 - player){
                    op_line_count ++;
                    if(free_fields+line_count>=4 && line_count == 3) eval += 500;
                    else if(free_fields+line_count>=4 && line_count == 2) eval += 100;
                    if(line_count>1) eval += free_fields*20;
                    free_fields = 0;
                    line_count = 0;
                    if(prev == 3 -player) op_in_row++;
                    prev = 3 - player;

                } else{
                    prev = 0;
                    free_fields++;
                    if(op_in_row == 3) eval += 10000;
                    else if(op_in_row == 4) eval -= 20000;
                    op_in_row = 1;
                }
                if(in_row == 3) eval -= 10000;
                else if(in_row == 4) eval += 20000;
                in_row = 1;
            }

        }
        if(free_fields < 5)eval += free_fields*20;
        if(op_in_row == 3) eval += 10000;
        else if(op_in_row == 4) eval -= 20000;
        if(in_row == 3) eval -= 10000; // 3 in a row
        else if(in_row == 4) eval += 20000; // 4 in a row
        if(free_fields+line_count>=4 && line_count == 3) eval += 500; // 3 in same line (that can form wining line)
        else if(free_fields+line_count>=4 && line_count == 2) eval += 100; // 2
        if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
        else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
    }

    // VERTICAL check
    for(size_t col = 0; col < COLS; col++){
        int free_fields = 0;
        // player stats
        int in_row = 1;
        int line_count = 0;
        // opposite player stats
        int op_in_row = 1;
        int op_line_count = 0;
        int prev = -1;
        for(size_t j = 0; j < ROWS; j++){
            if(position[j][col] == player){
                line_count++;
                if(prev == player) in_row++;
                prev = player;
                if(op_in_row == 3) eval += 10000;
                else if(op_in_row == 4) eval -= 20000;
                if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
                else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
                op_line_count = 0;
                op_in_row = 1;
            }
            else{
                if(position[col][j] == 3 - player){
                    op_line_count++;
                    if(free_fields+line_count>=4 && line_count == 3) eval += 500;
                    else if(free_fields+line_count>=4 && line_count == 2) eval += 100;
                    if(line_count>1) eval += free_fields*20;
                    free_fields = 0;
                    line_count = 0;
                    if(prev == 3 -player) op_in_row++;
                    prev = 3 - player;

                } else{
                    prev = 0;
                    free_fields++;
                    if(op_in_row == 3) eval += 10000;
                    else if(op_in_row == 4) eval -= 20000;
                    op_in_row = 1;
                }
                if(in_row == 3) eval -= 10000;
                else if(in_row == 4) eval += 20000;
                in_row = 1;
            }
        }
        if(free_fields < 5)eval += free_fields*20;
        if(op_in_row == 3) eval += 10000;
        else if(op_in_row == 4) eval -= 20000;
        if(in_row == 3) eval -= 10000; // 3 in a col
        else if(in_row == 4) eval += 20000; // 4 in a col
        if(free_fields+line_count>=4 && line_count == 3) eval += 500; // 3 in same line (that can form wining line)
        else if(free_fields+line_count>=4 && line_count == 2) eval += 100; // 2
        if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
        else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
    }

    // DIAGONALLY 1
    int free_fields = 0;
    // player stats
    int in_row = 1;
    int line_count = 0;
    // opposite player stats
    int op_in_row = 1;
    int op_line_count = 0;
    int prev = -1;
    for(int i = 0; i<ROWS;i++){
        if(position[i][i] == player){
            line_count++;
            if(prev==player) in_row++;
            prev = player;
            if(op_in_row == 3) eval += 10000;
            else if(op_in_row == 4) eval -= 20000;
            if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
            else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
            op_in_row = 1;
            op_line_count = 0;
        }
        else{
            if(position[i][i] == 3 - player){
                op_line_count++;
                if(prev == 3 - player) op_in_row++;
                prev = 3 - player;
                if(free_fields+line_count>=4 && line_count == 3) eval += 500;
                else if(free_fields+line_count>=4 && line_count == 2) eval += 100;
                if(line_count>1) eval += free_fields*20;
                line_count = 0;
                free_fields = 0;
            }
            else{
                free_fields++;
                if(op_in_row == 3) eval += 10000;
                else if(op_in_row == 4) eval -= 20000;
                op_in_row = 1;
                prev = 0;
            }
            if(in_row == 3) eval -= 10000;
            else if(in_row == 4) eval += 20000;
            in_row = 1;
        }
    }
    if(free_fields < 5)eval += free_fields*20;
    if(op_in_row == 3) eval += 10000;
    else if(op_in_row == 4) eval -= 20000;
    if(in_row == 3) eval -= 10000;
    else if(in_row == 4) eval += 20000;
    if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
    else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
    if(free_fields+line_count>=4 && line_count == 3) eval += 500;
    else if(free_fields+line_count>=4 && line_count == 2) eval += 100;

    // 21,32,43,54
    in_row = 1;
    line_count = 0;
    op_in_row = 1;
    op_line_count = 0;
    prev = -1;
    free_fields = 0;
    for(int i = 1;i<5; i++){
        if(position[i][i-1] == player){
            line_count++;
            if(prev==player) in_row++;
            prev = player;
            if(op_in_row == 3) eval += 10000;
            else if(op_in_row == 4) eval -= 20000;
            if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
            else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
            op_in_row = 1;
            op_line_count = 0;
        }
        else{
            if(position[i][i-1] == 3 - player){
                op_line_count++;
                if(prev == 3 - player) op_in_row++;
                prev = 3 - player;
                if(free_fields+line_count>=4 && line_count == 3) eval += 500;
                else if(free_fields+line_count>=4 && line_count == 2) eval += 100;
                if(line_count>1) eval += free_fields*20;
                line_count = 0;
                free_fields = 0;
            }
            else{
                free_fields++;
                if(op_in_row == 3) eval += 10000;
                else if(op_in_row == 4) eval -= 20000;
                op_in_row = 1;
                prev = 0;
            }
            if(in_row == 3) eval -= 10000;
            else if(in_row == 4) eval += 20000;
            in_row = 1;
        }
    }
    if(free_fields < 4)eval += free_fields*20;
    if(op_in_row == 3) eval += 10000;
    else if(op_in_row == 4) eval -= 20000;
    if(in_row == 3) eval -= 10000;
    else if(in_row == 4) eval += 20000;
    if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
    else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
    if(free_fields+line_count>=4 && line_count == 3) eval += 500;
    else if(free_fields+line_count>=4 && line_count == 2) eval += 100;

    // 12,23,34,45
    in_row = 1;
    line_count = 0;
    op_in_row = 1;
    op_line_count = 0;
    prev = -1;
    free_fields = 0;
    for(int i = 1;i<5; i++){

        if(position[i-1][i] == player){
            line_count++;
            if(prev==player) in_row++;
            prev = player;
            if(op_in_row == 3) eval += 10000;
            else if(op_in_row == 4) eval -= 20000;
            op_in_row = 1;
            if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
            else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
            op_line_count = 0;
        }
        else{
            if(position[i-1][i] == 3 - player){
                op_line_count++;
                if(prev == 3 - player) op_in_row++;
                prev = 3 - player;
                if(free_fields+line_count>=4 && line_count == 3) eval += 500;
                else if(free_fields+line_count>=4 && line_count == 2) eval += 100;
                if(line_count>1) eval += free_fields*20;
                line_count = 0;
                free_fields = 0;
            }
            else{
                free_fields++;
                if(op_in_row == 3) eval += 10000;
                else if(op_in_row == 4) eval -= 20000;
                op_in_row = 1;
                prev = 0;
            }
            if(in_row == 3) eval -= 10000;
            else if(in_row == 4) eval += 20000;
            in_row = 1;
        }
    }
    if(free_fields < 4)eval += free_fields*20;
    if(op_in_row == 3) eval += 10000;
    else if(op_in_row == 4) eval -= 20000;
    if(in_row == 3) eval -= 10000;
    else if(in_row == 4) eval += 20000;
    if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
    else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
    if(free_fields+line_count>=4 && line_count == 3) eval += 500;
    else if(free_fields+line_count>=4 && line_count == 2) eval += 100;

    // DIAGONALLY 2
    free_fields = 0;
    // player stats
    in_row = 1;
    line_count = 0;
    // opposite player stats
    op_in_row = 1;
    op_line_count = 0;
    prev = -1;
    for(int i = ROWS - 1; i>=0;i--){
        if(position[i][4-i] == player){
            line_count++;
            if(prev==player) in_row++;
            prev = player;
            if(op_in_row == 3) eval += 10000;
            else if(op_in_row == 4) eval -= 20000;
            op_in_row = 1;
        }
        else{
            if(position[i][4-i] == 3 - player){
                if(prev == 3 - player) op_in_row++;
                prev = 3 - player;
                if(free_fields+line_count>=4 && line_count == 3) eval += 500;
                else if(free_fields+line_count>=4 && line_count == 2) eval += 100;
                if(line_count>1) eval += free_fields*20;
                line_count = 0;
                free_fields = 0;
            }
            else{
                free_fields++;
                if(op_in_row == 3) eval += 10000;
                else if(op_in_row == 4) eval -= 20000;
                op_in_row = 1;
                prev = 0;
            }
            if(in_row == 3) eval -= 10000;
            else if(in_row == 4) eval += 20000;
            in_row = 1;
        }
    }
    if(free_fields < 5)eval += free_fields*20;
    if(op_in_row == 3) eval += 10000;
    else if(op_in_row == 4) eval -= 20000;
    if(in_row == 3) eval -= 10000;
    else if(in_row == 4) eval += 20000;
    if(free_fields+line_count>=4 && line_count == 3) eval += 500;
    else if(free_fields+line_count>=4 && line_count == 2) eval += 100;

    // 41,32,23,14
    in_row = 1;
    line_count = 0;
    op_line_count = 0;
    op_in_row = 1;
    prev = -1;
    free_fields = 0;
    for(int i = 1;i<5; i++){
        if(position[ROWS-1-i][i-1] == player){
            line_count++;
            if(prev==player) in_row++;
            prev = player;
            if(op_in_row == 3) eval += 10000;
            else if(op_in_row == 4) eval -= 20000;
            if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
            else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
            op_in_row = 1;
            op_line_count = 0;
        }
        else{
            if(position[ROWS-1-i][i-1] == 3 - player){
                op_line_count++;
                if(prev == 3 - player) op_in_row++;
                prev = 3 - player;
                if(free_fields+line_count>=4 && line_count == 3) eval += 500;
                else if(free_fields+line_count>=4 && line_count == 2) eval += 100;
                if(line_count>1) eval += free_fields*20;
                line_count = 0;
                free_fields = 0;
            }
            else{
                free_fields++;
                if(op_in_row == 3) eval += 10000;
                else if(op_in_row == 4) eval -= 20000;
                op_in_row = 1;
                prev = 0;
            }
            if(in_row == 3) eval -= 10000;
            else if(in_row == 4) eval += 20000;
            in_row = 1;
        }
    }
    if(free_fields < 4)eval += free_fields*20;
    if(op_in_row == 3) eval += 10000;
    else if(op_in_row == 4) eval -= 20000;
    if(in_row == 3) eval -= 10000;
    else if(in_row == 4) eval += 20000;
    if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
    else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
    if(free_fields+line_count>=4 && line_count == 3) eval += 500;
    else if(free_fields+line_count>=4 && line_count == 2) eval += 100;

    // 52,43,34,25
    in_row = 1;
    line_count = 0;
    op_line_count = 0 ;
    op_in_row = 1;
    prev = -1;
    free_fields = 0;
    for(int i = 1;i<5; i++){
        if(position[ROWS-i][i] == player){
            line_count++;
            if(prev==player) in_row++;
            prev = player;
            if(op_in_row == 3) eval += 10000;
            else if(op_in_row == 4) eval -= 20000;
            if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
            else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
            op_line_count = 0;
            op_in_row = 1;
        }
        else{
            if(position[ROWS-i][i] == 3 - player){
                op_line_count++;
                if(prev == 3 - player) op_in_row++;
                prev = 3 - player;
                if(free_fields+line_count>=4 && line_count == 3) eval += 500;
                else if(free_fields+line_count>=4 && line_count == 2) eval += 100;
                if(line_count>1) eval += free_fields*20;
                line_count = 0;
                free_fields = 0;
            }
            else{
                free_fields++;
                if(op_in_row == 3) eval += 10000;
                else if(op_in_row == 4) eval -= 20000;
                op_in_row = 1;
                prev = 0;
            }
            if(in_row == 3) eval -= 10000;
            else if(in_row == 4) eval += 20000;
            in_row = 1;
        }
    }
    if(free_fields < 4)eval += free_fields*20;
    if(op_in_row == 3) eval += 10000;
    else if(op_in_row == 4) eval -= 20000;
    if(in_row == 3) eval -= 10000;
    else if(in_row == 4) eval += 20000;
    if(free_fields+op_line_count>=4 && line_count == 3) eval -= 500;
    else if(free_fields+op_line_count>=4 && line_count == 2) eval -= 100;
    if(free_fields+line_count>=4 && line_count == 3) eval += 500;
    else if(free_fields+line_count>=4 && line_count == 2) eval += 100;

    return eval;
}

int minmax(int position[ROWS][COLS], int depth, bool maximizingPlayer, int player){
    int besteval;
    int bestmove;
    if(maximizingPlayer) {
        besteval = -99999;
    } else {besteval = 99999;}
    for (size_t i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if(position[i][j] == 0) {
                position[i][j] = player;
                if(loseCheck(position, player)){
                    position[i][j]=0;
                    continue;
                }
                // the deepest node
                if(depth == 0){
                    int eval = getEval(board, PLAYER);
                    if(maximizingPlayer){
                        if(eval > besteval){
                            besteval = eval;
                        }
                    }
                    else if(eval < besteval) besteval = eval;
                }
                else{
                    int eval = minmax(position, depth - 1, !maximizingPlayer, 3 - player);
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
