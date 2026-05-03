#!/bin/sh
# @brief finder.sh - simple script to find the number of files in the directory *filesdir*
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
    echo "Usage: finder.sh <filesdir> <searchstr>"
    echo "   Ex: finder.sh /tmp/aesd/assignment1 linux"
    exit 1
else 
    filesdir=$1
    searchstr=$2
    if [ $DEBUG ]
    then
      echo "DEBUG: filesdir=${filesdir}"
      echo "DEBUG: searchstr=${searchstr}"
    fi

    if [ -d "$filesdir" ]
    then
        if [ $DEBUG ]
        then
            echo "DEBUG: filesdir, ${filesdir}, exists!"
        fi
    else
        echo "\nERROR: ${filesdir} does not represent a directory on the filesystem\n"
        exit 1 
    fi
fi

# Determine X, the nuber of files, and Y, the number of files with *searchstr*.
X=`find ${filesdir} -type f | wc -l`
Y=`grep -r "${searchstr}" ${filesdir} | wc -l`

# Report the results
echo "The number of files are ${X} and the number of matching lines are ${Y}"
