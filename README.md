# Seeking Primes: Why are prime numbers so hard to find?

This project explores implementing various algorithims for determining the
primality of a number.

## Pre-requisites ##
1) Your system will need to be equipped with 'gcc'
2) This project relies on the GMP library for processing large numbers. You
will need to download this library and install it on your system.

## Compiling ##
To compile the source files, open the code/ folder and run: `make`

## Input Files ##
Because each algorithm will read the input number from a file (to support
very large numbers later), a set of test files have been included that are
all prefixed with "input" followed by the number of digits and a brief
description.

For example:
    - Filename: input_3_tiny will contain a number with 3 digits.

## Executing ##

All algorithms accept 1 command line argument which is an input file that
should contain a max number for finding primes up to.

Here are some sample commands to try the algorithms:

./prime_classical_1 input_1_base
./prime_classical_3 input_3_base
./prime_wilson input_2_wilson

If you are interested in timing the performance of these algorithms, use the
`time` command and compare.  For example:

time ./prime_classical_1 input_3_tiny
time ./prime_classical_3 input_3_tiny

Should produce drastically different timing results. Try it!

