
#! /bin/bash
# file_stat.sh
# finds and displays various information about the home directory 
# John Gleeson 12162213
# 06/03/14

echo
#finds the biggest file in the home directory
biggest_file=$(ls -la ~ | sort -grk5 | head -1)

#prints the name of the biggest file to screen
echo "The biggest file in the home directory is:"
echo  $biggest_file 
echo

# finds the no. of files (including hidden files) in the home directory
total_no_files=$(ls -la ~ | wc -l)

# prints the number of files in the home directory to screen
echo "There are number of files in the home dirctory is: $total_no_files"
echo

# finds the number of directories in the home folder (including hidden ones)
total_directories=$(ls -la ~ | grep "^d" | wc -l)

# prints the number oh directories in the home folder to screen
echo "The number of directories in the home folder is: $total_directories"
echo

# finds the number of free blocks in the home directory
free_blocks=$(df ~ | sort -grk4 | awk '{print$4}' | head -1)

# prints the number of free blocks in the home directory to screen
echo "The number of free blocks in the home directory is: $free_blocks"
echo

# finds the percentage of used space in the home directory
percentage_used=$(df ~ | sort -grk4 | awk '{print$5}' | head -1)

# prints the percentage of used space in the home directory to screen
echo "The percentage of home folder space used is: $percentage_used"
echo

# begin of if statement
if (( $free_blocks<5000000 ))
then
echo "Disk space is running low,  $percentage_used used"
fi #end of if statement

exit 0
