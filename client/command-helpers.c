/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file This file is the soure code for all of our helper functions
*/

//Socket Libraries
#include <sys/socket.h>
#include <unistd.h>

//Stat() Libraries
#include <sys/stat.h>
#include <errno.h>

//Standard Libraries
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Header Files
#include "command-helpers.h"
#include "config.h"

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
        printf("ERROR: Failed to send data to the server. Closing socket and exiting program\n");
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
        printf("ERROR: Failed to receive data from the server. Closing socket and exiting program\n");
        close(socket);
        exit(1000);
    }

    return;
}


/**
 * This helper function is used to ensure that the path we are passing to our
 * server is in the correct format
 * @param {char*} path - The path we want to format
 * @return {char*} - The formatted path or NULL
*/
char* format_path(char* path){
    if(path == NULL){
        printf("ERROR: The given remote path is NULL\n");
        return NULL;
    }

    //We want to traverse until we find our first non-period or non-slash
    int count = 0;

    char value = path[count];
    while(value != '\0'){

        if(value != '.' && value != '/'){
            return path + count;
        }
        count ++;
        value = path[count];
    }

    //If we get here then we have a path that is either all periods or all slashes
    printf("ERROR: The given path is invalid and cannot be used\n");
    return NULL;
}


/**
 * Given a file path, create the appropriate directories with respect to the root directory
 * @param {char*} path - The path we are analyzing
 * @return {int} - Zero if everything works else non-zero
*/
int create_path(char* path){
    
    //Create a copy of our path and then rebuild it using our root directory
    char temp_path[MAX_FILEPATH_LENGTH];
    strncpy(temp_path, path, MAX_FILEPATH_LENGTH-1);
    strncpy(path, ROOT_DIR, MAX_FILEPATH_LENGTH-1);
    strncat(path, temp_path, MAX_FILEPATH_LENGTH-1);

    //Copy our now correctly formatted path to our temp_path
    strncpy(temp_path, path, MAX_FILEPATH_LENGTH-1);

    //Create directories as needed
    char* temp;
    char slash = '/';
    //NOTE: The FIRST '/' separates our root directory from our actual file path
    temp = strrchr(temp_path, slash);

    int repeat = 0;
    while(1){
        //Remove any trailing '/' values
        if(strlen(temp) == 1){
            //Truncate our string
            temp_path[temp - temp_path] = '\0';
            strncpy(path, temp_path, MAX_FILEPATH_LENGTH-1);

            //Continue processing our string
            temp = strrchr(temp_path, slash);
            continue;
        }

        temp_path[temp - temp_path] = '\0';

        //Attempt to make our directory
        if(mkdir(temp_path, S_IRWXU) == 0){
            if(repeat == 1){
                repeat = 0;
                //Reset our temp_path to the original path for our recursive method
                strncpy(temp_path, path, MAX_FILEPATH_LENGTH-1);
                temp = strrchr(temp_path, slash);
                continue;
            }else{
                //There is no reason to perform our recursive method
                return 0;
            }
        }else{
            //Our directory was not created so we need to handle why
            if(errno == EEXIST){
                return 0;
            }else if(errno == ENOENT){
                repeat = 1; //We need to perform our recursive method
                temp = strrchr(temp_path, slash);
                continue;
            }else{
                printf("ERROR: Failed to create directory\n");
                return 1;
            }
        }
    }
}