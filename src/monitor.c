// monitor.c -- Brad Slayter

#include "monitor.h"

u16int cursorX;
u16int cursorY;
u16int *terminalBuffer = (u16int *)0xB8000;

static void moveCursor() {
	u16int cursorLocation = cursorY * 80 + cursorX;
	outb(0x3D4, 14);
	outb(0x3D5, cursorLocation >> 8);
	outb(0x3d4, 15);
	outb(0x3D5, cursorLocation);
}

static void scroll() {
	u8int attributeByte = (0 << 4) | (15 & 0x0F);
	u16int blank = 0x20 | (attributeByte << 8);

	if (cursorY >= 25) {
		int i;
		for (i = 0*80; i < 24*80; i++) {
			terminalBuffer[i] = terminalBuffer[i+80];
		}

		for (i = 24*80; i < 25*80; i++) {
			terminalBuffer[i] = blank;
		}

		cursorY = 24;
	}
}

void monitorPut(char c) {
	u8int backColor = 0;
	u8int foreColor = 15;

	u8int attributeByte = (backColor << 4) | (foreColor & 0x0F);
	u16int attribute = attributeByte << 8;
	u16int *location;

	if (c == 0x08 && cursorX) { // backspace byte
		cursorX--;
	} else if (c == 0x09) {
		cursorX = (cursorX + 4) & ~(4-1);
	} else if (c == '\r') {
		cursorX = 0;
	} else if (c == '\n') {
		cursorX = 0;
		cursorY++;
	} else if (c >= ' ') {
		location = terminalBuffer + (cursorY*80 + cursorX);
		*location = c | attribute;
		cursorX++;
	}

	if (cursorX >= 80) {
		cursorX = 0;
		cursorY++;
	}

	scroll();
	moveCursor();
}

void monitorClear() {
	u8int attributeByte = (0 << 4) | (15 & 0x0F);
	u16int blank = 0x20 | (attributeByte << 8);

	int i;
	for (i = 0; i < 80*25; i++) {
		terminalBuffer[i] = blank;
	}

	cursorX = 0;
	cursorY = 0;
	moveCursor();
}

void monitorWrite(const char *c) {
	int i = 0;
	while (c[i]) {
		monitorPut(c[i++]);
	}
}

void monitorBackSpace() {
	cursorX--;
	if (cursorX < 0)
		cursorX = 0;

	moveCursor();
	monitorPut(' ');

	cursorX--;
	if (cursorX < 0)
		cursorX = 0;

	moveCursor();
}

void monitorWriteHex(u32int n) {
	static char hex [] = { '0', '1', '2', '3', '4', '5', '6', '7',
                        '8', '9' ,'A', 'B', 'C', 'D', 'E', 'F' };

	char buffer[16];

	int len = 0, k = 0;
	do {
		buffer[len] = hex[n&0xF];
		len++;
		n >>= 4;
	} while (n != 0);

	for (; k < len/2; k++) {
		buffer[k] ^= buffer[len-k-1];
		buffer[len-k-1] ^= buffer[k];
		buffer[k] ^= buffer[len-k-1];
	}

	buffer[len] = '\0';

	char pre[100] = "0x\0";

	strcat(pre, buffer);

	monitorWrite(pre);
}

void monitorWriteDec(u32int n) {
	if (n == 0) {
        monitorPut('0');
        return;
    }

    s32int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0) {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0) {
        c2[i--] = c[j++];
    }
    monitorWrite(c2);
}