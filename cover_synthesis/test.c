#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char cmd[256];
    int r;
    
    //test emb/ext self undo   
    strcpy(cmd, "./synth -m tst/msg0.txt -o tst/song0.wav");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./ext -i tst/song0.wav -o tst/msg0.out"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg0.txt tst/msg0.out");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");
    
    strcpy(cmd, "./synth -m tst/msg1.txt -o tst/song1.wav");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./ext -i tst/song1.wav -o tst/msg1.out"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg1.txt tst/msg1.out");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");
    
    strcpy(cmd, "./synth -m tst/msg2.txt -o tst/song2.wav");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./ext -i tst/song2.wav -o tst/msg2.out"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg2.txt tst/msg2.out");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");

    
    
    return 0;
}