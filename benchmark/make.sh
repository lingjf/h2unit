#!/bin/bash
#coding=utf-8

cd ../build && python singlify.py
cd ../benchmark

t_start1=$(date +%s)
for a in {1..100}; do
    g++ -Wall -g -std=c++11 -include ../h2unit.h -c -o test_1_${a}.o test.cpp
done
t_end1=$(date +%s)
let t_cost1=t_end1-t_start1
echo 1 hunit compile cost $t_cost1 seconds

t_start2=$(date +%s)
for a in {1..100}; do
    g++ -Wall -g -std=c++11 -include ../build/h2unit.hpp -c -o test_2_${a}.o test.cpp
done
t_end2=$(date +%s)
let t_cost2=t_end2-t_start2
echo 2 hunit compile cost $t_cost2 seconds

rm -rf *.o
