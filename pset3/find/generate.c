/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    /* return an error message and exit the program if the user
    does not enter either 2 or 3 arguments at the command line */
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    /* take argv from the user command line input, and
    convert it from a string to an integer, called "n" */
    int n = atoi(argv[1]);

    /* if the user entered 3 arguments at the command line,
    execute the srand48 function using argv[2] as the seed
    otherwise use the time as the seed (and do not store it) */
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    /* set a counter equal to zero and iterate through it
    n times (n being argv[1] - run each number through
    the drand48 function until you reach n or the limit,
    printing each "random" number on a new line */
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
