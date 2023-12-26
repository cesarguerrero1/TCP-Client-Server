#!/bin/bash

#Cesar Guerrero
#12/17/23

#This is our MAIN CLIENT shell script. It is used to setup our test environment
#and this script is in charge of running each individual test script

#To run this script:
# 1. Give it permission (chmod +x main.sh)
# 2. Start the server
# 3. Run the script

#Clean up our executable
make clean
make

mkdir -p ./tests/test-results

rm -f ./tests/test-results/client-test.txt
touch ./tests/test-results/client-test.txt

#Test 1
echo "TEST 1" >> ./tests/test-results/client-test.txt
echo "Test Description: Ensure we get a usage error if no command is provided" >> ./tests/test-results/client-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/client-test.txt
echo "'USAGE ERROR: ./client <string: WRITE | string: GET | string: RM | string: LS | string: STOP>'" >> ./tests/test-results/client-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/client-test.txt
./client >> ./tests/test-results/client-test.txt
echo "Expect 2: $?" >> ./tests/test-results/client-test.txt
echo "---" >> ./tests/test-results/client-test.txt


#Test 2
echo -e "\nTEST 2" >> ./tests/test-results/client-test.txt
echo "Test Description: Ensure we get an error if an invalid command is provided" >> ./tests/test-results/client-test.txt
echo "EXPECTED OUTPUT:" >> ./tests/test-results/client-test.txt
echo "'ERROR: INVALID COMMAND - [nonsense]'" >> ./tests/test-results/client-test.txt
echo -e "\nACTUAL OUTPUT:" >> ./tests/test-results/client-test.txt
./client nonsense >> ./tests/test-results/client-test.txt
echo "Expect 5: $?" >> ./tests/test-results/client-test.txt
echo "---" >> ./tests/test-results/client-test.txt


#To keep the code clean and concise, we wrote method specific tests into their own scripts
./tests/write.sh
./tests/get.sh
./tests/stop.sh