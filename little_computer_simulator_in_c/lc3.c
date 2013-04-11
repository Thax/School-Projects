#include "lc3.h"

void lc3_init(lc3machine* state) 
{
	// Initialize the lc3 state according to the assignment.
    state->pc = 0x3000;
    state->cc = LC3_ZERO;
    for (int i = 0; i < 65536; i++) {
        state->mem[i] = 0;
    }
}

void lc3_load(lc3machine* state, FILE* program)
{
	// Reread the section on the .obj file format.
    u16 startAddr = (fgetc(program) << 8) | (fgetc(program));
    u32 instructionEnd = fgetc(program);
    u16 currentMem = startAddr;
    u16 fullInstruction;
    // Read and construct instruction lines.
    while(instructionEnd != EOF) {
        fullInstruction = fgetc(program) | (instructionEnd << 8);
        state->mem[currentMem] = fullInstruction;
        currentMem++;
        instructionEnd = fgetc(program);
    }
    // Close the file stream
    fclose(program);
}

void lc3_step_one(lc3machine* state)
{
	// If the machine is not halted
	// Fetch an instruction
	// And call lc3_execute 
    if (state->mem[state->pc] != LC3_TRAP_FULL_HALT) {
        u16 instruction = lc3_fetch(state);
        lc3_execute(state, instruction);
    }
}

void lc3_run(lc3machine* state, int num_steps)
{
    // If machine is haled, do nothing.  Otherwise, step through the program.
    if (state->mem[state->pc] != LC3_TRAP_FULL_HALT) {
        if (num_steps == -1) {
            while(state->mem[state->pc] != LC3_TRAP_FULL_HALT) {
                lc3_step_one(state);
            }
        } else {
            for (int i = 0; i < num_steps; i++) {
                lc3_step_one(state);
            }
        }
    }
}

unsigned short lc3_fetch(lc3machine* state)
{
	/* Think back to when we first started assembly to code this */
	/* What happens during the fetch stage? */
    u16 instruction = state->mem[state->pc];
    state->pc++;
    return instruction;
}

void lc3_execute(lc3machine* state, unsigned short instruction)
{
    // Get the opcode and choose actions based on that.
    u32 operation = GET_SUBWORD(instruction, 15, 12);
    switch (operation) {
        case (LC3_OP_ADD): {
            if (GET_SUBWORD(instruction, 5, 5)) {
                u32 dr = GET_SUBWORD(instruction, 11, 9);
                u32 sr1 = GET_SUBWORD(instruction, 8, 6);
                u32 imm5 = GET_SUBWORD(instruction, 4, 0);
                lc3_add_imm(state, dr, sr1, imm5);
            } else {
                u32 dr = GET_SUBWORD(instruction, 11, 9);
                u32 sr1 = GET_SUBWORD(instruction, 8, 6);
                u32 sr2 = GET_SUBWORD(instruction, 2, 0);
                lc3_add_reg(state, dr, sr1, sr2);
            }
            break;
        } case (LC3_OP_AND): {
            if (GET_SUBWORD(instruction, 5, 5)) {
                u32 dr = GET_SUBWORD(instruction, 11, 9);
                u32 sr1 = GET_SUBWORD(instruction, 8, 6);
                u32 imm5 = GET_SUBWORD(instruction, 4, 0);
                lc3_and_imm(state, dr, sr1, imm5);
            } else {
                u32 dr = GET_SUBWORD(instruction, 11, 9);
                u32 sr1 = GET_SUBWORD(instruction, 8, 6);
                u32 sr2 = GET_SUBWORD(instruction, 2, 0);
                lc3_and_reg(state, dr, sr1, sr2);
            }
            break;
        } case (LC3_OP_BR): {
            u32 cc = GET_SUBWORD(instruction, 11, 9);
            u32 offset = GET_SUBWORD(instruction, 8, 0);
            lc3_br(state, cc, offset);
            break;
        } case (LC3_OP_JMP): {
            u32 reg = GET_SUBWORD(instruction, 8, 6);
            lc3_jmp(state, reg);
            break;
        } case (LC3_OP_JSR): {
            if (GET_SUBWORD(instruction, 11, 11)) {
                u32 offset = GET_SUBWORD(instruction, 10, 0);
                lc3_jsr_offset(state, offset);
            } else {
                u32 baser = GET_SUBWORD(instruction, 8, 6);
                lc3_jsr_reg(state, baser);
            }
            break;
        } case (LC3_OP_LD): {
            u32 dr = GET_SUBWORD(instruction, 11, 9);
            u32 offset = GET_SUBWORD(instruction, 8, 0);
            lc3_ld(state, dr, offset);
            break;
        } case (LC3_OP_LDI): {
            u32 dr = GET_SUBWORD(instruction, 11, 9);
            u32 offset = GET_SUBWORD(instruction, 8, 0);
            lc3_ldi(state, dr, offset);
            break;
        } case (LC3_OP_LDR): {
            u32 dr = GET_SUBWORD(instruction, 11, 9);
            u32 baser = GET_SUBWORD(instruction, 8, 6);
            u32 offset = GET_SUBWORD(instruction, 5, 0);
            lc3_ldr(state, dr, baser, offset);
            break;
        } case (LC3_OP_LEA): {
            u32 dr = GET_SUBWORD(instruction, 11, 9);
            u32 offset = GET_SUBWORD(instruction, 8, 0);
            lc3_lea(state, dr, offset);
            break;
        } case (LC3_OP_NOT): {
            u32 dr = GET_SUBWORD(instruction, 11, 9);
            u32 sr = GET_SUBWORD(instruction, 8, 6);
            lc3_not(state, dr, sr);
            break;
        } case (LC3_OP_RTI): {
            // Not using RTI
            break;
        } case (LC3_OP_ST): {
            u32 sr = GET_SUBWORD(instruction, 11, 9);
            u32 offset = GET_SUBWORD(instruction, 8, 0);
            lc3_st(state, sr, offset);
            break;
        } case (LC3_OP_STI): {
            u32 sr = GET_SUBWORD(instruction, 11, 9);
            u32 offset = GET_SUBWORD(instruction, 8, 0);
            lc3_sti(state, sr, offset);
            break;
        } case (LC3_OP_STR): {
            u32 sr = GET_SUBWORD(instruction, 11, 9);
            u32 baser = GET_SUBWORD(instruction, 8, 6);
            u32 offset = GET_SUBWORD(instruction, 5, 0);
            lc3_str(state, sr, baser, offset);
            break;
        } case (LC3_OP_TRAP): {
            u32 trapvect = GET_SUBWORD(instruction, 7, 0);
            lc3_trap(state, trapvect);
            break;
        }
    }
}

