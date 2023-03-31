
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include "randstate.h"
#include "numtheory.h"
#include "ss.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#define OPTIONS "b:i:n:d:s:vh"

int main(int argc, char **argv) {

    char *user = ""; //NULL;
    char *pv_path = "ss.priv"; //-> path
    char *pb_path = "ss.pub"; //->path
    FILE *pbfile;
    pbfile = fopen(pb_path, "w");
    FILE *pvfile;
    pvfile = fopen(pv_path, "w");
    uint64_t bits = 256; //minimum number of bits required for public modulus n
    uint64_t iters = 50;
    bool v = false;
    uint64_t seed = time(NULL);

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': bits = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'i': iters = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'n':
            pbfile = fopen(optarg, "w");
            if (pbfile == NULL) {
                printf("Error!");
                return 0;
            }
            break;
        case 'd':
            pvfile = fopen(optarg, "w");
            if (pvfile == NULL) {
                printf("Error!");
                return 0;
            }
            break;
        case 's': seed = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'v': v = true; break;
        case 'h':
            printf("SYNOPSIS\n\tGenerates an SS public/private key pair.\n\nUSAGE\n\t./keygen "
                   "[OPTIONS]\n\nOPTIONS\n\t-h              Display program help and usage.\n\t-v  "
                   "            Display verbose program output.\n\t-b bits         Minimum bits "
                   "needed for public key n (default: 256).\n\t-i iterations   Miller-Rabin "
                   "iterations for testing primes (default: 50).\n\t-n pbfile       Public key "
                   "file (default: ss.pub).\n\t-d pvfile       Private key file (default: "
                   "ss.priv).\n\t-s seed         Random seed for testing.\n");
            break;
        }
    }
    mpz_t p, q, n, pq, d;
    mpz_inits(p, q, n, pq, d, NULL);

    fchmod(fileno(pvfile), 0600);

    randstate_init(seed);

    ss_make_pub(p, q, n, bits, iters);

    ss_make_priv(d, pq, p, q);

    //get username
    getenv(user);
    user = getenv("USER");

    //write generated key from ss_make_pub() to public key file
    ss_write_pub(n, user, pbfile);

    //write generated key from ss_make_priv() to private key file
    ss_write_priv(pq, d, pvfile);

    if (v == true) {
        uint64_t p_bits = mpz_sizeinbase(p, 2);
        uint64_t q_bits = mpz_sizeinbase(q, 2);
        uint64_t n_bits = mpz_sizeinbase(n, 2);
        uint64_t pq_bits = mpz_sizeinbase(pq, 2);
        uint64_t d_bits = mpz_sizeinbase(d, 2);

        gmp_printf("user = %s\n", user);
        gmp_printf("p (%lu bits) = %Zd\n", p_bits, p);
        gmp_printf("q (%lu bits) = %Zd\n", q_bits, q);
        gmp_printf("n (%lu bits) = %Zd\n", n_bits, n);
        gmp_printf("pq (%lu bits) = %Zd\n", pq_bits, pq);
        gmp_printf("d (%lu bits) = %Zd\n", d_bits, d);
    }

    fclose(pvfile);
    fclose(pbfile);

    randstate_clear();
    mpz_clears(p, q, n, pq, d, NULL);
    return 0;
}
