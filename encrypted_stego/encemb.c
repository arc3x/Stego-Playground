#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    //check proper usage
    if (argc != 9) {
        printf("usage: %s -c <coverfile (.ppm)> -m <message filename> -s <stegofile> -p <password>\n", argv[0]);
        return 0;
    }
    
    //argument containers (& other var decl)
    unsigned char* cover;
    unsigned char* message;
    unsigned char* stego;
    unsigned char* password;    
   
    //get args into their places
    for(int i=0; i<argc; i++) {
        //cover file
        if (!strcmp(argv[i], "-c")) {
            //allocate space for output filename
            cover = malloc(sizeof(argv[i+1])+1);
            //copy args
            strcpy(cover, argv[i+1]);
        }
        //message file
        else if (!strcmp(argv[i], "-m")) {
            message = malloc(sizeof(argv[i+1])+1);
            strcpy(message, argv[i+1]);
        }
        //stego file
        else if (!strcmp(argv[i], "-s")) {
            stego = malloc(sizeof(argv[i+1])+1);
            strcpy(stego, argv[i+1]);
        }
        //password
        else if (!strcmp(argv[i], "-p")) {
            password = malloc(sizeof(argv[i+1])+1);
            strcpy(password, argv[i+1]);
        }
    }


    char cmd[256];
    //encrypt
    strcpy(cmd, "./enc -i "); strcat(cmd, message); strcat(cmd, " -o crypto.txt -p "); strcat(cmd, password);       
    //printf("%s\n", cmd);
    system(cmd);
    //embed
    strcpy(cmd, "./emb -c "); strcat(cmd, cover); strcat(cmd, " -m crypto.txt -s "); strcat(cmd, stego);       
    //printf("%s\n", cmd);
    system(cmd);
    //cleanup
    system("rm crypto.txt");    
    
    return 0;
}


