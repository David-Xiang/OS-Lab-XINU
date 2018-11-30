/* sndok.c - sndok */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "xinudefs.h"
#include "rdisksys.h"
#include "rdserver.h"

/*------------------------------------------------------------------------
 * sndok - send an "OK" response (status is zero)
 *------------------------------------------------------------------------
 */
void	sndok (
	 struct	rd_msg_hdr *reqptr,	/* ptr to request		*/
	 struct	rd_msg_hdr *resptr,	/* ptr to response		*/
	 int	len			/* length of header to copy	*/
					/*    into response		*/
	)
{
	int	i;			/* counts bytes in header	*/
	char	*from, *to;		/* pointers used during copy	*/
	int	retval;
/*DEBUG*/ char	*typnams[] = {"error", "read", "write", "open", "close",
/*DEBUG*/	  "delete"};
/*DEBUG*/ printf("DEBUG: sending OK   %04x ---------->  %s\n",
/*DEBUG*/	ntohs(reqptr->rd_type),typnams[ntohs(reqptr->rd_type) >>4]);

	/* copy header to from request to response */

	from = (char *)reqptr;
	to =   (char *)resptr;
	for (i=0; i<sizeof(struct rd_msg_hdr); i++) {
		*to++ = *from++;
	}

	/* Set bit to indicate response */

	resptr->rd_type = htons(ntohs(resptr->rd_type) | RD_MSG_RESPONSE);

	/* Set status to indicate OK (should be zero anyway) */

	resptr->rd_status = htons(0);

	/* Return response to source */

// /*DEBUG*/ printf("Sending %d bytes\n", len);
// /*DEBUG*/ to = (char *)resptr;
// /*DEBUG*/ for (i=0; i<len; i++) {
// /*DEBUG*/	printf("%02x ",0xff&*to++);
// /*DEBUG*/	if ( ((i+1)%32) == 0) {
// /*DEBUG*/		printf("\n");
// /*DEBUG*/	}
// /*DEBUG*/	if (i> 190) {
// /*DEBUG*/		printf("...");
// /*DEBUG*/		break;
// /*DEBUG*/	}
// /*DEBUG*/}
// /*DEBUG*/	printf("\n");
	retval = sendto(sock, (const void *)resptr, len, 0,
		(struct sockaddr *)&senderip, addrlen);
/*DEBUG*/if (retval<0) {printf("DEBUG: sendto failed\n");}

	return;
}
