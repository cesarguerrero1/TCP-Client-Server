/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file This file handles client attempts to connect to the TCP Server Socket
*/

//Socket Libraries
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//File Interaction
#include <sys/stat.h>
#include <errno.h>

//Standard Libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Signal Library
#include <signal.h>

//Program Header Files
#include "config.h"
#include "commands.h"
#include "command-helpers.h"

//Function declaration
void signal_handler(int);

//Server Socket
int socket_desc;

/**
 * This is the main entry point into our program
 * @param {int} argc - Count of command line arguments
 * @param {char**} argv - Array of command line arguments
 * @return {int} - Return 0 if communication with the server is successful
*/
int main(int argc, char** argv){

    //Initialize our signal handler
    signal(SIGINT, signal_handler);

    //Make sure our root directory exists
    if(mkdir(ROOT_DIR, S_IRWXU) != 0 && errno != EEXIST){
        printf("ERROR: Failed to create root directory\n");
        return -1;
    };

    //Check our command line arguments
    if (argc < 2) {
        printf("USAGE ERROR: ./client <string: WRITE | string: GET | string: RM | string: LS | string: STOP>\n");
        return -2;
    }

    //Command Buffer array
    char command[COMMAND_BUFFER_SIZE];
    clear_buffer(command, COMMAND_BUFFER_SIZE);
    strncpy(command, argv[1], 9);

    //Map our commands to functions
    command_map_t command_map[NUM_COMMANDS] = {
        {"WRITE", command_write},
        {"GET", NULL},
        {"RM", NULL},
        {"LS", NULL},
        {"STOP", NULL}
    };

    //Use our given command to attempt to call a function
    for(int i = 0; i < NUM_COMMANDS; i++){
        //If we find a valid command then attempt to create a TCP connection and delegate to the mapped function
        if(strcmp(command, command_map[i].command_name) == 0){

            printf("[%s] command issued\n", command);
            
            //Socket Structure
            struct sockaddr_in server_addr;
        
            //Create our actual socket
            socket_desc = socket(AF_INET, SOCK_STREAM, 0);
            if(socket_desc < 0){
                printf("ERROR: Socket creation failed\n");
                return -3;
            }

            //Set the port and IP
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(SOCKET_PORT);
            server_addr.sin_addr.s_addr = inet_addr(SOCKET_IP_ADDRESS);

            //Attempt to connect to the server socket
            if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
                printf("ERROR: Cannot connect to server\n");
                return -4;
            }

            //Call our function
            return (command_map[i].function_pointer)(argc, argv, command, socket_desc);
        }
    }

    //Otherwise, we have an invalid command and since we never opened a socket we can just exit
    printf("ERROR: [%s] is an invalid command\n", command);
    return -5;
}

/**
 * This is a helper function for closing our socket if there is a CTRL+C interrupt by the user
 * @param {int} sig - Signal number that triggered this interruption
*/
void signal_handler(int sig){
    printf("Program interrupted by user -- Closing TCP Socket\n");
    close(socket_desc);
    exit(0);
}