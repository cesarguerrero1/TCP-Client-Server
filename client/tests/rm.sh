#!/bin/bash

#Cesar Guerrero
#12/25/23

#This our our RM shell script and it is used to test the RM command
#It is called by main.sh

#Clean up our tests output file
rm -f ./tests/test-results/rm-test.txt
touch ./tests/test-results/rm-test.txt

#Test 1
echo "TEST 1" >> ./tests/test-results/rm-test.txt
echo "Test Description: Test RM with no arguments" >> ./tests/test-results/rm-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/rm-test.txt
echo "'COMMAND: [RM]'" >> ./tests/test-results/rm-test.txt
echo "'USAGE ERROR: ./client RM <string: remote_path>'" >> ./tests/test-results/rm-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/rm-test.txt
./client RM >> ./tests/test-results/rm-test.txt
echo "Expect 10: $?" >> ./tests/test-results/rm-test.txt
echo "---" >> ./tests/test-results/rm-test.txt


#Test 2
echo -e "\nTEST 2" >> ./tests/test-results/rm-test.txt
echo "Test Description: Test RM with an invalid remote path" >> ./tests/test-results/rm-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/rm-test.txt
echo "'COMMAND: [RM]'" >> ./tests/test-results/rm-test.txt
echo "'ERROR: The given path is invalid and cannot be used'" >> ./tests/test-results/rm-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/rm-test.txt
./client RM .. >> ./tests/test-results/rm-test.txt
echo "Expect 11: $?" >> ./tests/test-results/rm-test.txt
echo "---" >> ./tests/test-results/rm-test.txt


#Test 3
echo -e "\nTEST 3" >> ./tests/test-results/rm-test.txt
echo "Test Description: Test RM with a file that does not exist on the server using a valid remote path" >> ./tests/test-results/rm-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/rm-test.txt
echo "'COMMAND: [RM]'" >> ./tests/test-results/rm-test.txt
echo "'Server Removal Destination: 'important-files/main.java" >> ./tests/test-results/rm-test.txt
echo "'[RM] -- SERVER RESPONSE: OK'" >> ./tests/test-results/rm-test.txt
echo "'[RM] -- SERVER RESPONSE: FILE NOT FOUND'" >> ./tests/test-results/rm-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/rm-test.txt
./client RM important-files/main.java >> ./tests/test-results/rm-test.txt
echo "Expect 0: $?" >> ./tests/test-results/rm-test.txt
echo "---" >> ./tests/test-results/rm-test.txt


#These tests will only pass if the WRITE Shell Script is run before this script
#Test 4
echo -e "\nTEST 4" >> ./tests/test-results/rm-test.txt
echo "Test Description: Test RM with a file that DOES exist on the server but there is only (1) version" >> ./tests/test-results/rm-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/rm-test.txt
echo "'COMMAND: [RM]'" >> ./tests/test-results/rm-test.txt
echo "'Server Removal Destination: 'Mario.jpeg" >> ./tests/test-results/rm-test.txt
echo "'[RM] -- SERVER RESPONSE: OK'" >> ./tests/test-results/rm-test.txt
echo "'[RM] -- SERVER RESPONSE: FILE REMOVED'" >> ./tests/test-results/rm-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/rm-test.txt
./client RM ./Mario.jpeg >> ./tests/test-results/rm-test.txt
echo "Expect 0: $?" >> ./tests/test-results/rm-test.txt
echo "---" >> ./tests/test-results/rm-test.txt


#Test 5
echo -e "\nTEST 5" >> ./tests/test-results/rm-test.txt
echo "Test Description: Test RM with a file that DOES exist on the server and there are multiple versions of the file" >> ./tests/test-results/rm-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/rm-test.txt
echo "'COMMAND: [RM]'" >> ./tests/test-results/rm-test.txt
echo "'Server Removal Destination: 'concurrent-test/test.txt" >> ./tests/test-results/rm-test.txt
echo "'[RM] -- SERVER RESPONSE: OK'" >> ./tests/test-results/rm-test.txt
echo "'[RM] -- SERVER RESPONSE: FILE REMOVED'" >> ./tests/test-results/rm-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/rm-test.txt
./client RM concurrent-test/test.txt >> ./tests/test-results/rm-test.txt
echo "Expect 0: $?" >> ./tests/test-results/rm-test.txt
echo "---" >> ./tests/test-results/rm-test.txt