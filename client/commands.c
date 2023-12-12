/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file This file is the soure code for all of the main commands for interacting with the server
*/

//Socket Libraries
#include <unistd.h>

//Standard Libraries
#include <stdio.h>

//Header Files
#include "config.h"
#include "commands.h"
#include "command-helpers.h"

//Buffers
char payload_buffer[PAYLOAD_BUFFER_SIZE];
char status_buffer[STATUS_BUFFER_SIZE];
char header_buffer[HEADER_BUFFER_SIZE];

/*

    //Clear our buffers
    clear_buffer(status_buffer, STATUS_BUFFER_SIZE);
    clear_buffer(payload_buffer, PAYLOAD_BUFFER_SIZE);
    clear_buffer(header_buffer, HEADER_BUFFER_SIZE);
*/
