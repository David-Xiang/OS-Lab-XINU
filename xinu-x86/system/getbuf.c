/* getbuf.c - getbuf, nbgetbuf */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getbuf  --  get a buffer from a preestablished buffer pool
 *------------------------------------------------------------------------
 */
char    *getbuf(
          bpid32        poolid          /* index of pool in buftab       */
        )
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	bpentry	*bpptr;		/* pointer to entry in buftab	*/
	struct	bpentry	*bufptr;	/* pointer to a buffer		*/

	mask = disable();

	/* Check arguments */

	if ( (poolid < 0  ||  poolid >= nbpools) ) {
		restore(mask);
		return (char *)SYSERR;

	}
	bpptr = &buftab[poolid];

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

/*------------------------------------------------------------------------
 * nbgetbuf - a non-blocking version of getbuf
 *------------------------------------------------------------------------
 */
char	*nbgetbuf(
          bpid32        poolid          /* index of pool in buftab       */
	)
{
	intmask	mask;
	char	*buf;

	mask = disable();

	/* Check arguments */

	if ( (poolid < 0  ||  poolid >= nbpools) ) {
		restore(mask);
		return (char *)SYSERR;

	}

	/* If the call will block, return an error */

	if (semcount(buftab[poolid].bpsem) <= 0) {
		restore(mask);
		return (char *)SYSERR;
	}
	buf = getbuf(poolid);
	restore(mask);
	return buf;
}
