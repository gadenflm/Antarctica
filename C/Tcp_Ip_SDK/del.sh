#! /bin/bash

del_count=0

function TRAVEL_ALL_FILE(){
    for file in ./*
    do
    if [ "${file##*.}x" == "ox"  -o "${file##*.}x" == "binx" -o "${file##*.}x" == "outx" -o "${file##*.}x" == "objx" ];then    #normal file
        echo ${file}
        rm -f ${file}
        ((del_count++))
    elif [ -d "${file}" ];then   #dir
        cd "${file}"
        TRAVEL_ALL_FILE
        cd ..
    fi
    done
}
 
TRAVEL_ALL_FILE
echo "There are $del_count files deleted in : ";pwd

