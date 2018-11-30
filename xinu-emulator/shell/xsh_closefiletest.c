/* xsh_closefiletest.c - xsh_closefiletest */
/*
   Exercise2 of Lab6
   Implement a feature which is closing all devices opened by a process
   when this process is being closed (or killed).
 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xsh_closefiletest
 *------------------------------------------------------------------------
 */
static void closefiletest();

shellcmd xsh_closefiletest(int nargs, char *args[])
{
	resume(create(closefiletest, 8192, 80, "closefiletest", 0));
	return 0;
}

void closefiletest(){
	int32 id;

	id = open(LFILESYS, "FILE0", "w");
	if (id != SYSERR){
		kprintf("successfully opened FILE0 (id = %d).\n", id);
	}

	id = open(LFILESYS, "FILE1", "w");
	if (id != SYSERR){
		kprintf("successfully opend FILE1 (id = %d).\n", id);
	}

	return;
}
