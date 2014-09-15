#include <stdio.h>

int main(int argc, char *argv[], char *env[])
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

	ebp = get_ebp();
	printf("EBP = %x\n", ebp);

	int i = 0;
	p = &w;
	printf("Address	 |		Hex			 Dec			Contents\n");
	while(i < 100) {
		printf("%10x|%10x||%10d\n", p, *p, *p);
		p--;
		i++;
	}
	printf("exit C\n");
}
