#!/bin/bash
set -e

# to have a . as the decimal separator
LC_NUMERIC="en_US.UTF-8"  

if  [[ $# -eq 1 ]];  then
  n=$1
else
  read n
fi

# remove temp info header and put hex data in correct order
m=${n:10:2}${n:8:2}${n:6:2}${n:4:2}

# get the exponent
e=$(( ((0x${m} >> 24) ^ 0xff) + 1 ))

# get the value
v=$(( 0x${m} & 0x0000ffff))

# get the service
s=${n:2:2}${n:0:2}

num=$(echo "scale=$e; $v/10^$e " | bc)

time=$(date +'%Y-%m-%d %H:%M:%S.%N')

echo "$time $s $num"

