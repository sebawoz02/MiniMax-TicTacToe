# MinMax-TicTacToe
Game client program for playing tic-tac-toe 5x5 with minmax algorithm with alpha-beta pruning.

# Author:
Sebastian Woźniak - 268491

# Heuristics:
For each winning combination [ player symbols : opponent symbols ] :
- [4 : 0] +20000 
- [0 : 4] -10000 
- [3 : 0] +9
- [0 : 3] -9
- [2 : 0] +5
- [0 : 2] -4
- [1 : 0] +2
- [0 : 1] -2

For each loosing combination [ player symbols : opponent symbols ] :
- [3 : 0] -20000
- [0 : 3] +10000
- [2 : 0] -3
- [0 : 2] +4
- [1 : 0] -1
- [0 : 1] +2

# Files:
- minimax.h         // minimax algorithm and game handling
- game_client.c       // game client

# Compiler: 
gcc (Ubuntu 12.2.0-3ubuntu1) 12.2.0

# Example of running the program:
gcc game_client.c -o game_client

./game_client [server_ip] [server_port] [player] [depth]
  
  
- Player is 1 for X and 2 for O.
- The depth parameter is the depth of the minimax algorithm from 1 to 10.
