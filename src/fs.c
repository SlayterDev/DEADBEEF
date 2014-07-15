// fs.c -- Brad Slayter

#include "fs.h"

fsNode_t *fsRoot = 0; // The root of our file system

u32int readFs(fsNode_t *node, u32int offset, u32int size, u8int *buffer) {
	if (node->read != 0)
		return node->read(node, offset, size, buffer);
	else
		return 0;
}

u32int writeFs(fsNode_t *node, u32int offset, u32int size, u8int *buffer) {
	if (node->write != 0)
		return node->write(node, offset, size, buffer);
	else
		return 0;
}

void openFs(fsNode_t *node, u8int read, u8int write) {
	if (node->open != 0)
		return node->open(node);
}

void closeFs(fsNode_t *node) {
	if (node->close != 0)
		return node->close(node);
}

struct dirent *readdirFs(fsNode_t *node, u32int index) {
	if ((node->flags&0x7) == FS_DIRECTORY && node->readdir != 0)
		return node->readdir(node, index);
	else
		return 0;
}

fsNode_t *finddirFs(fsNode_t *node, char *name) {
	if ((node->flags&0x7) == FS_DIRECTORY && node->finddir != 0)
		return node->finddir(node, name);
	else
		return 0;
}