/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file Header file for config.c
*/

#ifndef CONFIG_H
#define CONFIG_H

extern char ROOT_DIR[];
extern char SOCKET_IP_ADDRESS[];
extern int SOCKET_PORT;

//Constant values for our program
enum PROGRAM_CONSTANTS{
    NUM_COMMANDS = 5,
    MAX_FILEPATH_LENGTH = 100,
    SHUTDOWN_TIME = 10, //Seconds
};

//These values are for sizing our buffers
enum BUFFER_ARRAY_SIZES{
    COMMAND_BUFFER_SIZE = 10, //Buffer for commands: WRITE, GET, etc.
    PAYLOAD_BUFFER_SIZE = 10000, //1KB Payload Buffer
    STATUS_BUFFER_SIZE = 100, //Buffer used for conveying status messages
    HEADER_BUFFER_SIZE = 250, //Buffer used for passing information to help the server prep
};

#endif
