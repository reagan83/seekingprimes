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
 * Returns max(prime number) if prime is found 
 * Returns 0 if number is not prime
 */
unsigned long long is_prime (unsigned long long num, int debug) {
    if (num <= 1) return 0;

    unsigned long long *sieve;
    unsigned long long idx = 0;
    unsigned long long size = 0;

    sieve = (unsigned long long *)malloc(sizeof(unsigned long long)* (num/2));

    // populate list of nums
    for (unsigned long long i = 1; i <= (num); i+=2) {
//        printf("added number to list. %lld\n", i);
        sieve[idx] = i;
        size++;
        idx++;
    }
//    printf("sieve size: %lld\n", size);

    // iterate through list removing items
    for (unsigned long long i = 0; i < idx; i++) {
        if (sieve[i] < 2) continue; // ignore item

//        printf("working on %lld\n", sieve[i]);

        for (unsigned long long j = (i+1); j < idx; j++) {
            if (sieve[j] == 0) continue; // ignore item

            if (sieve[j] % sieve[i] == 0) {
//                printf("removing %lld from list\n", sieve[j]);
                sieve[j] = 0;
                size--;
            }
        }

//        printf("sieve size: %lld\n", size);
    }

    unsigned long long temp = 0;
    // iterate through list removing items
    for (unsigned long long i = (idx - 1); i > 0; i--) {
        if (sieve[i] != 0) {
//            printf("i: %lld\n", i);

            if (debug)
                printf("found a prime!: %lld\n", sieve[i]);

            temp = sieve[i]; // found a prime!
            break;
        }
    }

    free(sieve);
    return temp;
}


/**
 * Loop for prime number generation
 */
void gen_primes (unsigned long long max_number, int debug) {
    unsigned long long current = (max_number - 1);

/*
 * It doesn't make sense for us to loop through numbers to find
 * all primes as done in other algo implementations.
 * Rather, we will take an input number and find the largest
 * prime within it using the Sieve of Eratosthenes.
*/

    printf("finding largest prime using sieve\n");
    printf("largest prime: %lld\n", is_prime(current, debug));

}


/**
 * Main program
 */
int main (int argc, char *argv[]) {

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

                int debug = 0;
                // if input is small enough, setup to print output for debugging                
                if (w < 5)
                    debug = 1;

                gen_primes(max_number, debug);
            }

        }

    }

    return 0;
}


