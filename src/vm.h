#pragma once

#include "common.h"


// Constants
#define PC_START (0x3000)

// Memory
#define MAX_MEMORY (2 << 16)
extern u16 mem[MAX_MEMORY];

// Registers
enum Registers
{
	// General purpose Registers
	R_R0 = 0,
	R_R1,
	R_R2,
	R_R3,
	R_R4,
	R_R5,
	R_R6,
	R_R7,

	// Program Counter
	R_PC,

	// Condition
	R_COND,

	R_COUNT
};
extern u16 reg[R_COUNT];

// Memory Mapped Registers
enum MMRegisters
{
    MR_KBSR = 0xFE00, /* keyboard status */
    MR_KBDR = 0xFE02  /* keyboard data */
};

// Instruction Set
enum OpCodes
{
	OP_BR = 0, /* branch */
	OP_ADD,    /* add  */
	OP_LD,     /* load */
	OP_ST,     /* store */
	OP_JSR,    /* jump register */
	OP_AND,    /* bitwise and */
	OP_LDR,    /* load register */
	OP_STR,    /* store register */
	OP_RTI,    /* unused */
	OP_NOT,    /* bitwise not */
	OP_LDI,    /* load indirect */
	OP_STI,    /* store indirect */
	OP_JMP,    /* jump */
	OP_RES,    /* reserved (unused) */
	OP_LEA,    /* load effective address */
	OP_TRAP    /* execute trap */
};

enum Conditions
{
	FL_POS = 1 << 0, /* P */
	FL_ZRO = 1 << 1, /* Z */
	FL_NEG = 1 << 2, /* N */
};

enum TrapCodes
{
	TRAP_GETC  = 0x20,   /* get character from keyboard, not echoed onto the terminal */
	TRAP_OUT   = 0x21,   /* output a character */
	TRAP_PUTS  = 0x22,   /* output a word string */
	TRAP_IN    = 0x23,   /* get character from keyboard, echoed onto the terminal */
	TRAP_PUTSP = 0x24,   /* output a byte string */
	TRAP_HALT  = 0x25    /* halt the program */
};

// Procedures
void VM_Init(void);
void VM_Run(void);
int VM_Shutdown(void);

int VM_ReadImage(const char *image_path);