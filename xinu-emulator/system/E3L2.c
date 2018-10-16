#include <xinu.h>
#include <custom.h>
#include <stdio.h>
#include <stdlib.h>

/*
	Exercise3 of Lab2
	Design a programm that allocates memmory from an generated buffer
	pool.
	Use a loop to request 1024 bytes each second from buffer pool until
	the memory in the pool runs out.
	Print addr of buffer pool and memory usage and display what happens
	when resources run out.
*/
static void printBuf(bpid32 poolid);

void E3L2(){
	bpid32 poolid = mkbufpool(1024, 10);
	if (poolid == SYSERR){
		kprintf("Error in requesting buffer pool!\n");
		return;
	}

	int i = 0;
	for ( ; ; i++){
		getbuf(poolid);
		printBuf(poolid);
	}
	return;
}

static void printBuf(bpid32 poolid){
	intmask mask = disable();

	struct bpentry *bpptr = &buftab[poolid];
	struct bpentry *currbuf = bpptr->bpnext;

	kprintf("Current avaible buffers:\n");
	while(currbuf != NULL){
		kprintf("0x%8X\n", currbuf);
		currbuf = currbuf->bpnext;
	}

	restore(mask);
	return;
}
