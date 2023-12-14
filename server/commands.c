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

//Header Files
#include "config.h"
#include "commands.h"
#include "command-helpers.h"

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
    printf("This is should print after the lock\n");

    sleep(10);

    //Send a message
    strncpy(status_buffer, "READY", STATUS_BUFFER_SIZE-1);
    send_message(status_buffer, strlen(status_buffer), socket);

    //Release the lock and close the socket
    pthread_mutex_unlock(&mutex);
    close(socket);

    return 0;
}