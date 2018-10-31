/* getstk.c - getstk */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getstk  -  Allocate stack memory, returning highest word address
 *------------------------------------------------------------------------
 */
char  	*getstk(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr;	/* Walk through memory list	*/
	struct	memblk	*fits, *fitsprev; /* Record block that fits	*/

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use mblock multiples	*/

	prev = &memlist;

	/* XDW: searching from heaptop instead of memlist.mnext */
	curr = memlist.mnext;
	//curr = ((struct memblk*)heaptop)->mnext;
	
	fits = NULL;
	fitsprev = NULL;  /* Just to avoid a compiler warning */

	while (curr != NULL) {			/* Scan entire list	*/
		if (curr->mlength >= nbytes) {	/* Record block address	*/
			fits = curr;		/*   when request fits	*/
			fitsprev = prev;
		}
		prev = curr;
		curr = curr->mnext;
	}

	if (fits == NULL) {			/* No block was found	*/
		restore(mask);
		return (char *)SYSERR;
	}



	if (nbytes == fits->mlength) {		/* Block is exact match	*/
		/* XDW: check whether the emtpy block is beyond heaptop  */
		if ((void *)fits < heaptop){
			kprintf("getstk: fits < heaptop: fits=0x%08X heaptop=0x%08X\n",
				fits, heaptop);
			restore(mask);
			return (char *)SYSERR;
		}

		fitsprev->mnext = fits->mnext;
	} else {				/* Remove top section	*/
		struct memblk *stkbase = 
			(struct memblk *)((uint32)fits + fits->mlength - nbytes);
		
		/* XDW: check whether the stkbase is beyond headtop */
		if ((void *) stkbase < heaptop){
			kprintf("getstk: stkbase < heaptop: fits=0x%08X heaptop=0x%08X\n",
				stkbase, heaptop);
			restore(mask);
			return (char *)SYSERR;
		}

		fits->mlength -= nbytes;
		fits = stkbase;
	}
	memlist.mlength -= nbytes;

	/* XDW: update stkbtm */
	if ((void *)fits < stkbtm){
		kprintf("stkbtm goes down from 0x%08X to 0x%08X\n", stkbtm, fits);
		stkbtm = fits;
	}

	restore(mask);
	return (char *)((uint32) fits + nbytes - sizeof(uint32));
}
