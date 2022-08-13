#!/bin/bash
# a very simplistic script to identify the Linux OS distribution
# tested on: Raspbian, Ubuntu, CentOS

if [ -x /usr/bin/lsb_release ]
then os=`/usr/bin/lsb_release --id | sed -e 's/^Distributor ID:[\n\t]*\([A-Za-z0-9].*\).*$/\1/'`
     echo $os
else echo "unknown"
fi

