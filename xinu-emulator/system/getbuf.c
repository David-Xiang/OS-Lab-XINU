/* getbuf.c - getbuf */
/*
   Exercise1 of Lab7
   Modify the code relevant to buffer pool to enable their ability of 
   memory marking and self initialization.
   My understanding:
   Each time the system reboots, the buffer pools allocated stay in
   memory which can be utilized directly without reallocation. However
   the data inside the pool is invalid, thus each time the system
   reboots, the buffer pool need to emptied and semaphore needs to be 
   reinitialized -- that's what to do after checking the memory mark.
 */

#include <xinu.h>
static void poolinit(bpid32);

/*------------------------------------------------------------------------
 *  getbuf  -  Get a buffer from a preestablished buffer pool
 *------------------------------------------------------------------------
 */
char    *getbuf(
          bpid32        poolid          /* Index of pool in buftab	*/
        )
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	bpentry	*bpptr;		/* Pointer to entry in buftab	*/
	struct	bpentry	*bufptr;	/* Pointer to a buffer		*/

	mask = disable();

	/* Check arguments */

	if ( (poolid < 0  ||  poolid >= nbpools) ) {
		restore(mask);
		return (char *)SYSERR;

	}
	bpptr = &buftab[poolid];
	
	/* XDW: before allocating buf, make sure this pool is initialized  */
	//if (notmarked(bpptr->bploc)) 
	//	poolinit(poolid);

	/* Wait for pool to have > 0 buffers and allocate a buffer */

	wait(bpptr->bpsem);
	bufptr = bpptr->bpnext;

	/* Unlink buffer from pool */

	bpptr->bpnext = bufptr->bpnext;

	/* Record pool ID in first four bytes of buffer	and skip */

	*(bpid32 *)bufptr = poolid;
	bufptr = (struct bpentry *)(sizeof(bpid32) + (char *)bufptr);
	restore(mask);
	return (char *)bufptr;
}

void poolinit(bpid32 poolid){
	struct bpentry *bpptr;
	char* bufptr;

	intmask mask = disable();
	bpptr = &buftab[poolid];
	
	if (notmarked(bpptr->bploc)){
		int numbufs = bpptr->bpnum;
		int bufsize = bpptr->bpsize;
		// reinitialized semaphore
		semreset(bpptr->bpsem, numbufs);

		// add all buffers to freelist
		int i = 0;
		bufptr = (char *) bpptr;
		for (i = 0; i < numbufs; i++){
			bpptr = (struct bpentry *) bufptr;
			bufptr += bufsize;
			bpptr->bpnext = (struct bpentry *)bufptr;
		}
		bpptr = (struct bpentry *) bufptr;
		bpptr->bpnext = (struct bpentry *)NULL;

		mark(bpptr->bploc);
	}
	

	restore(mask);
	return;
}
