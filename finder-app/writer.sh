#!/bin/sh
# @brief writestr.sh - simple script to find the number of files in the directory *filesdir*
#  and the number of matches for the *searchstr*.
#
# -Prints error messages if rguments are not at least the required 2
# -Prints error message if *filesdir* is not a directory
# -Prints a message with results if no errors
#

#DEBUG=

# Check for required args
if [ $# -lt 2 ]
then
    echo "Usage: writestr.sh <writefile> <writestr>"
    echo "   Ex: writestr.sh /tmp/aesd/assignment1/sample.txt ios"
    exit 1
else 
    writefile=$1
    writestr=$2
    if [ $DEBUG ]
    then
      echo "DEBUG: writefile=${writefile}"
      echo "DEBUG: writestr=${writestr}"
    fi
fi

base_path="${writefile%/*}"
#echo "DEBUG: split writefile: base_path=${base_path}"


# Create the path if needed
mkdir -p "${base_path}"
path_created=$?
if ! [ path_created ]
then 
    echo "ERROR: can not create required file path: ${base_path}"
    exit 1
fi

# Create the file *writefile* with the *writestr* value in it
touch "${writefile}"
file_created=$?
#echo "DEBUG: touch returned ${file_created}"

if [ file_created ]
then
    #echo "DEBUG: writefile ${writefile} created!"
    echo "${writestr}" > ${writefile}
    file_updated=$?
    if [ file_updated ]
    then
        #echo "DEBUG: ${writestr} written to ${writefile}(${file_updated})!"
        #cat "$writefile"
        exit 0
    else
        echo "ERROR: Could not write to ${writefile} (${file_updated})"
        exit 1
    fi
else
    echo "ERROR: Could not create ${writefile} (${file_created})"
    exit 1
fi
