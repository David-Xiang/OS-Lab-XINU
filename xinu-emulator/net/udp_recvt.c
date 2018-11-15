/* udp_recvt.c */
/*
   Exercise3 of Lab4
   Use timer process to take place of recvtime and modify udp_recv.
 */

#include <xinu.h>

extern struct	udpentry udptab[UDP_SLOTS];	/* Table of UDP endpoints	*/

static void timer(pid32 pid, uint32 secs){
	 sleep(secs);
	 
	 // check if message arrives
	 struct procent* prptr = &proctab[pid];
	 if (prptr->prstate == PR_RECV && prptr->prhasmsg == FALSE){
		 send(pid, TIMEOUT);
	 }
}

/*------------------------------------------------------------------------
 * udp_recvt  -  Receive a UDP packet using timer process
 *------------------------------------------------------------------------
 */
int32	udp_recvt (
	 uid32	slot,			/* Slot in table to use		*/
	 char   *buff,			/* Buffer to hold UDP data	*/
	 int32	len,			/* Length of buffer		*/
	 uint32	timeout			/* Read timeout in msec		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	udpentry *udptr;	/* Pointer to udptab entry	*/
	umsg32	msg;			/* Message from recvtime()	*/
	struct	netpacket *pkt;		/* Pointer to packet being read	*/
	int32	i;			/* Counts bytes copied		*/
	int32	msglen;			/* Length of UDP data in packet	*/
	char	*udataptr;		/* Pointer to UDP data		*/

	/* Ensure only one process can access the UDP table at a time	*/

	mask = disable();

	/* Verify that the slot is valid */

	if ((slot < 0) || (slot >= UDP_SLOTS)) {
		restore(mask);
		return SYSERR;
	}

	/* Get pointer to table entry */

	udptr = &udptab[slot];

	/* Verify that the slot has been registered and is valid */

	if (udptr->udstate != UDP_USED) {
		restore(mask);
		return SYSERR;
	}

	/* Wait for a packet to arrive */

	if (udptr->udcount == 0) {		/* No packet is waiting	*/
		udptr->udstate = UDP_RECV;
		udptr->udpid = currpid;

		// create a timer process
		resume(create(timer, 8192, getprio(getpid()), "timer", 1, timeout));
		receive(); // wait until message arrives
		msg = recvclr();

		udptr->udstate = UDP_USED;
		if (msg == TIMEOUT) {
			restore(mask);
			return TIMEOUT;
		} else if (msg != OK) {
			restore(mask);
			return SYSERR;
		}
	}

	/* Packet has arrived -- dequeue it */

	pkt = udptr->udqueue[udptr->udhead++];
	if (udptr->udhead >= UDP_QSIZ) {
		udptr->udhead = 0;
	}
	udptr->udcount--;

	/* Copy UDP data from packet into caller's buffer */

	msglen = pkt->net_udplen - UDP_HDR_LEN;
	udataptr = (char *)pkt->net_udpdata;
	if (len < msglen) {
		msglen = len;
	}
	for (i=0; i<msglen; i++) {
		*buff++ = *udataptr++;
	}
	freebuf((char *)pkt);
	restore(mask);
	return msglen;
}

