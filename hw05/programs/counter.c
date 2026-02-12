#include <stdio.h>
#include <stdlib.h>

void count_bits(const char *filename, int *count_zeros, int *count_ones) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, file)) {
        for (int i = 7; i >= 0; i--) {
            if ((byte >> i) & 1) {
                (*count_ones)++;
            } else {
                (*count_zeros)++;
            }
        }
    }

    fclose(file);
}

int main() {
    const char *file1 = "cs-prng.bin";  // Replace with actual file paths
    const char *file2 = "drbg.bin";  // Replace with actual file paths

    int zeros1 = 0, ones1 = 0;
    int zeros2 = 0, ones2 = 0;

    count_bits(file1, &zeros1, &ones1);
    count_bits(file2, &zeros2, &ones2);

    printf("%s: Zeros = %d, Ones = %d\n", file1, zeros1, ones1);
    printf("%s: Zeros = %d, Ones = %d\n", file2, zeros2, ones2);

    return 0;
}

