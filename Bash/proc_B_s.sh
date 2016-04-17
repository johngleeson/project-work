#!/bin/bash
# John Gleeson 12162213	
# 20/03/2014
# A script file to illustrate the use of traps
# Requires proc_A_s.sh to function properly

#Signal trap that listens for SIGHUP
trap ' trap_service_routine' SIGHUP

# Define a function, called 'trap_service_routine' for the signal trap
trap_service_routine ()
{
rm file_A                     
rm pipe1
rm pipe2
echo -e "proc_B_s has been killed"
exit 0
}

#Create the pipe if it does not alredy exist
if [ ! -p pipe2 ]
then
mkfifo pipe2
else echo "Pipe2 already exists"
fi

#Wait for last message
while [ "$input" != "Last message!" ]
do
#Read input from pipe1
read input < pipe1

#Cat file_A into pipe2
cat file_A > pipe2
sleep 1
done

#Busy wait loop to keep the process alive
while true 
do
(( x++ ))
done

exit 0
