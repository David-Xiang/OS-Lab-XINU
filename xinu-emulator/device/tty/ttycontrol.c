/* ttycontrol.c - ttycontrol */
/*
   Exercise4 of Lab4
   Implement a control function allowing process to acquire control of 
   CONSOLE and another control function allowing process to release 
   control of it.
   I modified the original ttycontrol and add 2 func to it which are:
   1. TC_WAIT
   2. TC_SIGNAL
   Which process now possesses control of CONSOLE is recorded in ttytab
   ONLY this processs can call read/write/putc... on CONSOLE
 */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttycontrol  -  Control a tty device by setting modes
 *------------------------------------------------------------------------
 */
devcall	ttycontrol(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  int32	 func,			/* Function to perform		*/
	  int32	 arg1,			/* Argument 1 for request	*/
	  int32	 arg2			/* Argument 2 for request	*/
	)
{
	struct	ttycblk	*typtr;		/* Pointer to tty control block	*/
	char	ch;			/* Character for lookahead	*/

	typtr = &ttytab[devptr->dvminor];

	/* Process the request */

	switch ( func )	{

	case TC_NEXTC:
		wait(typtr->tyisem);
		ch = *typtr->tyitail;
		signal(typtr->tyisem);
		return (devcall)ch;

	case TC_MODER:
		typtr->tyimode = TY_IMRAW;
		return (devcall)OK;

	case TC_MODEC:
		typtr->tyimode = TY_IMCOOKED;
		return (devcall)OK;

	case TC_MODEK:
		typtr->tyimode = TY_IMCBREAK;
		return (devcall)OK;

	case TC_ICHARS:
		return(semcount(typtr->tyisem));

	case TC_ECHO:
		typtr->tyiecho = TRUE;
		return (devcall)OK;

	case TC_NOECHO:
		typtr->tyiecho = FALSE;
		return (devcall)OK;

	case TC_WAIT:
		if (devptr->dvnum == CONSOLE){
			wait(typtr->tysem);
			typtr->typid = getpid();
		}
		return (devcall)OK;

	case TC_SIGNAL:
		if (devptr->dvnum == CONSOLE && getpid() == typtr->typid){
			signal(typtr->tysem);
			typtr->typid = -1;
		}
		return (devcall)OK;

	default:
		return (devcall)SYSERR;
	}
}
