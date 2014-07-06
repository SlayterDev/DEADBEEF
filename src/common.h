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

int strcmp(char *str1, char *str2);
char *strcpy(char *dest, const char *source);
char *strcat(char *first, const char *second);

#endif
