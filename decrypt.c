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

#define OPTIONS "i:o:n:vh"

int main(int argc, char **argv) {

    char *pv_path = "ss.priv"; //-> path
    FILE *inputf = stdin;
    FILE *outputf = stdout;
    FILE *pvfile;
    pvfile = fopen(pv_path, "r");
    bool v = false;
    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            inputf = fopen(optarg, "r"); //input file
            if (inputf == NULL) {
                printf("File not available\n");
                return -1;
            }
            break;
        case 'o':
            outputf = fopen(optarg, "w");
            if (outputf == NULL) {
                printf("File not available\n");
                return -1;
            }
            break;
        case 'n': pvfile = fopen(optarg, "r"); break;
        case 'v': v = true; break;
        case 'h':
            printf("SYNOPSIS\n\tDecrypts data using SS decryption.\n\tEncrypted data is encrypted "
                   "by the encrypt program.\n\nUSAGE\n\t./test_decrypt [OPTIONS]\n\nOPTIONS\n\t-h  "
                   "            Display program help and usage.\n\t-v              Display verbose "
                   "program output.\n\t-i infile       Input file of data to decrypt (default: "
                   "stdin).\n\t-o outfile      Output file for decrypted data (default: "
                   "stdout).\n\t-n pvfile       Private key file (default: ss.priv).\n");
            return -1;
            break;
        }
    }
    mpz_t pq, d;
    mpz_inits(pq, d, NULL);

    //read private key from the opened private file
    ss_read_priv(pq, d, pvfile);

    if (v == true) {
        uint64_t pq_bits = mpz_sizeinbase(pq, 2);
        uint64_t d_bits = mpz_sizeinbase(d, 2);

        gmp_printf("pq  (%lu bits) = %Zd\n", pq_bits, pq);
        gmp_printf("d  (%lu bits) = %Zd\n", d_bits, d);
    }
    
    ss_decrypt_file(inputf, outputf, d, pq);

    fclose(pvfile);
    fclose(outputf);
    mpz_clears(pq, d, NULL);

    return 0;
}
