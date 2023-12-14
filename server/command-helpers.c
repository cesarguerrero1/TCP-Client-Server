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
 * This helper function is used to send data to the client. If there is an issue with the 
 * send then the program will only close the CLIENT socket and then return to listening for a new client
 * @param {char*} buffer - Buffer storing data we want to send
 * @param {int} data_size - Size of the data we are sending
 * @param {int} socket - Client Socket
 * @return {int} - Zero if everything goes well else 999 if send fails
*/
int send_message(char* buffer, int data_size, int socket){
    
  if(send(socket, buffer, data_size, 0) < 0){
    printf("CATASTROPHIC ERROR: Failed to send data to the client. Closing client socket. If the issue persists restart the server\n");
    close(socket);
    return 999;
  }

  return 0;
}


/**
 * This helper function is used to receive data from the client. If there is an issue with the
 * recv then the program will only close the CLIENT socket
 * @param {char*} buffer - The buffer we will store our client response in
 * @param {int} buffer_size - Size of the overall buffer
 * @param {int} socket - Client Socket
 * @return {int} - Zero if everything goes well else 1000 if recv fails
*/
int receive_message(char* buffer, int buffer_size, int socket){

  if(recv(socket, buffer, buffer_size, 0) < 0){
    printf("CATASTROPHIC ERROR: Failed to receive data to the client. Closing client socket. If the issue persists restart the server\n");
    close(socket);
    return 1000;
  }
  
  return 0;
}