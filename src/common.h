// common.h -- Brad Slayter

#ifndef COMMON_H
#define COMMON_H

#define DEBUG 1

typedef unsigned int   u32int;
typedef 		 int   s32int;
typedef unsigned short u16int;
typedef			 short s16int;
typedef unsigned char  u8int;
typedef			 char  s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, u32int line);
extern void panic_assert(const char *file, u32int line, const char *desc);

// Copy memory from src to dest
void memcpy(u8int *dest, const u8int *src, u32int len);

// Set len bytes in dest with val
void memset(void *dest, u8int val, u32int len);

// Get length of string
u32int strlen(char *str);

// compare str1 and str2
// -1 if str1 < str2
// 0 if str1 == str2
// 1 if str1 > str2
int strcmp(char *str1, char *str2);

// copy dest string into source string
char *strcpy(char *dest, const char *src);

// concatenate second string onto first string
char *strcat(char *first, const char *second);

#endif
