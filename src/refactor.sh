#! /bin/bash

REPLACE_FROM=$1
REPLACE_TO=$2

FILES=$(find . -name "*.cpp")

for f in $FILES
do
	sed -i  "s/${REPLACE_FROM}/${REPLACE_TO}/g" ${f}
done


FILES=$(find . -name "*.h")

for f in $FILES
do
	sed -i  "s/${REPLACE_FROM}/${REPLACE_TO}/g" ${f}
done
