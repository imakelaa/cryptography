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

    char *user = "";
    char *pb_path = "ss.pub"; //->path
    FILE *inputf = stdin;
    FILE *outputf = stdout;
    FILE *pbfile;
    pbfile = fopen(pb_path, "r");

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
        case 'n':
            fclose(pbfile);
            pbfile = fopen(optarg, "r");
            break;
        case 'v': v = true; break;
        case 'h':
            printf("SYNOPSIS\n\tEncrypts data using SS encryption.\n\tEncrypted data is decrypted "
                   "by the decrypt program.\n\nUSAGE\n\t./test_encrypt [OPTIONS]\n\nOPTIONS\n\t-h  "
                   "            Display program help and usage.\n\t-v              Display verbose "
                   "program output.\n\t-i infile       Input file of data to encrypt (default: "
                   "stdin).\n\t-o outfile      Output file for encrypted data (default: "
                   "stdout).\n\t-n pbfile       Public key file (default: ss.pub).\n");
            return -1;
            break;
        }
    }

    mpz_t n;
    mpz_init(n);

    getenv(user);
    user = getenv("USER");

    //read public key from the opened public file
    ss_read_pub(n, user, pbfile);

    if (v == true) {
        uint64_t n_bits = mpz_sizeinbase(n, 2);

        gmp_printf("user = %s\n", user);
        gmp_printf("n  (%lu bits) = %Zd\n", n_bits, n);
    }

    //ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n)
    ss_encrypt_file(inputf, outputf, n);

    fclose(pbfile);
    fclose(inputf);
    fclose(outputf);
    mpz_clear(n);

    return 0;
}
