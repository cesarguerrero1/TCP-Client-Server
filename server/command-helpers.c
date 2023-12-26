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

//Stat() Libraries
#include <sys/stat.h>
#include <errno.h>

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
 * This helper function is used to send data to the client. If there is an issue with the 
 * send then the program will only close the CLIENT socket
 * @param {char*} buffer - Buffer storing data we want to send to our client
 * @param {int} data_size - Size of the data we are sending
 * @param {int} socket - Client Socket
 * @return {int} - Zero if everything goes well else 999 if send fails
*/
int send_message(char* buffer, int data_size, int socket){
    
    if(send(socket, buffer, data_size, 0) < 0){
        printf("CATASTROPHIC ERROR: Failed to send data to the client, closing client socket. If the issue persists restart the server\n");
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
        printf("CATASTROPHIC ERROR: Failed to receive data from the client, closing client socket. If the issue persists restart the server\n");
        close(socket);
        return 1000;
    }
    
    return 0;
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
        printf("Creating directory: %s\n", temp_path);

        //Attempt to make our directory
        if(mkdir(temp_path, S_IRWXU) == 0){
            printf("Directory created\n");
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
                printf("Directory already exists\n");
                return 0;
            }else if(errno == ENOENT){
                printf("Could not create directory because parent directory does not exist\n");
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


/**
 * Apply a version to the given path
 * @param {char*} path - The path we are applying the version to
*/
void apply_version(char* path){
    //Copy our path so we can alter the copy safely
    char temp_path[MAX_FILEPATH_LENGTH];
    strncpy(temp_path, path, MAX_FILEPATH_LENGTH-1);

    //We need buffers as we piecewise build our new path
    char versioned_path[MAX_FILEPATH_LENGTH];
    char filename[MAX_FILEPATH_LENGTH];
    char extension[MAX_FILEPATH_LENGTH];

    //Recall that the LAST '/' separates our path from our filename.extension
    char* temp_filename;
    char* extension_to_remove;

    char slash = '/';
    char period = '.';

    temp_filename = strrchr(temp_path, slash);
    extension_to_remove = strrchr(temp_filename, period);

    //Check if we have an extension
    if(extension_to_remove != NULL){
        //Copy ONLY our extension
        strncpy(extension, extension_to_remove, MAX_FILEPATH_LENGTH-1);
        temp_filename[extension_to_remove - temp_filename] = '\0';
    }else{
        strcpy(extension, "");
    }

    //Copy our now isolated filename and truncate our temporary path
    strncpy(filename, temp_filename, MAX_FILEPATH_LENGTH-1);
    temp_path[temp_filename - temp_path] = '\0';

    //We now have all of our pieces and can sequentially add a version until we find one that works
    int version = 1;
    struct stat version_path_stat;

    while(1){
        snprintf(versioned_path, MAX_FILEPATH_LENGTH-1, "%s%s-V%d%s", temp_path, filename, version, extension);

        //When we find a version that works break the loop
        if(stat(versioned_path, &version_path_stat) != 0){
            break;
        }
        version++;
    }

    //Final step is to copy our versioned path to our actual path
    strcpy(path, versioned_path);
    return;
}


/**
 * Given a path, attempt to access the file, and then depending on the 'mode' perform:
 * - 1: Retrieve a specific version of a file or the latest version
 * - 2: Get ALL versioning information for a file
 * - 3: Remove all versions of a given file
 * @param {char*} path - The path for the file we are attempting to access
 * @param {int} version - The version of the file we are attempting to access
 * @param {int} mode - The mode we are using to access the file
 * @return {int} - Zero if everything works else non-zero
*/
int access_file(char* path, int version, int mode){

    //As before we need to append our root directory
    char temp_path[MAX_FILEPATH_LENGTH];
    strncpy(temp_path, path, MAX_FILEPATH_LENGTH-1);
    strncpy(path, ROOT_DIR, MAX_FILEPATH_LENGTH-1);
    strncat(path, temp_path, MAX_FILEPATH_LENGTH-1);

    //Copy our now correctly formatted path to our temp_path
    strncpy(temp_path, path, MAX_FILEPATH_LENGTH-1);

    //Remove trailing '/' values
    char* temp;
    char slash = '/';
    //NOTE: The LAST '/' separates our path from our filename.extension
    temp = strrchr(temp_path, slash);

    //Removing trailing slashes
    while(1){
        if(strlen(temp) == 1){
            //Truncate our string
            temp_path[temp - temp_path] = '\0';
            strncpy(path, temp_path, MAX_FILEPATH_LENGTH-1);

            //Continue processing our string
            temp = strrchr(temp_path, slash);
        }else{
            break;
        }
    }
    printf("Analyzing Path: %s (Version: %d)\n", path, version);

    //Decompose our path so we can check for the versioned path
    char filename[MAX_FILEPATH_LENGTH];
    char extension[MAX_FILEPATH_LENGTH];
    
    //Temporary holders
    char* extension_to_remove;
    char period = '.';

    extension_to_remove = strrchr(temp, period);
    if(extension_to_remove != NULL){
        //Copy ONLY our extension
        strncpy(extension, extension_to_remove, MAX_FILEPATH_LENGTH-1);
        temp[extension_to_remove - temp] = '\0';
    }else{
        strcpy(extension, "");
    }

    //Copy ONLY the filename
    strncpy(filename, temp, MAX_FILEPATH_LENGTH-1);

    //Truncate our temporary path
    temp_path[temp - temp_path] = '\0';

    //Depending on the mode, perform a different operation
    if(mode == 1){
        return access_mode_retrieve(path, temp_path, filename, extension, version);
    }else if(mode == 2){
        return access_mode_remove(temp_path, filename, extension);
    }else if(mode == 3){
        return 3;
    }else{
        printf("ERROR: Invalid mode spcified\n");
        return -1;
    }
}


/**
 * Given path components, craft a path with the specified version and attempt to retrieve the file
 * @param {char*} file_path - The actual path our server will eventually use if the file exists
 * @param {char*} dir_path - The directory portion of our path
 * @param {char*} filename - The actual name of the file we are looking for
 * @param {char*} extension - The extension of our file
 * @param {int} version - The version we are attempting to retrieve
 * @return {int} - Zero if everything goes well otherwise non-zero
 */
int access_mode_retrieve(char* file_path, char* directory_path, char* filename, char* extension, int version){
    
    char versioned_path[MAX_FILEPATH_LENGTH];
    char trailing_path[MAX_FILEPATH_LENGTH];
    //Clear our buffers
    clear_buffer(versioned_path, MAX_FILEPATH_LENGTH);
    clear_buffer(trailing_path, MAX_FILEPATH_LENGTH);

    //If the version is -1 it means we want the most recent version
    struct stat version_path_stat;
    if(version == 0){
        int count = 1;
        while(1){
            snprintf(versioned_path, MAX_FILEPATH_LENGTH-1, "%s%s-V%d%s", directory_path, filename, count, extension);
            printf("Searching: %s\n", versioned_path);

            if(stat(versioned_path, &version_path_stat) == 0){
                //The file exists but since a version was not specified we need to keep looking
                strncpy(trailing_path, versioned_path, MAX_FILEPATH_LENGTH-1);
                clear_buffer(versioned_path, MAX_FILEPATH_LENGTH);
                count++;
            }else{
                if(strlen(trailing_path) == 0){
                    //The file does not exist
                    return 1;
                }else{
                    //We found the most recent version
                    strncpy(file_path, trailing_path, MAX_FILEPATH_LENGTH-1);
                    return 0;
                }
            }
        }
    }else{
        //A version was specified
        snprintf(versioned_path, MAX_FILEPATH_LENGTH-1, "%s%s-V%d%s", directory_path, filename, version, extension);
        printf("Checking: %s\n", versioned_path);

        if(stat(versioned_path, &version_path_stat) == 0){
            //The file was found
            strncpy(file_path, versioned_path, MAX_FILEPATH_LENGTH-1);
            return 0;
        }else{
            //The file was not found
            return 1;
        }
    }
}


/**
 * Given path components, craft a path and attempt to delete all versions of the file
 * @param {char*} dir_path - The directory portion of our path
 * @param {char*} filename - The actual name of the file we are looking for
 * @param {char*} extension - The extension of our file
 * @return {int} - Zero if everything goes well otherwise non-zero
*/
int access_mode_remove(char* directory_path, char* filename, char* extension){

    char versioned_path[MAX_FILEPATH_LENGTH];
    clear_buffer(versioned_path, MAX_FILEPATH_LENGTH);

    struct stat version_path_stat;
    int count = 1;
    int files_deleted = 0;
    while(1){
        snprintf(versioned_path, MAX_FILEPATH_LENGTH-1, "%s%s-V%d%s", directory_path, filename, count, extension);
        printf("Attempting to delete: %s\n", versioned_path);

        //When we reach a version that does not exist we break the loop
        if(stat(versioned_path, &version_path_stat) == 0){
            if(remove(versioned_path) == -1){
                printf("CATASTROPHIC ERROR: Failed to delete file\n");
                return 2;
            }
            clear_buffer(versioned_path, MAX_FILEPATH_LENGTH);
            files_deleted++;
            count++;
        }else{
            //The file does not exist so we just need to check if we deleted anything
            if(files_deleted == 0){
                return 1;
            }else{
                //We deleted all possible versions
                return 0;
            }
        }
    }
}