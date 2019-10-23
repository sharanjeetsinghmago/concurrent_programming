#!/bin/bash

: <<'1'
echo "Hello World"

./mysort test_case14.txt -t 1 -o cppsoln.txt --alg==fj

sort -n test_case14.txt -o solution.txt

cmp --silent cppsoln.txt solution.txt && echo " Pass (5 pts )" || echo " Fail (0 pts )"


./mysort test_case14.txt -t 2 -o cppsoln.txt --alg==fj

sort -n test_case14.txt -o solution.txt

cmp --silent cppsoln.txt solution.txt && echo " Pass (5 pts )" || echo " Fail (0 pts )"


./mysort test_case14.txt -t 3 -o cppsoln.txt --alg==fj

sort -n test_case14.txt -o solution.txt

cmp --silent cppsoln.txt solution.txt && echo " Pass (5 pts )" || echo " Fail (0 pts )"


./mysort test_case14.txt -t 4 -o cppsoln.txt --alg==fj

sort -n test_case14.txt -o solution.txt

cmp --silent cppsoln.txt solution.txt && echo " Pass (5 pts )" || echo " Fail (0 pts )"


./mysort test_case14.txt -t 5 -o cppsoln.txt --alg==fj

sort -n test_case14.txt -o solution.txt

cmp --silent cppsoln.txt solution.txt && echo " Pass (5 pts )" || echo " Fail (0 pts )"

./mysort test_case7.txt -t 50 -o cppsoln.txt --alg==fj

sort -n test_case7.txt -o solution.txt

echo " test case 7 t50 fj"
cmp --silent cppsoln.txt solution.txt && echo " Pass (5 pts )" || echo " Fail (0 pts )"

./mysort test_case7.txt -t 50 -o cppsoln.txt --alg==bucket

sort -n test_case7.txt -o solution.txt

echo " test case 7 t50 bucket"
cmp --silent cppsoln.txt solution.txt && echo " Pass (5 pts )" || echo " Fail (0 pts )"

./mysort test_case15.txt -t 50 -o cppsoln.txt --alg==fj

sort -n test_case15.txt -o solution.txt

echo " test case 15 t50 fj"
cmp --silent cppsoln.txt solution.txt && echo " Pass (5 pts )" || echo " Fail (0 pts )"

1
./mysort test_case15.txt -t 50 -o cppsoln.txt --alg==bucket

sort -n test_case15.txt > solution.txt

echo " test case 15 t50 bucket"
cmp --silent cppsoln.txt solution.txt && echo " Pass (5 pts )" || echo " Fail (0 pts )"
