// kb.c -- Brad Slayter

#include "kb.h"
#include "monitor.h"
#include "isr.h"
#include "kheap.h"

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

u8int readingInput = 0; // 1 when scaning
char tempBuffer[256]; 	// buffer to hold user input
u32int bufferCount = 0; // counter to keep track of buffer length

u8int getchFlag = 0; 	// 1 when reading for getch()
char getchChar = '\0';

char charToSymbol(char c) {
	switch (c) {
		case '1':
			c = '!';
			break;
		case '2':
			c = '@';
			break;
		case '3':
			c = '#';
			break;
		case '4':
			c = '$';
			break;
		case '5':
			c = '%';
			break;
		case '6':
			c = '^';
			break;
		case '7':
			c = '&';
			break;
		case '8':
			c = '*';
			break;
		case '9':
			c = '(';
			break;
		case '0':
			c = ')';
			break;
		case '-':
			c = '_';
			break;
		case '=':
			c = '+';
			break;
	}

	return c;
}

char upperChar(char c) {
	char c2;
	if (c >= 'a' && c <= 'z') {
		c2 = ('A' + c - 'a');
		return c2;
	} else if ((c >= '0' && c <= '=') || c == '-') {
		return charToSymbol(c);
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
			monitorWrite("\n");
			if (readingInput)
				readingInput = 0;
			return;
		}

		if (mK.lShift)
			monitorPut(upperChar(kbdus[scancode]));
		else
			monitorPut(kbdus[scancode]);

		if (readingInput && (scancode != 42 && scancode != 54)) {
			if (!mK.lShift)
				tempBuffer[bufferCount] = kbdus[scancode];
			else
				tempBuffer[bufferCount] = upperChar(kbdus[scancode]);
			bufferCount++;
		} else if (getchFlag && (scancode != 42 && scancode != 54)) {
			if (!mK.lShift)
				getchChar = kbdus[scancode];
			else
				getchChar = upperChar(kbdus[scancode]);
			getchFlag = 0;
		}
		if (scancode == 42 || scancode == 54) {
			// shift keys
			mK.lShift = 1;
		}
	}
}

char getch() {
	getchFlag = 1;

	while (getchFlag) {
		// blocking
	}

	return getchChar;
}

void scanStr(char **buf) {
	tempBuffer[0] = 0;
	bufferCount = 0;
	readingInput = 1;

	while (readingInput) {
		// empty block?
	}

	tempBuffer[bufferCount++] = '\0';

	*buf = (char *)kmalloc(bufferCount);
	strcpy(*buf, tempBuffer);
}

void scanDec(int *n) {
	char *decBuf;
	scanStr(&decBuf);

	int temp = 0, i;
	for (i = 0; i < strlen(decBuf); i++) {
		temp = temp * 10 + (decBuf[i] - '0');
	}

	*n = temp;
}

// currently only works for one variable
// more than one is undefined behaivor and I can not be held responsible
// for what happens
void DBscanf(const char *format, ...) {
	u8int formNxt = 0; // next char is format
	char formList[64]; // list of arg types in order
	int formCnt = 0;   // count of args

	int i = 0;
	while (format[i]) {
		if (formNxt) {	 // this char is arg type
			formNxt = 0; // reset flag

			// only int and string supported
			if (format[i] == 'd' || format[i] == 's')
				formList[formCnt++] = format[i]; // add arg type to list
		}

		if (format[i] == '%')
			formNxt = 1; // next char is arg type

		i++;
	}

	va_list list;
	va_start(list, format);
	for (i = 0; i < formCnt; i++) {
		if (formList[i] == 'd') {
			scanDec(va_arg(list, int*)); // scan int
		} else if (formList[i] == 's') {
			scanStr(va_arg(list, char**)); // scan string
		}
	}
	va_end(list);
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