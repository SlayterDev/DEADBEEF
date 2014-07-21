// kb.h -- Brad Slayter
// This file contains code to communicate with the keyboard and
// recieve button presses from the user. As well it includes basic
// functions to get input from the user

#ifndef KB_H
#define KB_H

#include "common.h"
#include <stdarg.h>

// struct to hold the state of modifier keys
struct modifierKeys {
	u8int lShift;
	u8int rShift;
	u8int lCtrl;
	u8int rCtrl;
	u8int lAlt;
	u8int rAlt;
};
typedef struct modifierKeys modifierKeys_t;

// Install the keyboard handler
void installKeyboard();

char getch();
// Scans a string from the "command line" until new line is pressed
// Should pass un allocated char pointer
// function will allocate enough memory and store the scanned string
void scanStr(char **buf);
void scanDec(int *n);
void DBscanf(const char *format, ...);

#endif