/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file This file handles starting the server and spawning new threads to
 * handle each new client connection
*/

//Libraries for creating the Socket
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//Library to handle CTRL + C
#include <signal.h>

//Standard Libraries
#include <stdlib.h>
#include <stdio.h>

//Libraries for handling directory checks
#include <sys/stat.h>
#include <errno.h>

//Header Files
#include "config.h"
#include "commands.h"
#include "command-helpers.h"

//Function Declarations
void signal_handler(int);

//Server and client socket
int socket_desc;
int client_sock;

/**
 * Entrance into our program
 * @return {int} - Return zero if everything works as expected
*/
int main(){

    //Initialize our Signal Handler
    signal(SIGINT, signal_handler);

    //Make sure our rfs directory exists
    if(mkdir(ROOT_DIR, S_IRWXU) != 0 && errno != EEXIST){
        printf("ERROR: Failed to create root directory\n");
        return -1;
    }

    //Command Buffer Array
    char command_buffer[COMMAND_BUFFER_SIZE];
    clear_buffer(command_buffer, COMMAND_BUFFER_SIZE);

    //Socket Structures
    socklen_t client_size;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    //Create our actual socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc < 0){
        printf("ERROR: Socket creation failed\n");
        return -2;
    }

    //Set the Port and IP
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCKET_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SOCKET_IP_ADDRESS);

    //Bind to the port and IP
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("ERROR: Failed to bind to the port\n");
        return -3;
    }

    //Tell the server to start listening for clients
    if(listen(socket_desc, 1) < 0){
        printf("ERROR: Server failed to begin listening for clients\n");
        return -3;
    }
    printf("--- Server actively listening for incoming connections ---\n\n");

    //Connect to our client socket
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    if(client_sock < 0){
        printf("ERROR: Cannot accept client connection. If problem persists restart server\n");
        return -4;
    }
    printf("\nClient connected at Port# %i and IP: %s\n", ntohs(client_addr.sin_port), inet_ntoa(client_addr.sin_addr));

    //Attempt to receive the client command
    if(recv_message(command_buffer, COMMAND_BUFFER_SIZE, client_sock) == 1000){
        return -5;
    }
    printf("Client Command: %s\n", command_buffer);

    //Close everything
    close(client_sock);
    close(socket_desc);

    return 0;
}

/**
 * This is a helper function for closing our socket if there is a CTRL+C interrupt by the user
 * @param {int} sig - Signal number that triggered this interruption
*/
void signal_handler(int sig){
    printf("\nProgram interrupted by user -- Closing TCP sockets!\n");

    close(client_sock);
    close(socket_desc);
    exit(0);
}