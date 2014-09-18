#include "myprintf.h"
//int printd(int x): print an integer (which may be negative!!)
//int printo(u32 x): print x in OCTal as 0.....
//int printx(u32 x): print x in HEX.  as 0x....	
char *table = "0123456789ABCDEF";

int myprintf(char* fmtStr, ...) {
	/*
		int myprintf(char *fmt, ...) // SOME C compiler requires the 3 DOTs
		{
		   Assume the call is myprintf(fmt, a,b,c,d);
		   Upon entry, the stack contains:

		                            |-> "....%c ..%s ..%d .. %x ....\n"       
		                            |
		   HIGH                     |                                     LOW 
		   ---------------------- --|------------------------------------------
		       |  d  | c | b | a | fmt |retPC| ebp | locals
		   -------------------------------------|------------------------------
		                                        | 
		                                     CPU.ebp
		  
		  
		     1. Let char *cp point at the format string

		     2. Let int *ip  point at the first item to be printed on stack, e.g. a


		  *************** ALGORITHM ****************
		   Use cp to scan the format string:
		       spit out any char that's NOT %
		       for each \n, spit out an extra \r

		   Upon seeing a %: get next char, which must be one of 'c','s','u','d', 'o','x'

		   Advance ip to point to the next item on stack.
	*/
}


int prints(char* s) {
	char* sptr = s;
	
	while(s != 0) { //this might be backwards. If so, write a recursive function
		putchar(sptr);
		sptr++;
	}

	return 1;
}

int printd(int x) {
	//Check if +/-
	//If negative, 
		//print -
	//set most significant bit to 0
	//print that
	if(x == 0 ) {
		putchar('0');
	} else {
		int msb = x;
		msb = msb >>> 31; //shifts bits over so only the sign bit is still there
		if(msb) { //negative
			putchar('-');
		}
		x = (x <<< 1) >>> 1; //clears the sign bit
		rpd(x);

	}
}

int rpd(int x) {
	char c;
	if(x) {
		c = table[x % BASE];
		rpu(x / BASE);
		putchar(c);
	}
}

int printx(int x) {
	putchar('0');
	putchar('x');
	if(x == 0) {
		putchar('0');
		return;
	} else {
		rpx(x);
	}
}

int rpx(int x) {
	char c;
	if(x) {
		c = table[x % 16];
		rpu(x / 16);
		putchar(c);
	}
}

int printo(int x) { //Octal
	putchar('0');
	if(x == 0) {
		putchar('0');
		return; //We are done here
	} else {
		rpo(x);
	}

}

int rpo(int x) {
	char c;
	if(x) {
		c = table[x % 8];
		rpu(x / 8);
		putchar(c);
	}
}

int printu(u32 x)
{
	if(x == 0) {
		putchar('0');
	} else {
		rpu(x);
	}

	putchar(' ');
}

int rpu(u32 x)
{
	char c;
	if(x){
		c = table[x % BASE];
		rpu(x / BASE);
		putchar(c);
	}
} 