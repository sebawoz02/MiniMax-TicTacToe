# MinMax-TicTacToe
Game client program for playing tic tac toe 5x5 with minmax algorithm

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
