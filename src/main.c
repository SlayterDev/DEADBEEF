// main.c -- Brad Slayter

#include "common.h"
#include "monitor.h"
#include "descriptorTables.h"
#include "timer.h"
#include "paging.h"
#include "kheap.h"
#include "kb.h"
#include "multiboot.h"
#include "initrd.h"

extern u32int placementAddress;

int kernelMain(struct multiboot *mbootPtr) {
	// Init code here
	monitorClear();
	initDescriptorTables();

	asm volatile("sti");

	// Find location of ramdisk
	ASSERT(mbootPtr->mods_count > 0);
	u32int initrdLocation = *((u32int *)mbootPtr->mods_addr);
	u32int initrdEnd = *(u32int *)(mbootPtr->mods_addr + 4);
	placementAddress = initrdEnd;

	initialisePaging();

	fsRoot = initializeInitrd(initrdLocation);
	//monitorWrite("initrd Initialized! (F\' YEAH 0xDEADBEEF)\n");
	int i = 0;
	struct dirent *node = 0;
	while ((node = readdirFs(fsRoot, i)) != 0) {
		monitorWrite("Found file: ");
		monitorWrite(node->name);
		fsNode_t *fsnode = finddirFs(fsRoot, node->name);

		if ((fsnode->flags&0x7) == FS_DIRECTORY) {
			monitorWrite("\n\t(directory)\n");
		} else {
			monitorWrite("\n\t contents: \"");
			char buf[256];
			u32int sz = readFs(fsnode, 0, 256, buf);
			int j;
			for (j = 0; j < sz; j++)
				monitorPut(buf[j]);

			monitorWrite("\"\n");
		}
		i++;
	}

	installKeyboard();

	monitorPut('>');
	char *str;
	DBscanf("%s", &str);
	monitorWrite("Read from user: ");
	monitorWrite(str);
	monitorWrite("\n>");
	char c = getch();
	monitorWrite("Read from user: ");
	monitorPut(c);
	monitorWrite("\n>");
	

	return 0xDEADBEEF;
}