#!/bin/bash

#Cesar Guerrero
#12/18/23

#This is our STOP shell script. It is used to test the STOP command
#It is called by main.sh

#Clean up our tests output file
rm -f ./tests/test-results/stop-test.txt
touch ./tests/test-results/stop-test.txt

#Test 1
echo "TEST 1" >> ./tests/test-results/stop-test.txt
echo "Test Description: Calling our STOP command should instruct the server to stop" >> ./tests/test-results/stop-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/stop-test.txt
echo "'COMMAND: [STOP]'" >> ./tests/test-results/stop-test.txt
echo "'[STOP] -- SERVER RESPONSE: OK'" >> ./tests/test-results/stop-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/stop-test.txt
./client STOP >> ./tests/test-results/stop-test.txt
echo "Expect 0: $?" >> ./tests/test-results/stop-test.txt
echo "---" >> ./tests/test-results/stop-test.txt


#Test 2
echo -e "\nTEST 2" >> ./tests/test-results/stop-test.txt
echo "Test Description: Concurrent calls should lead to a server response indicating that if the server is beginning a shutdown, new connections should be refused" >> ./tests/test-results/stop-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/stop-test.txt
echo "'COMMAND: [STOP]'" >> ./tests/test-results/stop-test.txt
echo "'[STOP] -- SERVER RESPONSE: SHUT DOWN IN PROGRESS - CONNECTION REFUSED'" >> ./tests/test-results/stop-test.txt
echo "'ERROR: Server failed to [STOP]'" >> ./tests/test-results/stop-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/stop-test.txt
./client STOP >> ./tests/test-results/stop-test.txt
echo "Expect 10: $?" >> ./tests/test-results/stop-test.txt
echo "---" >> ./tests/test-results/stop-test.txt


#For Test 3 we are inputting a sleep() to ensure the server is fully shut down
sleep 10


#Test 3
echo -e "\nTEST 3" >> ./tests/test-results/stop-test.txt
echo "Test Description: Attempting to call a server that is stopped leads to an error" >> ./tests/test-results/stop-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/stop-test.txt
echo "'COMMAND: [STOP]'" >> ./tests/test-results/stop-test.txt
echo "'ERROR: Cannot connect to server'" >> ./tests/test-results/stop-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/stop-test.txt
./client STOP >> ./tests/test-results/stop-test.txt
echo "Expect 4: $?" >> ./tests/test-results/stop-test.txt
echo "---" >> ./tests/test-results/stop-test.txt
