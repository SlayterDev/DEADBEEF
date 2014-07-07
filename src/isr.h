// isr.h -- Brad Slayter

#include "common.h"

typedef struct registers {
	u32int ds;
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int intNo, errCode;
	u32int eip, cs, eflags, useresp, ss;
} registers_t;