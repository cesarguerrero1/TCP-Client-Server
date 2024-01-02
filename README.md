# Multi-threaded TCP Rmote File Server and Client

## Overview
This repository contains all of the files needed to run my implementation of a multi-threaded client/server file system. For my implementation I ran the server in (3) different environments:
1. I ran the client and server on separate Google Cloud VMs
2. I ran the client on my local machine and the server on a Google Cloud VM
3. I ran the client and server in separate terminal windows on my local machine.

For each environment I had to make changes to the IP Address. A global variable for setting the appropriate IP Address can be found in the "config.c" file

## Architecture
My basic client/server architecture that I implemented is as follows:
1. There is a single server socket that is monitored with a poll() event listener. The poll() will block until a socket connection event occcurs, a SIGINT signal is received, or a client command writes to a self-pipe that is also being monitored by the poll().
2. Whenever the server connects to a new client, a new thread is spawned and tasked with completing the client workflow. I chose to use a multi-threading solution as it allows for concurrency and it offered a good learning exercise for dealing with concurrency issues. Note that in order to solve for race conditions and deadlocks, I chose to utilize mutex locks whenever the server was performing CRUD operations.
4. The mutex locks help deal with multi-threading problems as the server is running, and they also work when the server needs to be stopped. Whether the client issues a STOP command or the server is stopped via a SIGINT signal, I used a comination of mutex locks and a sleep() command to allow for runnings threads to finish and prevent them from potentially stopping in an inconsistent state. As you can see in the "server.c" file when the server is instructed to stop the main process is put to sleep() which prevents new clients from connecting, but does not stop already running threads from working. Once the main process wakes up, it needs to acquire the mutex lock (which all threads must acquire when doing CRUD operations) before it can close the server socket and stop the entire program.

## Testing
All of the tests can be found in the "./tests" folder and the results of each test can be found in the "./tests/test-results" folder. The entire testing suite is controlled from the "main.sh" file. It will cleanup the environment and run all appropriate tests. The results of each test are stored in appropriate text files.

NOTE: You must have the following file path present for the tests to run correctly
1. ./client-root/images/Mario.jpeg

## How to run this program
1. Clone the repository
2. Give bash scripts appropriate permissions if you want to run and/or add to the test suite
3. Open the server directory in its own terminal and open the client directory in its own terminal
4. Start the server and connect via the client terminal as needed

## Commands
1. Store files on the server use the WRITE command
     - ./client WRITE <string: local_path> <(optional)string: remote_path>

2. Retrieve files from the server use the GET command
    - ./client GET <string: remote_path> <int: version> <(optional)string: save_path>

3. Lookup metadata for a file stored on the server use the LS command
    - ./client LS <string: remote_path>

4. Delete files from the server use the RM command
    - ./client RM <string: remote_path>

5. Stop the server
    - ./client STOP
