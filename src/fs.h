// fs.h -- Brad Slayter
// This file defines an abstraction for a file system

#ifndef FS_H
#define FS_H

#include "common.h"

struct fsNode;

typedef u32int (*readType_t)(struct fsNode*, u32int, u32int, u8int *);
typedef u32int (*writeType_t)(struct fsNode*, u32int, u32int, u8int *);
typedef void (*openType_t)(struct fsNode *);
typedef void (*closeType_t)(struct fsNode *);
typedef struct dirent * (*readdirType_t)(struct fsNode *, u32int);
typedef struct fsNode * (*finddirType_t)(struct fsNode *, char *name);

typedef struct fsNode {
	char name[128];
	u32int mask;
	u32int uid;
	u32int gid;
	u32int flags;
	u32int inode;
	u32int length;
	u32int impl;
	readType_t read;
	writeType_t write;
	openType_t open;
	closeType_t close;
	readdirType_t readdir;
	finddirType_t finddir;
	struct fsNode *ptr;
} fsNode_t;

struct dirent {
	char name[128];
	u32int ino;
};

#define FS_FILE 0x01
#define FS_DIRECTORY 0x02
#define FS_CHARDEVICE 0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE 0x05
#define FS_SYMLINK 0x06
#define FS_MOUNTPOINT 0x08

extern fsNode_t *fsRoot;

// Read from file
u32int readFs(fsNode_t *node, u32int offset, u32int size, u8int *buffer);

// Write to file
u32int writeFs(fsNode_t *node, u32int offset, u32int size, u8int *buffer);

// open file/dir
void openFs(fsNode_t *node, u8int read, u8int write);

// Close file/dir
void closeFs(fsNode_t *node);

// Read dir
struct dirent *readdirFs(fsNode_t *node, u32int index);

// Find dir
fsNode_t *finddirFs(fsNode_t *node, char *name);

#endif