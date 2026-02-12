#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/camellia.h>
#include <time.h>

int main(){
  
  char line[]="\n";
  printf("%s\n", line);
  
  /*generate a 128-bit (16-bytes) random key*/
  unsigned char key[16];
  RAND_bytes(key, sizeof(key));
  printf("128-bit key:");
  for(int i=0; i<sizeof(key); i++)
    printf("%02x", key[i]);
  printf("\n");
  printf("%s\n", line);
  
  /*generate files to work with*/
  FILE *file;
  unsigned char buffer_small[8];
  unsigned char buffer_medium[10240];
  unsigned char buffer_large[1024000];
  
  RAND_bytes(buffer_small, 8);
  file = fopen("random_small.txt", "w");
  if(file==NULL){
    printf("Error opening txt file\n");
    return 1;
  }
  for(int i=0; i<8; i++)
    fprintf(file, "%02x", buffer_small[i]);
  printf("Wrote 16 bytes of random data to random_small.txt\n");
  fclose(file);
  
  RAND_bytes(buffer_medium, 10240);
  file = fopen("random_medium.txt", "w");
  if(file==NULL){
    printf("Error opening txt file\n");
    return 1;
  }
  for(int i=0; i<10240; i++)
    fprintf(file, "%02x", buffer_medium[i]);
  printf("Wrote 20KB of random data to random_medium.txt\n");
  fclose(file);
  
  RAND_bytes(buffer_large, 1024000);
  file = fopen("random_large.bin", "wb");
  if(file==NULL){
    printf("Error opening bin file\n");
    return 1;
  }
  for(int i=0; i<1024000; i++)
    fprintf(file, "%02x", buffer_large[i]);
  printf("Wrote 2MB of random data to random_large.bin\n");
  fclose(file);
  
  printf("%s\n", line);
  
  /*encryption using AES CBC mode*/
  FILE *source_file, *destination_file;
  unsigned char iv[AES_BLOCK_SIZE];
  unsigned char input_buffer[1024], output_buffer[1024 + EVP_MAX_BLOCK_LENGTH];
  int input_length, output_length;
  
  clock_t start, end;
  int cpu_usage;
  
  RAND_bytes(iv, sizeof(iv));
  
  source_file = fopen("random_small.txt", "rb");
  destination_file = fopen("aes_small_enc.bin", "wb");
  
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_EncryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_EncryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Encryption of 16 byte file completed in %d ms using AES-128-CBC\n", cpu_usage);
  
  source_file = fopen("random_medium.txt", "rb");
  destination_file = fopen("aes_medium_enc.bin", "wb");
  
  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_EncryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_EncryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Encryption of 20KB file completed in %d ms using AES-128-CBC\n", cpu_usage);
  
  source_file = fopen("random_large.bin", "rb");
  destination_file = fopen("aes_large_enc.bin", "wb");
  
  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_EncryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_EncryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Encryption of 2MB file completed in %d ms using AES-128-CBC\n", cpu_usage);
  printf("%s\n", line);
  
  /*decryption using AES CBC mode*/
  source_file = fopen("aes_small_enc.bin", "rb");
  destination_file = fopen("aes_small_dec.bin", "wb");
  
  fread(iv, 1, AES_BLOCK_SIZE, source_file);
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_DecryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_DecryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Decryption of 16 byte file completed in %d ms using AES-128-CBC\n", cpu_usage);
  
  source_file = fopen("aes_medium_enc.bin", "rb");
  destination_file = fopen("aes_medium_dec.bin", "wb");
  
  fread(iv, 1, AES_BLOCK_SIZE, source_file);
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_DecryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_DecryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Decryption of 20KB file completed in %d ms using AES-128-CBC\n", cpu_usage);
  
  source_file = fopen("aes_large_enc.bin", "rb");
  destination_file = fopen("aes_large_dec.bin", "wb");
  
  fread(iv, 1, AES_BLOCK_SIZE, source_file);
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_DecryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_DecryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Decryption of 2MB file completed in %d ms using AES-128-CBC\n", cpu_usage);
  printf("%s\n", line);
  
  /*encryption using CAMELLIA CBC mode*/
  unsigned char ivc[CAMELLIA_BLOCK_SIZE];
  
  source_file = fopen("random_small.txt", "rb");
  destination_file = fopen("camellia_small_enc.bin", "wb");
  
  fwrite(ivc, 1, CAMELLIA_BLOCK_SIZE, destination_file);
  
  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_camellia_128_cbc(), NULL, key, ivc);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_EncryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_EncryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Encryption of 16 byte file completed in %d ms using CAMELLIA-128-CBC\n", cpu_usage);
  
  source_file = fopen("random_medium.txt", "rb");
  destination_file = fopen("camellia_medium_enc.bin", "wb");
  
  fwrite(ivc, 1, CAMELLIA_BLOCK_SIZE, destination_file);
  
  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_camellia_128_cbc(), NULL, key, ivc);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_EncryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_EncryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Encryption of 20KB file completed in %d ms using CAMELLIA-128-CBC\n", cpu_usage);
  
  source_file = fopen("random_large.bin", "rb");
  destination_file = fopen("camellia_large_enc.bin", "wb");
  
  fwrite(ivc, 1, CAMELLIA_BLOCK_SIZE, destination_file);
  
  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_camellia_128_cbc(), NULL, key, ivc);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_EncryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_EncryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Encryption of 2MB file completed in %d ms using CAMELLIA-128-CBC\n", cpu_usage);
  printf("%s\n", line);
  
  /*decryption using CAMELLIA CBC mode*/
  source_file = fopen("camellia_small_enc.bin", "rb");
  destination_file = fopen("camellia_small_dec.bin", "wb");
  
  fread(ivc, 1, CAMELLIA_BLOCK_SIZE, source_file);
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_camellia_128_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_DecryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_DecryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Decryption of 16 byte file completed in %d ms using CAMELLIA-128-CBC\n", cpu_usage);
  
  source_file = fopen("camellia_medium_enc.bin", "rb");
  destination_file = fopen("camellia_medium_dec.bin", "wb");
  
  fread(ivc, 1, CAMELLIA_BLOCK_SIZE, source_file);
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_camellia_128_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_DecryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_DecryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Decryption of 20KB file completed in %d ms using CAMELLIA-128-CBC\n", cpu_usage);
  
  source_file = fopen("camellia_large_enc.bin", "rb");
  destination_file = fopen("camellia_large_dec.bin", "wb");
  
  fread(ivc, 1, CAMELLIA_BLOCK_SIZE, source_file);
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_camellia_128_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_DecryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_DecryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Decryption of 2MB file completed in %d ms using CAMELLIA-128-CBC\n", cpu_usage);
  printf("%s\n", line);
  
  /*encryption using SM4 CBC mode*/
  source_file = fopen("random_small.txt", "rb");
  destination_file = fopen("sm4_small_enc.bin", "wb");
  
  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_sm4_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_EncryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_EncryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Encryption of 16 byte file completed in %d ms using SM4-128-CBC\n", cpu_usage);
  
  source_file = fopen("random_medium.txt", "rb");
  destination_file = fopen("sm4_medium_enc.bin", "wb");
  
  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_sm4_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_EncryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_EncryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Encryption of 20KB file completed in %d ms using SM4-128-CBC\n", cpu_usage);
  
  source_file = fopen("random_large.bin", "rb");
  destination_file = fopen("sm4_large_enc.bin", "wb");
  
  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_sm4_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_EncryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_EncryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Encryption of 2MB file completed in %d ms using SM4-128-CBC\n", cpu_usage);
  printf("%s\n", line);
  
  /*decryption using SM4 CBC mode*/
  source_file = fopen("sm4_small_enc.bin", "rb");
  destination_file = fopen("sm4_small_dec.bin", "wb");
  
  fread(iv, 1, AES_BLOCK_SIZE, source_file);
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_sm4_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_DecryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_DecryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Decryption of 16 byte file completed in %d ms using SM4-128-CBC\n", cpu_usage);
  
  source_file = fopen("sm4_medium_enc.bin", "rb");
  destination_file = fopen("sm4_medium_dec.bin", "wb");
  
  fread(iv, 1, AES_BLOCK_SIZE, source_file);
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_sm4_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_DecryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_DecryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Decryption of 20KB file completed in %d ms using SM4-128-CBC\n", cpu_usage);
  
  source_file = fopen("sm4_large_enc.bin", "rb");
  destination_file = fopen("sm4_large_dec.bin", "wb");
  
  fread(iv, 1, AES_BLOCK_SIZE, source_file);
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_sm4_cbc(), NULL, key, iv);
  
  start = clock();
  while((input_length = fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
    EVP_DecryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
    fwrite(output_buffer, 1, output_length, destination_file);
  }
  EVP_DecryptFinal_ex(ctx, output_buffer, &output_length);
  fwrite(output_buffer, 1, output_length, destination_file);
  end = clock();
  
  EVP_CIPHER_CTX_free(ctx);
  fclose(source_file);
  fclose(destination_file);
  
  cpu_usage = end - start;
  printf("Decryption of 2MB file completed in %d ms using SM4-128-CBC\n", cpu_usage);
  printf("%s\n", line);

  return 0;
}

