#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>

#define ONEMB (1024 * 1024)
#define TENMB (10 * ONEMB)
#define HUNDREDMB (100 * ONEMB)
#define KSIZE 16 //128-bit key = 16 bytes

void fileGenerator(const char *filename, size_t filesize){
  FILE *file=fopen(filename, "wb");
  unsigned char *buffer=(unsigned char *)malloc(filesize);
  RAND_bytes(buffer, filesize);
  fwrite(buffer, 1, filesize, file);
  fclose(file);
  free(buffer);
  printf("Wrote %zu random bytes to %s\n", filesize, filename);
}

void keyGenerator(const char *keyfilename){
  unsigned char key[KSIZE];
  FILE *keyfile=fopen(keyfilename, "wb");
  RAND_bytes(key, KSIZE);
  fwrite(key, 1, KSIZE, keyfile);
  fclose(keyfile);
  printf("128-bit encryption key: ");
  for(int i=0; i<KSIZE; i++) printf("%02x", key[i]);
  printf("\n");
}

int main(){
  keyGenerator("key.bin");
  fileGenerator("1mb.bin", ONEMB);
  fileGenerator("10mb.bin", TENMB);
  fileGenerator("100mb.bin", HUNDREDMB);
}
