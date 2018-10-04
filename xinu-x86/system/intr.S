/* intr.s - disable(), restore(), enable(), pause(), halt() */


#include <icu.s> 

	.text
	.globl	enable
	.globl	disable
	.globl	restore
	.globl	pause
	.globl	halt
	.globl	getirmask
	.globl	spurious_irq7
	.globl	spurious_irq15

/*------------------------------------------------------------------------
 * Xtrap - trap entry
 *------------------------------------------------------------------------
 */
Xtrap:
	call	trap
	movb	$OCW1_2,%al
	outb	%al,$EOI
	popl	%eax		/* exception # */
	testl	$35,%eax
	jb	3f
	shrl	$5,%eax		/* convert to IRQ # */

	/* issue global rearm to cards */
	testl	$7,%eax
	ja	1f
	orl	$0x2f0,%eax	/* 0x2fX for IRQ 3-7 */
	movb	%bl,(%eax)
	jmp	3f
1:	testl	$9,%eax
	jz	2f
	testl	$15,%eax
	ja	3f
	orl	$0x6f0,%eax	/* 0x2f(X-8) for IRQ 10-15 */
	movb	%bl,(%eax)
	jmp	3f
2:	movb	%al,0x2f2	/* 0x2f2 for IRQ 9 */
3:
	popal
	leave
	iret

/*------------------------------------------------------------------------
 * _Xint? - default exception and interrupt entry points
 *	NB: These must be contiguous
 *------------------------------------------------------------------------
 */
#include "../system/xint.s"

		.globl	defevec
defevec:
		.long	_Xint0		/* Divide error			*/
		.long	_Xint1		/* Debug exceptions		*/
		.long	_Xint2		/* Nonmaskable interrupt	*/
		.long	_Xint3		/* Breakpoint (one-byte INT 3	*/
					/*    instruction)		*/	
		.long	_Xint4		/* Overflow (INTO instruction)	*/
		.long	_Xint5		/* Bounds check (BOUND instr)	*/
		.long	_Xint6		/* Invalid opcode		*/
		.long	_Xint7		/* Coprocessor not available	*/
		.long	_Xint8		/* Double fault			*/
		.long	_Xint9		/* (reserved)			*/
		.long	_Xint10		/* Invalid TSS			*/
		.long	_Xint11		/* Segment not present		*/
		.long	_Xint12		/* Stack exception		*/
		.long	_Xint13		/* General protection		*/
		.long	_Xint14		/* Page fault			*/
		.long	_Xint15		/* (reserved)			*/
		.long	_Xint16		/* Coprocessor error		*/
		.long	_Xint17		/* (reserved)			*/
		.long	_Xint18		/* (reserved)			*/
		.long	_Xint19		/* (reserved)			*/
		.long	_Xint20		/* (reserved)			*/
		.long	_Xint21		/* (reserved)			*/
		.long	_Xint22		/* (reserved)			*/
		.long	_Xint23		/* (reserved)			*/
		.long	_Xint24		/* (reserved)			*/
		.long	_Xint25		/* (reserved)			*/
		.long	_Xint26		/* (reserved)			*/
		.long	_Xint27		/* (reserved)			*/
		.long	_Xint28		/* (reserved)			*/
		.long	_Xint29		/* (reserved)			*/
		.long	_Xint30		/* (reserved)			*/
		.long	_Xint31		/* (reserved)			*/
		.long	_Xint32		/* available for ext. interrupt	*/
		.long	_Xint33		/* available for ext. interrupt	*/
		.long	_Xint34		/* available for ext. interrupt	*/
		.long	_Xint35		/* available for ext. interrupt	*/
		.long	_Xint36		/* available for ext. interrupt	*/
		.long	_Xint37		/* available for ext. interrupt	*/
		.long	_Xint38		/* available for ext. interrupt	*/
		.long	spurious_irq7
		.long	_Xint40		/* available for ext. interrupt	*/
		.long	_Xint41		/* available for ext. interrupt	*/
		.long	_Xint42		/* available for ext. interrupt	*/
		.long	_Xint43		/* available for ext. interrupt	*/
		.long	_Xint44		/* available for ext. interrupt	*/
		.long	_Xint45		/* available for ext. interrupt	*/
		.long	_Xint46		/* available for ext. interrupt	*/
		.long	spurious_irq15

/*---------------------------------------------------------
 * pause: halt the processor until an interrupt occurs
 *---------------------------------------------------------
 */
pause:
	call enable
	hlt
	ret


/*---------------------------------------------------------
 * halt: do nothing forever
 *---------------------------------------------------------
 */
halt:
	jmp halt
	ret

/*---------------------------------------------------------
 * enable all interrupts
 *---------------------------------------------------------
 */
enable:
	sti
	ret

/*---------------------------------------------------------
 * disable - disable interrupts and return previous state
 *---------------------------------------------------------
 */
disable:
	pushfl
	cli
	popl	%eax
	andl	$0x00000200,%eax
	ret

/*---------------------------------------------------------
 * restore(mask)    - restore interrupts to value in mask
 * int16 mask
 *---------------------------------------------------------
 */
restore:
        cli
        movw    4(%esp),%ax
	andl	$0x00000200,%eax
	pushl	%eax
	popfl
        ret

/*------------------------------------------------------------------------
 * getirmask()  - return current interrupt mask
 *------------------------------------------------------------------------
 */
getirmask:
	inb	$IMR2,%al
	shlw	$8,%ax
	inb	$IMR1,%al
	ret

spurious_irq7:
	mov	$EOI,%al
	outb	%al,$OCW1_2
	iret

spurious_irq15:
	mov	$EOI,%al
	outb	%al,$OCW1_2
	mov	$EOI,%al
	outb	%al,$OCW2_2
	iret
