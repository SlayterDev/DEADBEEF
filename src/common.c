// common.c -- Brad Slayter

#include "common.h"
#include "monitor.h"

void outb(u16int port, u8int value) {
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port) {
	u8int ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

u16int inw(u16int port) {
	u16int ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

u32int strlen(char *str) {
	u32int i = 0;
	while (str[i])
		i++;

	return i;
}

void memcpy(u8int *dest, const u8int *src, u32int len) {
    u8int *pdest = (u8int*) dest;
    u8int *psrc = (u8int*) src;

    int loops = (int)(len / sizeof(u32int));
    for(int index = 0; index < loops; ++index) {
        *((u32int*)pdest) = *((u32int*)psrc);
        pdest += sizeof(u32int);
        psrc += sizeof(u32int);
    }

    loops = (len % sizeof(u32int));
    for (int index = 0; index < loops; ++index) {
        *pdest = *psrc;
        ++pdest;
        ++psrc;
    }
}

void memset(void *dest, u8int val, u32int len) {
    if (dest && len > 0) {
        unsigned char* pChar =  dest;
        int i = 0;
        for ( i = 0; i < len; ++i) {
            unsigned char temp = (unsigned char) val;
            *pChar++ = temp; // or pChar[i] = temp (they both don't work)
        }
    }
}

int strcmp(char *str1, char *str2) {
	int i = 0;
	int ret;
	while (str1[i]) {
		if (str1[i] == str2[i])
			ret = 0;
		else if (str1[i] < str2[i]) {
			ret = -1;
			break;
		} else if (str1[i] > str2[i]) {
			ret = 1;
			break;
		}
		i++;
	}

	if ((!str1[i] && str2[i]) && ret == 0)
		ret = -1;

	return ret;
}

char *strcpy(char *dest, const char *source) {
	int i = 0;
	while (source[i]) {
		dest[i] = source[i];
		i++;
	}

	return dest;
}

char *strcat(char *dest, const char *source) {
	char *a;
	for (a = dest; *a; a++)
		;

	for (; *source; a++, source++)
		*a = *source;
	*a = 0;

	return dest;
}

extern void panic(const char *message, const char *file, u32int line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    monitorWrite("PANIC(");
    monitorWrite(message);
    monitorWrite(") at ");
    monitorWrite(file);
    monitorWrite(":");
    monitorWriteDec(line);
    monitorWrite("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

extern void panic_assert(const char *file, u32int line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    monitorWrite("ASSERTION-FAILED(");
    monitorWrite(desc);
    monitorWrite(") at ");
    monitorWrite(file);
    monitorWrite(":");
    monitorWriteDec(line);
    monitorWrite("\n");
    // Halt by going into an infinite loop.
    for(;;);
}