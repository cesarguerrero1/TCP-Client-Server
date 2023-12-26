/**
 * Cesar Guerrero
 * 12/11/23
 * 
 * @file This file handles starting the server and spawning new threads to
 * handle each new client connection
*/

//Thread Library
#include <pthread.h>

//Libraries for creating the Socket
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//Library to handle CTRL + C
#include <signal.h>

//Standard Libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//Libraries for handling directory checks
#include <sys/stat.h>
#include <errno.h>

//Library for handling indeterminate events
#include <sys/poll.h>

//Header Files
#include "config.h"
#include "commands.h"
#include "command-helpers.h"

//Function Declarations
void signal_handler(int);
void* execute_thread(void*);

//Pipe
int self_pipe[2];

//Variable to handle preventing new clients from starting work as server is shutting down
int PREVENT_CONNECTIONS = 0;

//Initialize Mutex Lock
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Create our function map
command_map_t command_map[NUM_COMMANDS] = {
    {"WRITE", respond_to_write},
    {"GET", respond_to_get},
    {"RM", respond_to_rm},
    {"LS", respond_to_ls},
    {"STOP", respond_to_stop}
};

/**
 * Entrance into our program
 * @return {int} - Return zero if everything works as expected
*/
int main(){

    //Initialize our Signal Handler
    signal(SIGINT, signal_handler);

    //Make sure our root directory exists
    if(mkdir(ROOT_DIR, S_IRWXU) != 0 && errno != EEXIST){
        printf("ERROR: Failed to create root directory\n");
        return 1;
    }

    //Set up our pipe
    if(pipe(self_pipe) < 0){
        printf("ERROR: Failed to create self-pipe\n");
        return 2;
    }

    //Server
    int socket_desc;

    //Socket Structures
    socklen_t client_size;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    //Create our Server Socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc < 0){
        printf("ERROR: Socket creation failed\n");
        return 3;
    }

    //Set up our Poll Structure
    struct pollfd fds[2];
    fds[0].fd = socket_desc; //Listener for Server Socket
    fds[0].events = POLLIN;
    fds[1].fd = self_pipe[0]; //Listener for Pipe (NOTE: The pipe is an array, [read, write])
    fds[1].events = POLLIN;

    //Set the Port and IP
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCKET_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SOCKET_IP_ADDRESS);

    //Bind to the port and IP
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("ERROR: Failed to bind to the port\n");
        return 4;
    }

    //Tell the server to start listening for clients - Backlog of 10 clients allowed
    if(listen(socket_desc, 10) < 0){
        printf("ERROR: Server failed to begin listening for clients\n");
        return 5;
    }
    printf("--- Server actively listening for incoming connections ---\n");


    //We want our server to perpetually listen for clients
    while(1){

        //This will indefinitely block until a listener gets an event or a SIGINT signal is received
        int num_ready = poll(fds, 2, -1);
        if(num_ready < 0){
            if(errno == EINTR){
                //We received a "CTRL+C" signal which means we need to stop the server
                break;
            }
            printf("ERROR: Polling failed\n");
            return 6;
        }

        //Socket connection event handler
        if(fds[0].revents & POLLIN){

            client_size = sizeof(client_addr);
            int client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
            if(client_sock < 0){
                printf("ERROR: Cannot connect to client. If the problem persists restart server\n");
                continue;
            };

            //Malloc memory so we can pass a unique client socket to each thread
            int* socket_pointer = (int*)malloc(sizeof(int));
            if(socket_pointer == NULL){
                printf("ERROR: Failed to allocate memory for socket pointer. If the problem persists restart server\n");
                close(client_sock);
                continue;
            };
            *socket_pointer = client_sock;

            //Spawn a new thread to handle the client
            pthread_t thread;
            if(pthread_create(&thread, NULL, execute_thread, (void*)socket_pointer) != 0){
                printf("ERROR: Failed to created thread. If the problem persists restart server\n");

                //Close the socket and free our malloc
                close(*socket_pointer);
                free(socket_pointer);
                continue;
            };
        }

        //Self-pipe event handler
        if(fds[1].revents & POLLIN){
            printf("--- [STOPPING SERVER] ---\n");
            break;
        }
    }

    //We want to give the rest of the running threads time to finish their work
    printf("SHUTTING SERVER DOWN IN %d SECONDS\n", SHUTDOWN_TIME);
    sleep(SHUTDOWN_TIME);

    //Acquire the mutex lock as a final precaution to prevent a thread from potentially
    //finishing in an invalid state
    pthread_mutex_lock(&mutex);

    //Close server socket
    close(socket_desc);

    //Release the lock and then destroy it
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);

    return 0;
}


/**
 * This is a helper function that we are passing to each newly spawned thread so that it is able
 * to parse the given command and run the appropriate function against the given socket
 * @param {void*} socket_pointer - This will always be cast back to an int so we can utilize our client socket
 * @return {void*} - This is just standard convention for threads
*/
void* execute_thread(void* socket_pointer){
    //Get Thread ID
    pthread_t thread_id = pthread_self();
    
    //Cast our socket back to an int
    int socket = *((int*)socket_pointer);

    time_t current_time;
    current_time = time(NULL);

    //Command Buffer Array -- Each thread gets a unique buffer
    char command_buffer[COMMAND_BUFFER_SIZE];
    clear_buffer(command_buffer, COMMAND_BUFFER_SIZE);

    //Get the command from the client and delegate to our command handler
    if(receive_message(command_buffer, COMMAND_BUFFER_SIZE, socket) == 1000){
        //The socket is already closed so we just need to free our malloc
        free(socket_pointer);
        return NULL;
    }

    if(PREVENT_CONNECTIONS == 1){
        char msg[] = "SHUT DOWN IN PROGRESS - CONNECTION REFUSED";
        printf("%s\n", msg);
        send_message(msg, strlen(msg), socket);

        //Close the socket and free our malloc
        close(socket);
        free(socket_pointer);

        return NULL;
    }

    //Loop over our map and use our given command to attempt to call a function
    for(int i = 0; i < NUM_COMMANDS; i++){
        //If we find a valid command then we need to delegate accordingly
        if(strcmp(command_buffer, command_map[i].command_name) == 0){
            printf("\nStart Executing: %s Client Thread #%lu (Timestamp: %lu)\n", command_buffer, (unsigned long)thread_id, current_time);
            printf("CLIENT COMMAND: [%s]\n", command_buffer);
            int result = (command_map[i].function_pointer)(socket);
            current_time = time(NULL);
            printf("Finish Executing: %s Client Thread #%lu (Timestamp: %lu)\n", command_buffer, (unsigned long)thread_id, current_time);
            //Close and free the socket
            close(socket);
            free(socket_pointer);

            if(result == 1){
                PREVENT_CONNECTIONS = 1;
                //Inputting a sleep to allow for testing PREVENT_CONNECTIONS
                sleep(1);

                //Writing to the self-pipe to break out of poll()
                int val = 1;
                if(write(self_pipe[1], &val, 1) < 0){
                    printf("CATASTROPHIC ERROR: Failed to write to self-pipe\n");
                    return NULL;
                }

                return NULL;
            }

            return NULL;
        }
    }

    //We have an invalid command
    printf("ERROR: Server could not parse [%s]\n", command_buffer);
    close(socket);
    free(socket_pointer);

    return NULL;
}


/**
 * This is a helper function for shutting down the server if there is a CTRL+C interrupt by the user
 * @param {int} sig - Signal number that triggered this interruption
*/
void signal_handler(int sig){
    printf("\n--- [PROGRAM INTERRUPTION] ---\n");
    PREVENT_CONNECTIONS = 1;

    return; 
}