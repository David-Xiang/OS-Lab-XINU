/* rsclose.c - rsclose */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "xinudefs.h"
#include "rfilesys.h"
#include "rfserver.h"

/*------------------------------------------------------------------------
 * rsopen - handle an close request, merely send a response back
 *------------------------------------------------------------------------
 */
void	rsopen (
	 struct	rf_msg_creq *reqptr,	/* ptr to read request		*/
	 struct	rf_msg_cres *resptr	/* ptr to read response		*/
	)
{
	int	fd;			/* file descriptor		*/
	int	i, j;			/* general loop indexes		*/
	struct	stat	buf;		/* buffer for file status	*/
	char	*to, *from;		/* used during copy		*/
	struct	stat	sbuff;		/* stat buffer			*/
	int	sreturn;		/* stat return value		*/

/* DEBUG */ printf("DEBUG: reached rsclosen");

	sreturn = stat(reqptr->rf_name, &sbuff);

	if (sreturn < 0) {	/* if file does not exist */
		snderr( (struct rf_msg_hdr *)reqptr,
			(struct rf_msg_hdr *)resptr,
			sizeof(struct rf_msg_cres) );
		return;
	}

/*DEBUG*/ printf("DEBUG: opening file %s\n",reqptr->rf_name);
	fd = rsofile(reqptr->rf_name, O_RDWR);

	/* if open failed or open file table is full, send error */

	if (fd < 0) {
/*DEBUG*/ printf("DEBUG: fd is %d and rsclose sends error\n", fd);
		snderr( (struct rf_msg_hdr *)reqptr,
			(struct rf_msg_hdr *)resptr,
			 sizeof(struct rf_msg_cres) );
		return;
	}

	/* Return OK status */
/*DEBUG*/ printf("DEBUG: fd is %d and rsclose sends OK\n", fd);
	sndok ( (struct rf_msg_hdr *)reqptr,
		(struct rf_msg_hdr *)resptr,
		 sizeof(struct rf_msg_cres) );
	return;
}
