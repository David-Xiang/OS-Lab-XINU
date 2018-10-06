/* 
   xsh_psx.c - xsh_psx 
   Exercise1 of OS Lab1:
   Design a process to display all process and their attributes:
   Pid, Name, State, Prio, Ppid, StackBase, StackPtr, StackSize,
   SemWait, MsgRecv, Descriptor 
   (last line's stuffs are hard to display...)
   I modified xsh_ps.c to add a new built-in commmand -- psx.
   psx means it's enhanced version of command ps.
 */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------
 * xsh_ps - shell command to print the process table
 *------------------------------------------------------------------------
 */
shellcmd xsh_psx(int nargs, char *args[])
{
	struct	procent	*prptr;		/* pointer to process		*/
	int32	i;			/* index into proctabl		*/
	char *pstate[]	= {		/* names for process states	*/
		"FREE", "CURR", "READY", "RECV", "SLEEP", "SUSP", "WAIT", "RECTIM"
	};

	/* For argument '--help', emit help about the 'ps' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tDisplays information about running processes\n");
		printf("Options:\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs > 1) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}

	/*-------------- The Modification starts here ----------------- */

	/* Print header for items from the process table */

	printf("%-3s %-16s %-5s %-4s %-4s %-10s %-10s %-9s %-7s %-7s %-20s\n",
			"Pid", "Name", "State", "Prio", "Ppid", "StackBase","StackPtr",
			"StackSize", "SemWait", "MsgRecv", "Descriptor");

	printf("%3s %16s %5s %4s %4s %10s %10s %9s %7s %7s %10s\n",
			"---", "----------------", "-----", "----", "----",
			"----------", "----------", "---------", "-------",
			"-------", "--------------------");

	/* Output information for each process */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		if (prptr->prstate == PR_FREE) {  /* skip unused slots	*/
			continue;
		}
		printf("%3d %-16s %-5s %4d %4d 0x%08X 0x%08X %9d ",
			i, prptr->prname, pstate[(int)prptr->prstate],
			prptr->prprio, prptr->prparent, prptr->prstkbase,
			prptr->prstkptr, prptr->prstklen);

		// print semaphore on which process waits
		if (prptr->prstate == PR_WAIT){
			printf("%7d ", prptr->prsem);
		}else{
			printf("%7s ", "NA");
		}

		// print message sent to this process
		if (prptr->prhasmsg == TRUE){
			printf("%7d ", prptr->prmsg);
		}else{
			printf("%7s ", "NA");
		}

		// print descriptors for the process
		int j;
		for (j = 0; j < NDESC; j++){
			if (j < NDESC - 1)
				printf("%d/", prptr->prdesc[j]);
			else
				printf("%d\n", prptr->prdesc[j]);
		}
	}
	return 0;
}
