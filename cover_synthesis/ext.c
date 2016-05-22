#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "make_wav.h"
 

int main(int argc, char ** argv)
{
    //check proper usage
    if (argc != 5) {
        printf("usage: %s -i <wav file> -o <output file>\n", argv[0]);
        return 0;
    }
    
    //argument containers (& other var decl)
    int wav_len;
    short* wav;
    unsigned char* message;
    unsigned char* output_file_name;
    
    //get args into their places
    for(int i=0; i<argc; i++) {
        //wav file
        if (!strcmp(argv[i], "-i")) {
            FILE* fp = fopen(argv[i+1], "r");
            //check proper opening
            if (fp == NULL) {
                printf("error opening wav file\n");
                return 0;
            }
            //get file size
            long file_size;
            fseek(fp, 0, SEEK_END);
            file_size = ftell(fp);
            wav_len = file_size;
            rewind(fp);
            fseek(fp, 44, SEEK_SET);
            //allocate buffer
            wav = malloc(file_size+1);            
            //copy file to buffer
            fread(wav, 2, file_size, fp);
            //close file
            fclose(fp);            
        }
        else if (!strcmp(argv[i], "-o")) {
            //allocate space for output filename
            output_file_name = malloc(sizeof(argv[i+1])+1);
            //copy arg
            strcpy(output_file_name, argv[i+1]);
        }
    }
    
    //get message len
    int message_len = wav[0];
    message = malloc(message_len+1);
    
    //extract message
    for (int i=1; i<=message_len; i++) {
        //get stego sample
        int b = wav[i];
        //temp for char construction
        unsigned char c = 0;
        //extract char
        for (int k=0; k<8; k++) {
            short mask = 1 << k;
            short masked_b = b & mask;
            short bit = masked_b >> k;
            //printf("embedding: %d\n",bit);
            if (bit) {                    
                c |= mask;
            } else {                    
                c &= ~mask;
            }
        }
        message[i-1] = c;    
        //printf("%d\t[%c|%d]\n",b,c,c);
    }
 
    //write message to file   
    FILE* fp = fopen(output_file_name, "w");
    //check proper opening
    if (fp == NULL) {
        printf("error opening output file\n");
        return 0;
    }    
    //printf("writing out message of length %d\n",message_len);
    fwrite(message, 1, message_len, fp);
    //close file
    fclose(fp); 
    
    return 0;
}