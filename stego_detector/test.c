#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include "chisqr.h"
#include "gamma.h"

#define DEBUG 1

int main(int argc, char ** argv) {
        //check proper usage
    if (argc != 4) {
        printf("usage: %s <image filename> <n> <model filename>\n", argv[0]);
        return 0;
    }
    
    if (atoi(argv[2]) < 1) {
        printf("error: n must be > 0\n");
        return 0;
    }
    
    //argument containers (& other var decl)
    int run_len;    
    float* model_runs;
    int* test_runs;
    unsigned char* img_filename;
    unsigned char* img_file;
    unsigned char* model_filename;        
    unsigned char* model_file;        
        
    //get args into their places
    //img filename
    img_filename = malloc(strlen(argv[1]));
    strcpy(img_filename, argv[1]);
    //run_len
    run_len = atoi(argv[2]);
    //model filename
    model_filename = malloc(strlen(argv[3]));
    strcpy(model_filename, argv[3]);
    
    (!DEBUG)?: printf("img_filename: %s\trun_len: %d\tmodel_filename: %s\n", img_filename, run_len, model_filename); 
    
    //setup buckets for run_len (model data)
    int bucket_c = pow(2,run_len);   
    (!DEBUG)?: printf("bucket_c: %d\n", bucket_c);
    model_runs = malloc(bucket_c*sizeof(int));
    test_runs = malloc(bucket_c*sizeof(int));
    for (int i=0; i<bucket_c; i++) {
        model_runs[i]=0;
        test_runs[i]=0;
        (!DEBUG)?: printf("model runs[%d] = %f\n", i, model_runs[i]);        
    }       
    
    //read in model data
    FILE* fp = fopen(model_filename, "r");
    //check proper opening
    if (fp == NULL) {
        printf("error opening input file: %s\n", model_filename);
        return 0;
    }                               
    //get file size
    long file_size;
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);  
    rewind(fp);
    //allocate buffer
    model_file = malloc(file_size+1);            
    //copy file to buffer
    fread(model_file, 1, file_size, fp);
    //close file*/
    fclose(fp);         
    //process lines
    int pos = 0;
    for (int i=0; i<pow(2,run_len); i++) {
        int bucket = 0; //bucket builder
        float val = 0; //bucket value
        
        //process bucket
        for (int j=run_len-1; j>=0; j--) {
            unsigned char c = model_file[pos++];
            //(!DEBUG)?: printf("c: %c\n", c);
            if (c == '1') {
                int mask = 1 << j;
                bucket |= mask;
            }
        }       
        //skip space
        pos++;      
        //process val & save    
        val = atof(&model_file[pos]);       
        //printf("v: %f\n", model_file[pos]);
        model_runs[bucket] = val;               
        //skip to next line
        while(model_file[pos] != '\n') {                    
            pos++;
        }
        pos++;      
        (!DEBUG)?: printf("model bucket: %d\tval: %f\n", bucket, model_runs[bucket]);
    }
            
    //generate test data
    char cmd[256];
    strcpy(cmd, "./count ");
    strcat(cmd, img_filename);
    strcat(cmd, " ");
    char buf[10];
    snprintf(buf, 10, "%d", run_len);
    strcat(cmd, buf);
    strcat(cmd, " > test.model");
    (!DEBUG)?: printf("cmd: %s\n", cmd);
    system(cmd);
        
    //read in test data
    fp = fopen("test.model", "r");
    //check proper opening
    if (fp == NULL) {
        printf("error opening input file: %s\n", img_filename);
        return 0;
    }                               
    //get file size
    //long file_size;
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);  
    rewind(fp);
    //allocate buffer
    img_file = malloc(file_size+1);            
    //copy file to buffer
    fread(img_file, 1, file_size, fp);
    //close file*/
    fclose(fp);         
    //process lines
    pos = 0;
    for (int i=0; i<pow(2,run_len); i++) {
        int bucket = 0; //bucket builder
        float val = 0; //bucket value
        
        //process bucket
        for (int j=run_len-1; j>=0; j--) {
            unsigned char c = img_file[pos++];
            //(!DEBUG)?: printf("c: %c\n", c);
            if (c == '1') {
                int mask = 1 << j;
                bucket |= mask;
            }
        }       
        //skip space
        pos++;      
        //process val & save    
        val = atof(&img_file[pos]);     
        //printf("v: %f\n", model_file[pos]);
        test_runs[bucket] = val;                
        //skip to next line
        while(img_file[pos] != '\n') {                  
            pos++;
        }
        pos++;      
        (!DEBUG)?: printf("test bucket: %d\tval: %d\n", bucket, test_runs[bucket]);
    }       
            
            
    //delete test.model
    system("rm test.model");    
        
    //compute chi sq test
    int dof = run_len-1;
    float xsqr = 0;
    unsigned int total = 0;
    //calc total observed bits
    for (int i=0; i<run_len; i++) {
        total += test_runs[i];
    }   
    //compute chisqr
    for (int i=0; i<run_len; i++) {
        xsqr += pow(((float)test_runs[i])-(model_runs[i]*(float)total),2)/(model_runs[i]*(float)total);
    }   
    printf("xsqr: %f\ttotal: %d\n", xsqr, total);   
    
    /*
    //OPTION 2
    //http://www.codeproject.com/Articles/432194/How-to-Calculate-the-Chi-Squared-P-Value
    //public domain
    float* Expected = model_runs;
    int* Observed = test_runs; 
    double CriticalValue = 0.0;
    double XSqr;     
    unsigned int total = 0;
    for (int i=0; i<run_len; i++) {
        total += test_runs[i];
    }   
    for(int I = 0; I < run_len; I++) {
        XSqr = (float)Observed[I] - (Expected[I]*total);
        CriticalValue += ((XSqr * XSqr) / (Expected[I]*total));
    }            
    printf("XSqr: %f\tCV: %f\ttotal: %d\n", XSqr, CriticalValue, total);    
    double p = chisqr(total-1,CriticalValue);   
    printf("p: %f\n", p);
    */
    
    return 0;
}