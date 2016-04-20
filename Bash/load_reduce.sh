
#! /bin/bash
# load_reduce.sh
# finds and displays various information about users and processes 
# John Gleeson 12162213
# 06/03/14

echo

# total numver of process running on the system
no_of_processes=$(ps -A | awk '{print$3}' | grep "^[0-9]" | wc -l)
echo "There are $no_of_processes processes running on the system"


# Make list of all users to file: names
ps aux | awk '$1 != "USER" {print $1}'>names

# Make a unique of users in file: uniq_list
sort names | uniq > uniq_list

# counts the lines in uniq_users ad displays the number of users on the system
no_of_users=$( cat uniq_list | wc -l)
echo "There are $no_of_users unique users."

# Make simple column titles
echo -e "\nProcs \t Users \n"

# Loop to read each user name and count number of entries

while read xuser
do
x=$( grep "$xuser" names | wc -l )
echo -e "$x \t $xuser"
done < uniq_list

echo 


# find the %CPU, user, PID and name of busiest process. save to busiest_process
ps -eo pcpu,user,pid,args | sort -grk1 | head -1 > busiest_process

# read vaious values from busiest_process
busiest_percent=$( awk '{printf("%2d",$1)}' busiest_process)
busiest_user=$( awk '{print$2}' busiest_process)
busiest_pid=$( awk '{print$3}' busiest_process)
busiest_command=$( awk '{print$4,$5}' busiest_process)

echo "The busiest user is $busiest_user"
echo

echo "The busiest command is $busiest_command"
echo

# if the busiest process uses more than 60% of the cpu, this if statement kills it
if (( $busiest_percent > 60 ))
then
kill $busiest_pid
echo "$busiest_command, PID $busiest_pid was using $busiest_percent% of the CPU"
echo "It had to be killed."
fi

# remove any temporary files
rm names uniq_list busiest_process
echo


x2="25%"
x_on_own=$(awk '{printf("%2d",$x2)}')
echo "$x_on_own"

exit 0

