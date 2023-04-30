//
// Created by Sebastian Woźniak on 30.04.23.
//

#ifndef CROSSANDCIRCLEMINMAX_MINMAX_H
#define CROSSANDCIRCLEMINMAX_MINMAX_H

#define ROWS 5
#define COLS 5

char board[ROWS][COLS];

void makeMove(short move, char player){
    size_t row,col;
    row = (move/10)-1;
    col = (move%10)-1;
    board[row][col] = player;
};

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

short minmax(char position[ROWS][COLS], char depth, int alpha, int beta, char maximizingPlayer){
    /* TODO: implementation */
}

#endif //CROSSANDCIRCLEMINMAX_MINMAX_H
