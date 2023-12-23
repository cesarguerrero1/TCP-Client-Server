#!/bin/bash

#Cesar Guerrero
#12/22/23

#This is our GET shell script and it is used to test the GET command
#It is called by main.sh

#Clean up our executable
make clean
make

#Clean up our tests output file
rm -f ./tests/test-results/get-test.txt
touch ./tests/test-results/get-test.txt

#Test 1
echo "TEST 1" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with no arguments" >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'USAGE ERROR: ./client GET <string: remote_path> <int: version> <(optional)string: save_path>'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET >> ./tests/test-results/get-test.txt
echo "Expect 10: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt


#Test 2
echo -e "\nTEST 2" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with invalid remote path, NO version, and NO save path" >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'INVALID VERSION PROVIDED -- Defaulting to MOST RECENT VERSION'" >> ./tests/test-results/get-test.txt
echo "'ERROR: The given path is invalid and cannot be used'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET .. >> ./tests/test-results/get-test.txt
echo "Expect 11: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt


#Test 3
echo -e "\nTEST 3" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with valid remote path, NO version, and NO save path. HOWEVER the file does not exist on the server" >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'INVALID VERSION PROVIDED -- Defaulting to MOST RECENT VERSION'" >> ./tests/test-results/get-test.txt
echo "'Server Destination: valid-file-path.txt (Version: 0)'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: OK'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: FILE NOT FOUND'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET ./valid-file-path.txt >> ./tests/test-results/get-test.txt
echo "Expect 14: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt


#Test 4
echo -e "\nTEST 4" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with valid remote path, invalid version, and NO save path. HOWEVER the file does not exist on the server" >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'INVALID VERSION PROVIDED -- Defaulting to MOST RECENT VERSION'" >> ./tests/test-results/get-test.txt
echo "'Server Destination: valid-file-path.txt (Version: 0)'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: OK'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: FILE NOT FOUND'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET ./valid-file-path.txt nonsense-version >> ./tests/test-results/get-test.txt
echo "Expect 14: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt


# Test 5
echo -e "\nTEST 5" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with valid remote path, valid version, and NO save path. However the file does not exist on the server." >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'Server Destination: valid-file-path.txt (Version: 2)'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: OK'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: FILE NOT FOUND'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET ./valid-file-path.txt 2 >> ./tests/test-results/get-test.txt
echo "Expect 14: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt


#Test 6
echo -e "\nTEST 6" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with valid remote path, valid version, and invalid save path. However the file does not exist on the server." >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'Server Destination: valid-file-path.txt (Version: 2)'" >> ./tests/test-results/get-test.txt
echo "'ERROR: The given path is invalid and cannot be used'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET ./valid-file-path.txt 2 .. >> ./tests/test-results/get-test.txt
echo "Expect 12: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt


#Test 7
echo -e "\nTEST 7" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with valid remote path, valid version, and valid save path. However the file does not exist on the server." >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'Server Destination: valid-file-path.txt (Version: 2)'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: OK'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: FILE NOT FOUND'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET ./valid-file-path.txt 2 ./save-path.txt >> ./tests/test-results/get-test.txt
echo "Expect 14: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt