#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char cmd[256];
    int r;
    
    //test enc/dec self undo   
    strcpy(cmd, "./enc -i tst/msg0.txt -o tst/msg0.crypto -p fish");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./dec -i tst/msg0.crypto -o tst/msg0.plain -p fish"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg0.txt tst/msg0.plain");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");

    strcpy(cmd, "./enc -i tst/msg1.txt -o tst/msg1.crypto -p fish");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./dec -i tst/msg1.crypto -o tst/msg1.plain -p fish"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg1.txt tst/msg1.plain");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");
    
    strcpy(cmd, "./enc -i tst/msg2.txt -o tst/msg2.crypto -p fish");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./dec -i tst/msg2.crypto -o tst/msg2.plain -p fish"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg2.txt tst/msg2.plain");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");
    
    //test emb/ext self undo   
    strcpy(cmd, "./emb -c covers/logo.ppm -m tst/msg0.txt -s tst/msg0.ppm");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./ext -s tst/msg0.ppm -m tst/msg0.plain2"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg0.txt tst/msg0.plain2");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");
    
    strcpy(cmd, "./emb -c covers/logo.ppm -m tst/msg1.txt -s tst/msg1.ppm");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./ext -s tst/msg1.ppm -m tst/msg1.plain2"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg1.txt tst/msg1.plain2");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");
    
    strcpy(cmd, "./emb -c covers/logo.ppm -m tst/msg2.txt -s tst/msg2.ppm");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./ext -s tst/msg2.ppm -m tst/msg2.plain2"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg2.txt tst/msg2.plain2");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");
    
    //test encemb/extdec self undo        
    strcpy(cmd, "./encemb -c covers/logo.ppm -m tst/msg0.txt -s tst/msg0.ppm -p fish");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./extdec -s tst/msg0.ppm -m tst/msg0.plain3 -p fish"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg0.txt tst/msg0.plain3");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");
    
    strcpy(cmd, "./encemb -c covers/logo.ppm -m tst/msg1.txt -s tst/msg1.ppm -p fish");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./extdec -s tst/msg1.ppm -m tst/msg1.plain3 -p fish"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg1.txt tst/msg1.plain3");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");
    
    strcpy(cmd, "./encemb -c covers/logo.ppm -m tst/msg2.txt -s tst/msg2.ppm -p fish");
    printf("%s\n", cmd);
    system(cmd);
    strcpy(cmd, "./extdec -s tst/msg2.ppm -m tst/msg2.plain3 -p fish"); 
    printf("%s\n", cmd);
    system(cmd);
    r = system("diff tst/msg2.txt tst/msg2.plain3");
    (!r) ? printf("pass\n\n") : printf("fail\n\n");
           
    return 0;
}