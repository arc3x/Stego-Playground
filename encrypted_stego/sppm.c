#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sppm.h"

//advances ptr to the beginning of embedding area
unsigned char* advanceToEmbeddingArea(unsigned char* ptr) {
    for(int i=0; i<4; i++) {
        while(*ptr != '\n') {
            ptr+=sizeof(char);
        }
        ptr+=sizeof(char);
    }
    return ptr;
}

int getWidth(unsigned char* ptr) {
    for(int i=0; i<2; i++) {
        while(*ptr != '\n') {
            ptr+=sizeof(char);
        }
        ptr+=sizeof(char);
    }
    return atoi(ptr);
}

int getHeight(unsigned char* ptr) {
    for(int i=0; i<2; i++) {
        while(*ptr != '\n') {
            ptr+=sizeof(char);
        }
        ptr+=sizeof(char);
    }
    while(*ptr != ' ') {
        ptr+=sizeof(char);
    }
    return atoi(ptr);
}

//returns max payload size in bytes
int getMaxPayloadSize(unsigned char* ppm) {
    char* ptr = ppm;    
    int w = getWidth(ppm);
    int h = getHeight(ppm);
    return ((w*h*3)/8)-4;
}

//embeds an int in a cover work and returns a new ptr 
unsigned char* embedInt(unsigned char* ptr, int i) {    
    for (int k=(sizeof(int)*8)-1; k>=0; k--) {
        //calculate bit of int we are embedding
        int mask = 1 << k;
        int masked_i = i & mask;
        int bit = masked_i >> k;
        //get number we are embedding on
        int toEmbed = atoi(ptr);
        //embed
        if (bit) {
            toEmbed |= 0x01;
        } else {
            toEmbed &= 0xFE;
        }             
        //overwite        
        unsigned char t = *(ptr+sizeof(char)*3);
        unsigned int loc = sprintf(ptr, "%i", toEmbed);
        ptr[loc]=t;  
        //advance to next 
        while(*ptr!=' ' && *ptr!='\n') {
            ptr+=sizeof(char);
        }
        ptr+=sizeof(char);
        if (*ptr=='\n')
            ptr+=sizeof(char);
    }
    return ptr; 
}


//embeds a char in a cover work and returns a new ptr 
unsigned char* embedChar(unsigned char* ptr, unsigned char c) {        
    for (int k=(sizeof(char)*8)-1; k>=0; k--) {
        //calculate bit of int we are embedding
        int mask = 1 << k;
        int masked_c = c & mask;
        int bit = masked_c >> k;
        //get number we are embedding on
        int toEmbed = atoi(ptr);
        //embed
        if (bit) {
            toEmbed |= 0x01;
        } else {
            toEmbed &= 0xFE;
        }               
        //overwite        
        unsigned char t = *(ptr+sizeof(char)*3);
        unsigned int loc = sprintf(ptr, "%i", toEmbed);
        ptr[loc]=t;  
        toEmbed=atoi(ptr);
        //advance to next 
        while(*ptr!=' ' && *ptr!='\n') {
            ptr+=sizeof(char);
        }
        ptr+=sizeof(char);
        if (*ptr=='\n')
            ptr+=sizeof(char);
    }
    return ptr;
    
}




//embeds the stego message in the ppm image
void embedStego(unsigned char* ppm, unsigned char* message, int message_len) {
    char* ptr = advanceToEmbeddingArea(ppm);
    ptr = embedInt(ptr, message_len);
    for (int i=0; i<message_len; i++) {
        ptr = embedChar(ptr, message[i]);
    }
    
}


//extracts an int from a cover work and returns a new ptr 
unsigned char* extractInt(unsigned char* ptr, int* i) {    
    *i=0;
    for (int k=(sizeof(int)*8)-1; k>=0; k--) {
        //get bit from stego
        int toExtract = atoi(ptr);      
        int bit = toExtract & 0x01;        
        //put bit in int
        bit = bit << k;
        *i |= bit;           
        //advance to next 
        while(*ptr!=' ' && *ptr!='\n') {
            ptr+=sizeof(char);
        }
        ptr+=sizeof(char);
        if (*ptr=='\n')
            ptr+=sizeof(char);
    }
    return ptr; 
}

//extracts a char from a cover work and returns a new ptr 
unsigned char* extractChar(unsigned char* ptr, unsigned char* c) {    
    *c=0;
    for (int k=(sizeof(char)*8)-1; k>=0; k--) {
        //get bit from stego
        unsigned int toExtract = atoi(ptr);    
        unsigned int bit = toExtract & 0x01;               
        //put bit in char
        bit = bit << k;
        *c |= bit;               
        //advance to next 
        while(*ptr!=' ' && *ptr!='\n') {
            ptr+=sizeof(char);
        }
        if (*ptr==' ')
            ptr+=sizeof(char);
        if (*ptr=='\n')
            ptr+=sizeof(char);
    }
    return ptr; 
}

//extracts the stego message in the ppm image
unsigned char* extractStego(unsigned char* ppm, unsigned char* plain, int* len) {
    unsigned char* ptr = advanceToEmbeddingArea(ppm);
    int message_len;
    *len=message_len;
    plain = malloc(message_len+1);
    ptr = extractInt(ptr, &message_len);
    //printf("message len: %i\n", message_len);
    int i;
    for (i=0; i<message_len; i++) {
        unsigned char c;
        ptr = extractChar(ptr, &c);
        plain[i]=c;
    }    
    return plain;
}
