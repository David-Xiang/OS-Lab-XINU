/*  main.c  - main */
#include <xinu.h>
#include <stdlib.h>
#include <stdio.h>
#include <custom.h>

/* Xinu's main process */
process	main(void)
{

	/* Run the Xinu shell */
	recvclr();
	//resume(create(E3L3, 8192, 50, "E3L3", 0));
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(600000);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;

}
