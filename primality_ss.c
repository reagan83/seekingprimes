#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <math.h>
#include <gmp.h>

struct bignum_cs51 {
    unsigned long long large;
    mpz_t massive;
};

typedef struct bignum_cs51 bignum;

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
mpz_t read_file_data (const char *filename)
{
    FILE *file = fopen(filename, "r");
    mpz_t i;

    mpz_init(i);
    mpz_inp_str(i, file, 10);

    return i;
}



/**
 * NONDETERMINISTIC: Solovay-Strassen primality
 *
 * Determines if number is prime or not.
 * Returns 1 if number is prime 
 * Returns 0 if number is not prime
 */

int is_prime (unsigned long long num) {
    if (num <= 1) return 0;
    if (num % 2 == 0) return 0; // found a divisor

    unsigned long long k = 99999; // accuracy of the test


    for (unsigned long long i = 0; i < k; i++) {
        
    }




    unsigned long long sqrtnum = sqrt(num);

    /* accumulator to store factorial */
    unsigned long long acc = 1;

    for (unsigned long long i = 1; i < (num - 1); i++) {
        acc = i * acc;
        printf("i: %lld, acc: %lld\n", i, acc);
    }

    if (acc % num == 0) return 0; /* found a divisor! */

    return 1;
}


/**
 * Loop for prime number generation
 */
void gen_primes (unsigned long long max_number) {
    unsigned long long current = 3;
    unsigned long long find_to_max = max_number - 1;

    unsigned long long largest_prime = 3;
    unsigned long long primes_found;

    while (1) {
        if (current > find_to_max) {
            printf("finished!\n");
            printf("prime numbers found: %lld\n", primes_found);
            printf("largest prime: %lld\n", largest_prime);
            break;

        } else {
            if (is_prime(current) == 1) {
                printf ("prime found: %lld\n", current);
                primes_found++;
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

            unsigned long long w;
            w = fsize(argv[1]);
            printf("Finding primes up to: %Zd digits long.\n", (w - 1));

            if (w > LONG_LONG_MAX) {
                printf("Unsupported input size!\n");
            } else {
                mpz_init_set_str(max_number, read_file_data(argv[1]), 10);
                gen_primes(max_number);
                mpz_clear(max_number);
            }

        }

    }

    return 0;
}


