/* xsh_rfstest.c - xsh_rfstest */
/*
 	Exercise3 of Lab6
	Modify remote file server and rflclose.c so that every time a process
	close a remote file rflclose will send a message to server and receive
	response from it.
	THIS file is a commmand to test whether remote file system works.
 */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_rfstest - shell cmd to test whether remote file system works
 *------------------------------------------------------------------------
 */
shellcmd xsh_rfstest(int nargs, char *args[])
{
	int32 fd = open(RFILESYS, "file0", "rw");
	if (fd == SYSERR){
		kprintf("Error in opening file0\n");
		return 0;
	}else{
		kprintf("Success opening file0.\n");
	}

	return 0;
}
