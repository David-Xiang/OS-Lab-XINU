/* xsh_ethtest.c - xsh_ethtest */
/*
   Exercise1 of Lab5
   Design a program to send ethernet packet as fast as possible.
   How many large packets it can send per second?
   How many small packets it can send per second?
 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xsh_ethtest - test the capability of ethernet packet transmission
 *------------------------------------------------------------------------
 */
static void ethtest(int);

shellcmd xsh_ethtest(int nargs, char *args[])
{
	resume(create(ethtest, 8192, 80, "ethtest", 1, 10));
	return 0;
}

static void ethtest(int secs){
	uint32 minunit = 46;
	uint32 maxunit = 1500;
	struct dentry* devptr = &devtab[ETHER0];
	char data[2000] = "";
	uint32 count = 0;

	// fill data with all '0'
	int i = 0;
	for (i = 0; i < 2000 - 1; i++)
		data[i] = '0';

	// start test sending small packets for 'secs' seconds
	kprintf("Start sending small packets to ETHER0.\n");
	uint32 start = clktime;
	while(start + secs > clktime){
		++count;
		ethwrite(devptr, data, minunit);
	}
	kprintf("After %d seconds' test, speed of sending small packets is %d packets per second.\n", secs, count/secs);

	// start test sending large packets for 'secs' seconds
	kprintf("Start sending large packets to ETHER0.\n");
	start = clktime;
	count = 0;
	while(start + secs > clktime){
		++count;
		ethwrite(devptr, data, maxunit);
	}
	kprintf("After %d seconds' test, speed of sending large packets is %d packets per second.\n", secs, count/secs);

}
