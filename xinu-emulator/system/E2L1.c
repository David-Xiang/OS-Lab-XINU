#include <xinu.h>
#include <custom.h>

/*
	Exercise2 of Lab1
	1. Create a process to print increasing numbers each second
	2. In main process, check the process above and if the process is 
	found alive, suspend it for 3 seconds and then wake up it up.
 */
void print_num();
void E2L1(){
	pid32 pid = create(print_num, 4096, 60, "print_num", 0);
	resume(pid);

	struct procent* prptr = &proctab[pid];
	
	while(TRUE){
		if (prptr->prstate == PR_FREE){
			kprintf("The process's state is not valid!");
			return;
		}else if (prptr->prstate == PR_SUSP){
			kprintf("resume\n");
			resume(pid);
		}else{
			kprintf("suspend\n");
			suspend(pid);
			sleep(3);
			resume(pid);
		}

		sleep(0.1);
	}
}

void print_num(){
	int i = 0;
	while(TRUE){
		printf("%d", i++);
		sleep(1);
	}
	return;
}
