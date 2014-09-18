/*
 * Name: Alex Bahm
 * File: myprintf.c
 */

#include "myprintf.h"

char *table = "0123456789ABCDEF";

int myprintf(char* fmtStr, ...) {
	/*
		                            |-> "....%c ..%s ..%d .. %x ....\n"       
		                            |
		   HIGH                     |                                     LOW 
		   ---------------------- --|------------------------------------------
		       |  d  | c | b | a | fmt |retPC| ebp | locals
		   -------------------------------------|------------------------------
		                                        | 
		                                     CPU.ebp
		  *************** ALGORITHM ****************
		   Use cp to scan the format string:
		       spit out any char that's NOT %
		       for each \n, spit out an extra \r
		   Upon seeing a %: get next char, which must be one of 'c','s','u','d', 'o','x'
		   Advance ip to point to the next item on stack.
	*/

	char *cp = fmtStr; //cp now points at fmtStr
	int *ip = get_ebp();
	ip += 3; //now points at first argument

	while(*cp != 0) {
		if(*cp == '%') { //logic for calling printf options
			cp++; //We won't bother to output the %
			if(*cp == 'c') {
				putchar(*ip);
				ip += 1; //advance to next parameter
			} else if(*cp == 's') {
				prints(*ip);
				ip += 1;
			} else if(*cp == 'u') {
				printu(*ip);
				ip += 1;
			} else if(*cp == 'd') {
				printd(*ip);
				ip += 1;
			} else if(*cp == 'o') {
				printo(*ip);
				ip += 1;
			} else if(*cp == 'x') {
				printx(*ip);
				ip += 1;
			} //If it's none of the above, we will ignore it
		} else if(*cp == '/') { //could be a /n
			if(*(cp + 1) == 'n') {
				putchar('\n'); //newline
				putchar('\r'); //return
			} else if(*(cp + 1) == '\\') { //users wants a \
				putchar('\\');
			} else if(*(cp + 1) == 't') { //tab cahracter
				putchar('\t');
			}
		} else {
			putchar(*cp);
		}
		cp++;
	}
}


int prints(char* s) {
	char* sptr = s;
	
	//while we have not reached the end of the string, keep putting the current char
	while(*sptr != 0) { 
		putchar(*sptr);
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
		msb = msb >> 31; //shifts bits over so only the sign bit is still there
		if(msb) { //x is negative
			putchar('-');
			x *= -1; //make x positive
		}
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
	putchar('0'); //needed for formating of hex
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
		rpx(x / 16);
		putchar(c);
	}
}

int printo(int x) { //Octal 
	putchar('0'); //needed for how octal is displayed
	if(x < 0) x *= -1; //print OCT doesn't seem to work when the value is negative
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
		rpo(x / 8);
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