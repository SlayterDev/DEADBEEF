// kb.h -- Brad Slayter

#ifndef KB_H
#define KB_H

#include "common.h"

struct modifierKeys {
	u8int lShift;
	u8int rShift;
	u8int lCtrl;
	u8int rCtrl;
	u8int lAlt;
	u8int rAlt;
};
typedef struct modifierKeys modifierKeys_t;

void installKeyboard();

// Scans a string from the "command line" until new line is pressed
// Should pass un allocated char pointer
// function will allocate enough memory and store the scanned string
void scanStr(char **buf);

#endif