void lc3_trap(lc3machine* state, unsigned char vector8)
{
    // For predefined traps, use 'fake' solutions.
    switch (vector8) {
        case (LC3_TRAP_GETC): {
            state->regs[0] = fgetc(stdin);
            break;
        }
        case (LC3_TRAP_OUT): {
            printf("%c", state->regs[0]);
            break;
        }
        case (LC3_TRAP_PUTS): {
            int i = state->regs[0];
            while (state->mem[i] != 0x0000) {
                printf("%c", state->mem[i]);
                i++;
            }
            break;
        }
        case (LC3_TRAP_IN): {
            printf("Enter a single character: ");
            state->regs[0] = fgetc(stdin);
            break;
        }
        case (LC3_TRAP_PUTSP): {
            int i = state->regs[0];
            while (state->mem[i] != 0x0000) {
                char char1 = state->mem[i] >> 8;
                char char2 = state->mem[i] ^ (char1 << 8);
                printf("%c%c", char1, char2);
                i++;
            }
            break;
        }
        case (LC3_TRAP_HALT): {
            // Find some way to 'stop' the program.
            state->pc--;
            break;
        }
        // For custom traps, behave like a normal trap.
        default: {
            state->regs[7] = state->pc;
            state->pc = state->mem[vector8]; 
            break;
        }
    }
}

/* lc3_add_reg
   ADD (register) execution function called from within lc3_execute.
*/
void lc3_add_reg(lc3machine* state, int dr, int sr1, int sr2) {
    state->regs[dr] = state->regs[sr1] + state->regs[sr2];
    lc3_update_cc(state, state->regs[dr]);
}

/* lc3_add_imm
   ADD (imm) execution function called from within lc3_execute.
*/
void lc3_add_imm(lc3machine* state, int dr, int sr1, int imm) {
    imm = lc3_sign_extend(imm, 5, 16);
    state->regs[dr] = state->regs[sr1] + (s16)imm;
    lc3_update_cc(state, state->regs[dr]);
}

// Helper function to ensure we sign extend.
int lc3_sign_extend(int value, int currentSize, int endSize) {
    int extendedBit = GET_SUBWORD(value, currentSize - 1, currentSize - 1);
    if (extendedBit) {
        int mask = 1;
        for (int i = 1; i < (endSize - currentSize); i++) {
            mask = mask << 1;
            mask = mask | 1;
        }
        mask = mask << currentSize;
        value = value | (mask);
    }
    return value;
}

