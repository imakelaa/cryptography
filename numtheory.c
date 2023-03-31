
#include "numtheory.h"

#include "randstate.h"

#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t, b_cp, temp2;
    mpz_inits(t, b_cp, temp2, NULL);
    mpz_set(b_cp, b); // copy of b
    mpz_set(temp2, a); // copy of a; temp2 = a
    while (mpz_cmp_ui(b_cp, 0) != 0) {

        mpz_set(t, b_cp);

        mpz_mod(b_cp, temp2, t);
        mpz_set(temp2, t);
    }
    mpz_set(g, temp2);
    mpz_clears(t, b_cp, temp2, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {

    mpz_t q, r, rp, t, tp, r_cp, mul_qrp, t_cp, mul_qtp;
    mpz_inits(q, r, rp, t, tp, r_cp, mul_qrp, t_cp, mul_qtp, NULL);

    mpz_set(r, n);
    mpz_set(rp, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(tp, 1);

    while (mpz_cmp_ui(rp, 0) != 0) { 
        mpz_fdiv_q(q, r, rp);

        mpz_set(r_cp, r);
        mpz_set(r, rp);
        mpz_mul(mul_qrp, q, rp);
        mpz_sub(rp, r_cp, mul_qrp);

        mpz_set(t_cp, t);
        mpz_set(t, tp);
        mpz_mul(mul_qtp, q, tp);
        mpz_sub(tp, t_cp, mul_qtp);
    }

    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0);
        return;
    }

    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(o, t);
    mpz_clears(q, r, rp, t, tp, r_cp, mul_qrp, t_cp, mul_qtp, NULL);
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t a_cp, d_cp, temp, v, p;
    mpz_inits(a_cp, d_cp, temp, v, p, NULL);

    mpz_set_ui(v, 1);
    mpz_set(a_cp, a);
    mpz_set(p, a_cp);

    mpz_set(d_cp, d);

    while (mpz_cmp_ui(d_cp, 0) > 0) {
        if mpz_odd_p (d_cp) { 

            mpz_mul(v, v, p); 
            mpz_mod(v, v, n); 
        }
        
        mpz_mul(p, p, p); 
        mpz_mod(p, p, n); 

        mpz_fdiv_q_ui(d_cp, d_cp, 2);
    }
    mpz_set(o, v); 
    mpz_clears(a_cp, d_cp, temp, v, p, NULL);
    return;
}

bool is_prime(const mpz_t n, uint64_t iters) {
    mpz_t j, y, a, s, r, n_cp, n_sub1, n_sub2, s_minus1, two, n_minus2;
    mpz_inits(j, y, a, s, r, n_cp, n_sub1, n_sub2, s_minus1, two, n_minus2, NULL);
    uint64_t i;

    if (mpz_cmp_ui(n, 0) == 0 || mpz_cmp_ui(n, 1) == 0) {
        return false;
    } else if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        return true;
    }

    mpz_set(n_cp, n);

    mpz_sub_ui(n_sub1, n_cp, 1);

    mpz_sub_ui(n_sub2, n_cp, 1);

    mpz_set_ui(s, 0);

    mpz_sub_ui(n_minus2, n_cp, 3);

    mpz_set_ui(two, 2);

    while (mpz_even_p(n_sub1)) {
        mpz_fdiv_q_ui(n_sub1, n_sub1, 2);
        mpz_add_ui(s, s, 1);
    }
    mpz_set(r, n_sub1); 

    mpz_sub_ui(s_minus1, s, 1);

    for (i = 1; i < iters; i++) { 

        mpz_urandomm(a, state, n_minus2);
        mpz_add_ui(a, a, 2);

        pow_mod(y, a, r, n);

        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, n_sub2) != 0)) {
            mpz_set_ui(j, 1);

            while ((mpz_cmp(j, s_minus1) <= 0) && (mpz_cmp(y, n_sub2) != 0)) {

                pow_mod(y, y, two, n);

                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(j, y, a, s, r, n_cp, n_sub1, n_sub2, s_minus1, two, n_minus2, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }

            if (mpz_cmp(y, n_sub2) != 0) {
                mpz_clears(j, y, a, s, r, n_cp, n_sub1, n_sub2, s_minus1, two, n_minus2, NULL);
                return false;
            }
        } //if loop end
    } //for loop end

    mpz_clears(j, y, a, s, r, n_cp, n_sub1, n_sub2, s_minus1, two, n_minus2, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    while (is_prime(p, iters) == false) {
        mpz_urandomb(p, state, bits);
        mpz_setbit(p, bits);
    }
}
