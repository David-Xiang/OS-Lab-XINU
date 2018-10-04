/* sleep.c - sleep sleepms */

#include <xinu.h>

#define	MAXSECONDS	4294967		/* max seconds per 32-bit msec	*/
int32	slnonempty;			/* is sleepq empty?		*/
int32	*sltop;				/* ptr to first key on sleepq	*/

/*------------------------------------------------------------------------
 *  sleep  -  Delay the calling process n seconds
 *------------------------------------------------------------------------
 */
syscall	sleep(
	  uint32	delay		/* time to delay in seconds	*/
	)
{
	if (delay > MAXSECONDS) {
		return(SYSERR);
	}
	sleepms(1000*delay);
	return OK;
}

/*------------------------------------------------------------------------
 *  sleepms  -  Delay the calling process n milliseconds
 *------------------------------------------------------------------------
 */
syscall	sleepms(
	  uint32	delay		/* time to delay in msec.	*/
	)
{
	intmask	mask;			/* saved interrupt mask		*/

	mask = disable();
	if (delay == 0) {
		yield();
		restore(mask);
		return OK;
	}

	/* Delay calling process */

	if (insertd(currpid, sleepq, delay) == SYSERR) {
		restore(mask);
		return SYSERR;
	}
	sltop = &queuetab[firstid(sleepq)].qkey;
	slnonempty = TRUE;
	proctab[currpid].prstate = PR_SLEEP;
	resched();
	restore(mask);
	return OK;
}
