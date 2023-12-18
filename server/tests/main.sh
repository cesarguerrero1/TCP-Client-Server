#!/bin/bash

#Cesar Guerrero
#12/17/23

#This is our MAIN SERVER shell script. It is used to run a test of starting our 
#server and killing it. Other tests are all driven by the client
#and can be found in those client-side test scripts.

#To run this script:
# 1. Give it permission (chmod +x main.sh)
# 2. Call it (./main.sh)

#Clean up our directory and then compile our program
make clean
make

mkdir -p ./tests/test-results

rm -f ./tests/test-results/server-test.txt
touch ./tests/test-results/server-test.txt

#Test 1
echo "TEST 1" >> ./tests/test-results/server-test.txt
echo "Test Description: Start the server, then 'kill' it and verify that it displays the appropriate information" >> ./tests/test-results/server-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/server-test.txt
echo "'--- Server actively listening for incoming connections ---'" >> ./tests/test-results/server-test.txt
echo "'--- [PROGRAM INTERRUPTION] ---'" >> ./tests/test-results/server-test.txt
echo "'SHUTTING DOWN SERVER IN 5 SECONDS'" >> ./tests/test-results/server-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/server-test.txt
./server >> ./tests/test-results/server-test.txt &
pid=$!
#Sleep for 2 seconds
sleep 2
#Simulate CTRL + C
kill -SIGINT $pid >> ./tests/test-results/server-test.txt
#Capture the exit code
echo "Expect 0: $?" >> ./tests/test-results/server-test.txt