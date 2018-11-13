/* xsh_open.c - xsh_open */
/* Exercise2 of Lab4
   Develop a program to check whether XINU allow user to type in device's
   name to open the device.
 */
#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_open - use open to open a device based on the name
 *------------------------------------------------------------------------
 */
extern int strcmp(char*, char*);
shellcmd xsh_open(int32 nargs, char *args[])
{
	kprintf("please type in a device's name CAREFULLY:");
	char buffer[20] = "";
	read(CONSOLE, buffer, 20);

	// filter the LF char
	int i = 0;
	for (i = 0; i < 20; i++){
		if (buffer[i] == 10)
			buffer[i] = 0;
	}

	// look for the same name in device table
	bool8 founddev = 0;

	for (i = 0; i < NDEVS && !founddev; i++){
		if (strcmp(buffer, devtab[i].dvname) == 0){
			int32 retval = open(i, buffer, NULL);
			if (retval == OK){
				founddev = 1;
				kprintf("Name: %s\nDescriptor: %d\n", buffer, i);
				return 0;
			}
		}else{
			kprintf("not %s\n", devtab[i].dvname);
		}
	}
	kprintf("Wrong device name!\n");
	return 0;
}
