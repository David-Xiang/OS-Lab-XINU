/* freestk.c - freestk */
/*
   Exercise4 of Lab2
   Implement a stack protection machanism to prevent process's stack from
   illegal access of other process.
  
   My idea:
   1. Split stack space and heap space. Maintain 2 pointers individually
   pointing to bottom of stack space and top of heap space. When 
   allocating memory to a stack or a heap, always make sure:
   					heaptop <= stkbtm
   2. Add a random-size buffer area to each stack and heap.
   
   Implementation:
   modify getmem(), getstk(), freemem() and freestk().
	
 */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  freestk -  Free stack memmory allocate by getstk
 *------------------------------------------------------------------------
 */
syscall	freestk(
	  char		*stkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/

	// actual blkaddr = stkaddr - roundmb(nbytes) + sizeof(uint32)
	char *blkaddr = stkaddr - nbytes + sizeof(uint32);
	/* XDW: blkaddr and stkaddr are different  */ 
	
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*next, *prev, *block;
	uint32	top;

	mask = disable();
	if ((nbytes == 0) || ((uint32) blkaddr < (uint32) minheap)
			  || ((uint32) blkaddr > (uint32) maxheap)) {
		restore(mask);
		return SYSERR;
	}

	block = (struct memblk *)blkaddr;

	prev = &memlist;			/* Walk along free list	*/
	next = memlist.mnext;
	while ((next != NULL) && (next < block)) {
		prev = next;
		next = next->mnext;
	}

	if (prev == &memlist) {		/* Compute top of previous block*/
		top = (uint32) NULL;
	} else {
		top = (uint32) prev + prev->mlength;
	}

	/* Ensure new block does not overlap previous or next blocks	*/

	if (((prev != &memlist) && (uint32) block < top)
	    || ((next != NULL)	&& (uint32) block+nbytes>(uint32)next)) {
		restore(mask);
		return SYSERR;
	}

	memlist.mlength += nbytes;

	/* Either coalesce with previous block or add to free list */

	if (top == (uint32) block) { 	/* Coalesce with previous block	*/
		prev->mlength += nbytes;
		block = prev;
		
	} else {			/* Link into list as new node	*/
		block->mnext = next;
		block->mlength = nbytes;
		prev->mnext = block;
	}

	/* Coalesce with next block if adjacent */

	if (((uint32) block + block->mlength) == (uint32) next) {
		/* XDW: this block should coalesce with next free block */
		if ((void *)block == stkbtm){
		/* XDW: update stkbtm  */
			//kprintf("stkbtm goes up from 0x%08X to 0x%08X\n", 
			//	stkbtm, (void*)next + next->mlength);
			stkbtm = (void *)next + next->mlength;
		}
		block->mlength += next->mlength;
		block->mnext = next->mnext;
	}else{
		if ((void *) block == stkbtm){
			/* XDW: no coalescence, just update stkbtm  */
			//kprintf("stkbtm goes up from 0x%08X to 0x%08X\n", 
			//	stkbtm, (void*)block + block->mlength);
			stkbtm = (void *)block + block->mlength;
		}
	}
	restore(mask);
	return OK;
}
