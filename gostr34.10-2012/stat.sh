#! /bin/bash

echo -n "" > measures

for (( i = 0; $i < 100; i = $i + 1 ))
do
	./timing 2>> measures
done

./stat

