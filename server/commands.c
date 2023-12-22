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
    
    //Respond to the client
    strncpy(status_buffer, "OK", STATUS_BUFFER_SIZE-1);
    if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
        //Failed to send message
        return 2;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Await the WRITE HEADER
    if(receive_message(header_buffer, HEADER_BUFFER_SIZE, socket) == 1000){
        //Failed to receive message
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

    //Before we begin creating directories and files we need to lock the mutex
    pthread_mutex_lock(&mutex);

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

    //Receive the file data
    FILE* file = fopen(file_path, "wb");
    if(file == NULL){
        printf("CATASTROPHIC ERROR: Failed to open file with valid path. Closing socket\n");
        pthread_mutex_unlock(&mutex);
        return 7;
    }

    size_t bytes_received = 0;
    while(file_size > 0){
        bytes_received = recv(socket, payload_buffer, PAYLOAD_BUFFER_SIZE, 0);
        if(bytes_received == 0 || bytes_received == -1){
            printf("ERROR: Failed to receive file data from the client. Closing socket\n");
            //Close the file and then delete it
            fclose(file);
            remove(file_path);

            pthread_mutex_unlock(&mutex);
            return 8;
        }

        //Write to our file
        fwrite(payload_buffer, 1, bytes_received, file);
        file_size -= bytes_received;
    }

    //Inform our user that we have received the entire file and then clean up
    fclose(file);

    strncpy(status_buffer, "FILE RECEIVED", STATUS_BUFFER_SIZE-1);
    if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
        //Failed to send message
        pthread_mutex_unlock(&mutex);
        return 9;
    }

    //Release the lock
    pthread_mutex_unlock(&mutex);
    return 0;
}


/**
 * This function deals with handling the GET command issued by the client. The function
 * attemps to retrieve and return the file specified by the client
 * @param {int} socket - The client socket we are communicating with
 * @return {int} - Zero if all goes well else non-zero
*/
int respond_to_get(int socket){

    //Since we are using multithreading each thread needs to get its own buffers
    char status_buffer[STATUS_BUFFER_SIZE];
    char payload_buffer[PAYLOAD_BUFFER_SIZE];
    char header_buffer[HEADER_BUFFER_SIZE];

    //Clear your buffers
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);
    clear_buffer(payload_buffer, PAYLOAD_BUFFER_SIZE);
    clear_buffer(header_buffer, HEADER_BUFFER_SIZE);

    //Respond to the client
    strncpy(status_buffer, "OK", STATUS_BUFFER_SIZE-1);
    if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
        //Failed to send message
        return 2;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Await the GET HEADER
    if(receive_message(header_buffer, HEADER_BUFFER_SIZE, socket) == 1000){
        //Failed to receive message
        return 3;
    }

    //Parse the GET HEADER
    int version;
    char file_path[MAX_FILEPATH_LENGTH];
    char* token;

    token = strtok(header_buffer, ",");
    version = atoi(token);
    token = strtok(NULL, ",");
    strncpy(file_path, token, MAX_FILEPATH_LENGTH-1);

    //Now we need lock the mutex so that we are not attempting to read files that are in the middle of being created or removed
    pthread_mutex_lock(&mutex);

    if(access_file(file_path, version, 1) != 0){
        //The file was not found
        strncpy(status_buffer, "FILE NOT FOUND", STATUS_BUFFER_SIZE-1);
        if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
            //Failed to send message
            pthread_mutex_unlock(&mutex);
            return 4;
        }

        //Stop the rest of the execution
        printf("ERROR: The requested file version was not found. Closing socket\n");
        pthread_mutex_unlock(&mutex);
        return 5;
    };
    printf("Read Path: %s\n", file_path);

    //The file was found
    strncpy(status_buffer, "FILE FOUND", STATUS_BUFFER_SIZE-1);
    if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
        //Failed to send message
        pthread_mutex_unlock(&mutex);
        return 6;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);
    clear_buffer(header_buffer, HEADER_BUFFER_SIZE);

    //Release the lock
    pthread_mutex_unlock(&mutex);
    return 0;
}