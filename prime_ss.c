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
    // create max fermat tests variable & init to 18
    mpz_t max_ss_tests;
    mpz_init(max_ss_tests);
    mpz_set_ui(max_ss_tests, 999);

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

    // setup stateful loop exponent variable
    mpz_t exponent;
    mpz_init(exponent);
    mpz_set_ui(exponent, 0);

    // setup stateful loop results variable: resultsleft
    mpz_t resultsleft;
    mpz_init(resultsleft);

    // setup stateful loop results variable: resultsright
    mpz_t resultsright;
    mpz_init(resultsright);

    // setup stateful loop results for jacobi
    mpz_t j;
    mpz_init(j);

    // loop through k times to determine accuracy of test
    // if equal, number is possibly prime
//    gmp_printf ("=========================: num: %Zd\n", num);
    while (1) {
        // loop until max tests has satisfied
        if (mpz_cmp(counter, max_ss_tests) >= 0) break;

        // generate random number and store in 'rop'
        mpz_urandomm(rop, rstate, num_minus_one);
        if (mpz_cmp(rop, zero) == 0)
            mpz_add(rop, rop, two); // make sure we add +1 to this number in case 0 is selected.
        if (mpz_cmp(rop, one) == 0)
            mpz_add(rop, rop, one); // make sure we add +1 to this number in case 0 is selected.

//        gmp_printf ("-- random number: %Zd\n", rop);

        // left side of SS test
        mpz_divexact(exponent, num_minus_one, two);
        mpz_powm(resultsleft, rop, exponent, num);
//        gmp_printf ("2/ left: %Zd\n", exponent, resultsleft);

        // right side of SS test
        int jacobi = mpz_jacobi(rop, num);
        if (jacobi == 0) {
//            gmp_printf ("composite found: (zero jacobi) %Zd\n", num);
            return 0;
        }

        mpz_set_ui(j, jacobi);

        mpz_mod(resultsright, resultsright, num);
//        gmp_printf ("3/ right: %Zd\n", resultsright);

        // if left != right, then number is a composite!
        if (mpz_cmp(resultsleft, resultsright) == 0) {
//            gmp_printf ("composite found: (num) %d\n", mpz_cmp(resultsleft, resultsright));
            return 0;
        }

        // number could be a prime! keep looping for accuracy
        mpz_add(counter, counter, one);

        // loop iterations larger than num
        if (mpz_cmp(counter, num) > 0) break;
    }


    // possibly prime
    return 1;
}


/**
 * Loop for prime number generation
 */
void gen_primes (mpz_t max_number) {
    mpz_t current;
    mpz_init(current);
    mpz_set_ui(current, 1);

    mpz_t one;
    mpz_init(one);
    mpz_set_ui(one, 1);

    unsigned long long primes_found = 0;

    while (1) {
        if (is_prime(current) == 1) {
            gmp_printf ("possible prime found: %Zd\n", current);
            primes_found++;
        }

        mpz_add(current, current, one);

        if (mpz_cmp(current, max_number) > 0) break;
    }

    printf("finished!\n");
    printf("prime numbers found: %lld\n", primes_found);
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

            // determine number length
            unsigned long long w;
            w = fsize(argv[1]);
            printf("Finding primes up to: %lld digits long.\n", (w - 1));

            // support REALLY LARGE numbers
            mpz_t number;

            mpz_init_set_str(number, read_file_data(argv[1]), 10);
            gen_primes(number);
            mpz_clear(number);
        }

    }

    return 0;
}


