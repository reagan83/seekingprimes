#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
char *read_file_data (const char *filename)
{
    FILE *file = fopen(filename, "r");
    long size;
    char *buff;

    // find out total length of file contents
    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    if (!buffer) { // if memory was unable to allocate
        fclose(file);
        return NULL;
    }

    // read file in to buffer
    fread(buffer, size, 1, file);
    fclose(file);

    return buffer;
}


unsigned long long power(unsigned long long base, unsigned long long pow) {
    unsigned long long i = 0;

    while (i <= pow) {
        base = base * pow;
        i++;
    }

    return base;
}


/**
 * NONDETERMINISTIC: Solovay-Strassen primality
 *
 * Determines if number is prime or not.
 * Returns 1 if number is prime 
 * Returns 0 if number is not prime
 */

 /*

mpz_t calc_legendre(mpz_t a, mpz_t p) {
    if (mpz_cmp(a, 0) || mpz_cmp(a, 1)) {
        return 0;
    }

    mpz_t r;

    // 
    if (mpz_cmp(mpz_mod(r, a, 2), 0)) {

        mpz_t d;
        // divide a/2, set value to d
        mpz_divexact(d, a, 2);

        r = calc_legendre(d, p);
    }

    if (mpz_)
}

*/

int is_prime (mpz_t num) {
    if (num <= 1) return 0;
    if (num % 2 == 0) return 0; // found a divisor

    unsigned long long k = 99999; // accuracy of the test

    // loop through k times to determine accuracy of test
    // if equal, number is possibly prime
    srand(time(NULL));
    int r = 0;

    for (unsigned long long i = 0; i < k; i++) {
        r = rand() % num;

        // loop until random number beteween 2 - (n - 1) is reached
        while (1) {
            if (r >= 2) break;
            if (r % 2 != 0) break; // keep searching for odd numbers only
            r = rand() % num;
        }

        int jacobi = mpz_jacobi(r, num);

        r = power(r, (num - 1)/2);
        if ((r % num) != (jacobi % num)) { // either prime of Eular liar
            printf("composite found: %lld %lld\n", i, num);
            return 0;
        }
    }

    // possibly prime
    return 1;
}


/**
 * Loop for prime number generation
 */
void gen_primes (mpz_t max_number) {
    mpz_t current;
    unsigned long long primes_found;

    current = 0;

    while (current < max_number) {
        if (is_prime(current) == 1) {
            printf ("possible prime found: %lld\n", current);
            primes_found++;
        }

        current = current + 1;
    }

    printf("finished!\n");
    printf("prime numbers found: %lld\n", primes_found);
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
            printf("Finding primes up to: %lld digits long.\n", (w - 1));

            if (w > LONG_LONG_MAX) {
                printf("Unsupported input size!\n");
            } else {

                mpz_t number;

                mpz_init_set_str(number, read_file_data(argv[1]), 10);
                gen_primes(number);
                mpz_clear(number);
            }

        }

    }

    return 0;
}


