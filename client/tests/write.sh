#!/bin/bash

#Cesar Guerrero
#12/20/23

#This is our WRITE shell script and it is used to test the WRITE command
#It is called by main.sh

#NOTE: Please ensure you have a Mario.jpeg file in the appropriate directory
# ./root/images/Mario.jpeg

#Clean up our tests output file
rm -f ./tests/test-results/write-test.txt
touch ./tests/test-results/write-test.txt

#Test 1
echo "TEST 1" >> ./tests/test-results/write-test.txt
echo "Test Description: Test WRITE with no arguments" >> ./tests/test-results/write-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/write-test.txt
echo "'COMMAND: [WRITE]'" >> ./tests/test-results/write-test.txt
echo "'USAGE ERROR: ./client WRITE <string: local_path> <(optional)string: remote_path>'" >> ./tests/test-results/write-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/write-test.txt
./client WRITE >> ./tests/test-results/write-test.txt
echo "Expect 10: $?" >> ./tests/test-results/write-test.txt
echo "---" >> ./tests/test-results/write-test.txt


#Test 2
echo -e "\nTEST 2" >> ./tests/test-results/write-test.txt
echo "Test Description: Test WRITE with a nonexistent file path" >> ./tests/test-results/write-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/write-test.txt
echo "'COMMAND: [WRITE]'" >> ./tests/test-results/write-test.txt
echo "'ERROR: Failed to verify local file path'" >> ./tests/test-results/write-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/write-test.txt
./client WRITE ./client-root/images/doesnotexist.jpeg >> ./tests/test-results/write-test.txt
echo "Expect 11: $?" >> ./tests/test-results/write-test.txt
echo "---" >> ./tests/test-results/write-test.txt


#Test 3
echo -e "\nTEST 3" >> ./tests/test-results/write-test.txt
echo "Test Description: Test WRITE with a directory path" >> ./tests/test-results/write-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/write-test.txt
echo "'COMMAND: [WRITE]'" >> ./tests/test-results/write-test.txt
echo "'ERROR: Local file path points to a directory'" >> ./tests/test-results/write-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/write-test.txt
./client WRITE ./client-root/images >> ./tests/test-results/write-test.txt
echo "Expect 12: $?" >> ./tests/test-results/write-test.txt
echo "---" >> ./tests/test-results/write-test.txt


#Test 4
echo -e "\nTEST 4" >> ./tests/test-results/write-test.txt
echo "Test Description: Test WRITE with a valid file path but no specified remote path" >> ./tests/test-results/write-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/write-test.txt
echo "'COMMAND: [WRITE]'" >> ./tests/test-results/write-test.txt
echo "'Server Destination: client-root/images/Mario.jpeg'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: OK'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: PATH ACCEPTED'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: FILE RECEIVED'" >> ./tests/test-results/write-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/write-test.txt
./client WRITE ./client-root/images/Mario.jpeg >> ./tests/test-results/write-test.txt
echo "Expect 0: $?" >> ./tests/test-results/write-test.txt
echo "---" >> ./tests/test-results/write-test.txt
#Note: There should now be a Mario.jpeg on the server located at ./server-root/client-root/images/Mario.jpeg


#Test 5
echo -e "\nTEST 5" >> ./tests/test-results/write-test.txt
echo "Test Description: Test WRITE with a valid file path but an invalid remote path" >> ./tests/test-results/write-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/write-test.txt
echo "'COMMAND: [WRITE]'" >> ./tests/test-results/write-test.txt
echo "'ERROR: The given path is invalid and cannot be used'" >> ./tests/test-results/write-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/write-test.txt
./client WRITE ./client-root/images/Mario.jpeg .. >> ./tests/test-results/write-test.txt
echo "Expect 14: $?" >> ./tests/test-results/write-test.txt
echo "---" >> ./tests/test-results/write-test.txt


#Test 6
echo -e "\nTEST 6" >> ./tests/test-results/write-test.txt
echo "Test Description: Test WRITE with a valid file and a valid remote path" >> ./tests/test-results/write-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/write-test.txt
echo "'COMMAND: [WRITE]'" >> ./tests/test-results/write-test.txt
echo "'Server Destination: Mario.jpeg'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: OK'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: PATH ACCEPTED'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: FILE RECEIVED'" >> ./tests/test-results/write-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/write-test.txt
./client WRITE ./client-root/images/Mario.jpeg Mario.jpeg >> ./tests/test-results/write-test.txt
echo "Expect 0: $?" >> ./tests/test-results/write-test.txt
echo "---" >> ./tests/test-results/write-test.txt
#Note: There should now be a Mario.jpeg on the server located at ./server-root/Mario.jpeg


#Test 7
echo -e "\nTEST 7" >> ./tests/test-results/write-test.txt
echo "Test Description: Test WRITE with 3 concurrent calls all attempting to save different files under the same name" >> ./tests/test-results/write-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/write-test.txt
echo "'COMMAND: [WRITE]'" >> ./tests/test-results/write-test.txt
echo "'Server Destination: concurrent-test/test.txt'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: OK'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: PATH ACCEPTED'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: FILE RECEIVED'" >> ./tests/test-results/write-test.txt
echo "'COMMAND: [WRITE]'" >> ./tests/test-results/write-test.txt
echo "'Server Destination: concurrent-test/test.txt'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: OK'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: PATH ACCEPTED'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: FILE RECEIVED'" >> ./tests/test-results/write-test.txt
echo "'COMMAND: [WRITE]'" >> ./tests/test-results/write-test.txt
echo "'Server Destination: concurrent-test/test.txt'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: OK'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: PATH ACCEPTED'" >> ./tests/test-results/write-test.txt
echo "'[WRITE] -- SERVER RESPONSE: FILE RECEIVED'" >> ./tests/test-results/write-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/write-test.txt
#Make all of our files and then run the concurrent calls
rm -rf ./client-root/concurrent-test
mkdir ./client-root/concurrent-test

rm -f ./client-root/concurrent-test/test1-con.txt
rm -f ./client-root/concurrent-test/test2-con.txt
rm -f ./client-root/concurrent-test/test3-con.txt

touch ./client-root/concurrent-test/test1-con.txt
touch ./client-root/concurrent-test/test2-con.txt
touch ./client-root/concurrent-test/test3-con.txt

echo "Concurrent Test 1" >> ./client-root/concurrent-test/test1-con.txt
echo "Concurrent Test 2" >> ./client-root/concurrent-test/test2-con.txt
echo "Concurrent Test 3" >> ./client-root/concurrent-test/test3-con.txt
sleep 1

#We want all of the concurrent calls to run at the same time
./client WRITE ./client-root/concurrent-test/test1-con.txt ./concurrent-test/test.txt >> ./tests/test-results/write-test.txt &
./client WRITE ./client-root/concurrent-test/test2-con.txt ./concurrent-test/test.txt >> ./tests/test-results/write-test.txt &
./client WRITE ./client-root/concurrent-test/test3-con.txt ./concurrent-test/test.txt >> ./tests/test-results/write-test.txt &
wait
echo "All concurrent calls have finished. There should now be (3) versions of test.txt each with their own unique text saved on the server" >> ./tests/test-results/write-test.txt
