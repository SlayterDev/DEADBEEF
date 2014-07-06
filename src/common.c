// common.c -- Brad Slayter

#include "common.h"

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