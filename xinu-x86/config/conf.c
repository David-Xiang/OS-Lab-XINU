/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include <xinu.h>


extern	devcall	ioerr(void);
extern	devcall	ionull(void);

/* Device independent I/O switch */

struct	dentry	devtab[NDEVS] =
{
/**
 * Format of entries is:
 * dev-number, minor-number, dev-name,
 * init, open, close,
 * read, write, seek,
 * getc, putc, control,
 * dev-csr-address, intr-handler, irq
 */

/* CONSOLE is tty */
	{ 0, 0, "CONSOLE",
	  (void *)ttyInit, (void *)ionull, (void *)ionull,
	  (void *)ttyRead, (void *)ttyWrite, (void *)ioerr,
	  (void *)ttyGetc, (void *)ttyPutc, (void *)ttyControl,
	  (void *)0x3f8, (void *)ttyDispatch, 36 },

/* SERIAL1 is tty */
	{ 1, 1, "SERIAL1",
	  (void *)ttyInit, (void *)ionull, (void *)ionull,
	  (void *)ttyRead, (void *)ttyWrite, (void *)ioerr,
	  (void *)ttyGetc, (void *)ttyPutc, (void *)ttyControl,
	  (void *)0x2f8, (void *)ttyDispatch, 35 },

/* NULLDEV is null */
	{ 2, 0, "NULLDEV",
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0 },

/* ETHER0 is eth */
	{ 3, 0, "ETHER0",
	  (void *)ethInit, (void *)ioerr, (void *)ioerr,
	  (void *)ethRead, (void *)ethWrite, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ethControl,
	  (void *)0x0, (void *)ethDispatch, 0 },

/* RFILESYS is rfs */
	{ 4, 0, "RFILESYS",
	  (void *)rfsInit, (void *)rfsOpen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)rfsControl,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE0 is rfl */
	{ 5, 0, "RFILE0",
	  (void *)rflInit, (void *)ioerr, (void *)rflClose,
	  (void *)rflRead, (void *)rflWrite, (void *)rflSeek,
	  (void *)rflGetc, (void *)rflPutc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE1 is rfl */
	{ 6, 1, "RFILE1",
	  (void *)rflInit, (void *)ioerr, (void *)rflClose,
	  (void *)rflRead, (void *)rflWrite, (void *)rflSeek,
	  (void *)rflGetc, (void *)rflPutc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE2 is rfl */
	{ 7, 2, "RFILE2",
	  (void *)rflInit, (void *)ioerr, (void *)rflClose,
	  (void *)rflRead, (void *)rflWrite, (void *)rflSeek,
	  (void *)rflGetc, (void *)rflPutc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE3 is rfl */
	{ 8, 3, "RFILE3",
	  (void *)rflInit, (void *)ioerr, (void *)rflClose,
	  (void *)rflRead, (void *)rflWrite, (void *)rflSeek,
	  (void *)rflGetc, (void *)rflPutc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE4 is rfl */
	{ 9, 4, "RFILE4",
	  (void *)rflInit, (void *)ioerr, (void *)rflClose,
	  (void *)rflRead, (void *)rflWrite, (void *)rflSeek,
	  (void *)rflGetc, (void *)rflPutc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE5 is rfl */
	{ 10, 5, "RFILE5",
	  (void *)rflInit, (void *)ioerr, (void *)rflClose,
	  (void *)rflRead, (void *)rflWrite, (void *)rflSeek,
	  (void *)rflGetc, (void *)rflPutc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RDISK is rds */
	{ 11, 0, "RDISK",
	  (void *)rdsInit, (void *)rdsOpen, (void *)rdsClose,
	  (void *)rdsRead, (void *)rdsWrite, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)rdsControl,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILESYS is lfs */
	{ 12, 0, "LFILESYS",
	  (void *)lfsInit, (void *)lfsOpen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE0 is lfl */
	{ 13, 0, "LFILE0",
	  (void *)lflInit, (void *)ioerr, (void *)lflClose,
	  (void *)lflRead, (void *)lflWrite, (void *)lflSeek,
	  (void *)lflGetc, (void *)lflPutc, (void *)lflControl,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE1 is lfl */
	{ 14, 1, "LFILE1",
	  (void *)lflInit, (void *)ioerr, (void *)lflClose,
	  (void *)lflRead, (void *)lflWrite, (void *)lflSeek,
	  (void *)lflGetc, (void *)lflPutc, (void *)lflControl,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE2 is lfl */
	{ 15, 2, "LFILE2",
	  (void *)lflInit, (void *)ioerr, (void *)lflClose,
	  (void *)lflRead, (void *)lflWrite, (void *)lflSeek,
	  (void *)lflGetc, (void *)lflPutc, (void *)lflControl,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE3 is lfl */
	{ 16, 3, "LFILE3",
	  (void *)lflInit, (void *)ioerr, (void *)lflClose,
	  (void *)lflRead, (void *)lflWrite, (void *)lflSeek,
	  (void *)lflGetc, (void *)lflPutc, (void *)lflControl,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE4 is lfl */
	{ 17, 4, "LFILE4",
	  (void *)lflInit, (void *)ioerr, (void *)lflClose,
	  (void *)lflRead, (void *)lflWrite, (void *)lflSeek,
	  (void *)lflGetc, (void *)lflPutc, (void *)lflControl,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE5 is lfl */
	{ 18, 5, "LFILE5",
	  (void *)lflInit, (void *)ioerr, (void *)lflClose,
	  (void *)lflRead, (void *)lflWrite, (void *)lflSeek,
	  (void *)lflGetc, (void *)lflPutc, (void *)lflControl,
	  (void *)0x0, (void *)ionull, 0 },

/* TESTDISK is ram */
	{ 19, 0, "TESTDISK",
	  (void *)ramInit, (void *)ramOpen, (void *)ramClose,
	  (void *)ramRead, (void *)ramWrite, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* NAMESPACE is nam */
	{ 20, 0, "NAMESPACE",
	  (void *)namInit, (void *)namOpen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0 }
};
