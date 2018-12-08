/* myttyread.c - myttyread */
/*
   Exercise3 of Lab7
   Implement a function myttyread which iteratively call ttygetc() to get
   input. And then modify configuration to replace ttyread.
 */
#include <xinu.h>

/*------------------------------------------------------------------------
 *  myttyread  -  Read character(s) from a tty device (interrupts disabled)
 *------------------------------------------------------------------------
 */
devcall	myttyread(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char	*buff,			/* Buffer of characters		*/
	  int32	count 			/* Count of character to read	*/
	)
{
	int32	nread;			/* Number of characters read	*/
	int32	firstch;		/* First input character on line*/
	char	ch;			/* Next input character		*/

	firstch = ttygetc(devptr);

	/* Read up to a line */

	ch = (char) firstch;
	*buff++ = ch;
	nread = 1;
	while ( (nread < count) && (ch != TY_NEWLINE) &&
			(ch != TY_RETURN) ) {
		ch = ttygetc(devptr);
		*buff++ = ch;
		nread++;
	}
	return nread;
}
