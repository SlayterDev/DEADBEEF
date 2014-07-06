// monitor.h -- Bradley Slayter

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

void monitorPut(char c);
void monitorClear();
void monitorWrite(char *c);
void monitorWriteHex(u32int n);

#endif