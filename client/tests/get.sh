#!/bin/bash

#Cesar Guerrero
#12/22/23

#This is our GET shell script and it is used to test the GET command
#It is called by main.sh

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
echo "Test Description: Test GET with an invalid remote path, NO specified version, and NO save path" >> ./tests/test-results/get-test.txt
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
echo "Test Description: Test GET with a file that does not exist on the server using a valid remote path, NO version, and NO save path" >> ./tests/test-results/get-test.txt
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
echo "Test Description: Test GET with a file that does not exist on the server using a valid remote path, invalid version, and NO save path" >> ./tests/test-results/get-test.txt
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


#NOTE: The following tests will ONLY pass if the WRITE Shell Script is run before this script
#Test 5
echo -e "\nTest 5" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with a file that DOES exist on the server using an invalid version and NO save path" >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'INVALID VERSION PROVIDED -- Defaulting to MOST RECENT VERSION'" >> ./tests/test-results/get-test.txt
echo "'Server Destination: concurrent-test/test.txt (Version: 0)'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: OK'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: FILE FOUND'" >> ./tests/test-results/get-test.txt
echo "'Save Destination: ./client-root/concurrent-test/test.txt'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- FILE SAVED'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET ./concurrent-test/test.txt nonsense-version >> ./tests/test-results/get-test.txt
echo "Expect 0: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt
#Note: There should now be a new text file called test.txt that refers to the 3rd version of the file on the server


#Test 6
echo -e "\nTest 6" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with a file that DOES exist on the server using a nonexistent version and NO save path" >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'Server Destination: concurrent-test/test.txt (Version: 4)'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: OK'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: FILE NOT FOUND'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET ./concurrent-test/test.txt 4 >> ./tests/test-results/get-test.txt
echo "Expect 14: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt


#Test 7
echo -e "\nTest 7" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with a file that DOES exist on the server using an existing version and a valid save path" >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'Server Destination: concurrent-test/test.txt (Version: 1)'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: OK'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: FILE FOUND'" >> ./tests/test-results/get-test.txt
echo "'Save Destination: ./client-root/valid-path/text_files/_.txt'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- FILE SAVED'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET concurrent-test/test.txt 1  ./valid-path/text_files/_.txt >> ./tests/test-results/get-test.txt
echo "Expect 0: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt
#Note: There should now be a new text file called _.txt that refers to the 1st version of the file on the server


#Test 8
echo -e "\nTest 8" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with a file that DOES exist on the server using an existing version and using an invalid save path" >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'Server Destination: concurrent-test/test.txt (Version: 1)'" >> ./tests/test-results/get-test.txt
echo "'ERROR: The given path is invalid and cannot be used'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET concurrent-test/test.txt 1 .. >> ./tests/test-results/get-test.txt
echo "Expect 12: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt


#Test 9
echo -e "\nTest 9" >> ./tests/test-results/get-test.txt
echo "Test Description: Test GET with a file that DOES exist on the server using an existing version and using a valid save path" >> ./tests/test-results/get-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/get-test.txt
echo "'COMMAND: [GET]'" >> ./tests/test-results/get-test.txt
echo "'Server Destination: client-root/images/Mario.jpeg (Version: 1)'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: OK'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- SERVER RESPONSE: FILE FOUND'" >> ./tests/test-results/get-test.txt
echo "'Save Destination: ./client-root/new-images/_/Mario.jpeg'" >> ./tests/test-results/get-test.txt
echo "'[GET] -- FILE SAVED'" >> ./tests/test-results/get-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/get-test.txt
./client GET client-root/images/Mario.jpeg 1 new-images/_/Mario.jpeg >> ./tests/test-results/get-test.txt
echo "Expect 0: $?" >> ./tests/test-results/get-test.txt
echo "---" >> ./tests/test-results/get-test.txt
#There should now be a new Mario image saved on the client