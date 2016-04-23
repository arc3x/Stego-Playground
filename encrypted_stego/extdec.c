#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    //check proper usage
    if (argc != 7) {
        printf("usage: %s -s <stegofile (.ppm)> -m <message filename> -p <password>\n", argv[0]);
        return 0;
    }
    
    //argument containers (& other var decl)
    unsigned char* message;
    unsigned char* stego;
    unsigned char* password;    
   
    //get args into their places
    for(int i=0; i<argc; i++) {
        //message file
        if (!strcmp(argv[i], "-m")) {
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
    //extract
    strcpy(cmd, "./ext -s "); strcat(cmd, stego); strcat(cmd, " -m "); strcat(cmd, "crypto.txt");       
    printf("%s\n", cmd);
    system(cmd);
    
    strcpy(cmd, "./dec -i crypto.txt -o "); strcat(cmd, message); strcat(cmd, " -p "); strcat(cmd, password);       
    printf("%s\n", cmd);
    system(cmd);
        
    system("rm crypto.txt");    
    return 0;
}


