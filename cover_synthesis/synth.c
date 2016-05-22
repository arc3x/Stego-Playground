#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "make_wav.h"
 
#define S_RATE  (44100)
//#define BUF_SIZE (S_RATE*2) /* 2 second buffer */
 

 
int main(int argc, char ** argv)
{
    //check proper usage
    if (argc != 5) {
        printf("usage: %s -m <input file> -o <output file>\n", argv[0]);
        return 0;
    }
    
    //argument containers (& other var decl)
    int message_len;
    unsigned char* message;
    unsigned char* output_file_name;

    
    //get args into their places
    for(int i=0; i<argc; i++) {
        //message file
        if (!strcmp(argv[i], "-m")) {
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
            message_len = file_size;      
            rewind(fp);
            //allocate buffer
            message = malloc(file_size+1);            
            //copy file to buffer
            fread(message, 1, file_size, fp);
            //close file
            fclose(fp); 
            if (message_len > 32766) {
                printf("error maximum message length 32766 bytes\n");
                exit(0);
            }
        }
        else if (!strcmp(argv[i], "-o")) {
            //allocate space for output filename
            output_file_name = malloc(sizeof(argv[i+1])+1);
            //copy arg
            strcpy(output_file_name, argv[i+1]);
        }
    }
    
    //srate = 44100 bits = 5512.5 bytes
    int buffer_size = S_RATE;
    //increase buffer till message fits
    while (buffer_size/8 < message_len) {
        buffer_size+=S_RATE;
    }
    short buffer[buffer_size];
    int i;
    float t;
    float amplitude = 32000;
    float freq_Hz = 440;
    float phase=0;
 
    float freq_radians_per_sample = freq_Hz*2*3.14159265359/S_RATE; 
 
    /* fill buffer with a sine wave */
    buffer[0]=message_len;
    //printf("writing mesg len: %d\n", buffer[0]);      
    for (i=1; i<buffer_size; i++)
    {
        phase += freq_radians_per_sample;
        buffer[i] = (int)(amplitude * sin(phase));
        
        if (message_len>i-1) {        
            //printf("b:%d\tm:[%c|%d]\t", buffer[i], message[i-1], message[i-1]);  
            //get character of message
            unsigned char c = message[i-1];
            //get contents of buffer
            short b = buffer[i];
            //embed c in b
            for (int k=0; k<8; k++) {
                short mask = 1 << k;
                short masked_c = c & mask;
                short bit = masked_c >> k;
                //printf("embedding: %d\n",bit);
                if (bit) {                    
                    b |= mask;
                } else {                    
                    b &= ~mask;
                }
            }
            buffer[i] = b;
                  
            //printf("b2: %d\n", buffer[i]); 
        } 
        
    }
 
    write_wav(output_file_name, buffer_size+1, buffer, S_RATE);
 
    return 0;
}