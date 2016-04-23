all: 
	gcc -std=c99 -o enc enc.c -lcrypto
	gcc -std=c99 -o dec dec.c -lcrypto
	gcc -std=c99 -o emb emb.c sppm.c
	gcc -std=c99 -o ext ext.c sppm.c
enc: enc.c
	gcc -std=c99 -o enc enc.c -lcrypto
dec: dec.c
	gcc -std=c99 -o dec dec.c -lcrypto
emb: emb.c sppm.c
	gcc -std=c99 -o emb emb.c sppm.c
emb: ext.c sppm.c
	gcc -std=c99 -o ext ext.c sppm.c
