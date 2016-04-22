#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    //check proper usage
    if (argc != 7) {
        printf("usage: %s -i <plaintext file> -o <ciphertext file> -p <password>\n", argv[0]);
        return 0;
    }
    
    //argument containers (& other var decl)
    char* plain;
    char* cipher;
    char* cipherfile;
    char* password;
   
    //get args into their places
    for(int i=0; i<argc; i++) {
        //input file
        if (!strcmp(argv[i], "-i")) {
            FILE* plainfile = fopen(argv[i+1], "r");
            //check proper opening
            if (plainfile == NULL) {
                printf("error opening input file\n");
                return 0;
            }
            //get file size
            long plainfile_size;
            fseek(plainfile, 0, SEEK_END);
            plainfile_size = ftell(plainfile);
            rewind(plainfile);
            //allocate buffer
            plain = malloc(plainfile_size);
            //copy file to buffer
            fread(plain, 1, plainfile_size, plainfile);
            //close file
            fclose(plainfile);            
        }
        //output filename
        else if (!strcmp(argv[i], "-o")) {
            //allocate space for output filename
            cipherfile = malloc(sizeof(argv[i+1]));
            //copy args
            strcpy(cipherfile, argv[i+1]);
        }
        //password
        else if (!strcmp(argv[i], "-p")) {
            password = malloc(sizeof(argv[i+1]));
            strcpy(password, argv[i+1]);
        }
    }

    printf("argc: %i\n", argc);    
    printf("-i %s", plain);
    printf("-o %s\n", cipherfile);
    printf("-p %s\n", password);
    return 0;
}
