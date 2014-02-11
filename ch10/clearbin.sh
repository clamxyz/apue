#!/bin/sh

echo "$0"
flag=0
list=`ls *[^.sh]`
for i in ${list} 
do
	if [ ! "${i}" -ef "$0" ] && [ -x ${i} ] && [ ! -d ${i} ]; then
		echo "正在删除可执行文件${i}"
		rm ${i} 
		flag=1
	fi
done
if [ "${flag}" == "0" ];then
	echo "没有需要删除的文件"
else
	echo "删除完成"
fi

