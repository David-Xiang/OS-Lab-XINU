#include <xinu.h>
#include <custom.h>
#include <stdio.h>
#include <stdlib.h>

/*
	Exercise1 of Lab2
	Traverse all free memory blocks and print all blocks' addr and length.
	Request a 6000 bytes' memory block and then print all blocks' addr 
	and length.
	Display what will happen in above procedure when disabling interrupts
	and enabling interrupts.
*/
void printBlks();

void E1L2(){
	kprintf("\nTraverse all free memory blocks...\n");
	printBlks();
	kprintf("\nNow request 6000 bytes and free mem blocks become:\n");
	char* blk = getmem(6000);
	printBlks();
	freemem(blk, 6000);

	kprintf("\nLet's disable interrupts and do it again:\n");
	intmask mask = disable();
	printBlks();
	kprintf("\nNow request 6000 bytes and free mem blocks become:\n");
	blk = getmem(6000);
	printBlks();
	freemem(blk, 6000);
	restore(mask);

	return;
}

void printBlks(){
	struct memblk *curr;
	curr = memlist.mnext;
	
	int count = 0;
	
	// printf a table of index, addr and size
	kprintf("%-5s %-10s %-8s\n", "index", "addr", "size");
	kprintf("%-5s %-10s %-8s\n", "-----", "----------", "--------");
	while(curr != NULL){
		kprintf("%5d 0x%08X %8d\n", count++, curr, curr->mlength);
		curr = curr->mnext;
	}
	kprintf("-------------------------\n");
	kprintf("%25d\n", memlist.mlength);
	return;
}
