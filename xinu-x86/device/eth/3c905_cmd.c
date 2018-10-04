#include <xinu.h>

/*#define DEBUG*/

int	_3c905_cmd (struct ether *ethptr, uint16 cmd, uint16 arg)
{
	int i;

	_3C905_CMD(ethptr, cmd, arg);
	for (i = 0; i < 2000; i++) {
		if (! (inw(ethptr->iobase + _3C905_OFF_INTSTATUS) & 
			_3C905_FLG_CMDINPROGR)) {
			return OK;
		}
	}

	kprintf("3c905: %X %X failed\n", cmd, arg);

	return SYSERR;
}
