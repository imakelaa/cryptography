

#include "randstate.h"

#include <gmp.h>
#include <stdint.h>
#include <stdio.h>

gmp_randstate_t state; //extern means that I don't have to initialise; in randstate.h

void randstate_init(uint64_t seed) {
    //srandom(state, seed);
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

void randstate_clear(void) {
    gmp_randclear(state);
}
