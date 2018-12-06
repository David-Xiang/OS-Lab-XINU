/* rflclose.c - rflclose */

#include <xinu.h>

/*------------------------------------------------------------------------
 * rflclose  -  Close a remote file device
 *------------------------------------------------------------------------
 */
devcall	rflclose (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	struct	rflcblk	*rfptr;		/* Pointer to control block	*/
	struct	rf_msg_creq msg;
struct	rf_msg_cres resp;
char* name;
int32 retval;
	/* Wait for exclusive access */

	wait(Rf_data.rf_mutex);

	/* Verify remote file device is open */

	rfptr = &rfltab[devptr->dvminor];
	if (rfptr->rfstate == RF_FREE) {
		signal(Rf_data.rf_mutex);
		return SYSERR;
	}
	/* Handicraft a request */
name = rfptr->rfname;
msg.rf_type = htons(RF_MSG_CREQ);
msg.rf_status = htons(0);
msg.rf_seq = 0;
char* nptr = msg.rf_name;
memset(nptr, NULLCH, RF_NAMLEN);
while ( (*nptr++ = *name++) != NULLCH){
;
}
/* Send message and receive response */
retval = rfscomm((struct rf_msg_hdr *)&msg,
sizeof(struct rf_msg_creq),
(struct rf_msg_hdr *)&resp,
sizeof(struct rf_msg_cres) );
if (retval == SYSERR){
kprintf("Error encounterd during remote file close!\n");
signal(Rf_data.rf_mutex);
return SYSERR;
}else if(retval == TIMEOUT){
kprintf("Timeout during remote file close\n");
signal(Rf_data.rf_mutex);
return SYSERR;
}else if (ntohs(resp.rf_status) != 0){
signal(Rf_data.rf_mutex);
return SYSERR;
}

	/* Mark device closed */

	rfptr->rfstate = RF_FREE;
	signal(Rf_data.rf_mutex);
	return OK;
}
