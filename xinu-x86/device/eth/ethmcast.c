#include <xinu.h>

local	int32	hashbit();
local	int32	_3c905_mcast(struct ether *, Eaddr[], uint16);

/*------------------------------------------------------------------------
 * eth_mcadd - add a multicast addresses
 *------------------------------------------------------------------------
 */
status	eth_mcadd(
	  struct ether	*ethptr,	/* ptr to Ethernet control blk	*/
	  Eaddr mca			/* ptr to 6-byte multicase addr.*/
	)
{
	int	i;

	for (i = 0; i < ethptr->ed_mcc; ++i) {
		if (memcmp((char *)mca, (char *)ethptr->ed_mca[i],
							EP_ALEN)) {
		return OK;  /* already there */
		}
	}

	/* Put address in the array and increment the count */

	memcpy(ethptr->ed_mca[i], mca, EP_ALEN);
	ethptr->ed_mcc++;
	ethptr->ed_mcset = 1;

	/* Send to the device and return the result */

	return _3c905_mcast(ethptr,ethptr->ed_mca,ethptr->ed_mcc);
}

/*------------------------------------------------------------------------
 * eth_mcdel - delete a multicast addresses
 *------------------------------------------------------------------------
 */
int	eth_mcdel(
	  struct ether	*ethptr,	/* ptr to Ethernet control blk	*/
	  Eaddr mca			/* ptr to 6-byte multicase addr.*/
	)
{
	int	i;

	for (i = 0; i < ethptr->ed_mcc; ++i) {
		if (memcmp((char *)mca, (char *)ethptr->ed_mca[i],
							EP_ALEN)) {
			break;
		}
	}

	if (i >= ethptr->ed_mcc) {
		return SYSERR;	/* specified address was not found */
	}
    
	/* Move other addresses down */

	for (i++; i < ethptr->ed_mcc; i++) {
		memcpy(ethptr->ed_mca[i-1], ethptr->ed_mca[i], EP_ALEN);
	}

	/* Decrement the count */

	ethptr->ed_mcc--;

	/* Delete from the device and return the result */

	return _3c905_mcast(ethptr, ethptr->ed_mca, ethptr->ed_mcc); 
	}

/*------------------------------------------------------------------------
 * _3c905_mcast - reset all multicast addresses on a 3com 3C905C card
 *------------------------------------------------------------------------
 */
local	int32	_3c905_mcast (
	  struct ether	*ethptr,	/* ptr to Ethernet control blk	*/
	  Eaddr		mc_addr[],	/* array of multicast addresses	*/
	  uint16	mc_count	/* count of items in the array	*/
	)
{
	int	i;	 

	/* Stop the receiver */

	_3c905_cmd(ethptr, _3C905_CMD_RXDISABLE, 0);

	/* Clear all multicast addresses on the card */

	for (i = 0; i < _3C905_MSK_MCASTFILTER + 1; i++) {
		_3C905_CMD(ethptr, _3C905_CMD_SETMCASTBIT, i);
	}

	/* Set all multicast addresses */

	for (i = 0; i < mc_count; i++) {
		_3C905_CMD(ethptr, _3C905_CMD_SETMCASTBIT,
			_3C905_FLG_MCASTENABLE | hashbit(mc_addr[i]));
	}

	/* R-enable packet reception */

	_3C905_CMD(ethptr, _3C905_CMD_RXENABLE, 0);

	return OK;
}

/*------------------------------------------------------------------------
 * hashbit - calculate the hash bit for an address - (from 3com manual)
 *------------------------------------------------------------------------
 */
local	int32	hashbit(
		  Eaddr		mca	/* ptr to multicast address	*/
		)
{
	uint32		crc;		/* CRC value			*/
	uint32		carry;		/* Carry from calaculation	*/
	uint32		i, j;
	byte		nxtbyte;

	crc = 0xffffffff;

	for (i = 0; i < 6; i++) {
	    nxtbyte = mca[i];
	    for (j = 0; j < 8; j++) {
		carry = ((crc & 0x80000000) ? 1 : 0) ^ (nxtbyte & 0x01);
		crc <<= 1;
		nxtbyte >>= 1;
		if (carry != 0) {
			crc = (crc ^ 0x04c11db6) | carry;
		}
	    }
	}

	return crc & _3C905_MSK_MCASTFILTER;
}
