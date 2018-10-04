/* netin.c - netin, ipcksum, eth_ntoh, eth_hton, ip_ntoh, ip_hton */

#include <xinu.h>

bpid32	netbufpool;			/* ID of network buffer pool	*/
struct	netpacket *currpkt;		/* packet buffer being used now	*/
struct	network NetData;		/* local network interface	*/

/*------------------------------------------------------------------------
 * netin - continuously read the next incoming packet and handle it
 *------------------------------------------------------------------------
 */

process	netin(void) {

	status	retval;			/* return value from function	*/

	netbufpool = mkbufpool(PACKLEN, UDP_SLOTS * UDP_QSIZ +
				ICMP_SLOTS * ICMP_QSIZ + ICMP_OQSIZ + 1);

	if (netbufpool == SYSERR) {
		kprintf("Cannot allocate network buffer pool");
		kill(getpid());
	}

	/* Copy Ethernet address to global variable */

	control(ETHER0, ETH_CTRL_GET_MAC, (int32)NetData.ethaddr, 0);

	/* Indicate that IP address, mask, and router are not yet valid	*/

	NetData.ipvalid = FALSE;

	NetData.ipaddr = 0;
	NetData.addrmask = 0;
	NetData.routeraddr = 0;

	/* Initialize ARP cache */

	arp_init();

	/* Initialize UDP table */

	udp_init();

	/* Initialize ICMP table */

	//icmp_init();

	currpkt = (struct netpacket *)getbuf(netbufpool);

	/* Do forever: read packets from the network and process */

	while(1) {
	    	retval = read(ETHER0, (char *)currpkt, PACKLEN);
	    	if (retval == SYSERR) {
			panic("Ethernet read error");
	    	}

		/* Convert Ethernet Type to host order */

		eth_ntoh(currpkt);

		/* Demultiplex on Ethernet type */

		switch (currpkt->net_ethtype) {

			case ETH_ARP:
				arp_in();	/* Handle an ARP packet	*/
				continue;

			case ETH_IP:

				/* Verify checksum */

				if (ipcksum(currpkt) != 0) {
					kprintf("checksum failed\n\r");
					continue;
				}

				if (currpkt->net_ipvh != 0x45) {
					kprintf("version failed\n\r");
					continue;
				}

				/* Convert IP packet to host order */

				ip_ntoh(currpkt);

				if ( (currpkt->net_ipdst != IP_BCAST) &&
				     (NetData.ipvalid) &&
				     (currpkt->net_ipdst != NetData.ipaddr) ) {
					continue;
				}

				/* Demultiplex ICMP or UDP and ignore others */

				if (currpkt->net_ipproto == IP_ICMP) {
				//	icmp_in();/* Handle an ICMP packet*/

				} else if (currpkt->net_ipproto == IP_UDP) {
					udp_in();/* Handle a UDP packet	*/
				}
				continue;

			default: /* Ignore all other Ethernet types */
				kprintf("\n");
				continue;		
		}
	}
}


/*------------------------------------------------------------------------
 * ipcksum - compute the IP checksum for a packet
 *------------------------------------------------------------------------
 */

uint16	ipcksum(
	 struct  netpacket *pkt		/* ptr to a packet		*/
	)
{
	uint16	*hptr;			/* ptr to 16-bit header values	*/
	int32	i;			/* counts 16-bit values in hdr	*/
	uint32	cksum;			/* computed value of checksum	*/

	hptr= (uint16 *) &pkt->net_ipvh;
	cksum = 0;
	for (i=0; i<10; i++) {
		cksum += (uint32) *hptr++;
	}
	cksum += (cksum >> 16);
	cksum = 0xffff & ~cksum;
	if (cksum == 0xffff) {
		cksum = 0;
	}
	return (uint16) (0xffff & cksum);
}

/*------------------------------------------------------------------------
 * eth_ntoh - convert Ethernet type field to host byte order
 *------------------------------------------------------------------------
 */
void 	eth_ntoh(
	  struct netpacket *pktptr
	)
{
	pktptr->net_ethtype = ntohs(pktptr->net_ethtype);
}

/*------------------------------------------------------------------------
 * eth_hton - convert Ethernet type field to network byte order
 *------------------------------------------------------------------------
 */
void 	eth_hton(
	  struct netpacket *pktptr
	)
{
	pktptr->net_ethtype = htons(pktptr->net_ethtype);
}

/*------------------------------------------------------------------------
 * ip_ntoh - convert IP header fields to host byte order
 *------------------------------------------------------------------------
 */
void 	ip_ntoh(
	  struct netpacket *pktptr
	)
{
	pktptr->net_iplen = ntohs(pktptr->net_iplen);
	pktptr->net_ipid = ntohs(pktptr->net_ipid);
	pktptr->net_ipfrag = ntohs(pktptr->net_ipfrag);
	pktptr->net_ipsrc = ntohl(pktptr->net_ipsrc);
	pktptr->net_ipdst = ntohl(pktptr->net_ipdst);
}

/*------------------------------------------------------------------------
 * ip_hton - convert IP header fields to network byte order
 *------------------------------------------------------------------------
 */
void 	ip_hton(
	  struct netpacket *pktptr
	)
{
	pktptr->net_iplen = htons(pktptr->net_iplen);
	pktptr->net_ipid = htons(pktptr->net_ipid);
	pktptr->net_ipfrag = htons(pktptr->net_ipfrag);
	pktptr->net_ipsrc = htonl(pktptr->net_ipsrc);
	pktptr->net_ipdst = htonl(pktptr->net_ipdst);
}
