/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file This file handles starting the server and spawning new threads to
 * handle each new client connection
*/

//Thread Library
#include <pthread.h>

//Libraries for creating the Socket
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//Library to handle CTRL + C
#include <signal.h>

//Standard Libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//Libraries for handling directory checks
#include <sys/stat.h>
#include <errno.h>

//Header Files
#include "config.h"
#include "commands.h"
#include "command-helpers.h"

//Function Declarations
void signal_handler(int);
void* execute_thread(void*);

//Server and client socket
int socket_desc;
int client_sock;

//Variables to handle stopping the server while allowing currently running threads to finish
int PREVENT_CONNECTIONS = 0;
int TERMINATE_PROGRAM = 0;

//Create our function map
command_map_t command_map[NUM_COMMANDS] = {
    {"WRITE", respond_to_write},
    {"GET", NULL},
    {"RM", NULL},
    {"LS", NULL},
    {"STOP", NULL}
};

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
        return -4;
    }
    printf("--- Server actively listening for incoming connections ---\n");


    //We want our server to perpetually listen for clients
    while(1){

        //If the STOP command is sent we need a way to break out of the loop
        if(TERMINATE_PROGRAM == 1){
            break;
        }

        //Connect to our client socket
        client_size = sizeof(client_addr);
        client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
        if(client_sock < 0){
            printf("ERROR: Cannot accept client connection. If problem persists restart server\n");
            continue;
        }

        //Malloc memory so we can pass a unique client socket to each thread
        int* socket_pointer = (int*)malloc(sizeof(int));
        *socket_pointer = client_sock;

        //Spawn a new thread to handle the client
        pthread_t thread;
        pthread_create(&thread, NULL, execute_thread, (void*)socket_pointer);

    }

    close(socket_desc);
    return 0;
}

/**
 * This is a helper function that we are passing to each newly spawned thread so that it is able
 * to discern the given command and run the appropriate function against the given socket
 * @param {void*} socket - This will always be cast back to an int so we can utilize our client socket
 * @return {void*} - This is just standard convention for threads
*/
void* execute_thread(void* socket_pointer){

    //Cast our socket back to an int
    int socket = *((int*)socket_pointer);

    if(PREVENT_CONNECTIONS == 1){
        printf("--- The server is no longer accepting new connections and will be shutting down in %d seconds ---\n", WAIT_TIME);
        close(socket);
        free(socket_pointer);
        return NULL;
    }

    time_t current_time;
    current_time = time(NULL);
    printf("Client Thread Started Successfully at %lu\n", current_time);

    //Command Buffer Array -- Notice that we put the buffer here to avoid threads from overriding the buffer
    char command_buffer[COMMAND_BUFFER_SIZE];
    clear_buffer(command_buffer, COMMAND_BUFFER_SIZE);

    //Get the command from the client and delegate to our command handler
    if(receive_message(command_buffer, COMMAND_BUFFER_SIZE, socket) == 1000){
        //The socket is already closed so we just need to free our malloc
        free(socket_pointer);
        return NULL;
    }

    //Loop over our map and use our given command to attempt to call a function
    for(int i = 0; i < NUM_COMMANDS; i++){
        //If we find a valid command then attempt to create a TCP connection and delegate to the correct function
        if(strcmp(command_buffer, command_map[i].command_name) == 0){
            printf("Client Command: [%s]\n", command_buffer);

            int result = (command_map[i].function_pointer)(socket);
            current_time = time(NULL);
            printf("Client Thread Completed Successfully at %lu\n", current_time);
            free(socket_pointer);

            if(result == 1){
                printf("--- Client instructed server to [STOP]. The server will be shutting down in %d seconds ---\n", WAIT_TIME);
                PREVENT_CONNECTIONS = 1;
                
                sleep(WAIT_TIME);

                TERMINATE_PROGRAM = 1;
                return NULL;
            }

            return NULL;
        }
    }

    //We have an invalid command
    printf("ERROR: Server could not parse [%s]\n", command_buffer);
    close(socket);
    free(socket_pointer);

    return NULL;
}

/**
 * This is a helper function for closing our socket if there is a CTRL+C interrupt by the user
 * @param {int} sig - Signal number that triggered this interruption
*/
void signal_handler(int sig){
    printf("\nProgram interrupted by user -- Closing TCP sockets in %d seconds!\n", WAIT_TIME);
    PREVENT_CONNECTIONS = 1;

    //We want to give the rest of the threads time to finish their work
    sleep(WAIT_TIME);

    //Close the open client socket and server socket
    close(client_sock);
    close(socket_desc);

    exit(0);
}