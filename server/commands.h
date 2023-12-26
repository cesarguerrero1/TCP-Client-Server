/**
 * Cesar Guerrero
 * 12/12/23
 * 
 * @file Header file for the commands.c source file
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#include <pthread.h>

//Structure used to map a command to a given function
typedef struct{
    char* command_name;
    int (*function_pointer)(int);
} command_map_t;

//Mutex lock
extern pthread_mutex_t mutex;

//Handle a STOP command issued by the client
int respond_to_stop(int);

//Handle a WRITE command issued by the client
int respond_to_write(int);

//Handle a GET command issued by the client
int respond_to_get(int);

//Handle a RM command issued by the client
int respond_to_rm(int);

//Hnadle a LS command issued by the client
int respond_to_ls(int);

#endif