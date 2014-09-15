#include <stdio.h>

main(int argc, char *argv[], char *env[])
{
  int a,b,c;
  printf("enter main\n");
  
  printf("&argc=%x argv=%x env=%x\n", &argc, argv, env);
  printf("&a=%x &b=%x &c=%x\n", &a, &b, &c);

  a=1; b=2; c=3;
  A(a,b);
  printf("exit main\n");
}

int A(int x, int y)
{
  int d,e,f;
  printf("enter A\n");
  printf("&d: %x, &e: %x, &f: %x\n", &d, &e, &e);

  d=3; e=4; f=5;
  B(d,e);
  printf("exit A\n");
}

int B(int x, int y)
{
  int g,h,i;
  printf("enter B\n");
  printf("&g: %x, &h: %x, &i: %x\n", &g, &h, &i);

  g=6; h=7; i=8;
  C(g,h);
  printf("exit B\n");
}

int *p, ebp;

int C(int x, int y)
{
  int u, v, w;
  printf("enter C\n");
  printf("&u: %x, &v: %x, &w: %x\n", &u, &v, &w);
  // PRINT ADDRESS OF u,v,w;
  
  u=9; v=10; w=11;

  /*********** Write C code to DO ************ 
        (1). ebp = get_ebp();   // call get_ebp() in s.s file
             Print ebp value in HEX

        (2). Let p point at the local variable w;
             Print in HEX 100 entries of the stack contents starting from p:

                      Address      Contents
                     ---------    ----------

                     ---------    ---------- 

        (3). Run the program as   a.out > outfile
             On a hard copy of the outfile, use the addresses of the local variables to
             IDENTIFY and EXPLAIN the stack contents in terms of the function stack 
             frames, i.e. local variables, parameters, return address, etc. 

        (4). ebp is the STACK FRAME pointer of the function C.
             PRINT the STACK FRAME link list, which ends with a 0.

        (5). If you run the program as   
                    a.out one two three > outfile
             IDENTIFY the parameters to main(), i.e. where are   
                    argc, argv[ ], env[ ] 
             located?
  *******************************************/

  printf("exit C\n");
}
