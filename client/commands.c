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

//Stat Library
#include <sys/stat.h>

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

    //Close our socket
    close(socket);

    printf("SERVER RESPONSE: %s\n", status_buffer);
    if(strcmp(status_buffer, "OK") != 0){
        printf("ERROR: Server failed to [STOP]\n");
        return 10;
    }

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

    //Path holder variables
    char local_path[MAX_FILEPATH_LENGTH];
    char remote_path[MAX_FILEPATH_LENGTH];

    //Check our command line arguments
    if(argc < 3){
        printf("USAGE ERROR: ./client WRITE <string: local_path> <(optional)string: remote_path>\n");
        return 10;
    }

    strncpy(local_path, argv[2], MAX_FILEPATH_LENGTH-1);
    strncpy(remote_path, local_path, MAX_FILEPATH_LENGTH-1);
    if(argc > 3){
        strncpy(remote_path, argv[3], MAX_FILEPATH_LENGTH-1);
    }

    //Use stat() to:
    //1. Ensure our local file path points to a file and not to a directory
    //2. Get the size of the file
    struct stat local_path_stat;
    if(stat(local_path, &local_path_stat) != 0){
        printf("ERROR: Failed to verify local file path\n");
        return 11;
    }

    if(S_ISDIR(local_path_stat.st_mode)){
        printf("ERROR: Local file path points to a directory\n");
        return 12;
    }

    //Open the local file in binary mode
    printf("Attempting to open file: %s\n", local_path);
    FILE* file = fopen(local_path, "rb");
    if(file == NULL){
        printf("ERROR: Failed to open file\n");
        return 13;
    }

    //We do no want to send relative file paths to the server so we need to clean up our path
    char* server_path = format_path(remote_path);
    if(server_path == NULL){
        fclose(file);
        return 14;
    }
    printf("Server Destination: %s\n", server_path);

    //Get the size of the file
    long file_size = local_path_stat.st_size;

    //Send the command
    send_message(command, strlen(command), socket);

    //Always close the socket and file
    fclose(file);
    close(socket);
    return 0;
}