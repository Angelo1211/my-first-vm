#include "vm.h"

// Memory
u16 mem[MAX_MEMORY];

// Registers
u16 reg[R_COUNT];

bool g_running = false;

// -----------------------------------------------------------------------------
// INTERNAL
// -----------------------------------------------------------------------------
u16 
sign_extend(u16 x, int bit_count)
{
	// If the most significant bit is set and we're parsing two's complement
	// data, then we're looking at a negative number. Extend by adding ones.
	if (x >> (bit_count - 1) & 1)
	{
		x |= (0xFFFF << bit_count);
	}
	return x;
}


void 
update_flags(u16 r)
{
	if (reg[r] == 0)
		reg[R_COND] = FL_ZRO;
	else if (reg[r] >> 15)
		reg[R_COND] = FL_NEG;
	else 
		reg[R_COND] = FL_POS;
}

void 
mem_write(u16 address, u16 val)
{
	mem[address] = val;
}

u16 
mem_read(u16 address)
{
	if (address == MR_KBSR)
	{
		if (check_key())
		{
			mem[MR_KBSR] = (1 << 15);
			mem[MR_KBDR] = (u16)getchar();
		}
		else
			mem[MR_KBSR] = 0;
	}

	return mem[address];
}
void 
op_add(u16 instr)
{
	// Get the destination register
	u16 dr = (instr >> 9) & 0x7; 

	// First operand
	u16 sr0 = (instr >> 6) & 0x7; 

	// Check mode immediate/register 
	if ((instr >> 5) & 0x1)
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


void
op_ldi(u16 instr)
{
	// Get the destination register
	u16 dr = (instr >> 9) & 0x7; 

	// Get the offset to the PC
	u16 PCoffset9 = sign_extend((instr & 0x1FF), 9);

	// The first address we gotta go fetch
	u16 ad0 = reg[R_PC] + PCoffset9;

	// The address at the offset + PC
	u16 ad1 = mem_read(ad0);
	u16 mr0 = mem_read(ad1);

	// writing the memory
	reg[dr] = mr0;

	// Updating the flags
	update_flags(dr);
}


void
op_and(u16 instr)
{
	// Get the destination register
	u16 dr = (instr >> 9) & 0x7; 

	// First operand
	u16 sr0 = (instr >> 6) & 0x7; 

	// Check mode immediate/register 
	if ((instr >> 5) & 0x1)
	{
		u16 imm5 = sign_extend(instr & 0x1F, 5);
		reg[dr] = reg[sr0] & imm5;
	}
	else 
	{
		u16 sr1 = instr & 0x7;
		reg[dr] = reg[sr1] & reg[sr0];
	}

	update_flags(dr);
}

void
op_not(u16 instr)
{
	// Get the destination register
	u16 dr = (instr >> 9) & 0x7; 

	// First operand
	u16 sr0 = (instr >> 6) & 0x7; 

	reg[dr] = ~reg[sr0];

	update_flags(dr);
}

void
op_br(u16 instr)
{
// This is what I wrote before I checked the cheatsheet, I feel dumb 
#if 0
	bool branch = false;
	if ((instr >> 11) & 0x1)
	{
		if (reg[R_COND] == FL_NEG)
			branch = true;
	}
	else if ((instr >> 10) & 0x1)
	{
		if (reg[R_COND] == FL_ZRO)
			branch = true;
	}
	else if ((instr >> 9) & 0x1)
	{
		if (reg[R_COND] == FL_POS)
			branch = true;
	}

	if (branch)
		reg[R_PC] += sign_extend(instr & 0x1FF, 9);
#else
	if (((instr >> 9) & 0x7) & reg[R_COND])
		reg[R_PC] += sign_extend(instr & 0x1FF, 9);
#endif
}

void
op_jmp(u16 instr)
{
	// Also handles the asm instruction RET, since it's just sr0 == 7
	reg[R_PC] = reg[(instr >> 6) & 0x7];
}

void
op_jsr(u16 instr)
{
	// Save the PC in register 7
	reg[R_R7] = reg[R_PC];

	if ((instr >> 11) & 0x1)
		reg[R_PC] += sign_extend(instr & 0x7FF, 11);
	else 
		reg[R_PC] = reg[(instr >> 6) & 0x7];
}

void
op_ld(u16 instr)
{
	// Get the destination register
	u16 dr = (instr >> 9) & 0x7; 

	// Get the address
	u16 adr = reg[R_PC] +  sign_extend(instr & 0x1FF, 9);

	// Read the values at the address
	reg[dr] = mem_read(adr);

	update_flags(dr);
}

void
op_ldr(u16 instr)
{
	// Get the destination register
	u16 dr = (instr >> 9) & 0x7; 

	// Get the register where the base address is stored
	u16 sr = (instr >> 6) & 0x7; 

	// Get the address
	u16 adr = reg[sr] + sign_extend(instr & 0x3F, 6);

	// Read the value at the address
	reg[dr] = mem_read(adr);

	update_flags(dr);
}

void
op_lea(u16 instr)
{
	// Get the destination register
	u16 dr = (instr >> 9) & 0x7; 

	// Get the address
	u16 adr = reg[R_PC] + sign_extend(instr & 0x1FF, 9);

	// Store the address in the register
	reg[dr] = adr;

	update_flags(dr);
}

void
op_st(u16 instr)
{
	// Get the register that has our data
	u16 sr = (instr >> 9) & 0x7; 

	// Get the address we want to write to
	u16 adr = reg[R_PC] + sign_extend(instr & 0x1FF, 9);

	// Write the data on the register to memory at address
	mem_write(adr, reg[sr]);
}

void
op_sti(u16 instr)
{
	// Get the register that has our data
	u16 sr = (instr >> 9) & 0x7; 

	// Get the address that has our address
	u16 adr0 = reg[R_PC] + sign_extend(instr & 0x1FF, 9);

	// The address we want to write to
	u16 adr1 = mem_read(adr0);

	// Write the data to memory
	mem_write(adr1, reg[sr]);
}

void
op_str(u16 instr)
{
	// Get the register that has our data
	u16 sr0 = (instr >> 9) & 0x7; 

	// Get the register that has our address
	u16 sr1 = (instr >> 6) & 0x7;

	// Get the full address by reading the register + offset
	u16 adr = reg[sr1] + sign_extend(instr & 0x3F , 6);

	// Write the data to memory
	mem_write(adr, reg[sr0]);
}

void
op_trap(u16 instr)
{
	// Store the register for when we return
	reg[R_R7] = reg[R_PC];
	
	switch (instr & 0xFF)
	{
		case TRAP_GETC:
		{
			reg[R_R0] = (u16)getchar();
			update_flags(R_R0);
		}break;

		case TRAP_OUT:
		{
			putc((char)reg[R_R0], stdout);
			fflush(stdout);
		}break;

		case TRAP_PUTS:
		{
			// one char per word
			u16* c = mem + reg[R_R0];
			while (*c)
			{
				putc((char)*c, stdout);
				++c;
			}
			fflush(stdout);
		}break;

		case TRAP_IN:
		{
			printf("Enter a character: ");
			char c = (char)getchar();
			putc(c, stdout);
			fflush(stdout);
			reg[R_R0] = (u16)c;
			update_flags(R_R0);
		}break;

		case TRAP_PUTSP:
		{
			u16* c = mem + reg[R_R0];

			while (*c)
			{
				char char1 = (*c) & 0xFF;
				putc(char1, stdout);

				char char2 = (*c) >> 8;
				if (char2) 
					putc(char2, stdout);

				++c;
			}
			fflush(stdout);
		}break;

		case TRAP_HALT:
		{
			puts("HALT: ACHTUNG!");
			fflush(stdout);
			g_running = false;
		}break;
	}
}

u16
swap16(u16 a)
{
	return (a << 8) | (a >> 8);
}

void
read_image_file(FILE *file)
{
	// The origin tells us where in memory to place the image
	u16 origin;
	fread(&origin, sizeof(origin), 1, file);
	origin = swap16(origin);

	u16 max_read = (u16)(MAX_MEMORY - origin);
	u16 *p = mem + origin;
	size_t read = fread(p, sizeof(u16), max_read, file);

	// Little endian swap
	while (read-- > 0)
	{
		*p = swap16(*p);
		++p;
	}
}

// -----------------------------------------------------------------------------
// EXTERNAL
// -----------------------------------------------------------------------------

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
	g_running = true;

	while (g_running)
	{
		// Fetch next instruction and increment the program counter
		u16 instr = mem_read(reg[R_PC]++);

		// Decode the instruction (get the top 4 bits) to get the OP code
		u16 op = instr >> 12; 

		switch (op)
		{
			case OP_ADD:  op_add(instr);  break;
			case OP_AND:  op_and(instr);  break;
			case OP_NOT:  op_not(instr);  break;
			case OP_BR:   op_br(instr);   break;
			case OP_JMP:  op_jmp(instr);  break;
			case OP_JSR:  op_jsr(instr);  break;
			case OP_LD:   op_ld(instr);   break;
			case OP_LDI:  op_ldi(instr);  break;
			case OP_LDR:  op_ldr(instr);  break;
			case OP_LEA:  op_lea(instr);  break;
			case OP_ST:   op_st(instr);   break;
			case OP_STI:  op_sti(instr);  break;
			case OP_STR:  op_str(instr);  break;
			case OP_TRAP: op_trap(instr); break;

			case OP_RES: case OP_RTI:
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

int 
VM_ReadImage(const char *image_path)
{
	FILE* file = fopen(image_path, "rb");
	if (!file) 
		return 0;
	
	read_image_file(file);
	fclose(file);
	return 1;
}