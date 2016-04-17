#!/bin/bash
#Create the pipe if it doesn't already exist

if [ ! -p pipe1 ]
then 
mkfifo pipe1
else echo "pipe already exists"
fi

#Send a message 3 times
for (( x=1; x<4; x++ ))
do
echo "Hello from $0" > pipe1
sleep 1
done

#Send the last message and remove the pipe
echo "Last message!" > pipe1
sleep 1 
rm pipe1

exit 0
