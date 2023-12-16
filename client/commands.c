/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file This file is the soure code for all of the main commands for interacting with the server
*/

//Socket Libraries
#include <unistd.h>

//Standard Libraries
#include <stdio.h>
#include <string.h>

//Header Files
#include "config.h"
#include "commands.h"
#include "command-helpers.h"

//Create Buffers
char payload_buffer[PAYLOAD_BUFFER_SIZE];
char status_buffer[STATUS_BUFFER_SIZE];
char header_buffer[HEADER_BUFFER_SIZE];

/**
 * This function handles completion of the STOP command cycle. When issued the
 * client is instructing the server to STOP
 * @param {int} argc - Count of command line arguments
 * @param {char**} argv - Array of command line arguments
 * @param {char*} command - The command the client wishes to perform
 * @param {int} socket - This is our server socket
 * @return {int} - Zero if everything goes well else non-zero
*/
int command_stop(int argc, char** argv, char* command, int socket){

    //Clear the status buffer
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Send our STOP command to the server
    send_message(command, strlen(command), socket);

    //Receive response from the server
    receive_message(status_buffer, STATUS_BUFFER_SIZE, socket);
    printf("SERVER RESPONSE: %s\n", status_buffer);
    if(strcmp(status_buffer, "OK") != 0){
        printf("ERROR: Server failed to [STOP]\n");
        return 10;
    }

    //Always close the socket
    close(socket);
    return 0;
}


/**
 * This function handles completion of the WRITE command cycle. When issued the client
 * will send a local file to the server to be written in the remote file system
 * @param {int} argc - Count of command line arguments
 * @param {char**} argv - Array of command line arguments
 * @param {char*} command - The command the client wishes to perform
 * @param {int} socket - This is our server socket
 * @return {int} - Zero if everything goes well else non-zero
*/
int command_write(int argc, char** argv, char* command, int socket){

    //Clear Buffers
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);
    clear_buffer(payload_buffer, PAYLOAD_BUFFER_SIZE);
    clear_buffer(header_buffer, HEADER_BUFFER_SIZE);

    //Send a message
    send_message(command, strlen(command), socket);

    //Receive a response
    receive_message(status_buffer, STATUS_BUFFER_SIZE, socket);

    //Always close the socket
    close(socket);
    return 0;
}