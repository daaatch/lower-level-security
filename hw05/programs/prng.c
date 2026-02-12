#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>

// SHA-256 hashing function
void sha256(const unsigned char *input, size_t length, unsigned char *output) {
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, input, length);
    SHA256_Final(output, &sha256_ctx);
}

// DRBG (deterministic random bit generator)
typedef struct {
    unsigned char state[SHA256_DIGEST_LENGTH];  // internal state
    unsigned char output[SHA256_DIGEST_LENGTH]; // output buffer
} DRBG;

void drbg_init(DRBG *drbg, const unsigned char *seed, size_t seed_len) {
    sha256(seed, seed_len, drbg->state);
}

void drbg_generate(DRBG *drbg) {
    sha256(drbg->state, SHA256_DIGEST_LENGTH, drbg->state);
    memcpy(drbg->output, drbg->state, SHA256_DIGEST_LENGTH);
}

void drbg_get_bits(DRBG *drbg, unsigned char *output, size_t num_bits) {
    size_t full_bytes = num_bits / 8;
    size_t remaining_bits = num_bits % 8;

    for (size_t i = 0; i < full_bytes; i++) {
        output[i] = drbg->output[i];
    }

    if (remaining_bits > 0) {
        unsigned char last_byte = drbg->output[full_bytes];
        output[full_bytes] = last_byte & ((1 << remaining_bits) - 1);  // mask remaining bits
    }
}

// CS-PRNG (cryptographically secure PRNG)
typedef struct {
    unsigned char state[SHA256_DIGEST_LENGTH];  // Internal state
} CS_PRNG;

void cs_prng_init(CS_PRNG *prng, const unsigned char *seed, size_t seed_len) {
    sha256(seed, seed_len, prng->state);
}

void cs_prng_generate(CS_PRNG *prng, unsigned char *output, size_t output_len) {
    size_t bytes_generated = 0;
    unsigned char hash_output[SHA256_DIGEST_LENGTH];

    while (bytes_generated < output_len) {
        sha256(prng->state, SHA256_DIGEST_LENGTH, hash_output);  // Generate new hash
        size_t bytes_to_copy = (output_len - bytes_generated < SHA256_DIGEST_LENGTH)
                                   ? output_len - bytes_generated
                                   : SHA256_DIGEST_LENGTH;
        memcpy(output + bytes_generated, hash_output, bytes_to_copy);
        bytes_generated += bytes_to_copy;
        memcpy(prng->state, hash_output, SHA256_DIGEST_LENGTH);  // Update the state
    }
}

// Utility function to print binary string for visualization
void print_binary_string(const unsigned char *data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (data[i] >> j) & 1);
        }
    }
    printf("\n");
}

// main function to compare time and space of both PRNGs
int main() {
    unsigned char seed[] = "random_seed";  // seed for both PRNGs
    size_t seed_len = strlen((char *)seed);

    DRBG drbg;
    drbg_init(&drbg, seed, seed_len);

    unsigned char drbg_output[1024]; // 1 MB of random bits
    clock_t start_time = clock();  
    for (int i = 0; i < 1024 / SHA256_DIGEST_LENGTH; i++) {
        drbg_generate(&drbg);        
        drbg_get_bits(&drbg, drbg_output + i * SHA256_DIGEST_LENGTH, SHA256_DIGEST_LENGTH);
    }
    clock_t end_time = clock(); 
    
    // DRBG generated output
    // printf("DRBG Output:\n");
    // print_binary_string(drbg_output, 1024);  // print output as binary
    
    // DRBG file write
    FILE *file = fopen("drbg.bin", "wb");  // open file for writing in binary mode
    if (file == NULL) {
        perror("Error opening file for writing");
        return 1;
    }

    size_t written = fwrite(drbg_output, 1, sizeof(drbg_output), file);
    if (written != sizeof(drbg_output)) {
        perror("Error writing to file");
        fclose(file);
        return 1;
    }

    fclose(file);

    // DRBG time stats
    double drbg_time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time taken for DRBG to generate 1024 random bits: %.6f seconds\n", drbg_time_taken);

    // DRBG space usage
    printf("DRBG internal state size: %lu bytes\n", sizeof(drbg.state));
    printf("DRBG output buffer size: %lu bytes\n", sizeof(drbg.output));

    CS_PRNG prng;
    cs_prng_init(&prng, seed, seed_len);

    unsigned char prng_output[1024]; // 1 MB of random bits
    start_time = clock(); 
    cs_prng_generate(&prng, prng_output, sizeof(prng_output));
    end_time = clock(); 
    
    // CS-PRNG generated output
    // printf("CS-PRNG Output:\n");
    // print_binary_string(prng_output, 1024);  // print output as binary
    
    // CS-PRNG file write
    file = fopen("cs-prng.bin", "wb");  // open file for writing in binary mode
    if (file == NULL) {
        perror("Error opening file for writing");
        return 1;
    }

    written = fwrite(prng_output, 1, sizeof(prng_output), file);
    if (written != sizeof(prng_output)) {
        perror("Error writing to file");
        fclose(file);
        return 1;
    }

    fclose(file);

    // CS-PRNG time stats
    double prng_time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time taken for CS-PRNG to generate 1024 random bits: %.6f seconds\n", prng_time_taken);

    // CS-PRNG space usage
    printf("CS-PRNG internal state size: %lu bytes\n", sizeof(prng.state));

    return 0;
}

