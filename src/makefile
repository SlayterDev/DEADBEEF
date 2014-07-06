# makefile -- Brad Slayter

SOURCES=boot.o main.o monitor.o common.o

CC=i686-elf-gcc

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -lgcc -std=gnu99
LDFLAGS=-T Link.ld -ffreestanding -O2 -nostdlib
ASFLAGS=-felf

all: $(SOURCES) link
	cp myos.bin isodir/boot/myos.bin

clean:
	-rm *.o myos.bin

link:
	i686-elf-gcc $(LDFLAGS) -o myos.bin $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<