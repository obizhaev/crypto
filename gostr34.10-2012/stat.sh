#! /bin/bash

echo -n "" > measures

for (( j = 0; $j < 100; j = $j + 1 ))
do
	./timing 2>> measures
done

./stat

