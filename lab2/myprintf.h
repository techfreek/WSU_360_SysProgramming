/*
 * Name: Alex Bahm
 * File: myprintf.h
 */

#define BASE 10
typedef unsigned int u32;


int myprintf(char* fmtStr, ...);

int printd(int x);
int rpd(int x);

int prints(char* s);

int printx(int x);
int rpx(int x);

int printo(int x);
int rpo(int x);

int printu(u32 x);
int rpu(u32 x);
