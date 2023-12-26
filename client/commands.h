/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file Header file for the commands.c source file
*/

#ifndef COMMANDS_H
#define COMMANDS_H

//Struct that alows us to map a command to a function
typedef struct{
    char* command_name;
    int (*function_pointer)(int, char**, char*, int);
} command_map_t;

//Function to handle STOP command
int command_stop(int, char**, char*, int);

//Function to handle WRITE command
int command_write(int, char**, char*, int);

//Function to handle GET command
int command_get(int, char**, char*, int);

//Function to handle RM command
int command_rm(int, char**, char*, int);

#endif