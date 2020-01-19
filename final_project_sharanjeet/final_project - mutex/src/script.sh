#!/bin/bash

echo "Hello World"

make clean

make test

make

echo "________________________"

echo "Unit Testing"

./utest

echo "________________________"

echo "Normal execution"

./contree -t 10 -i 10

echo "________________________"

echo "Normal execution"

./contree -t 10 -i 100

echo "________________________"

echo "Normal execution"


./contree -t 10 -i 1000
echo "________________________"

echo "Normal execution"

./contree -t 50 -i 1000

echo "________________________"

echo "Normal execution"

./contree -t 100 -i 1000

echo "________________________"

echo "Low Contention"

./contree -t 100 -i 1000 -c 0

echo "________________________"

echo "High Contention"

./contree -t 100 -i 1000 -c 1

echo "________________________"
