# build an executable named myprog from myprog.c
all: 
	gcc -g -Wall -o prime_classical_1 prime_classical_1.c
	gcc -g -Wall -o prime_classical_2 prime_classical_2.c
	gcc -g -Wall -o prime_classical_3 prime_classical_3.c
	gcc -g -Wall -o prime_wilson prime_wilson.c
	gcc -g -Wall -o prime_sieve prime_sieve.c
	gcc -g -Wall -o prime_fermat prime_fermat.c -lgmp
	
clean:
	$(RM) classical_1 classical_2
