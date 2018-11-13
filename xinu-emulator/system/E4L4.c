#include <xinu.h>
#include <custom.h>

/*
 	Exercise4 of Lab4
	Implement a control function allowing a process to apply for or release control of CONSOLE.
	It's implemented based mainly /device/tty/ttyXXXX.c
	This E4L4() is only a demo.
 */


void E4L4(){
	char* str1 = "Write to CONSOLE without applying for control.\n";
	char* str2 = "Write to CONSOLE after applying for control.\n";
	write(CONSOLE, str1, 50);
	control(CONSOLE, TC_WAIT, 0, 0);
	write(CONSOLE, str2, 50);
	control(CONSOLE, TC_SIGNAL, 0, 0);
}
