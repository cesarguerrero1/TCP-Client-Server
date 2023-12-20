/**
 * Cesar Guerrero
 * 12/12/23
 * 
 * @file This file is the soure code for all of the main commands for interacting with the client
*/

//Thread Library
#include <pthread.h>

//Socket Libraries
#include <sys/socket.h>
#include <unistd.h>

//Standard Libraries
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Header Files
#include "config.h"
#include "commands.h"
#include "command-helpers.h"

/**
 * This function deals with handling the STOP command issued by the client.
 * It will respond to the client that is stopping the server
 * @param {int} socket - The client socket
 * @return {int} - A return value of 1 will stop the server
*/
int respond_to_stop(int socket){

    //Make and clear your buffers
    char status_buffer[STATUS_BUFFER_SIZE];
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    strncpy(status_buffer, "OK", STATUS_BUFFER_SIZE-1);
    if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
        //Failed to send message
        return 2;
    }

    return 1;
}


/**
 * This function deals with handling the WRITE command issued by the client. The function will get
 * the file from the client and then attempt to store it on the server
 * @param {int} socket - The client socket we are going to communicate with
 * @return {int} - Zero if all goes well else non-zero
*/
int respond_to_write(int socket){

    //Since we are using multithreading each thread needs to get its own buffers
    char status_buffer[STATUS_BUFFER_SIZE];
    char payload_buffer[PAYLOAD_BUFFER_SIZE];
    char header_buffer[HEADER_BUFFER_SIZE];

    //Clear your buffers
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);
    clear_buffer(payload_buffer, PAYLOAD_BUFFER_SIZE);
    clear_buffer(header_buffer, HEADER_BUFFER_SIZE);

    //Get the mutex lock
    pthread_mutex_lock(&mutex);
    
    //Respond to the client
    strncpy(status_buffer, "OK", STATUS_BUFFER_SIZE-1);
    if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
        //Failed to send message
        pthread_mutex_unlock(&mutex);
        return 2;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Await the WRITE HEADER
    if(receive_message(header_buffer, HEADER_BUFFER_SIZE, socket) == 1000){
        //Failed to receive message
        pthread_mutex_unlock(&mutex);
        return 3;
    }

    //Parse the WRITE HEADER
    long file_size;
    char file_path[MAX_FILEPATH_LENGTH];
    char* token;

    //Since we know the structure of the header we can parse it accordingly
    token = strtok(header_buffer, ",");
    file_size = (long) atoi(token);
    token = strtok(NULL, ",");
    strncpy(file_path, token, MAX_FILEPATH_LENGTH-1);

    //Verify that our path will work
    if(create_path(file_path) != 0){
        strncpy(status_buffer, "PATH REJECTED", STATUS_BUFFER_SIZE-1);
        if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
            //Failed to send message
            pthread_mutex_unlock(&mutex);
            return 4;
        }

        //Stop the rest of the execution
        printf("ERROR: Could not validate/create requested file path. Closing socket\n");
        pthread_mutex_unlock(&mutex);
        return 5;
    }

    //Add a version to it
    apply_version(file_path);
    printf("Save Path: %s\n", file_path);

    //Tell client to send the actual file data
    strncpy(status_buffer, "PATH ACCEPTED", STATUS_BUFFER_SIZE-1);
    if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
        //Failed to send message
        pthread_mutex_unlock(&mutex);
        return 6;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Release the lock
    pthread_mutex_unlock(&mutex);

    return 0;
}