#ifndef VM_H
#define VM_H
#include <stdint.h>
#define VM_REGISTER_COUNT 32
#define VM_MEMORY_SIZE 4096

typedef struct {
	int64_t registers[VM_REGISTER_COUNT];	// General-purpose registers
	uint8_t memory[VM_MEMORY_SIZE];	// Memory space
	int64_t pc;			// Program Counter
} MotysVM;

// Function to initialize the VM
void motys_vm_init(MotysVM *vm);
void run_vm(uint8_t *bytecode, int64_t length);

#endif // VM_H
