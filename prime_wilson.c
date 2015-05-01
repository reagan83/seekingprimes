#include <stdio.h>
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
 * CLASSICAL ALGO 4: test for primality using Wilson's Theorem
 * Findings:
 * This algorithm caps out for uns long long at input of 20 because
 * the factorial of that number will overflow at input of 21. Also,
 * isolated performance tests show this algorithm to be slower than
 * the algorithm defined in classical_3 which can support a much
 * larger ceiling.
 *
 * Determines if number is prime or not.
 * Returns 1 if number is prime 
 * Returns 0 if number is not prime
 */
int is_prime (unsigned long long num) {
    if (num <= 1) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0; // found a divisor

    /* accumulator to store factorial */
    unsigned long long acc = 1;

    for (unsigned long long i = 1; i < (num - 1); i++) {
        acc = i * acc;
//        printf("i: %lld, acc: %lld\n", i, acc);
    }

    if (acc % num == 0) return 0; /* found a divisor! */

    return 1;
}


/**
 * Loop for prime number generation
 */
void gen_primes (unsigned long long max_number, int debug) {
    unsigned long long current = 1;
    unsigned long long find_to_max = max_number - 1;

    unsigned long long largest_prime = 1;
    unsigned long long primes_found = 0;

    while (1) {
        if (current > find_to_max) {
            printf("finished!\n");
            printf("prime numbers found: %lld\n", primes_found);
            printf("largest prime: %lld\n", largest_prime);
            break;

        } else {
            if (is_prime(current) == 1) {
                if (debug)
                    printf ("prime found: %lld\n", current);
                largest_prime = current;
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


