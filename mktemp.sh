#!/bin/bash
# 创建文件夹模板，在模板文件夹中添加:
# 1.Makefile
# 2.bin/

echo "参数数目:$#"
if [ $# -ne 1 ]; then
	echo "Usage: ./mktemp.sh character-number"
	exit 0
fi

ch=ch$1

mkdir -p ${ch}
if [ $? -ne 0 ]; then
	echo "mkdir ${ch} failed.[$?]"
	exit $?
fi

cp mvbin.sh ${ch} 
if [ $? -ne 0 ]; then
	echo "cp mvbin.sh failed.[$?]"
fi
cd ${ch}
mkdir -p bin/
if [ $? -ne 0 ]; then
	echo "mkdir bin/ failed.[$?]"
	exit $?
fi

touch Makefile
echo "创建成功"


