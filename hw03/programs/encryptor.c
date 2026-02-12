#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OPENSSL_API_COMPAT 0x10100000L

#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#define KEYSIZE 16 //128 bits = 16 bytes

int EtM(const unsigned char *key, const int size, const int mode){
  FILE *source_file, *destination_file;
  unsigned char iv[16]; //16 bytes for AES, 12 bytes for ChaCha20
  unsigned char input_buffer[1024], output_buffer[1024 + EVP_MAX_BLOCK_LENGTH];
  unsigned char tag[32]; //32 bytes for HMAC, 16 bytes for GCM
  int input_length, output_length;
  clock_t start, end;
  int cpu_usage;
  int avg=0;
  /*open file to encrypt*/
  for(int i=0; i<10; i++){
    if(size==1){
      source_file=fopen("1mb.bin", "rb");
      destination_file=fopen("encrypted/1mb.bin", "wb");
    }
    if(size==10){
      source_file=fopen("10mb.bin", "rb");
      destination_file=fopen("encrypted/10mb.bin", "wb");
    }
    if(size==100){
      source_file=fopen("100mb.bin", "rb");
      destination_file=fopen("encrypted/100mb.bin", "wb");
    }  
    EVP_CIPHER_CTX *ctx=EVP_CIPHER_CTX_new();
    HMAC_CTX *hmac_ctx = NULL;
    unsigned char *hmac_result = NULL;
    unsigned int hmac_len = 0;    
    /*select cipher based on mode*/
    if(mode==1){ //AES-128-CTR + HMAC
      RAND_bytes(iv, sizeof(iv));
      EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv);
      fwrite(iv, 1, sizeof(iv), destination_file);
      hmac_ctx = HMAC_CTX_new();
      HMAC_Init_ex(hmac_ctx, key, 16, EVP_sha256(), NULL);
      HMAC_Update(hmac_ctx, iv, sizeof(iv)); // include IV in HMAC
    }
    if(mode==2){ //ChaCha20 + HMAC
      RAND_bytes(iv, 12); //ChaCha20 uses 12-byte nonce
      EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv);
      fwrite(iv, 1, 12, destination_file);
      hmac_ctx = HMAC_CTX_new();
      HMAC_Init_ex(hmac_ctx, key, 32, EVP_sha256(), NULL);
      HMAC_Update(hmac_ctx, iv, 12);
    }
    if(mode==3){ //AES-128-GCM
        RAND_bytes(iv, 12);  //GCM uses 12-byte iv
        EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL);
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL);
        EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv);
        fwrite(iv, 1, 12, destination_file);
    }
    if(mode==4){ //ChaCha20 + Poly1305
      RAND_bytes(iv, 12);
      EVP_EncryptInit_ex(ctx, EVP_chacha20_poly1305(), NULL, NULL, NULL);
      EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, 12, NULL);
      EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv);
      fwrite(iv, 1, 12, destination_file);
    }   
    start=clock();
    /*start encryption loop*/
    while((input_length=fread(input_buffer, 1, sizeof(input_buffer), source_file))>0){
      EVP_EncryptUpdate(ctx, output_buffer, &output_length, input_buffer, input_length);
      fwrite(output_buffer, 1, output_length, destination_file);
      if(mode==1||mode==2) HMAC_Update(hmac_ctx, output_buffer, output_length);
    }
    EVP_EncryptFinal_ex(ctx, output_buffer, &output_length);
    fwrite(output_buffer, 1, output_length, destination_file);
    if(mode==1||mode==2){ //Finalize HMAC authentication
      HMAC_Update(hmac_ctx, output_buffer, output_length);
      HMAC_Final(hmac_ctx, tag, &hmac_len);
      fwrite(tag, 1, hmac_len, destination_file);
      HMAC_CTX_free(hmac_ctx);
    }
    if(mode==3){
      EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag);
      fwrite(tag, 1, 16, destination_file);
    }
    if(mode==4){
      EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_GET_TAG, 16, tag);
      fwrite(tag, 1, 16, destination_file);
    }
    EVP_CIPHER_CTX_free(ctx);
    fclose(source_file);
    fclose(destination_file);
    end=clock();
    cpu_usage=(end-start);
    avg+=cpu_usage;
  }   
  return (avg/10);
}

int main(int argc, char *argv[]){
  if(argc!=2){
    printf("error occurred opening keyfile.\n");
    exit(1);
  }
  FILE *keyfile=fopen(argv[1],"rb");
  unsigned char key[KEYSIZE];
  size_t keylength=fread(key, 1, KEYSIZE, keyfile);
  fclose(keyfile);
  printf("128-bit key: ");
  for(int i=0; i<KEYSIZE; i++) printf("%02X", key[i]);
  printf("\n");
  printf("File size: 1mb   Average encryption time: %dms   Mode: AES-128-CTR+HMAC\n", EtM(key, 1, 1));
  printf("File size: 1mb   Average encryption time: %dms   Mode: ChaCha20+HMAC\n", EtM(key, 1, 2));
  printf("File size: 1mb   Average encryption time: %dms   Mode: AES-128-GCM\n", EtM(key, 1, 3));
  printf("File size: 1mb   Average encryption time: %dms   Mode: ChaCha20+Poly1305\n", EtM(key, 1, 4));
  printf("File size: 10mb  Average encryption time: %dms  Mode: AES-128-CTR+HMAC\n", EtM(key, 10, 1));
  printf("File size: 10mb  Average encryption time: %dms  Mode: ChaCha2+HMAC\n", EtM(key, 10, 2));
  printf("File size: 10mb  Average encryption time: %dms  Mode: AES-128-GCM\n", EtM(key, 10, 3));
  printf("File size: 10mb  Average encryption time: %dms  Mode: ChaCha20+Poly1305\n", EtM(key, 10, 4));
  printf("File size: 100mb Average encryption time: %dms Mode: AES-128-CTR+HMAC\n", EtM(key, 100, 1));
  printf("File size: 100mb Average encryption time: %dms Mode: ChaCha20+HMAC\n", EtM(key, 100, 2));
  printf("File size: 100mb Average encryption time: %dms Mode: AES-128-GCM\n", EtM(key, 100, 3));
  printf("File size: 100mb Average encryption time: %dms Mode: ChaCha20+Poly1305\n", EtM(key, 100, 4));
  return 0;
}
