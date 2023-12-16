# Multi-threaded Client/Server File System

## Overview

This repository contains all of the files needed to run my implementation of a multi-threaded client/server file system. For my implementation I ran the server on a Google Cloud VM and tested via my local Macbook computer, however the code in this respository has the server and client pointing to PORT 2000 and IP Address: 127.0.0.1

## Architecture

My basic client/server architecture that I implemented is as follows:
    1. There is a single server socket that is monitored with a poll() event listener. The poll() will block until a socket connection event occcurs, a SIGINT signal is received, or a client command writes to a self-pipe that is also being monitored by the poll().
    2. Whenever the server connects to a new client, a new thread is spawned and tasked with completing the client workflow. I chose to use a multi-threading solution as it allows for concurrency, and it is a good exercise in working with mutual exclusion locks.
    3. Eventually when the server needs to be stopped, the idea that I implemented was allowing currently running threads to finish so as to prevent data loss or inconsitent thread states. When the server is instructed to close, a blocking condition is activated to prevent any new threads from attempting to do work as the server is shutting down. Additionally a sleep() is executed to, as mentioned before, allow threads that were already running to complete their work before the server closes
    4. To prevent potential data corruption, if a modifying action is being performed on the server (WRITE, RM, STOP) then a mutex lock must be acquired. This limits concurrency for these specific actions, bt it does ensure synchronization and helps to avoid potential data corruption.

## Testing

All of the tests can be found in the ./tests/ folder and the results of each test can be found in ./tests/test-results/ folder upon successfully running the main test script. The "main.sh" script is in charge of running all subsequent testing scripts.

Note that you must have the following file path present for the tests to run correctly
1. ./client/root/images/Mario.jpeg
