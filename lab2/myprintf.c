//int printd(int x): print an integer (which may be negative!!)
//int printo(u32 x): print x in OCTal as 0.....
//int printx(u32 x): print x in HEX.  as 0x....	


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
		   Then call

		        putchar(*ip) for 'c';
		        prints(*ip)  for 's';
		        printu(*ip)  for 'u';
		        printd(*ip)  for 'd';
		        printo(*ip)  for 'o';
		        printx(*ip)  for 'x';

		   Advance ip to point to the next item on stack.
	*/
}

int printd(int x) {

}


int printo(u32 x) {

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

int printu(u32 x)
{
	if(x == 0) {
		putchar('0');
	} else {
		rpu(x);
	}

	putchar(' ');
}