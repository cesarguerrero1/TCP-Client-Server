# Multi-threaded Client/Server File System

## Overview

This repository contains all of the files needed to run my implementation of a multi-threaded client/server file system. For my implementation I ran the server on a Google Cloud VM and tested via my local Macbook computer. When a client conencts to the server a new thread is spawned to handle the client command.

## Testing

All of the tests can be found in the ./tests/ folder and the results of each test can be found in ./tests/test-results/ folder upon successfully running the main test script. The "main.sh" script is in charge of running all subsequent testing scripts.

Note that you must have the following file path present for the tests to run correctly
1. ./client/root/images/Mario.jpeg
