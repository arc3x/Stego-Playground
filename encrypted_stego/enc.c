#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void handleErrors() {
    printf("error");
    exit(0);
}

//code from https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *aad,
	int aad_len, unsigned char *key, unsigned char *iv,
	unsigned char *ciphertext, unsigned char *tag)
{
	EVP_CIPHER_CTX *ctx;

	int len;

	int ciphertext_len;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    
	/* Initialise the encryption operation. */
	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) handleErrors();

	/* Set IV length if default 12 bytes (96 bits) is not appropriate */
	if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL)) handleErrors();

	/* Initialise key and IV */
	if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

	/* Provide any AAD data. This can be called zero or more times as
	 * required
	 */
	if(1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len)) handleErrors();

	/* Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	 */
	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();	
    ciphertext_len = len;

	/* Finalise the encryption. Normally ciphertext bytes may be written at
	 * this stage, but this does not occur in GCM mode
	 */
	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
	ciphertext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}


int main(int argc, char* argv[]) {
    //check proper usage
    if (argc != 7) {
        printf("usage: %s -i <plaintext file> -o <ciphertext file> -p <password>\n", argv[0]);
        return 0;
    }
    
    //argument containers (& other var decl)
    unsigned char* plain;
    unsigned char* cipher;
    unsigned char* cipherfile;
    unsigned char* password;
    
    //variable defs
    unsigned char* key = malloc(1001);
   
    //get args into their places
    for(int i=0; i<argc; i++) {
        //input file
        if (!strcmp(argv[i], "-i")) {
            FILE* fp = fopen(argv[i+1], "r");
            //check proper opening
            if (fp == NULL) {
                printf("error opening input file\n");
                return 0;
            }
            //get file size
            long file_size;
            fseek(fp, 0, SEEK_END);
            file_size = ftell(fp);
            rewind(fp);
            //allocate buffers
            plain = malloc(file_size+1);
            cipher = malloc(file_size+1);
            //copy file to buffer
            fread(plain, 1, file_size, fp);
            //close file
            fclose(fp);            
        }
        //output filename
        else if (!strcmp(argv[i], "-o")) {
            //allocate space for output filename
            cipherfile = malloc(sizeof(argv[i+1])+1);
            //copy args
            strcpy(cipherfile, argv[i+1]);
        }
        //password
        else if (!strcmp(argv[i], "-p")) {
            password = malloc(sizeof(argv[i+1])+1);
            strcpy(password, argv[i+1]);
        }
    }

    //produce a hash from the password to use as a key
    PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), "", 0, 10000, 1000, key);
    
    //encrypt in GCM mode
    unsigned char aad[] = {
        0x4d, 0x23, 0xc3, 0xce, 0xc3, 0x34, 0xb4, 0x9b, 0xdb, 0x37, 0x0c, 0x43,
        0x7f, 0xec, 0x78, 0xde
    };
    unsigned char iv[] = {
        0x99, 0xaa, 0x3e, 0x68, 0xed, 0x81, 0x73, 0xa0, 0xee, 0xd0, 0x66, 0x84
    };
    unsigned char tag[] = "";    
    int cipher_len = encrypt(plain, strlen(plain), aad, sizeof(aad), key, iv, cipher, tag);
    
    //save ciphertext
    FILE* fp = fopen(cipherfile, "w");
    //check proper opening
    if (fp == NULL) {
        printf("error opening output file\n");
        return 0;
    }
    //write ciphertext file
    fwrite(cipher, 1, cipher_len, fp);
    //close file
    fclose(fp);      
    
    return 0;
}


