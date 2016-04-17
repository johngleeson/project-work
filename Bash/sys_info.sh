#! /bin/bash
# sys_info.sh
# finds and displays various information about the system and user 
# John Gleeson 12162213
# 06/03/14

echo

# finds and displays the current user
user=$(whoami)
echo "Current user is $user"
echo

#finds and diplays the UID of the current user
user_ID=$(id -u)
echo "The user ID is $user_ID"
echo

#finds and displays the HOSTNAME
host_name=$(hostname)
echo "$host_name is the host name"
echo

# finds and displays the HOSTTYPE
host_type=$(uname -m)
echo "The type of host is :$host_type"
echo

# finds and displays the number of partitions on the disk
no_of_partitions=$(df | awk '{print$2}' | grep "[0-9]$" | wc -l)
echo "There are $no_of_partitions system partitions."
echo

# finds and displays the available disk space (human-readable) 
available_space=$(df -h ~ | awk '{print$4}' | grep "[0-9]")
echo "There is $available_space of disk space available."
echo

# finds and displays the % of disk space used
percentage_used=$(df -h ~ | awk '{print$5}' | grep "[0-9]")
echo "There is $percentage_used of disk space used."
echo

# finds and displays the CPU model
cpu_model=$(grep "model name" /proc/cpuinfo | head -1)
echo "The CPU $cpu_model"
echo

# finds and displays the CPU speed
cpu_speed=$(grep -i "Mhz" /proc/cpuinfo | head -1)
echo "$cpu_speed"
echo

# finds and displays the CPU cache size
cache_size=$(grep "cache size" /proc/cpuinfo | head -1)
echo "The CPU $cache_size"
echo

# finds and displays the total system memory
total_memory=$(grep MemTotal /proc/meminfo | awk '{print$2,$3}')
echo "Total system memory= $total_memory"
echo

# finds and displays the current free system memory
free_memory=$(grep MemFree /proc/meminfo | awk '{print$2,$3}')
echo "Free system memory= $free_memory"
echo

exit 0
