// monitor.h -- Bradley Slayter

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

// Write char to terminal
void monitorPut(char c);

// Clear terminal
void monitorClear();

// Write string to terminal
void monitorWrite(char *c);

// Write hex representation of n to terminal
void monitorWriteHex(u32int n);

#endif