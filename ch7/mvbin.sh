#! /bin/sh

echo "$0"
flag=0
list=`ls -a`
for i in ${list} 
do
	if [ ! "${i}" -ef "$0" ] && [ -x ${i} ] && [ ! -d ${i} ]; then
		echo "正在移动可执行文件${i}"
		mv ${i} bin/
		flag=1
	fi
done
if [ "${i}" == "0" ];then
	echo "没有需要移动的文件"
else
	echo "移动完成"
fi

