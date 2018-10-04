/* clkinit.c - clkinit */

#include <xinu.h>
#include <interrupt.h>
#include <clock.h>

uint32	clktime;		/* seconds since boot			*/
uint32	ctr1000 = 0;		/* milliseconds since boot		*/
qid16	sleepq;			/* queue of sleeping processes		*/
int32	slnempty;		/* zero if the sleep queue is empty;	*/
				/*   non-zero otherwise			*/
int32	*sltop;			/* ptr to key in first entry of sleepq	*/
				/*   if sleepq is not empty		*/
uint32	preempt;		/* preemption counter			*/
   
/*------------------------------------------------------------------------
 * clkinit - initialize the clock and sleep queue at startup
 *------------------------------------------------------------------------
 */
void	clkinit(void)
{
	uint16	intv;			/* clock rate in KHz		*/

	/* Set interrupt vector for clock to invoke clkint */

	set_evec(IRQBASE, (uint32)clkint);

	/* clock rate is 1.190 Mhz; this is 1 ms interrupt rate */

	intv = 1193;	/* using 1193 instead of 1190 to fix clock skew	*/

	sleepq = newqueue();	/* allocate a queue to hold the delta	*/
				/* list of sleeping processes		*/
	preempt = QUANTUM;	/* initial time quantum			*/

	/* Specify that seepq is initially empty */

	slnonempty = FALSE;

	clktime = 0;		/* start counting seconds		*/

	/*  set to: timer 0, 16-bit counter, rate generator mode,
		counter is binary */
	outb(CLKCNTL, 0x34);

	/* must write LSB first, then MSB */
	outb(CLOCK0, (char) (0xff & intv) );
	outb(CLOCK0, (char) (0xff & (intv>>8)));
	return;
}
