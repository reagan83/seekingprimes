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
        printf ("%lld", i);
        fscanf (file, "%lld", &i);      
    }

    fclose (file);

    return i;
}



/**
 * CLASSICAL ALGO 2: test for primes up to sqrt(n)
 *
 * Determines if number is prime or not.
 * Returns 1 if number is prime 
 * Returns 0 if number is not prime
 */
int is_prime (long long num) {
    long long sqrtnum = sqrt(num);

    for (long long i = 2; i < sqrtnum; i++) {
        if (num % i == 0) /* found a divisor! */
            return 0;
    }

    return 1;
}


/**
 * Loop for prime number generation
 */
void gen_primes (long long max_number) {
    long long current = 3;
    long long find_to_max = max_number - 1;

    long long largest_prime = 3;

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
            printf("file size: %lld", w);

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


