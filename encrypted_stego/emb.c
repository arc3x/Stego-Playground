#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sppm.h"

int main(int argc, char* argv[]) {
    //check proper usage
    if (argc != 7) {
        printf("usage: %s -c <cover file> -m <message file> -s <output stego filename>\n", argv[0]);
        return 0;
    }
    
    //argument containers (& other var decl)
    int message_len;
    unsigned char* message;
    unsigned char* cover;
    unsigned char* stegofile;
    
    //get args into their places
    for(int i=0; i<argc; i++) {
        //cover file
        if (!strcmp(argv[i], "-c")) {
            FILE* fp = fopen(argv[i+1], "r");
            //check proper opening
            if (fp == NULL) {
                printf("error opening cover file\n");
                return 0;
            }
            //get file size
            long file_size;
            fseek(fp, 0, SEEK_END);
            file_size = ftell(fp);
            message_len = file_size;
            rewind(fp);
            //allocate buffer
            cover = malloc(file_size+1);            
            //copy file to buffer
            fread(cover, 1, file_size, fp);
            //close file
            fclose(fp);            
        }
        //message file
        else if (!strcmp(argv[i], "-m")) {
            FILE* fp = fopen(argv[i+1], "r");
            //check proper opening
            if (fp == NULL) {
                printf("error opening cover file\n");
                return 0;
            }
            //get file size
            long file_size;
            fseek(fp, 0, SEEK_END);
            file_size = ftell(fp);
            message_len = file_size;
            rewind(fp);
            //allocate buffer
            message = malloc(file_size+1);            
            //copy file to buffer
            fread(message, 1, file_size, fp);
            //close file
            fclose(fp);            
        }
        else if (!strcmp(argv[i], "-s")) {
            //allocate space for output filename
            stegofile = malloc(sizeof(argv[i+1])+1);
            //copy args
            strcpy(stegofile, argv[i+1]);
        }
    }

    int payload_size = getMaxPayloadSize(cover);
    
    //if message is bigger than payload fail
    if (message_len > payload_size) {
        printf("error: message exceeds max payload size. try a bigger image.\n");
        exit(0);
    }
    
    //embed the message in the cover
    embedStego(cover, message, message_len);
    
    //save stegowork
    FILE* fp = fopen(stegofile, "w");
    //check proper opening
    if (fp == NULL) {
        printf("error opening output file\n");
        return 0;
    }
    //write plaintext file
    fwrite(cover, 1, strlen(cover), fp);
    //close file
    fclose(fp);      
    
    return 0;
}


