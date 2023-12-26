/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file This file is the soure code for all of the main commands for interacting with the server
*/

//Socket Libraries
#include <sys/socket.h>
#include <unistd.h>

//Standard Libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

    printf("[STOP] -- SERVER RESPONSE: %s\n", status_buffer);
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

    //Confirm the server is ready to receive our WRITE HEADER
    receive_message(status_buffer, STATUS_BUFFER_SIZE, socket);
    printf("[WRITE] -- SERVER RESPONSE: %s\n", status_buffer);
    if(strcmp(status_buffer, "OK") != 0){
        printf("ERROR: Server is not ready to accept WRITE HEADER. Closing file and socket\n");
        fclose(file);
        return 15;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Pass WRITE HEADER to the server
    snprintf(header_buffer, HEADER_BUFFER_SIZE-1, "%lu,%s", file_size, server_path);
    send_message(header_buffer, strlen(header_buffer), socket);

    //Confirm the server is ready to receive our file data
    receive_message(status_buffer, STATUS_BUFFER_SIZE, socket);
    printf("[WRITE] -- SERVER RESPONSE: %s\n", status_buffer);
    if(strcmp(status_buffer, "PATH ACCEPTED") != 0){
        printf("ERROR: Server is not ready to accept file data. Closing file and socket\n");
        fclose(file);
        return 16;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //We need to send our data -- Recall that in networking, it is BAD practice to attempt to send files all at 
    //once so we are going to send our data in chunks. As we use fread() the pointer in the file will move
    //and so we utilize a loop to pickup where left off as we fill our buffer.
    size_t bytes_read;
    while(1){
        bytes_read = fread(payload_buffer, 1, PAYLOAD_BUFFER_SIZE, file);
        if(bytes_read > 0){
            send_message(payload_buffer, bytes_read, socket);
        }else{
            break;
        }
    }

    //Close the file and confirm that the server received all of our data
    fclose(file);
    receive_message(status_buffer, STATUS_BUFFER_SIZE, socket);
    printf("[WRITE] -- SERVER RESPONSE: %s\n", status_buffer);

    return 0;
}


/**
 * This function handles completion of the GET command cycle. When issued the client
 * will request a file from the server and store it in the local file system
 * @param {int} argc - Count of command line arguments
 * @param {char**} argv - Array of command line arguments
 * @param {char*} command - The command the client wishes to perform
 * @param {int} socket - This is our server socket
 * @return {int} - Zero if everything goes well else non-zero
*/
int command_get(int argc, char** argv, char* command, int socket){

    //Clear Buffers
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);
    clear_buffer(payload_buffer, PAYLOAD_BUFFER_SIZE);
    clear_buffer(header_buffer, HEADER_BUFFER_SIZE);

    //Path holder variables
    int version = 0;
    char remote_path[MAX_FILEPATH_LENGTH];
    char temp_save_path[MAX_FILEPATH_LENGTH];

    if(argc < 3){
        printf("USAGE ERROR: ./client GET <string: remote_path> <int: version> <(optional)string: save_path>\n");
        return 10;
    }
    
    //Assign our variables
    strncpy(remote_path, argv[2], MAX_FILEPATH_LENGTH-1);
    strncpy(temp_save_path, remote_path, MAX_FILEPATH_LENGTH-1);
    if(argc > 3){
        version = atoi(argv[3]);
        if(argc > 4){
            strncpy(temp_save_path, argv[4], MAX_FILEPATH_LENGTH-1);
        }
    }

    if(version <= 0){
        printf("INVALID VERSION PROVIDED -- Defaulting to MOST RECENT VERSION\n");
    }

    //We do no want to send relative file paths to the server so we need to clean up our path
    char* server_path = format_path(remote_path);
    if(server_path == NULL){
        return 11;
    }
    printf("Server Destination: %s (Version: %d)\n", server_path, version);

    //Ensure that our save path is in the correct format as well
    char* save_path = format_path(temp_save_path);
    if(save_path == NULL){
        return 12;
    }

    //Send GET command to the server
    send_message(command, strlen(command), socket);

    //Confirm the server is ready to receive our GET HEADER
    receive_message(status_buffer, STATUS_BUFFER_SIZE, socket);
    printf("[GET] -- SERVER RESPONSE: %s\n", status_buffer);
    if(strcmp(status_buffer, "OK") != 0){
        printf("ERROR: Server is not ready to accept GET HEADER. Closing socket\n");
        return 13;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Send GET HEADER to the server
    snprintf(header_buffer, HEADER_BUFFER_SIZE-1, "%d,%s", version, server_path);
    send_message(header_buffer, strlen(header_buffer), socket);
    clear_buffer(header_buffer, HEADER_BUFFER_SIZE);

    //Confirm that the server FOUND THE FILE
    receive_message(status_buffer, STATUS_BUFFER_SIZE, socket);
    printf("[GET] -- SERVER RESPONSE: %s\n", status_buffer);
    if(strcmp(status_buffer, "FILE FOUND") != 0){
        return 14;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Since the server found the file we need to create the path to where we will store it
    if(create_path(save_path) != 0){
        strncpy(status_buffer, "ERROR", STATUS_BUFFER_SIZE-1);
        send_message(status_buffer, strlen(status_buffer), socket);
        return 15;
    }
    printf("Save Destination: %s\n", save_path);

    strncpy(status_buffer, "OK", STATUS_BUFFER_SIZE-1);
    send_message(status_buffer, strlen(status_buffer), socket);
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Acknowledge the file size AFTER opening the file
    long file_size;
    receive_message(header_buffer, HEADER_BUFFER_SIZE, socket);
    file_size = (long) atoi(header_buffer);

    FILE* file = fopen(save_path, "wb");
    if(file == NULL){
        printf("CATASTROPHIC ERROR: Failed to open file with valid path. Closing socket.\n");
        strncpy(status_buffer, "NOT READY", STATUS_BUFFER_SIZE-1);
        send_message(status_buffer, strlen(status_buffer), socket);
        return 16;
    }

    strncpy(status_buffer, "READY", STATUS_BUFFER_SIZE-1);
    send_message(status_buffer, strlen(status_buffer), socket);
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Intake the data and write to our file
    size_t bytes_received;
    while(file_size > 0){
        bytes_received = recv(socket, payload_buffer, PAYLOAD_BUFFER_SIZE, 0);
        if(bytes_received == 0 || bytes_received == -1){
            //Failed to retrieve data
            printf("ERROR: Failed to receive file data from the server\n");
            //Close the file and delete it
            fclose(file);
            remove(save_path);
            return 17;
        }

        fwrite(payload_buffer, 1, bytes_received, file);
        file_size -= bytes_received;
    }
    fclose(file);

    printf("[GET] -- FILE SAVED\n");
    return 0;
}