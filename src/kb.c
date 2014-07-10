// kb.c -- Brad Slayter

#include "kb.h"
#include "monitor.h"
#include "isr.h"

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

modifierKeys_t mK;

char upperChar(char c) {
	char c2;
	if (c >= 'a' && c <= 'z') {
		c2 = ('A' + c - 'a');
		return c2;
	} else {
		return c;
	}
}

void keyboardHandler(registers_t regs) {
	unsigned char scancode;

	scancode = inb(0x60);

	if (scancode & 0x80) {
		// key release
		unsigned char newSc = scancode - 128;

		if (newSc == 42 || newSc == 54) {
			mK.lShift = 0;
		}
	} else {
		if (kbdus[scancode] == '\b') {
			monitorBackSpace();
			return;
		} else if (kbdus[scancode] == '\n') {
			monitorWrite("\n>");
			return;
		}

		if (mK.lShift)
			monitorPut(upperChar(kbdus[scancode]));
		else
			monitorPut(kbdus[scancode]);

		if (scancode == 42 || scancode == 54) {
			// shift keys
			mK.lShift = 1;
		}
	}
}

void installKeyboard() {
	registerInteruptHandler(IRQ1, &keyboardHandler);

	mK.lShift = 0;
	mK.rShift = 0;
	mK.lCtrl = 0;
	mK.rCtrl = 0;
	mK.lAlt = 0;
	mK.rAlt = 0;
}