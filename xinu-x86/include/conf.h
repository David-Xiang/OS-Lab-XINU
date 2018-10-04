/* conf.h (GENERATED FILE; DO NOT EDIT) */

/* Device switch table declarations */

/* Device table entry */
struct	dentry	{
	int32   dvnum;
	int32   dvminor;
	char    *dvname;
	devcall (*dvinit) (struct dentry *);
	devcall (*dvopen) (struct dentry *, char *, char *);
	devcall (*dvclose)(struct dentry *);
	devcall (*dvread) (struct dentry *, void *, uint32);
	devcall (*dvwrite)(struct dentry *, void *, uint32);
	devcall (*dvseek) (struct dentry *, int32);
	devcall (*dvgetc) (struct dentry *);
	devcall (*dvputc) (struct dentry *, char);
	devcall (*dvcntl) (struct dentry *, int32, int32, int32);
	void    *dvcsr;
	void    (*dvintr)(void);
	byte    dvirq;
};

extern	struct	dentry	devtab[]; /* one entry per device */

/* Device name definitions */

#define CONSOLE     0       /* type tty      */
#define SERIAL1     1       /* type tty      */
#define NULLDEV     2       /* type null     */
#define ETHER0      3       /* type eth      */
#define RFILESYS    4       /* type rfs      */
#define RFILE0      5       /* type rfl      */
#define RFILE1      6       /* type rfl      */
#define RFILE2      7       /* type rfl      */
#define RFILE3      8       /* type rfl      */
#define RFILE4      9       /* type rfl      */
#define RFILE5      10       /* type rfl      */
#define RDISK       11       /* type rds      */
#define LFILESYS    12       /* type lfs      */
#define LFILE0      13       /* type lfl      */
#define LFILE1      14       /* type lfl      */
#define LFILE2      15       /* type lfl      */
#define LFILE3      16       /* type lfl      */
#define LFILE4      17       /* type lfl      */
#define LFILE5      18       /* type lfl      */
#define TESTDISK    19       /* type ram      */
#define NAMESPACE   20       /* type nam      */

/* Control block sizes */

#define	Nnull	1
#define	Ntty	2
#define	Neth	1
#define	Nrfs	1
#define	Nrfl	6
#define	Nrds	1
#define	Nram	1
#define	Nlfs	1
#define	Nlfl	6
#define	Nnam	1

#define DEVMAXNAME 24
#define NDEVS 21


/* Configuration and Size Constants */

#define	NPROC	     100	/* number of user processes		*/
#define	NSEM	     100	/* number of semaphores			*/
#define	IRQ_TIMER    IRQ_HW5	/* timer IRQ is wired to hardware 5	*/
#define	IRQ_ATH_MISC IRQ_HW4	/* Misc. IRQ is wired to hardware 4	*/
#define CLKFREQ      200000000	/* 200 MHz clock			*/

#define	LF_DISK_DEV	TESTDISK
