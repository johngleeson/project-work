#!/bin/bash
#Create the pipe if it does not alredy exist

if [ ! -p pipe1 ]
then
mkfifo pipe1
else echo "pipe already exists"
fi

#Wait for last message
while [ "$input" != "Last message!" ]

#read the pipe
do
read input < pipe1
echo $input
sleep 1
done

echo I am finished!
exit 0