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

    // create max fermat tests variable & init to 18
    mpz_t max_fermat_tests;
    mpz_init(max_fermat_tests);
    mpz_set_ui(max_fermat_tests, 18);

    // create zero variable
    mpz_t zero;
    mpz_init(zero);
    mpz_set_ui(zero, 0);

    // create one variable
    mpz_t one;
    mpz_init(one);
    mpz_set_ui(one, 1);

    // create two variable
    mpz_t two;
    mpz_init(two);
    mpz_set_ui(two, 2);

    // if number is less than or equal to 1, exit
    if (mpz_cmp(num, one) <= 0) return 0;

    // if number is 2, then is prime, exit
    if (mpz_cmp(num, two) == 0) return 1; // number is 2 and prime

    // if number is num % 2 = 0, then exit
    mpz_t rop;
    mpz_init(rop);
    mpz_mod(rop, num, two);

    if (mpz_cmp(rop, zero) == 0) return 0; // found a divisor

    // setup num - 1 variable
    mpz_t num_minus_one;
    mpz_init(num_minus_one);
    mpz_sub(num_minus_one, num, one);

    // initialize random state for use in fermat's algo
    gmp_randstate_t rstate;
    gmp_randinit_default(rstate);
    gmp_randseed(rstate, num);

    // setup counter variable
    mpz_t counter;
    mpz_init(counter);
    mpz_set_ui(counter, 2);

    // store loop state values in rop2
    mpz_t rop2;
    mpz_init(rop2);


    while (1) {
        // loop until max tests has satisfied
        if (mpz_cmp(counter, max_fermat_tests) >= 0) break;
        printf("in test\n");

        // generate random number and store in 'rop'
        mpz_urandomm(rop, rstate, num);

        if (mpz_cmp(rop, zero) == 0)
            mpz_add(rop, rop, one); // add +1 to this number in case 0 is selected.

        mpz_powm(rop2, rop, num_minus_one, num);

        // if mod is not equal to 1 then composite
        // this is a confusing conditional because mzp_cmp returns 0
        // when strings are equal
        // t basically checks if r = 1
        if (mpz_cmp(rop2, one) != 0) {
            gmp_printf ("==== composite number: %Zd\n", num);
            return 0;
        }

        mpz_add(counter, counter, one);
    }

    return 1;
}


/**
 * Loop for prime number finding: starting at 1->max_number
 */
void gen_primes (mpz_t max_number, int debug, int check_only) {

    // check max number only, or all numbers up to max_number?
    mpz_t current;
    mpz_init(current);

    if (check_only == 1) // only check max number for primality
        mpz_set(current, max_number);
    else
        mpz_set_ui(current, 1);

    mpz_t one;
    mpz_init(one);
    mpz_set_ui(one, 1);

    unsigned long long primes_found = 0;

    while (1) {
        if (is_prime(current) == 1) {
            if (debug || check_only)
                gmp_printf ("possible prime found: %Zd\n", current);
            primes_found++;
        }

        mpz_add(current, current, one);

        if (mpz_cmp(current, max_number) > 0) break;
    }

    mpz_clear(current);

    printf("finished!\n");
    printf("possible primes found: %lld\n", primes_found);
}


/**
 * Main program
 */
int main (int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s max_number_file check_only\n", argv[0]);
        printf("  max_number_file is a file only with a number in it.\n");
        printf("  check_only expects a 0 or 1. \n");
        printf("    > 0 checks all numbers up to max_number_file for primality.\n");
        printf("    > 1 checks only number in max_number_file for primality.\n");
        printf("Supports numbers up to ALMOST INFINITE!\n");

        return -1;
    } else {
        FILE *fd = fopen (argv[1], "r");
        int check_only = atoi(argv[2]);

        if (fd == 0) {
            printf("Unable to open file.\n");
        } else {

            // determine number length
            unsigned long long w;
            w = fsize(argv[1]);
            printf("Finding primes up to: %lld digits long.\n", (w - 1));

            // support REALLY LARGE numbers
            mpz_t number;
            mpz_init_set_str(number, read_file_data(argv[1]), 10);

            int debug = 0;
            // if input is small enough, setup to print output for debugging                
            if (w < 5)
                debug = 1;

            gen_primes(number, debug, check_only);
            mpz_clear(number);

        }

    }

    return 0;
}


