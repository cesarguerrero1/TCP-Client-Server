/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file Header file for the command-helpers.c source file
*/

#ifndef COMMAND_HELPERS_H
#define COMMAND_HELPERS_H

//Helper function to clear a buffer
void clear_buffer(char*, int);

//Helper function to send data to the server from a buffer
void send_message(char*, int, int);

//Helper function to store data from the server in a buffer
void receive_message(char*, int, int);

//Helper function to ensure that the given file path is in the correct format
char* format_path(char*);

#endif