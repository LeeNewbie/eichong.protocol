#!/bin/bash
function ergodic(){
for file in ` ls $1*.log*`
do
    if [ -d $1"/"$file ] #如果 file存在且是一个目录则为真
    then
          ergodic $1"/"$file
    else
          local path=$1"/"$file #得到文件的完整的目录
          local name=$file       #得到文件的名字
         grep 'invaild ep' $name|cut -b66-81 >> invalidEpCode.txt2
          echo $name
   fi

done
}
INIT_PATH="./"
rm -f *txt*
ergodic $INIT_PATH
sort invalidEpCode.txt2|uniq  >> invalidEpCode.txt
