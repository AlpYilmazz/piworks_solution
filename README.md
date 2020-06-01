# Solution to P.I Works interview question

Compile: <br>
g++ -std=c++14 -Wall -Werror piworks.cpp -o piworks

Run: <br>
piworks [input file name]

e.g. -> piworks input.txt <br>
default file name to be used when no file name is given is "input.txt"

## Method Options
- index:
  - n: total number of integers in the input
  - maxn: biggest integer in the input
  - nd: number of different integers in the input

- Checking primality:
  1. For every integer present in the input, store the primality by checking them individually <br>
     O(nd * sqrt(maxn))
  2. Use sieve method to get the primality of every integer from 0 to maxn <br>
     O(maxn * loglog(maxn))

If nd is low, then method 1 would be better <br>
As nd gets closer to maxn, method 2 gets better and better <br>

 -> I have implemented both, but chosen method 1 to use

## General Explanations
- Infinity in the program is chosen to be 1e9
- Assumption: no negative integers in the input
- time complexity: O(n + nd * sqrt(maxn))
- space complexity: O(n)
