#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "vm.h"




// Define opcodes
enum OpCode
{
    NuLL = 0x00,
    // Memory instructions
    STORE = 0xEA,      // STORE registerIndex, value
    STORE_VAR = 0xEB,  // STORE_VAR sizeOfName name sizeOfValue value
    LOAD_REG = 0xFA,   // LOAD registerIndex sizeOfValue Value
    LOAD_VAR = 0xFB,   // LOAD_VAR registerIndex $varname
    CREATE_VAR = 0xCA, // CREATE_VAR sizeOfName Name
    ADD = 0xF3,        // ADD reg1, reg2, destReg
    SUB = 0xF4,        // SUB reg1, reg2, destReg
    MUL = 0xF5,        // MUL reg1, reg2, destReg
    DIV = 0xF6,        // DIV reg1, reg2, destReg
    CMP = 0xF8,        // CMP reg1, reg2
    JMP = 0xF9,        // JMP addr
    JMPIFZ = 0xF0,     // JMPIFZ reg dest
    PRINT = 0xAA,      // PRINT reg
    READ = 0xAB,       // PRINT reg
    HALT = 0xFF,       // HALT (stop execution)
    READ_R = 0xFE,     // READ_R registerIndex
    POP_STACK = 0xC0,  // pops stack value into RET
    PUSH_STACK = 0xC1, // PUSH_STACK sizeOfValue value
    RSIZE_OF = 0xA0,   // RSIZE_OF registerIndex
    VSIZE_OF = 0xA1,   // VSIZE_OF sizeOfName name
};

// Registers
#define REGISTER_COUNT 16
int64_t registers[REGISTER_COUNT];

// Memory (simplified)
#define MEMORY_SIZE 4096
uint8_t memory[MEMORY_SIZE];

// Program Counter
int64_t pc = 0;

// Function to fetch the next byte in bytecode
uint8_t fetch(uint8_t *bytecode)
{
    return bytecode[pc++];
}

// Function to fetch multiple bytes
void fetch_bytes(uint8_t *bytecode, uint8_t *dest, int size)
{
    for (int i = 0; i < size; i++)
    {
        dest[i] = fetch(bytecode);
    }
}

// Function to set a register's value
void set_register(int reg, int64_t value)
{
    if (reg >= 0 && reg < REGISTER_COUNT)
    {
        registers[reg] = value;
    }
}

// Function to get a register's value
int64_t get_register(int reg)
{
    if (reg >= 0 && reg < REGISTER_COUNT)
    {
        return registers[reg];
    }
    return 0;
}

// Convert byte array to long (for simplicity, assume 8-byte values)
int64_t bytes_to_long(uint8_t *bytes)
{
    int64_t value = 0;
    for (int i = 0; i < 8; i++)
    {
        value |= ((int64_t)bytes[i] << (i * 8));
    }
    return value;
}

// Run the bytecode
void run_vm(uint8_t *bytecode, int64_t length)
{
    while (pc < length)
    {
        uint8_t instruction = fetch(bytecode);

        switch (instruction)
        {
        case STORE:
        {
            uint8_t reg = fetch(bytecode);
            uint8_t value = fetch(bytecode);
            set_register(reg, value);
            break;
        }
        case ADD:
        {
            uint8_t reg1 = fetch(bytecode);
            uint8_t reg2 = fetch(bytecode);
            uint8_t dest = fetch(bytecode);
            set_register(dest, get_register(reg1) + get_register(reg2));
            break;
        }
        case SUB:
        {
            uint8_t reg1 = fetch(bytecode);
            uint8_t reg2 = fetch(bytecode);
            uint8_t dest = fetch(bytecode);
            set_register(dest, get_register(reg1) - get_register(reg2));
            break;
        }
        case MUL:
        {
            uint8_t reg1 = fetch(bytecode);
            uint8_t reg2 = fetch(bytecode);
            uint8_t dest = fetch(bytecode);
            set_register(dest, get_register(reg1) * get_register(reg2));
            break;
        }
        case DIV:
        {
            uint8_t reg1 = fetch(bytecode);
            uint8_t reg2 = fetch(bytecode);
            uint8_t dest = fetch(bytecode);
            int64_t divisor = get_register(reg2);
            if (divisor != 0)
            {
                set_register(dest, get_register(reg1) / divisor);
            }
            else
            {
                printf("Error: Division by zero\n");
                exit(1);
            }
            break;
        }
        case LOAD_REG:
        {
            uint8_t reg = fetch(bytecode);
            uint8_t size = fetch(bytecode);
            uint8_t value_bytes[8];
            fetch_bytes(bytecode, value_bytes, size);
            int64_t value = bytes_to_long(value_bytes);
            set_register(reg, value);
            break;
        }
        case HALT:
        {
            printf(":: HALT. Exiting.\n");
            return;
        }
        case PRINT:
        {
            uint8_t size = fetch(bytecode);
            uint8_t value_bytes[size];
            fetch_bytes(bytecode, value_bytes, size);
            write(1, value_bytes, size);
            return;
        }
        default:
        {
            printf("Error: Unknown instruction %x\n", instruction);
            return;
        }
        }
    }
}

// Function to read bytecode from a file
uint8_t *load_bytecode(const char *filename, size_t *length)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *bytecode = (uint8_t *)malloc(*length);
    if (!bytecode)
    {
        printf("Error: Could not allocate memory for bytecode\n");
        fclose(file);
        exit(1);
    }

    fread(bytecode, 1, *length, file);
    fclose(file);
    return bytecode;
}

void motys_vm_init(MotysVM *vm)
{
    for (int i = 0; i < VM_REGISTER_COUNT; i++)
        vm->registers[i] = 0;
    for (int i = 0; i < VM_MEMORY_SIZE; i++)
        vm->memory[i] = 0;
    vm->pc = 0;
}
