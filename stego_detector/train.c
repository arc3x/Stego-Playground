#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include "../encrypted_stego/sppm.h"

#define DEBUG 0

int main(int argc, char ** argv) {
    //check proper usage
    if (argc != 3) {
        printf("usage: %s <diretory> <n>\n", argv[0]);
        return 0;
    }
	
	if (atoi(argv[2]) < 1) {
		printf("error: n must be > 0\n");
		return 0;
	}
    
    //argument containers (& other var decl)
    int run_len;    
    int* runs;
	long total = 0;
    unsigned char* dirname;        
	int file_count = 0;	
	unsigned char* file;
        
    //get args into their places
    //dirname
    dirname = malloc(strlen(argv[1]));
    strcpy(dirname, argv[1]);
    //run_len
    run_len = atoi(argv[2]);
    
    (!DEBUG)?: printf("dir: %s\trun_len: %d\n", dirname, run_len); 
	
	//setup buckets for run_len 
    int bucket_c = pow(2,run_len);   
    (!DEBUG)?: printf("bucket_c: %d\n", bucket_c);
    runs = malloc(bucket_c*sizeof(int));
    for (int i=0; i<bucket_c; i++) {
        runs[i]=0;
        (!DEBUG)?: printf("runs[%d] = %d\n", i, runs[i]);
    }
    
	//count files in dir & process into .partial_model files
	struct dirent *dp;
	DIR *fd;
	if ((fd = opendir(dirname)) == NULL) {
		fprintf(stderr, "error: can't open %s\n", dirname);
		return 0;
	}
	char cmd[512];
	while ((dp = readdir(fd)) != NULL) {
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") || strstr(dp->d_name, ".partial_model"))
			continue;    /* skip self and parent */
		(!DEBUG)?: printf("%s/%s\n", dirname, dp->d_name);
		strcpy(cmd, "./count ");
		strcat(cmd, dirname);
		strcat(cmd, "/");
		strcat(cmd, dp->d_name);
		strcat(cmd, " ");
		char buf[10];
		snprintf(buf, 10, "%d", run_len);
		strcat(cmd, buf);
		strcat(cmd, " > ");		
		strcat(cmd, dirname);
		strcat(cmd, "/");
		strcat(cmd, dp->d_name);
		strcat(cmd, ".partial_model");
		(!DEBUG)?: printf("cmd: %s\n", cmd);
		system(cmd);
		
		file_count++;
	}
	closedir(fd);
	(!DEBUG)?: printf("files: %d\n", file_count); 
	
	//read all partial models and combine into total probabilities
	//struct dirent *dp;
	//DIR *fd;
	if ((fd = opendir(dirname)) == NULL) {
		fprintf(stderr, "error: can't open %s\n", dirname);
		return 0;
	}	
	while ((dp = readdir(fd)) != NULL) {
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
			continue;    /* skip self and parent */
		//for each partial model
		if (strstr(dp->d_name, ".partial_model") != NULL) {
			char path[512];
			strcpy(path, dirname);
			strcat(path, "/");
			strcat(path, dp->d_name);
			FILE* fp = fopen(path, "r");
			//check proper opening
			if (fp == NULL) {
				printf("error opening input file: %s\n", path);
				return 0;
			}						
			
			//read in partial model
			//get file size
			long file_size;
			fseek(fp, 0, SEEK_END);
			file_size = ftell(fp);	
			rewind(fp);
			//allocate buffer
			file = malloc(file_size+1);            
			//copy file to buffer
			fread(file, 1, file_size, fp);
			//close file*/
			fclose(fp);       
			
			//process lines
			int pos = 0;
			for (int i=0; i<pow(2,run_len); i++) {
				int bucket = 0; //bucket builder
				int val = 0; //bucket value
				
				//process bucket
				for (int j=run_len-1; j>=0; j--) {
					unsigned char c = file[pos++];
					(!DEBUG)?: printf("c: %c\n", c);
					if (c == '1') {
						int mask = 1 << j;
						bucket |= mask;
					}
				}
				
				//skip space
				pos++;
				
				//process val & save
				val = atoi(&file[pos]);
				runs[bucket] += val;
				total += val;
				
				//skip to next line
				while(file[pos] != '\n') {					
					pos++;
				}
				pos++;
				
				(!DEBUG)?: printf("bucket: %d\tval: %d\ttotal: %d\n", bucket, val, (int)total);
			}
			
			
			//free memory
			free(file);
		}
		
		
	}
	closedir(fd);
	
	//compute probability
	for (int i=0; i<pow(2,run_len); i++) {				
		for (int j=run_len-1; j>=0; j--) {
			int mask = 1 << j;
			printf("%d", (i&mask)>>j);
		}
		printf(" %f\n", (float)runs[i]/(float)total);
	}
	
    return 0;
}