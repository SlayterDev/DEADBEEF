// initrd.c -- Brad Slayter

#include "initrd.h"
#include "monitor.h"

initrdHeader_t *initrdHeader;
initrdFileHeader_t *fileHeaders;
fsNode_t *initrdRoot;
fsNode_t *initrdDev;
fsNode_t *rootNodes;
int nRootNodes;

struct dirent dirent;

static u32int initrdRead(fsNode_t *node, u32int offset, u32int size, u8int *buffer) {
	//monitorWrite("Get Header\n");
	initrdFileHeader_t header = fileHeaders[node->inode];
	if (offset > header.length)
		return 0;

	if (offset + size > header.length)
		size = header.length - offset;

	//monitorWrite("Copy Mem\n");
	memcpy(buffer, (u8int *)(header.offset + offset), size);
	return size;
}

static struct dirent *initrdReaddir(fsNode_t *node, u32int index) {
	//monitorWrite("Read Dir\n");
	if (node == initrdRoot && index == 0) {
		strcpy(dirent.name, "dev");
		dirent.name[3] = 0; // make sure string is null terminated
		dirent.ino = 0;
		return &dirent;
	}

	if (index - 1 >= nRootNodes)
		return 0;

	//monitorWrite("Copy Names");
	strcpy(dirent.name, rootNodes[index-1].name);
	dirent.name[strlen(rootNodes[index-1].name)] = 0; // again null terminate
	dirent.ino = rootNodes[index-1].inode;
	return &dirent;
}

static fsNode_t *initrdFinddir(fsNode_t *node, char *name) {
	if (node == initrdRoot && !strcmp(name, "dev"))
		return initrdDev;

	int i;
	for (i = 0; i < nRootNodes; i++) {
		if (!strcmp(name, rootNodes[i].name))
			return &rootNodes[i];
	}

	return 0;
}

fsNode_t *initializeInitrd(u32int location) {
	initrdHeader = (initrdHeader_t *)location;
	fileHeaders = (initrdFileHeader_t *)(location + sizeof(initrdHeader_t));

	//monitorWrite("Create root\n");
	// init Root ('/') dir
	initrdRoot = (fsNode_t *)kmalloc(sizeof(fsNode_t));
	strcpy(initrdRoot->name, "initrd");
	initrdRoot->mask = initrdRoot->uid = initrdRoot->gid = initrdRoot->inode = initrdRoot->length = 0;
	initrdRoot->flags = FS_DIRECTORY;
	initrdRoot->read = 0;
	initrdRoot->write = 0;
	initrdRoot->open = 0;
	initrdRoot->close = 0;
	initrdRoot->readdir = &initrdReaddir;
	initrdRoot->finddir = &initrdFinddir;
	initrdRoot->ptr = 0;
	initrdRoot->impl = 0;

	//monitorWrite("Create /dev/\n");
	// init /dev/ dir
	initrdDev = (fsNode_t *)kmalloc(sizeof(fsNode_t));
	strcpy(initrdDev->name, "dev");
	initrdDev->mask = initrdDev->uid = initrdDev->gid = initrdDev->inode = initrdDev->length = 0;
	initrdDev->flags = FS_DIRECTORY;
	initrdDev->read = 0;
	initrdDev->write = 0;
	initrdDev->open = 0;
	initrdDev->close = 0;
	initrdDev->readdir = &initrdReaddir;
	initrdDev->finddir = &initrdFinddir;
	initrdDev->ptr = 0;
	initrdDev->impl = 0;

	//monitorWrite("Malloc rootNodes\n");
	rootNodes = (fsNode_t *)kmalloc(sizeof(fsNode_t ) * initrdHeader->nfiles);
	nRootNodes = initrdHeader->nfiles;

	//monitorWrite("Enumerate rootNodes\n");
	int i;
	for (i = 0; i < initrdHeader->nfiles; i++) {
		fileHeaders[i].offset += location;
		strcpy(rootNodes[i].name, fileHeaders[i].name);
		rootNodes[i].mask = rootNodes[i].uid = rootNodes[i].gid = 0;
		rootNodes[i].length = fileHeaders[i].length;
		rootNodes[i].inode = i;
		rootNodes[i].flags = FS_FILE;
		rootNodes[i].read = &initrdRead;
		rootNodes[i].write = 0;
		rootNodes[i].readdir = 0;
		rootNodes[i].finddir = 0;
		rootNodes[i].open = 0;
		rootNodes[i].close = 0;
		rootNodes[i].impl = 0;
	}
	return initrdRoot;
}