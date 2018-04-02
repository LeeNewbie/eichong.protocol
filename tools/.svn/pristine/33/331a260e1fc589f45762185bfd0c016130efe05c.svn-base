#!/bin/bash



export filename='/root/epserver/gate/logs/EpGate/EpGateLog.log'

for i in $( seq 1 2499)
do
   grep "handleYc vsq==1,address:$i,value:" $filename |tail -1 
done
