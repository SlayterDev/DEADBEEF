// descriptorTable.h -- Brad Slayter

// This struct contains one GDT entry
// "packed" tells GCC not to change alignment
struct gdtEntryStruct {
	u16int limitLow;	// lower 16 bits of the limit
	u16int baseLow;		// lower 16 bits of the base
	u8int baseMiddle;	// the next 8 bits of the base
	u8int access;		// access flags. What ring it can be used in
	u8int granularity;
	u8int baseHigh;		// the last 8 bits of the base
} __attribute__((packed));
typedef struct gdtEntryStruct gdtEntry_t;

struct gdtPtrStruct {
	u16int limit;
	u32int base;
} __attribute__((packed));
typedef struct gdtPtrStruct gdtPtr_t;

struct idtEntryStruct {
	u16int baseLo;
	u16int sel;
	u8int  always0;
	u8int  flags;
	u16int baseHi;
} __attribute__((packed));
typedef struct idtEntryStruct idtEntry_t;

struct idtPtrStruct {
	u16int limit;
	u32int base;
}__attribute__((packed));
typedef struct idtPtrStruct idtPtr_t;

// ASM ISR handlers
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void initDescriptorTables();