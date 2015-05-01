/**** compile command

gcc primality_fermat.c -o primality_fermat -v -lgmp

******/

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



/**
 * NONDETERMINISTIC: Fermat's Little Theorem for determining primality
 * Built to support very large numbers
 *
 * Determines if number is prime or not.
 * Returns 1 if number is prime 
 * Returns 0 if number is not prime
 */

int is_prime (mpz_t num) {
    mpz_t temp;
    mpz_init(temp);

    mpz_set_ui(temp, 1);
    if (mpz_cmp(num, temp) <= 0) return 0;

    mpz_t r;
    mpz_init(r);

    mpz_set_ui(temp, 2);
    mpz_mod(r, num, temp);

    mpz_set_ui(temp, 0);
    if (mpz_cmp(r, temp) == 0) return 0; // found a divisor

    mpz_t rop;
    mpz_init(rop);

    mpz_t counter;
    mpz_init(counter);

    mpz_t rop2;
    mpz_init(rop2);

    mpz_set_ui(counter, 2);

    mpz_set_ui(temp, 1);

//    gmp_printf ("0/ num: %Zd\n", num);
    mpz_sub(rop, num, temp);
//    gmp_printf ("1/ rop number (n - 1): %Zd\n", rop);

    while (1) {
        if (mpz_cmp(counter, rop) >= 0) break;
        mpz_powm(rop2, counter, rop, num);
 //       gmp_printf ("2/ counter: %Zd, rop mod: %Zd\n", counter, rop2);

        // if mod is not equal to 1 (which is the value of temp) then composite
        // this is a confusing conditional because mzp_cmp returns 0 when strings are equal
        // and the value of 'temp' is 1. So it basically checks if r = 1
        if (mpz_cmp(rop2, temp) != 0) {
//            gmp_printf ("composite number: %Zd\n", num);
            return 0;
        }

        mpz_add(counter, counter, temp);
    }

    return 1;
}


/**
 * Loop for prime number generation
 */
void gen_primes (mpz_t max_number) {
    mpz_t current;
    mpz_init(current);

    mpz_t temp;
    mpz_init(temp);
    mpz_set_ui(temp, 1);
    unsigned long long primes_found = 0;

    while (1) {
        if (is_prime(current) == 1) {
//            gmp_printf ("prime found: %Zd\n", current);
            primes_found++;
        }

        mpz_add(current, current, temp);

        if (mpz_cmp(current, max_number) > 0) break;
    }

    mpz_clear(current);

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


