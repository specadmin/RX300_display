#!/bin/bash

filename="include/version.h"
if [ -f $filename ]; then
    ver=`sed -r 's/(.*)(\#define VERSION [0-9]+\.[0-9]+\.)([0-9]+)(.*)/echo "\1\2$((\3+1))"/ge' $filename` 
    sleep 0.1 
    echo $ver | tee $filename
else
    echo "#define VERSION 0.1.0" | tee $filename
fi