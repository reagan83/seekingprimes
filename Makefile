# build an executable named myprog from myprog.c
all: classical_1.c classical_2.c
	gcc -g -Wall -o classical_1 classical_1.c
	gcc -g -Wall -o classical_2 classical_2.c
	gcc -g -Wall -o classical_3 classical_3.c
	gcc -g -Wall -o prime_wilson prime_wilson.c
	gcc -g -Wall -o prime_sieve prime_sieve.c
	
clean:
	$(RM) classical_1 classical_2
