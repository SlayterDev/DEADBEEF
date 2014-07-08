// main.c -- Brad Slayter

#include "common.h"
#include "monitor.h"
#include "descriptorTables.h"
#include "timer.h"
#include "paging.h"

int main(struct multiboot *mbootPtr) {
	// Init code here
	monitorClear();
	initDescriptorTables();
	initialisePaging();

	monitorWriteHex(0xDEADBABA);
	monitorWrite("\n==================\n");

	char dog[4] = "dog\0";
	int ret = strcmp(dog, dog);
	if (ret == 0)
		monitorWrite("Dog is equal to dog\n");
	else
		monitorWrite("Dog is wrong\n");

	monitorWrite("==================\n");

	char cat[4] = "cat\0";
	ret = strcmp(dog, cat);
	if (ret == 1)
		monitorWrite("Dog is greater than cat\n");
	else
		monitorWrite("Dog v. cat is wrong\n");

	monitorWrite("==================\n");

	char epitome[8] = "epitome\0";
	ret = strcmp(dog, epitome);
	if (ret == -1)
		monitorWrite("Dog is less than epitome\n");
	else
		monitorWrite("Dog v. epitome is wrong\n");

	monitorWrite("==================\n");

	char str1[50] = "Brad Slayter";
	char str2[50];
	strcpy(str2, str1);
	monitorWrite("str2 contains: ");
	monitorWrite(str2);
	monitorPut('\n');
	monitorWrite("==================\n");

	u32int *ptr = (u32int *)0xA0000000;
	u32int doPageFault = *ptr;

	return 0xDEADBABA;
}