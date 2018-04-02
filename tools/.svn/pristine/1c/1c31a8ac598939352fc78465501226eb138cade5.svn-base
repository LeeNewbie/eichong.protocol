#!/bin/bash



export filename='/root/epserver/gate/logs/EpGate/EpGateLog.log'

for i in $( seq 1 127)
do
   grep "handleOneBitYx vsq==1,address:$i,value:" $filename |tail -1 
done

for i in $( seq 1 127)
do
   grep "handleTwoBitYx vsq==1,address:$i,value:" $filename |tail -1 
done
