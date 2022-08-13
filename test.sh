#!/bin/bash
# A simple test script, for unit-testing of the matching function of the Master Mind program

# function to check output from running $cmd against expected output
check () {
    #echo "Cmd: $cmd"
    echo "Output: \n$out"
    echo "Expected: \n$exp"
    if [ "$out" = "$exp" ]
    then echo ".. OK"
	 ok=$(( $ok + 1))
    else echo "** WRONG"
	 ret=1
    fi
    n=$(( $n + 1))
}

# name of the application to run
cw=crack_rot
# return code; 0 = ok, anything else is an error
ret=0
ok=0
n=0

# -------------------------------------------------------
# a sequence of unit tests

#filein="filein.txt"
#cmd="./crack_rot -f filein.txt  | egrep '^Key'"
out="`./crack_rot -f filein.txt  | egrep '^Key'`"
# echo "$out"
exp=$(cat <<EOS
Key: 13
Key: 13
Key: 13
Key: 19
Key: 6
Key: 9
EOS
)
check



# return status code (0 for ok, 1 for not)
echo "$ok of $n tests are OK"
exit $ret

