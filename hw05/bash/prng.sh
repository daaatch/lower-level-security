$ gcc -o prng prng.c -lssl -lcrypto
$ ./prng

Time taken for DRBG to generate 1024 random bits: 0.000027 seconds
DRBG internal state size: 32 bytes
DRBG output buffer size: 32 bytes
Time taken for CS-PRNG to generate 1024 random bits: 0.000026 seconds
CS-PRNG internal state size: 32 bytes
