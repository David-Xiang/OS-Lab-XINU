/* xsh_trycatchtest.c - xsh_trycatchtest */
/*
   Exercise2 of Lab7
   Implement a mechanism which allows a process to catch exceptions 
   occuried in its execution.
 */

#include <xinu.h>
#define try setjmp(&&out);		
#define catch out:
#define endtry ; 


/*------------------------------------------------------------------------
 * xsh_trycatchtest
 *------------------------------------------------------------------------
 */
shellcmd xsh_trycatchtest(int nargs, char *args[])
{
	kprintf("Try-Catch test:\n");
	kprintf("Execute int a = 1 / 0;\n");
	
	try{
		int a = 1/0;
	}catch{
		kprintf("Divided-By-Zero Exception is captured!\n");
		exit();
	}
	endtry

	return 0;
}
