/* lflclose.c - lflclose.c */
/*
   Exercise2 of Lab6
 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * lflclose  -  Close a file by flushing output and freeing device entry
 *------------------------------------------------------------------------
 */
devcall	lflclose (
	  struct dentry *devptr		/* Entry in device switch table */
	)
{
	int i;
	struct	lflcblk	*lfptr;		/* Ptr to open file table entry	*/
	struct	procent *prptr;
	prptr = &proctab[getpid()];

	/* Obtain exclusive use of the file */

	lfptr = &lfltab[devptr->dvminor];
	wait(lfptr->lfmutex);

	/* If file is not open, return an error */

	if (lfptr->lfstate != LF_USED) {
		signal(lfptr->lfmutex);
		return SYSERR;
	}

	/* If file is not opened by current process, return an error */
	bool8 found = FALSE;
	int slot;
	for (i = 0; i < NDESC; i++){
		if (prptr->prdesc[i] == devptr->dvminor){
			found = TRUE;
			slot = i;
			break;
		}
	}
	if (!found){
		signal(lfptr->lfmutex);
		return SYSERR;
	}

	/* Write index or data blocks to disk if they have changed */

	if (Lf_data.lf_dirdirty || lfptr->lfdbdirty || lfptr->lfibdirty) {
		lfflush(lfptr);
	}

	/* Set device state to FREE and return to caller */

	lfptr->lfstate = LF_FREE;

	/* Free the slot in prptr->prdesc */
	prptr->prdesc[slot] = -1;

	signal(lfptr->lfmutex);
	return OK;
}
