/* xsh_read.c - xsh_syncread & xsh_asyncread */
/* Exercise1 of Lab4
   Design a program to demonstrate the difference between 
   synchronous IO and asynchorous IO.
 */
#include <xinu.h>
#include <stdio.h>
void readworker(pid32 pid, char* buffer, int count);

/*------------------------------------------------------------------------
 * xhs_syncread - synchronous version of read()
 *------------------------------------------------------------------------
 */
shellcmd xsh_syncread(int32 nargs, char *args[])
{
	char buffer[1000] = "";
	kprintf("syncread called!\n");
	kprintf("please enter a line of sentences and PRESS ENTER:\n");
	read(CONSOLE, buffer, 999);
	kprintf("%s", buffer);
	kprintf("syncread returns!\n");
	return 0;
}


/*------------------------------------------------------------------------
 * xhs_asyncread - asynchronous version of read()
 *------------------------------------------------------------------------
 */
shellcmd xsh_asyncread(int32 nargs, char* args[]){
	char buffer[1000] = "";
	kprintf("asyncread called!\n");
	kprintf("creating a readworker.\n");
	resume(create(readworker, 8192, 50, 
				"readworker", 3, getpid(), buffer, 1000));
	while(recvclr() == OK){
		kprintf("waiting for readworkder to finish.\n");
		sleep(2);
	}
	kprintf("readworker: %s", buffer);
	kprintf("asyncread finished!\n");
	return 0;
}

void readworker(pid32 pid, char* buffer, int count){
	read(CONSOLE, buffer, count);
	send(pid, 'f');
	return;
}
