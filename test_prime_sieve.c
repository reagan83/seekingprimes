#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <math.h>

/**
 * Return file size
 */
off_t fsize (const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}


/**
 * Read file
 */
long long read_file_data (const char *filename)
{
    FILE *file = fopen(filename, "r");
    long long i = 0;

    fscanf (file, "%lld", &i);    
    while (!feof (file))
    {  
        printf ("Number ceiling: %lld\n", i);
        fscanf (file, "%lld", &i);      
    }

    fclose (file);

    return i;
}

/**
 * CLASSICAL ALGO 5: test for primality using Sieve of Eratosthenes
 * Findings:
 * This is not ideal to be ran against a range of numbers (over and over)
 * It would be better just to find a prime of a max number.
 *
 * Determines if number is prime or not.
 * Returns 1 if number is prime 
 * Returns 0 if number is not prime
 */
int is_prime (unsigned long long num) {
    if (num <= 1) return 0;
    if (num % 2 == 0) return 0; // found a divisor

    unsigned long long *sieve;

    unsigned long long idx = 0;

    sieve = (unsigned long long *)malloc(sizeof(unsigned long long)* (num/2));

    // populate list of nums
    for (unsigned long long i = 3; i < num; i+=2) {
        printf("added number to list. %lld\n", i);
        sieve[idx] = i;
        idx++;
    }

    printf("all added!\n");

    // iterate through list removing items
    for (unsigned long long i = 0; i < (sizeof(sieve) * (num/2)); i++) {
        if (sieve[i] % num == 0) {
            sieve[i] = 0;
        }
    }


    // iterate through list removing items
    for (unsigned long long i = 0; i < (sizeof(sieve) * (num/2)); i++) {
        if (sieve[i] != 0) {
            free(sieve);
            return 1; /* found a prime! */
        }
    }

    free(sieve);
    return 0;
}


/**
 * Loop for prime number generation
 */
void gen_primes (long long max_number) {
    long long current = 3;
    long long find_to_max = max_number - 1;

    long long largest_prime = 3;

/*
 * It doesn't make sense for us to loop through numbers to find
 * all primes. Rather, we will take an input number and find the largest
 * prime within it using the Sieve of Eratosthenes.
*/

/*

    while (1) {
        if (current > find_to_max) {
            printf("finished!\n");
            printf("largest prime: %lld\n", largest_prime);
            break;

        } else {
            if (is_prime(current) == 1) {
 //               printf ("prime found: %lld\n", current);
                largest_prime = current;
            }
        }

        current = current + 1;
    }
*/

    while (1) {
        if (current > find_to_max) {
            printf("finished!\n");
            printf("largest prime: %lld\n", largest_prime);
            break;
        } else {
            if (is_prime(current) == 1) {
                largest_prime = current;
            }
        }

        current = current + 1;

    }
}


/**
 * Main program
 */
int main (int argc, char *argv[]) {
    char input[19];

    if (argc != 2) {
        printf("Usage: %s max_number\n.", argv[0]);
        printf("Supports numbers up to: %lld\n", LONG_LONG_MAX);

        return -1;
    } else {
        FILE *fd = fopen (argv[1], "r");

        if (fd == 0) {
            printf("Unable to open file.\n");
        } else {

            long long w;
            w = fsize(argv[1]);
            printf("Finding primes up to: %lld digits long.\n", (w - 1));

            if (w > LONG_LONG_MAX) {
                printf("Unsupported input size!\n");
            } else {
                long long max_number;
                max_number = read_file_data(argv[1]);
                gen_primes(max_number);
            }

        }

    }

    return 0;
}


