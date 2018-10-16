#include <xinu.h>
#include <custom.h>
#include <stdio.h>
#include <stdlib.h>

/*
	Exercise2 of Lab2
	Design a program to display the difference of freestk and freemem
*/
void E2L2(){
	kprintf("Traverse all free memmory blocks...\n");
	printBlks();
	
	kprintf("Call getstk(1024):\n");
	char *stk = getstk(1024);
	printBlks();

	kprintf("Call freestk(stk, 1024):\n");
	freestk(stk, 1024);
	printBlks();

	kprintf("Call getmem(1024):\n");
	char *blk = getmem(1024);
	printBlks();

	kprintf("Call freemem(blk, 1024):\n");
	freemem(blk, 1024);
	printBlks();
	return;
}
