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

//Given a path, attempt to access the file, and then depending on the 'mode' perform another function call
int access_file(char*, int, int);

//Helper function called by access_file() to retrieve a file
int access_mode_retrieve(char*, char*, char*, char*, int);

//Helper function called by access_file() to remove a file and all its versions
int access_mode_remove(char*, char*, char*);

#endif