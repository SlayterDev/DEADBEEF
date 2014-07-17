// common.h -- Brad Slayter
// Common type definitions and functions used throughout all files

#ifndef COMMON_H
#define COMMON_H

// Set this to 1 to write debug statements to the screen
// using DEBUGmonitorWrite
#define DEBUG 1

// Typedefs for common data types
typedef unsigned int   u32int;
typedef 		 int   s32int;
typedef unsigned short u16int;
typedef			 short s16int;
typedef unsigned char  u8int;
typedef			 char  s8int;

// Functions to speak directly to the hardware
void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

// Kernel panic. Use this for fatal errors. This will stop the halt the OS
// using an infinite loop and display the file and line where the panic
// was called.
#define PANIC(msg) panic(msg, __FILE__, __LINE__);

// ASSERT. Use this when if something is not true, a fatal error will occur
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

// Panic and assert definitions
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
