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

//Helper function to send data to the client from a buffer
int send_message(char*, int, int);

//Helper function to store data from the client in a buffer
int receive_message(char*, int, int);

//Ensure the path to where the file will be stored exists
int create_path(char*);

//Apply a version to our file
void apply_version(char*);

#endif