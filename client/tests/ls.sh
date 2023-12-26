#!/bin/bash

#Cesar Guerrero
#12/25/23

#This our our LS shell script and it is used to test the LS command
#It is called by main.sh

#Clean up our tests output file
rm -f ./tests/test-results/ls-test.txt
touch ./tests/test-results/ls-test.txt

#Test 1
echo "TEST 1" >> ./tests/test-results/ls-test.txt
echo "Test Description: Test LS with no arguments" >> ./tests/test-results/ls-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/ls-test.txt
echo "'COMMAND: [LS]'" >> ./tests/test-results/ls-test.txt
echo "'USAGE ERROR: ./client LS <string: remote_path>'" >> ./tests/test-results/ls-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/ls-test.txt
./client LS >> ./tests/test-results/ls-test.txt
echo "Expect 10: $?" >> ./tests/test-results/ls-test.txt
echo "---" >> ./tests/test-results/ls-test.txt


#Test 2
echo -e "\nTEST 2" >> ./tests/test-results/ls-test.txt
echo "Test Description: Test LS with an invalid remote path" >> ./tests/test-results/ls-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/ls-test.txt
echo "'COMMAND: [LS]'" >> ./tests/test-results/ls-test.txt
echo "'ERROR: The given path is invalid and cannot be used'" >> ./tests/test-results/ls-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/ls-test.txt
./client LS ./ >> ./tests/test-results/ls-test.txt
echo "Expect 11: $?" >> ./tests/test-results/ls-test.txt
echo "---" >> ./tests/test-results/ls-test.txt


#Test 3
echo -e "\nTEST 3" >> ./tests/test-results/ls-test.txt
echo "Test Description: Test LS with a file that does not exist on the server using a valid remote path" >> ./tests/test-results/ls-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/ls-test.txt
echo "'COMMAND: [LS]'" >> ./tests/test-results/ls-test.txt
echo "'Server Destination: 'important-files/main.java" >> ./tests/test-results/ls-test.txt
echo "'[LS] -- SERVER RESPONSE: OK'" >> ./tests/test-results/ls-test.txt
echo "'[LS] -- SERVER PAYLOAD:'" >> ./tests/test-results/ls-test.txt
echo "'./main.java METADATA'" >> ./tests/test-results/ls-test.txt
echo "'NO FILE DATA FOUND'" >> ./tests/test-results/ls-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/ls-test.txt
./client LS ./important-files/main.java >> ./tests/test-results/ls-test.txt
echo "Expect 0: $?" >> ./tests/test-results/ls-test.txt
echo "---" >> ./tests/test-results/ls-test.txt


#These tests will only pass if the WRITE Shell Script is run before this script
#Test 4
echo -e "\nTEST 4" >> ./tests/test-results/ls-test.txt
echo "Test Description: Test LS with a file that DOES exist on the server but there is only (1) version" >> ./tests/test-results/ls-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/ls-test.txt
echo "'COMMAND: [LS]'" >> ./tests/test-results/ls-test.txt
echo "'Server Destination: 'client-root/images/Mario.jpeg" >> ./tests/test-results/ls-test.txt
echo "'[LS] -- SERVER RESPONSE: OK'" >> ./tests/test-results/ls-test.txt
echo "'[LS] -- SERVER PAYLOAD:'" >> ./tests/test-results/ls-test.txt
echo "'./Mario.jpeg METADATA'" >> ./tests/test-results/ls-test.txt
echo "'(Version: 1): File Size = X bytes, Last Modified = Y'" >> ./tests/test-results/ls-test.txt
#I am not sure the formatting yet
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/ls-test.txt
./client LS client-root/images/Mario.jpeg >> ./tests/test-results/ls-test.txt
echo "Expect 0: $?" >> ./tests/test-results/ls-test.txt
echo "---" >> ./tests/test-results/ls-test.txt


#Test 5
echo -e "\nTEST 5" >> ./tests/test-results/ls-test.txt
echo "Test Description: Test LS with a file that DOES exist on the server and there are multiple versions of the file" >> ./tests/test-results/ls-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/ls-test.txt
echo "'COMMAND: [LS]'" >> ./tests/test-results/ls-test.txt
echo "'Server Destination: 'concurrent-test/test.txt" >> ./tests/test-results/ls-test.txt
echo "'[LS] -- SERVER RESPONSE: OK'" >> ./tests/test-results/ls-test.txt
echo "'[LS] -- SERVER PAYLOAD:'" >> ./tests/test-results/ls-test.txt
echo "'./test.txt METADATA'" >> ./tests/test-results/ls-test.txt
echo "'(Version: 1): File Size = X bytes, Last Modified = Y'" >> ./tests/test-results/ls-test.txt
echo "'(Version: 2): File Size = X bytes, Last Modified = Y'" >> ./tests/test-results/ls-test.txt
echo "'(Version: 3): File Size = X bytes, Last Modified = Y'" >> ./tests/test-results/ls-test.txt
#I am not sure the formatting yet
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/ls-test.txt
./client LS concurrent-test/test.txt >> ./tests/test-results/ls-test.txt
echo "Expect 0: $?" >> ./tests/test-results/ls-test.txt
echo "---" >> ./tests/test-results/ls-test.txt
