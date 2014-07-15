// FSGen.c -- Brad Slayter

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct initrdHeader {
	unsigned char magic; // For consistency (0xDEADBEEF \m/)
	char name[64];
	unsigned int offset; // Offset in the initrd where the file starts
	unsigned int length; // Length of file
};

int main(int argc, char *argv[]) {
	int nHeaders = (argc - 1) / 2;
	struct initrdHeader headers[64];
	printf("[*] Size of header: %d\n", sizeof(struct initrdHeader));
	unsigned int off = sizeof(struct initrdHeader) * 64 + sizeof(int);
	int i;
	for (i = 0; i < nHeaders; i++) {
		printf("[+] Writing file %s->%s at 0x%x\n", argv[i*2+1], argv[i*2+2], off);
		strcpy(headers[i].name, argv[i*2+2]);
		FILE *stream = fopen(argv[i*2+1], "r");
		if (stream == 0) {
			printf("[-] ERROR: file not found: %s\n", argv[i*2+1]);
			return 1;
		}
		fseek(stream, 0, SEEK_END);
		headers[i].length = ftell(stream);
		off += headers[i].length;
		fclose(stream);
		headers[i].magic = 0xDB; // short for 0xDEADBEEF \m/
	}

	FILE *wstream = fopen("../initrd.img", "w");
	unsigned char *data = (unsigned char *)malloc(off);
	fwrite(&nHeaders, sizeof(int), 1, wstream);
	fwrite(headers, sizeof(struct initrdHeader), 64, wstream);

	for (i = 0; i < nHeaders; i++) {
		FILE *stream = fopen(argv[i*2+1], "r");
		unsigned char *buf = (unsigned char *)malloc(headers[i].length);
		fread(buf, 1, headers[i].length, stream);
		fwrite(buf, 1, headers[i].length, wstream);
		fclose(stream);
		free(buf);
	}

	fclose(wstream);
	free(data);

	return 0;
}