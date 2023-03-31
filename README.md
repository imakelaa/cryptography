


## Short Description:
	This program implements the Schmidt-Samoa Algorithm of cryptography.

## Build:
	To build/compile this program we can make use of the following commands:
	$ make
	$ make all
	
	To compile all necessary files for keygen, encrypt, decrypt:
	$ make keygen
	$ make encrypt
	$ make decrypt
	
	To delete all the files that are compiler generated use the following command:
	$ make clean
	
	To make all the files clang formatted use the following command:
	$ make format
	

## Running:
	Keygen:
	    SYNOPSIS
           Generates an SS public/private key pair.

        USAGE
           ./keygen [OPTIONS]

        OPTIONS
           -h              Display program help and usage.
           -v              Display verbose program output.
           -b bits         Minimum bits needed for public key n (default: 256).
           -i iterations   Miller-Rabin iterations for testing primes (default: 50).
           -n pbfile       Public key file (default: ss.pub).
           -d pvfile       Private key file (default: ss.priv).
           -s seed         Random seed for testing.
    
    Encrypt:
        SYNOPSIS
           Encrypts data using SS encryption.
           Encrypted data is decrypted by the decrypt program.

        USAGE
           ./test_encrypt [OPTIONS]

        OPTIONS
           -h              Display program help and usage.
           -v              Display verbose program output.
           -i infile       Input file of data to encrypt (default: stdin).
           -o outfile      Output file for encrypted data (default: stdout).
           -n pbfile       Public key file (default: ss.pub).
           
    Decrypt:
        SYNOPSIS
           Decrypts data using SS decryption.
           Encrypted data is encrypted by the encrypt program.

        USAGE
           ./test_decrypt [OPTIONS]

        OPTIONS
           -h              Display program help and usage.
           -v              Display verbose program output.
           -i infile       Input file of data to decrypt (default: stdin).
           -o outfile      Output file for decrypted data (default: stdout).
           -n pvfile       Private key file (default: ss.priv).

