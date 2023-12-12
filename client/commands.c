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

//Header Files
#include "config.h"
#include "commands.h"
#include "command-helpers.h"

//Buffers
char payload_buffer[PAYLOAD_BUFFER_SIZE];
char status_buffer[STATUS_BUFFER_SIZE];
char header_buffer[HEADER_BUFFER_SIZE];

/**
 * Given a proposed client command, interact with the server
 * @param {int} argc - Count of command line arguments
 * @param {char**} argv - Array of command line arguments
 * @param {char*} command - The command the client wishes to perform
 * @param {int} socket - The TCP socket that was created
 * @return {int} - Return 0 if everything went well otherwise nonzero
*/
int execute_command(int argc, char** argv, char* command, int socket){

    //Clear our buffers
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);
    clear_buffer(payload_buffer, PAYLOAD_BUFFER_SIZE);
    clear_buffer(header_buffer, HEADER_BUFFER_SIZE);

    //Map our commands to functions
    function_map_t function_map[NUM_COMMANDS] = {
        {"WRITE", NULL},
        {"GET", NULL},
        {"RM", NULL},
        {"LS", NULL},
        {"STOP", NULL}
    };

    //Use our given command to attempt to call a function
    printf("[%s] command issued\n", command);
    for(int i = 0; i < NUM_COMMANDS; i++){

        if(strcmp(command, function_map[i].command_name) == 0){
            //Call the function
            return 0;
        }
    }

    //If we get here it means no function was called
    printf("ERROR: Failed to provide a valid command\n");
    close(socket);

    return -5;
}
