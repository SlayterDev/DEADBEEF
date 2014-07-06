// common.h -- Brad Slayter

#ifndef COMMON_H
#define COMMON_H

typedef unsigned int   u32int;
typedef 		 int   s32int;
typedef unsigned short u16int;
typedef			 short s16int;
typedef unsigned char  u8int;
typedef			 char  s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

// Copy memory from src to dest
void memcpy(u8int *dest, const u8int *src, u32int len);

// Set len bytes in dest with val
void memset(u8int *dest, u8int val, u32int len);

// compare str1 and str2
// -1 if str1 < str2
// 0 if str1 == str2
// 1 if str1 > str2
int strcmp(char *str1, char *str2);

// copy dest string into source string
char *strcpy(char *dest, const char *source);

// concatenate second string onto first string
char *strcat(char *first, const char *second);

#endif
