#include <xinu.h>
#include <custom.h>

/*
 	Exercise2 of Lab3
	Design a program to trace 2 sleeping processes woke up by clock 
	interupt in which one process's prority are higher than current
	running process.
 */

void sleepy(int prio){
	kprintf("priority %d: start to sleep.\n", prio);
	sleep(1);
	kprintf("priority %d: wake up.\n", prio);
}
void E2L3(){
	resume(create(sleepy, 8192, 60, "shell", 1, 60));
	resume(create(sleepy, 8192, 40, "shell", 1, 40));
	while(1)
		sleep(5);
}

