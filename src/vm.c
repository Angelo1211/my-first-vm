#include "vm.h"

// Memory
u16 mem[MAX_MEMORY];

// Registers
u16 reg[R_COUNT];

// Internal
u16 sign_extend(u16 x, int bit_count);
void update_flags(u16 r);
void op_add(u16 instr);

// External
void 
VM_Init(void)
{
	// The condition flag must always have a flag set at all times
	// so we initialize it to zero
	reg[R_COND] = FL_ZRO;

	// The PC must always start at the same address
	reg[R_PC] = PC_START;
}

void 
VM_Run(void)
{
	bool running = true;

	while (running)
	{
		// Fetch next instruction and increment the program counter
		u16 instr = mem_read(reg[R_PC]++);

		// Decode the instruction (get the top 4 bits) to get the OP code
		u16 op = instr >> 12; 

		switch (op)
		{
			case OP_ADD: op_add(instr); break;
			case OP_AND: break;
			case OP_NOT: break;
			case OP_BR: break;
			case OP_JMP: break;
			case OP_JSR: break;
			case OP_LD: break;
			case OP_LDI: break;
			case OP_LDR: break;
			case OP_LEA: break;
			case OP_ST: break;
			case OP_STI: break;
			case OP_STR: break;
			case OP_TRAP: break;

			case OP_RES:
			case OP_RTI:
			default:
				assert(false);
			return;
		}
	}
}

int 
VM_Shutdown(void)
{
	int result = 0;

	return result;
}

// Internal
u16 sign_extend(u16 x, int bit_count)
{
	// If the most significant bit is set and we're parsing two's complement
	// data, then we're looking at a negative number. Extend by adding ones.
	if (x >> (bit_count - 1) & 1)
	{
		x |= (0xFFFF << bit_count);
	}
	return x;
}

void update_flags(u16 r)
{
	if (reg[r] == 0)
		reg[R_COND] = FL_ZRO;
	else if (reg[r] >> 15)
		reg[R_COND] = FL_NEG;
	else 
		reg[R_COND] = FL_POS;
}


void op_add(u16 instr)
{
	// Get the destination register
	u16 dr = (instr >> 9) & 0x7; 

	// First operand
	u16 sr0 = (instr >> 6) & 0x7; 

	// Check immediate mode or register mode
	bool im = (instr >> 5) & 0x1;

	if (im)
	{
		u16 imm5 = sign_extend(instr & 0x1F, 5);
		reg[dr] = reg[sr0] + imm5;
	}
	else 
	{
		u16 sr1 = instr & 0x7;
		reg[dr] = reg[sr1] + reg[sr0];
	}

	update_flags(dr);
}