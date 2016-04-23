#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sppm.h"

int main(int argc, char* argv[]) {
    //check proper usage
    if (argc != 5) {
        printf("usage: %s -s <stego file> -m <message file>\n", argv[0]);
        return 0;
    }
    
    //argument containers (& other var decl)
    int stego_len;
    unsigned char* stego;
    unsigned char* plain;
    unsigned char* messagefile;    
    
    
    //get args into their places
    for(int i=0; i<argc; i++) {
        //cover file
        if (!strcmp(argv[i], "-s")) {
            FILE* fp = fopen(argv[i+1], "r");
            //check proper opening
            if (fp == NULL) {
                printf("error opening stego file\n");
                return 0;
            }
            //get file size
            long file_size;
            fseek(fp, 0, SEEK_END);
            file_size = ftell(fp);
            stego_len = file_size;
            rewind(fp);
            //allocate buffer
            stego = malloc(file_size+1);            
            //copy file to buffer
            fread(stego, 1, file_size, fp);
            //close file
            fclose(fp);            
        }
        //message file
        else if (!strcmp(argv[i], "-m")) {
            //allocate space for output filename
            messagefile = malloc(sizeof(argv[i+1])+1);
            //copy args
            strcpy(messagefile, argv[i+1]);
        }
    }

    //dump args for debug
    printf("argc: %i\n", argc);    
    //printf("-s %s\n", stego);
    printf("-m %s\n", messagefile);

    
    //embed the message in the cover
    plain = extractStego(stego, plain);
    
    printf("plain: %s\n", plain);
    
    FILE* fp = fopen(messagefile, "w");
    //check proper opening
    if (fp == NULL) {
        printf("error opening output file\n");
        return 0;
    }
    //write plaintext file
    fwrite(plain, 1, strlen(plain), fp);
    //close file
    fclose(fp);      
    
    return 0;
}


