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
        printf("usage: %s -i <input file> -o <output file>\n", argv[0]);
        return 0;
    }
    
    //argument containers (& other var decl)
    int message_len;
    unsigned char* message;
    unsigned char* output_file_name;

    
    //get args into their places
    for(int i=0; i<argc; i++) {
        //message file
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
            message_len = file_size;      
            rewind(fp);
            //allocate buffer
            message = malloc(file_size+1);            
            //copy file to buffer
            fread(message, 1, file_size, fp);
            //close file
            fclose(fp); 
            if (message_len > 2147483646) {
                printf("error maximum message length 2147483646 bytes\n");
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
    
    //srate = 44100 bytes
    int buffer_size = S_RATE+2;
    //increase buffer till message fits
    while (buffer_size/8 < message_len) {
        buffer_size+=S_RATE;
    }
    unsigned short buffer[buffer_size];
    int i;
    float t;
    float amplitude = 32000;
    float freq_Hz = 440;
    
 
    float change = freq_Hz*2*3.14159265359/S_RATE; 
 
    float phase=0;
    //write message length into buffer
    buffer[0]=message_len >> 16;     
    buffer[1]=(message_len << 16) >> 16;   
    /* fill buffer with a sine wave */    
    for (i=2; i<buffer_size; i++)
    {
        phase += change;
        buffer[i] = (int)(amplitude * sin(phase));
        
        if (message_len>i-2) {                      
            //get character of message
            unsigned char c = message[i-2];
            //get contents of buffer
            unsigned short b = buffer[i];
            //embed c in b
            for (int k=0; k<8; k++) {
                short mask = 1 << k;
                short masked_c = c & mask;
                short bit = masked_c >> k;
                if (bit) {                    
                    b |= mask;
                } else {                    
                    b &= ~mask;
                }
            }
            buffer[i] = b;
        } 
        
    }
 
    write_wav(output_file_name, buffer_size, buffer, S_RATE);
 
    return 0;
}