/* implements universal hashing using random bit-vectors in x */
/* assumes number of elements in x is at least BITS_PER_CHAR * MAX_STRING_SIZE */

/* Source: http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=%28CategoryAlgorithmNotes%29*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>

//#include "dict.h"

#define MULTIPLIER (97)

static unsigned long
hash_function(const char *s)
{
    unsigned const char *us;
    unsigned long h;

    h = 0;

    for(us = (unsigned const char *) s; *us; us++) {
        h = h * MULTIPLIER + *us;
    }

    return h;
}