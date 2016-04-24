all: 
	gcc -std=c99 -o enc enc.c -lcrypto
	gcc -std=c99 -o dec dec.c -lcrypto
	gcc -std=c99 -o emb emb.c sppm.c
	gcc -std=c99 -o ext ext.c sppm.c
	gcc -std=c99 -o encemb encemb.c
	gcc -std=c99 -o extdec extdec.c
	gcc -std=c99 -o test test.c
