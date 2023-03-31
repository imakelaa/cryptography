
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include "randstate.h"
#include "numtheory.h"
#include <stdlib.h>

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {

    mpz_t cond1, cond2, p_2, p_minus1, q_minus1;
    mpz_inits(cond1, cond2, p_2, p_minus1, q_minus1, NULL);

    uint64_t lower = nbits / 5;

    uint64_t pbits = (random() % lower + lower);

    uint64_t qbits = nbits - (pbits * 2);

    do {
        make_prime(p, pbits, iters);
        make_prime(q, qbits, iters);

        
        mpz_sub_ui(q_minus1, q, 1);
        
        mpz_sub_ui(p_minus1, p, 1);

        mpz_mod(cond1, q_minus1, p);
        mpz_mod(cond2, p_minus1, q);

        //n = p^q
        mpz_mul(p_2, p, p); //p square = p*p
        mpz_mul(n, p_2, q);

    } while (mpz_cmp_ui(cond1, 0) == 0 || mpz_cmp_ui(cond2, 0) == 0);

    mpz_clears(cond1, cond2, p_2, p_minus1, q_minus1, NULL);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t p_minus1, q_minus1, mul, gcd_pq, lcm, n;
    mpz_inits(p_minus1, q_minus1, mul, gcd_pq, lcm, n, NULL);

    mpz_sub_ui(p_minus1, p, 1); // p-1
    mpz_sub_ui(q_minus1, q, 1); //q-1

    gcd(gcd_pq, p_minus1, q_minus1); //gcd(p-1, q-1)
    mpz_mul(mul, p_minus1, q_minus1); //(p-1)*(q-1)

    mpz_fdiv_q(lcm, mul, gcd_pq); //lcm = mul/ gcd

    mpz_mul(pq, p, q);

    //n = p^2 q
    mpz_mul(n, pq, p);

    mod_inverse(d, n, lcm);

    mpz_clears(p_minus1, q_minus1, mul, gcd_pq, lcm, n, NULL);
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n %s\n", n, username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n %Zx\n", pq, d);
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    mpz_t var, store, m, sqrt_n;
    mpz_inits(var, store, m, sqrt_n, NULL);

    uint64_t k;
    uint64_t j;

    mpz_sqrt(sqrt_n, n);
    k = (mpz_sizeinbase(sqrt_n, 2) - 1) / 8;

    uint8_t *array = (uint8_t *) calloc(k, sizeof(uint8_t));

    //set 0th byte of array to 0xFF
    array[0] = 0xFF;


    while ((j = fread(array + 1, sizeof(uint8_t), k - 1, infile)) > 0) {

        mpz_import(m, j + 1, 1, sizeof(array[1]), 1, 0, array);

        ss_encrypt(store, m, n);

        gmp_fprintf(outfile, "%Zx\n", store); //print store to outfile
    }
    free(array);
    mpz_clears(var, store, m, sqrt_n, NULL);
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    mpz_t m, c, var;
    mpz_inits(m, c, var, NULL);

    uint64_t k, j;

    k = (mpz_sizeinbase(pq, 2) - 1) / 8;

    uint8_t *array = (uint8_t *) calloc(k, sizeof(uint8_t));

    //iterate over lines in infile
    while (gmp_fscanf(infile, "%Zx\n", c) != EOF) {
        //scan in hex string, and save in mpz_t c
        ss_decrypt(m, c, d, pq);

        //using mpz_export() convert m back to bytes
        mpz_export(array, &j, 1, sizeof(array[1]), 1, 0, m);

        fwrite(array + 1, sizeof(uint8_t), j - 1, outfile);
    }
    free(array);
    mpz_clears(m, c, var, NULL);
}
