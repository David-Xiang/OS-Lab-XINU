/* catch.h  */
struct jmpent{
	bool8 valid;
	int32 ** addr;
	int32 inum;
	intmask mask;
};

extern struct jmpent jmptab[];
int setjmp();
void longjmp();