/* lc3_and_reg
   AND (register) execution function called from within lc3_execute.
*/
void lc3_and_reg(lc3machine* state, int dr, int sr1, int sr2) {
    state->regs[dr] = state->regs[sr1] & state->regs[sr2];
    lc3_update_cc(state, state->regs[dr]);
}

/* lc3_and_imm
   AND (imm) execution function called from within lc3_execute.
*/
void lc3_and_imm(lc3machine* state, int dr, int sr1, int imm) {
    imm = lc3_sign_extend(imm, 5, 16);
    state->regs[dr] = state->regs[sr1] & (s16)imm;
    lc3_update_cc(state, state->regs[dr]);
}

/* lc3_br
   BR execution function called from within lc3_execute.
*/
void lc3_br(lc3machine* state, int cc, int offset) {
    if ((state->cc & cc) > 0) {
        offset = lc3_sign_extend(offset, 9, 16);
        state->pc = state->pc + offset;
    }
}

/* lc3_jmp
   JMP execution function called from within lc3_execute. (Also used for ret)
*/
void lc3_jmp(lc3machine* state, int baser) {
    state->pc = state->regs[baser];
}

/* lc3_jsr_offset
    JSR execution function called from within lc3_execute.
*/
void lc3_jsr_offset(lc3machine* state, int offset) {
    offset = lc3_sign_extend(offset, 11, 16);
    int temp = state->pc;
    state->pc = state->pc + offset;
    state->regs[7] = temp;
}

/* lc3_jsr_reg
    JSRR execution function called from within lc3_execute.
*/
void lc3_jsr_reg(lc3machine* state, int baser) {
    int temp = state->pc;
    state->pc = state->regs[baser];
    state->regs[7] = temp;
}

/* lc3_ld
   LD execution function called from within lc3_execute.
*/
void lc3_ld(lc3machine* state, int dr, int offset) {
    offset = lc3_sign_extend(offset, 9, 16);
    state->regs[dr] = state->mem[state->pc + offset];
    lc3_update_cc(state, state->regs[dr]);
}

/* lc3_ldi
   LDI execution function called from within lc3_execute.
*/
void lc3_ldi(lc3machine* state, int dr, int offset) {
    offset = lc3_sign_extend(offset, 9, 16);
    state->regs[dr] = state->mem[state->mem[state->pc + offset]];
    lc3_update_cc(state, state->regs[dr]);
}

/* lc3_ldr
   LDR execution function called from within lc3_execute.
*/
void lc3_ldr(lc3machine* state, int dr, int baser, int offset) {
    offset = lc3_sign_extend(offset, 6, 16);
    state->regs[dr] = state->mem[state->regs[baser] + offset];
    lc3_update_cc(state, state->regs[dr]);
}

/* lc3_lea
   LEA execution function called from within lc3_execute.
*/
void lc3_lea(lc3machine* state, int dr, int offset) {
    offset = lc3_sign_extend(offset, 9, 16);
    state->regs[dr] = state->pc + offset;
    lc3_update_cc(state, state->regs[dr]);
}

/* lc3_not
   NOT execution function called from within lc3_execute.
*/
void lc3_not(lc3machine* state, int dr, int sr) {
    state->regs[dr] = ~(state->regs[sr]);
    lc3_update_cc(state, state->regs[dr]);
}

/* lc3_st
   ST execution function called from within lc3_execute.
*/
void lc3_st(lc3machine* state, int sr, int offset) {
    offset = lc3_sign_extend(offset, 9, 16);
    state->mem[state->pc + offset] = state->regs[sr];
}

/* lc3_sti
   STI execution function called from within lc3_execute.
*/
void lc3_sti(lc3machine* state, int sr, int offset) {
    offset = lc3_sign_extend(offset, 9, 16);
    state->mem[state->mem[state->pc + offset]] = state->regs[sr];
}

/* lc3_str
   STR execution function called from within lc3_execute.
*/
void lc3_str(lc3machine* state, int sr, int baser, int offset) {
    offset = lc3_sign_extend(offset, 6, 16);
    state->mem[state->regs[baser] + offset] = state->regs[sr];
}

/* lc3_update_cc
    Update the condition code of the machine.
*/
void lc3_update_cc(lc3machine* state, int codeVal) {
    if (codeVal > 0) {
        state->cc = LC3_POSITIVE;
    } else if (codeVal == 0) {
        state->cc = LC3_ZERO;
    } else {
        state->cc = LC3_NEGATIVE;
    }
}
