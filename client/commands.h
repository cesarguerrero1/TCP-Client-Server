/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file Header file for the commands.c source file
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#include <string.h>

//Struct that alows us to map a command to a function
typedef struct{
    char* command_name;
    int (*function_pointer)(int, char**, char*, int);

} function_map_t;

//Given a proposed client command, delege to correct command
int execute_command(int, char**, char*, int);

#endif