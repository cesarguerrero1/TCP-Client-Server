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

//Stat() Libraries
#include <sys/stat.h>
#include <errno.h>

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

    //Confirm that the client knows the file exists
    if(receive_message(status_buffer, STATUS_BUFFER_SIZE, socket) == 1000){
        //Failed to receive message
        pthread_mutex_unlock(&mutex);
        return 7;
    }
    
    printf("[GET] -- CLIENT RESPONSE: %s\n", status_buffer);
    if(strcmp(status_buffer, "OK") != 0){
        printf("ERROR: The client is not ready to receive the file. Closing socket\n");
        pthread_mutex_unlock(&mutex);
        return 8;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Inform the client how large the file is
    struct stat file_path_stat;
    stat(file_path, &file_path_stat);
    long file_size = file_path_stat.st_size;

    snprintf(header_buffer, HEADER_BUFFER_SIZE-1, "%lu", file_size);
    if(send_message(header_buffer, strlen(header_buffer), socket) == 999){
        //Failed to send message
        pthread_mutex_unlock(&mutex);
        return 9;
    }

    //Confirm that the client is ready to accept the file
    if(receive_message(status_buffer, STATUS_BUFFER_SIZE, socket) == 1000){
        //Failed to receive message
        pthread_mutex_unlock(&mutex);
        return 10;
    }

    printf("[GET] -- CLIENT RESPONSE: %s\n", status_buffer);
    if(strcmp(status_buffer, "READY") != 0){
        printf("ERROR: The client could not confirm the file size. Closing socket\n");
        pthread_mutex_unlock(&mutex);
        return 11;
    }

    //Open our file and send the data
    FILE* file = fopen(file_path, "rb");
    if(file == NULL){
        printf("CATASTROPHIC ERROR: Failed to open file with valid path. Closing socket\n");
        pthread_mutex_unlock(&mutex);
        return 12;
    }

    size_t bytes_read;
    while(1){
        bytes_read = fread(payload_buffer, 1, PAYLOAD_BUFFER_SIZE, file);
        if(bytes_read > 0){
            if(send_message(payload_buffer, bytes_read, socket) == 999){
                fclose(file);
                pthread_mutex_unlock(&mutex);
                return 13;
            }
        }else{
            break;
        }
    }

    //Close our file and release the lock
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}


/**
 * This function deals with handling the RM command issued by the client. The function
 * will attempt to find the given file and remove it (and all of its versions)
 * @param {int} socket - The client socket we are communicating with
 * @return {int} - Zero if all goes well else non-zero
*/
int respond_to_rm(int socket){
    
    //Clear Buffers
    char status_buffer[STATUS_BUFFER_SIZE];
    char header_buffer[HEADER_BUFFER_SIZE];

    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);
    clear_buffer(header_buffer, HEADER_BUFFER_SIZE);

    //Respond to the client
    strncpy(status_buffer, "OK", STATUS_BUFFER_SIZE-1);
    if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
        //Failed to send message
        return 2;
    }
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);

    //Await the RM HEADER
    if(receive_message(header_buffer, HEADER_BUFFER_SIZE, socket) == 1000){
        //Failed to receive message
        return 3;
    }

    //Parse the RM HEADER
    char file_path[MAX_FILEPATH_LENGTH];
    strncpy(file_path, header_buffer, MAX_FILEPATH_LENGTH-1);

    //Acquire the Mutex Lock so we can safely delete all of these files
    pthread_mutex_lock(&mutex);

    int result = access_file(file_path, 0, 2);
    if(result == 0){
       //All files removed
       strncpy(status_buffer, "FILE REMOVED", STATUS_BUFFER_SIZE-1);
    }else if(result == 1){
        //No files removed
        strncpy(status_buffer, "FILE NOT FOUND", STATUS_BUFFER_SIZE-1);
    }else{
        //Error
        strncpy(status_buffer, "ERROR DELETING FILE", STATUS_BUFFER_SIZE-1);
    }

    if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
        //Failed to send message
        pthread_mutex_unlock(&mutex);
        return 4;
    }

    //Release the lock
    pthread_mutex_unlock(&mutex);
    return 0;
}


/**
 * This function deals with handling the LS command issued by the client. The function
 * will attempt to find metadata on the given file and return it to the client
 * @param {int} socket - The client socket we are communicating with
 * @return {int} - Zero if all goes well else non-zero
*/
int respond_to_ls(int socket){

    //Clear Buffers
    char status_buffer[STATUS_BUFFER_SIZE];
    char header_buffer[HEADER_BUFFER_SIZE];
    char payload_buffer[PAYLOAD_BUFFER_SIZE];

    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);
    clear_buffer(header_buffer, HEADER_BUFFER_SIZE);
    clear_buffer(payload_buffer, PAYLOAD_BUFFER_SIZE);

    //Respond to the client
    strncpy(status_buffer, "OK", STATUS_BUFFER_SIZE-1);
    if(send_message(status_buffer, strlen(status_buffer), socket) == 999){
        //Failed to send message
        return 2;
    }

    //Await the LS HEADER
    if(receive_message(header_buffer, HEADER_BUFFER_SIZE, socket) == 1000){
        //Failed to receive message
        return 3;
    }

    //Parse the LS HEADER
    char file_path[MAX_FILEPATH_LENGTH];
    strncpy(file_path, header_buffer, MAX_FILEPATH_LENGTH-1);

    //Acquire the Mutex Lock so we can safely read our file and any versions
    pthread_mutex_lock(&mutex);

    //NOTE: I made a QUICK FIX HERE. Since I need to send a payload to the client containing
    //the metadata I opted to just override the file_path as there is no way for me to access
    //the payload_buffer from the access_file() function since we are implementing multi-threading.
    //This is a quick fix and should berevisited in the future
    if(access_file(file_path, 0, 3) != 0){
        //ERROR
        printf("CATASTROPHIC ERROR: Failed to access file(s)\n");
        pthread_mutex_unlock(&mutex);
        return 4;
    }

    strncpy(payload_buffer, file_path, PAYLOAD_BUFFER_SIZE-1);
    if(send_message(payload_buffer, strlen(payload_buffer), socket) == 999){
        //Failed to send message
        pthread_mutex_unlock(&mutex);
        return 5;
    }

    //Release the lock
    pthread_mutex_unlock(&mutex);
    return 0;
}