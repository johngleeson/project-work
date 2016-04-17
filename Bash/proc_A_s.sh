#!/bin/bash
# John Gleeson 12162213	
# 20/03/2014
# A script file to illustrate the use of traps
# Requires proc_B_s.sh to function properly

#Signal trap that listens for the SIGINT (ctrl + c) command
trap 'trap_function' SIGINT

#Run proc_B_s in the background
./proc_B_s.sh &

#Trap function to kill the last background process
trap_function()
{
#Send HUP signal to the last run background process
kill -HUP $!
echo -e "\nPID for proc_B_s is $!"
return 0
}

#Create the pipe if it doesn't already exist
if [ ! -p pipe1 ]
then 
mkfifo pipe1
else echo "Pipe1 already exists"
fi

#Get PID of proc_B_s for later use
proc_B_s_PID=$(ps -eo args,pid | grep "^/bin/bash ./proc_B_s.sh" | awk '{print$3}')

#Send the last message and remove the pipe
echo "Last message!" > pipe1

#Reads the contents of pipe2
cat pipe2
sleep 5

#Send a message 3 times
for (( x=1; x<4; x++ ))
do
echo  "I'm alive!"
sleep 5
done

#Does the following while loop as long as proc_B_s is the last run background
#process and file_A is not there. i.e. When proc_B_s is finished
while [[ "$proc_B_s_PID" == "$!" && ! -f file_A ]]
do
	#For loop to print 'I'm alone' every 5 seconds
	for (( x=1; x<4; x++ ))
	do
	echo  "I'm alone"
	sleep 5
	done
	break
done

wait
exit 0
