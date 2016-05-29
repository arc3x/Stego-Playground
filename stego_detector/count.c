#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../encrypted_stego/sppm.h"

#define DEBUG 0

int main(int argc, char ** argv) {
    //check proper usage
    if (argc != 3) {
        printf("usage: %s <filename> <n>\n", argv[0]);
        return 0;
    }
	
	if (atoi(argv[2]) < 1) {
		printf("error: n must be > 0\n");
		return 0;
	}
    
    //argument containers (& other var decl)
    int run_len;    
    int* runs;
    unsigned char* filename;
    unsigned char* img;
    int img_size;
        
    //get args into their places
    //filename
    filename = malloc(strlen(argv[1]));
    strcpy(filename, argv[1]);
    //run_len
    run_len = atoi(argv[2]);
    
    (!DEBUG)?: printf("file: %s\trun_len: %d\n", filename, run_len); 
    
    //setup buckets for run_len 
    int bucket_c = pow(2,run_len);   
    (!DEBUG)?: printf("bucket_c: %d\n", bucket_c);
    runs = malloc(bucket_c*sizeof(int));
    for (int i=0; i<bucket_c; i++) {
        runs[i]=0;
        (!DEBUG)?: printf("runs[%d] = %d\n", i, runs[i]);
    }
    
    //read in .ppm file
    FILE* fp = fopen(filename, "r");
    //check proper opening
    if (fp == NULL) {
        printf("error opening input file\n");
        return 0;
    }
    //get file size
    long file_size;
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    img_size = file_size;
    rewind(fp);
    //allocate buffer
    img = malloc(file_size+1);            
    //copy file to buffer
    fread(img, 1, file_size, fp);
    //close file
    fclose(fp);       
    
    //get pointer to data area of message and calc num pixels
    unsigned char* ptr = img;
    int width = getWidthFix(ptr);
    int height = getHeightFix(ptr);
	(!DEBUG)?: printf("width: %d\theight: %d\n", width, height);           
    int pixels = width * height * 3;	
    int count = 1;
    //get ptr to data area
    ptr = advanceToEmbeddingArea(ptr);    
	(!DEBUG)?: printf("pixels: %d\timg_size: %d\n", pixels, ((int)strlen(img)));
    //read through all pixels
    while(count < pixels) {                    
        //fill buckets with counts
        //which bucket? (builder)
        int b = 0;
        for (int i=run_len-1; i>=0; i--) {  
            //advance ptr to last 'digit'
            ptr+=2;        
            //get bit 
            (!DEBUG)?: printf("extracting from: [%c|%d]\t", *ptr, *ptr);
            int bit = *ptr & 0x1;
            (!DEBUG)?: printf("bit: %d\t", bit);
            //put bit in builder
            bit <<= i;
            (!DEBUG)?: printf("bit shifted(%d): %d\t", i, bit);
            b |= bit;
            (!DEBUG)?: printf("b: %d\tcount: %d/%d\n", b, count, pixels);            
            
            //save in bucket
			runs[b]++;
            
            //advance ptr to next chunk
			if (count<pixels) {
				ptr = advanceToNextChunk(ptr);
			}
            
            count++; //count of 'pixels' we have processed
        }        
    }    
    
	for (int i=0; i<bucket_c; i++) {
		//print bin of i
		for (int k=run_len-1; k>=0; k--) {
			int mask = 1 << k;
			int bit = i & mask;
			bit >>= k;
			printf("%d", bit);
		}
		printf(" %d\n", runs[i]);
	}
	
    return 0;
}