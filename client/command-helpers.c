/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file This file is the soure code for all of our helper functions
*/

//Socket Libraries
#include <sys/socket.h>
#include <unistd.h>

//Standard Libraries
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Header Files
#include "command-helpers.h"

/**
 * Helper function to clear a buffer
 * @param {char*} buffer - The buffer that we want to clear
 * @param {int} buffer_size - The size of the buffer we are clearing
*/
void clear_buffer(char* buffer, int buffer_size){
    memset(buffer, '\0', buffer_size);
    return;
}


/**
 * This helper function is used to send data to the server. If there is an issue with the 
 * send then the program will close the socket and exit the program entirely
 * @param {char*} buffer - Buffer storing data we want to send
 * @param {int} data_size - Size of the data we are sending
 * @param {int} socket - Server Socket
*/
void send_message(char* buffer, int data_size, int socket){
  
    if(send(socket, buffer, data_size, 0) < 0){
        printf("CATASTROPHIC ERROR: Failed to send data to the server. Closing socket and exiting program\n");
        close(socket);
        exit(999);
    }

    return;
}


/**
 * This helper function is used to receive data from the server. If there is an issue with the
 * recv then the program will close the socket and exit the program entirely
 * @param {char*} buffer - The buffer we will store our server response in
 * @param {int} buffer_size - Size of the overall buffer
 * @param {int} socket - Server Socket
*/
void receive_message(char* buffer, int buffer_size, int socket){
    
    if(recv(socket, buffer, buffer_size, 0) < 0){
        printf("CATASTROPHIC ERROR: Failed to receive data from the server. Closing socket and exiting program\n");
        close(socket);
        exit(1000);
    }

    return;
}