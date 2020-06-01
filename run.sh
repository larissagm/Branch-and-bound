#!/bin/bash

echo "--TSP Benchmark--"

make

k=1
for instance in instances/*; do
	echo $instance >> ./benchmark/bm.txt

	echo "Running $instance"
	echo "Instance $k of 8" 

	for i in {1..10}; do
		./tsp ${instance} >> ./benchmark/bm.txt
	done

	k=$(($k + 1))
done

echo "-" >> ./benchmark/bm.txt

echo "Benchmark completed."
