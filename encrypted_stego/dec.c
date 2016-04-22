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
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *aad,
	int aad_len, unsigned char *tag, unsigned char *key, unsigned char *iv,
	unsigned char *plaintext)
{
	EVP_CIPHER_CTX *ctx;
	int len;
	int plaintext_len;
	int ret;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	/* Initialise the decryption operation. */
	if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
		handleErrors();

	/* Set IV length. Not necessary if this is 12 bytes (96 bits) */
	if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
		handleErrors();

	/* Initialise key and IV */
	if(!EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

	/* Provide any AAD data. This can be called zero or more times as
	 * required
	 */
	if(!EVP_DecryptUpdate(ctx, NULL, &len, aad, aad_len))
		handleErrors();

	/* Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary
	 */
	if(!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		handleErrors();
	plaintext_len = len;

	/* Set expected tag value. Works in OpenSSL 1.0.1d and later */
	if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag))
		handleErrors();

	/* Finalise the decryption. A positive return value indicates success,
	 * anything else is a failure - the plaintext is not trustworthy.
	 */
	ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	if(ret > 0)
	{
		/* Success */
		plaintext_len += len;
		return plaintext_len;
	}
	else
	{
		/* Verify failed */
		return -1;
	}
}


int main(int argc, char* argv[]) {
    //check proper usage
    if (argc != 7) {
        printf("usage: %s -i <ciphertext file> -o <plaintext file> -p <password>\n", argv[0]);
        return 0;
    }
    
    //argument containers (& other var decl)
    int cipher_len;
    unsigned char* cipher;
    unsigned char* plain;
    unsigned char* plainfile;
    unsigned char* password;
    unsigned char* key = malloc(101);
   
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
            cipher_len = file_size;
            rewind(fp);
            //allocate buffers
            plain = malloc(file_size+1);
            cipher = malloc(file_size+1);
            //copy file to buffer
            fread(cipher, 1, file_size, fp);
            //close file
            fclose(fp);            
        }
        //output filename
        else if (!strcmp(argv[i], "-o")) {
            //allocate space for output filename
            plainfile = malloc(sizeof(argv[i+1])+1);
            //copy args
            strcpy(plainfile, argv[i+1]);
        }
        //password
        else if (!strcmp(argv[i], "-p")) {
            password = malloc(sizeof(argv[i+1])+1);
            strcpy(password, argv[i+1]);
        }
    }

    //dump args for debug
    printf("argc: %i\n", argc);    
    //printf("-i %s", cipher);
    printf("-o %s\n", plainfile);
    printf("-p %s\n", password);
    
    //code from https://wiki.openssl.org/index.php/Libcrypto_API
    /* Load the human readable error strings for libcrypto */
    ERR_load_crypto_strings();
    /* Load all digest and cipher algorithms */
    OpenSSL_add_all_algorithms();
    /* Load config file, and other important initialisation */
    OPENSSL_config(NULL);
    //produce a hash from the password to use as a key
    PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), "", 0, 10000, 100, key);
    //printf("key: %s\n", key);
    
    //encrypt in GCM mode
    unsigned char aad[] = {
        0x4d, 0x23, 0xc3, 0xce, 0xc3, 0x34, 0xb4, 0x9b, 0xdb, 0x37, 0x0c, 0x43,
        0x7f, 0xec, 0x78, 0xde
    };
    unsigned char iv[] = {
        0x99, 0xaa, 0x3e, 0x68, 0xed, 0x81, 0x73, 0xa0, 0xee, 0xd0, 0x66, 0x84
    };
    unsigned char tag[] = "";       
    int plain_len = decrypt(cipher, strlen(cipher), aad, 0, tag, key, iv, plain);
    //printf("plain: %s\n", plain);
    
    /* Clean up */
    /* Removes all digests and ciphers */
    EVP_cleanup();
    /* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g. base64 transformations */
    CRYPTO_cleanup_all_ex_data();
    /* Remove error strings */
    ERR_free_strings();
    
    FILE* fp = fopen(plainfile, "w");
    //check proper opening
    if (fp == NULL) {
        printf("error opening input file\n");
        return 0;
    }
    //write plaintext file
    fwrite(plain, 1, strlen(plain), fp);
    //close file
    fclose(fp);      
    
    return 0;
}


