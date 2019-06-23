#!/bin/bash
set -e

# to have a . as the decimal separator
LC_NUMERIC="en_US.UTF-8"  

if  [[ $# -ne 1 ]];  then
  echo "usage $0 <bletemp>"
  echo "e.g. $0 0918450900fe"
  exit 1
fi

# remove temp info header and put hex data in correct order
n=$1
m=${n:10:2}${n:8:2}${n:6:2}${n:4:2}

# get the exponent
e=$(( ((0x${m} >> 24) ^ 0xff) + 1 ))

# get the value
v=$(( 0x${m} & 0x0000ffff))

echo "scale=$e; $v/10^$e " | bc

