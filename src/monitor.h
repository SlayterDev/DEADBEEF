// monitor.h -- Bradley Slayter
// This file contains funtions for interfacing with the monitor.
// Currently the monitor works by writing to memory starting 
// at address 0xB80000

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

// Write char to terminal
void monitorPut(char c);

// Clear terminal
void monitorClear();

// Write string to terminal
void monitorWrite(const char *c);

// Write \n termintated string to terminal
void monitorWriteLine(const char *c);

// If DEBUG flag in common.h is set, this will write a line to the
// terminal with a DEBUG message.
void DEBUGmonitorWrite(const char *c);

// Remove a character and move the cursor back one space
void monitorBackSpace();

// Write hex representation of n to terminal
void monitorWriteHex(u32int n);

// Write an integer to the terminal
void monitorWriteDec(u32int n);

#endif