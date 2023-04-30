#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "minmax.h"

int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[16], client_message[16];

    short msg, move;
    char depth, player;
    int alpha, beta;

    if(argc != 5){
        printf("Wrong number of arguments\n");
        return -1;
    }

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc<0){
        printf("Unable to create socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP the same as server-side
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Send connection request to server
    if( connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0 ) {
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");

    // Receive the server message
    memset(server_message, '\0', sizeof(server_message));
    if( recv(socket_desc, server_message, sizeof(server_message), 0)<0 ) {
        printf("Error while receiving server's message\n");
        return -1;
    }
    printf("Server message: %s\n",server_message);

    memset(client_message, '\0', sizeof(client_message));
    strcpy(client_message, argv[3]);
    // Send the message to server
    if( send(socket_desc, client_message, strlen(client_message), 0)<0 ) {
        printf("Unable to send message\n");
        return -1;
    }

    bool end_game = false;
    player = atoi(argv[3]);
    depth = atoi(argv[4]);
    initializeBoard();

    while (!end_game){
        memset(server_message, '\0', sizeof(server_message));
        if( recv(socket_desc, server_message, sizeof(server_message), 0)<0 ) {
            printf("Error while receiving server's message\n");
            return -1;
        }
        printf("Server message: %s\n", server_message);
        msg = atoi(server_message);
        move = msg%100;
        msg = msg/100;
        if(move != 0){
            makeMove(move, 3 - player);
            printBoard();
        }
        if( (msg==0) || (msg==6) ) {
            printf("Your move: ");
            if( fgets(client_message, sizeof(client_message), stdin)==NULL ) {
                printf("Error while reading move\n");
                return -1;
            }
            move = atoi(client_message);
            makeMove(move, player);
            printBoard();
            memset(client_message, '\0', sizeof(client_message));
            // Find the best next move
            sprintf(client_message, "%d", minmax(board, depth, alpha, beta, player));

            if( send(socket_desc, client_message, strlen(client_message), 0)<0 ) {
                printf("Unable to send message\n");
                return -1;
            }
            printf("Client message: %s\n", client_message);
        }
        else {
            end_game = true;
            switch( msg ) {
                case 1 : printf("You won.\n"); break;
                case 2 : printf("You lost.\n"); break;
                case 3 : printf("Draw.\n"); break;
                case 4 : printf("You won. Opponent error.\n"); break;
                case 5 : printf("You lost. Your error.\n"); break;
            }
        }


    }

    close(socket_desc);
    return 0;
}
