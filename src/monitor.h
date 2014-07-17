// monitor.h -- Bradley Slayter

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

// Write char to terminal
void monitorPut(char c);

// Clear terminal
void monitorClear();

// Write string to terminal
void monitorWrite(const char *c);
void monitorWriteLine(const char *c);
void DEBUGmonitorWrite(const char *c);

void monitorBackSpace();

// Write hex representation of n to terminal
void monitorWriteHex(u32int n);

void monitorWriteDec(u32int n);

#endif