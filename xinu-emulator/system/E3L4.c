#include <xinu.h>
#include <custom.h>

/*
 	Exercise3 of Lab4
	Test what will happen if two process simultaneously call ttyread when
	handling a large amount of input.
 */


static void readworker(int id){
	char buffer[8192] = "";
	ttyread(&devtab[CONSOLE], buffer, 8192);
	sleep(3*id);
	kprintf("readworker %d:\n%s\n", id, buffer);
}
void E3L4(){
	resume(create(readworker, 8192, 60, "readworker", 1, 0));
	resume(create(readworker, 8192, 60, "readworker", 1, 1));
}
