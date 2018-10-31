#include <xinu.h>
#include <custom.h>

/*
 	Exercise3 of Lab3
	Design a program to verify that preemption will lead to reschedule.
	I designed 2 function with dead loops which will keep adding 1 to a
	give address. If reschedule never happens in preemption then when the
	main process checks, one of the address's output will be zero.
	
	Attention: make sure all process have equal priority.
 */

void addOne(int* i){
	while (1){
	 *i = (*i) + 1;
	}
}
void E3L3(){
	kprintf("start");
	int a, b;
	resume(create(addOne, 8192, 50, "addOne", 1, &a));
	resume(create(addOne, 8192, 50, "addOne", 1, &b));
	if (a == 0 || b == 0)
		kprintf("reschedule does not happen.\n");
	else
		kprintf("reschedule happens in preemption.\n");
}
