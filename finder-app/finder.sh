#!/bin/sh
# @brief finder.sh - simple script to find the number of files in the directory *filesdir*
#  and the number of matches for the *searchstr*.

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
fi

# Determine X, the nuber of files, and Y, the number of files with *searchstr*.
X=`find ${filesdir} | wc -l`
Y=`grep -r "${searchstr}" ${filesdir} | wc -l`

# Report the results
echo "The number of files are ${X} and the number of matching lines are ${Y}"
