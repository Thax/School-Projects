// DO NOT CHANGE ANY PROTOTYPE NAMES
// OR ARGUMENTS PASSED IN TO ANY PROTOTYPE HERE.

// YOU MAY ADD ADDITIONAL FUNCTIONS BUT YOU MUST IMPLEMENT THE ONES BELOW

#include <stdio.h>

typedef char s8;
typedef short s16;
typedef int s32;

typedef volatile char vs8;
typedef volatile short vs16;
typedef volatile int vs32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned int vu32;

enum lc3_cc
{
	LC3_NEGATIVE = 4,
	LC3_ZERO = 2,
	LC3_POSITIVE = 1
};

/**
 * A structure that represents the state of the LC3 machine.
 */
typedef struct 
{
	short mem[65536]; /* Memory */
	short regs[8]; /* The eight registers in the LC-3 */
	unsigned short pc; /* The pc register */
	unsigned char cc; /* The condition code register the value will be one of the enum values above */
} lc3machine;

enum lc3_ops
{
    LC3_OP_ADD = 1,
    LC3_OP_AND = 5,
    LC3_OP_BR = 0,
    LC3_OP_JMP = 12,
    LC3_OP_JSR = 4,
    LC3_OP_LD = 2,
    LC3_OP_LDI = 10,
    LC3_OP_LDR = 6,
    LC3_OP_LEA = 14,
    LC3_OP_NOT = 9,
    LC3_OP_RTI = 8,
    LC3_OP_ST = 3,
    LC3_OP_STI = 11,
    LC3_OP_STR = 7,
    LC3_OP_TRAP = 15
};

enum lc3_traps
{
    LC3_TRAP_PUTS = 0x22,
    LC3_TRAP_HALT = 0x25,
    LC3_TRAP_GETC = 0x20,
    LC3_TRAP_OUT = 0x21,
    LC3_TRAP_IN = 0x23,
    LC3_TRAP_PUTSP = 0x24,
    LC3_TRAP_FULL_HALT = 0xF025
};

#define TRUE 1
#define FALSE 0

#define SHORTTOBINARYPATTERN "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d"
#define SHORTTOBINARY(val) \
    (((val) & 0x8000) ? 1 : 0), \
    (((val) & 0x4000) ? 1 : 0), \
    (((val) & 0x2000) ? 1 : 0), \
    (((val) & 0x1000) ? 1 : 0), \
    (((val) & 0x0800) ? 1 : 0), \
    (((val) & 0x0400) ? 1 : 0), \
    (((val) & 0x0200) ? 1 : 0), \
    (((val) & 0x0100) ? 1 : 0), \
    (((val) & 0x0080) ? 1 : 0), \
    (((val) & 0x0040) ? 1 : 0), \
    (((val) & 0x0020) ? 1 : 0), \
    (((val) & 0x0010) ? 1 : 0), \
    (((val) & 0x0008) ? 1 : 0), \
    (((val) & 0x0004) ? 1 : 0), \
    (((val) & 0x0002) ? 1 : 0), \
    (((val) & 0x0001) ? 1 : 0)

#define GET_SUBWORD(val, big_index, little_index) (((val) >> (little_index)) & ((2 << ((big_index) - (little_index))) - 1))

/* lc3_init
   This function initializes the lc3machine to its initial state
   The pc should be set to 3000 (hex)
   and the conditional code register should be set to indicate zero (LC3_ZERO)
*/
void lc3_init(lc3machine* state);

/* lc3_load
   This function is responsible for reading a .obj file
   produced from lc3as the format is explained in the
   assignment */
void lc3_load(lc3machine* state, FILE* program);

/* lc3_step_one
   This function just does one simulation step (goes through steps to fetch
   and execute one instruction */
void lc3_step_one(lc3machine* state);

/* lc3_run
   This function will run for the number of steps given or until you halt.
   @param num_steps number of steps to run (-1 for infinite)
*/
void lc3_run(lc3machine* state, int num_steps);

/* lc3_fetch
   Fetches an instruction from memory BE SURE to do everything that is needed
   to be done in the fetch stage
   @return the fetch'd instruction. */
unsigned short lc3_fetch(lc3machine* state);

/* lc3_execute
   Execution function. in charge of executing the instruction given
   @param instruction instruction to execute
*/
void lc3_execute(lc3machine* state, unsigned short instruction);

/* lc3_trap
   Trap execution function called from within lc3_execute.
*/
void lc3_trap(lc3machine* state, unsigned char vector8);

/* lc3_add_reg
   ADD (register) execution function called from within lc3_execute.
*/
void lc3_add_reg(lc3machine* state, int dr, int sr1, int sr2);

/* lc3_add_imm
   ADD (imm) execution function called from within lc3_execute.
*/
void lc3_add_imm(lc3machine* state, int dr, int sr1, int imm);

/* lc3_and_reg
   AND (register) execution function called from within lc3_execute.
*/
void lc3_and_reg(lc3machine* state, int dr, int sr1, int sr2);

/* lc3_and_imm
   AND (imm) execution function called from within lc3_execute.
*/
void lc3_and_imm(lc3machine* state, int dr, int sr1, int imm);

/* lc3_br
   BR execution function called from within lc3_execute.
*/
void lc3_br(lc3machine* state, int cc, int offset);

/* lc3_jmp
   JMP execution function called from within lc3_execute. (Also used for ret)
*/
void lc3_jmp(lc3machine* state, int baser);

/* lc3_jsr_offset
    JSR execution function called from within lc3_execute.
*/
void lc3_jsr_offset(lc3machine* state, int offset);

/* lc3_jsr_reg
    JSRR execution function called from within lc3_execute.
*/
void lc3_jsr_reg(lc3machine* state, int baser);

/* lc3_ld
   LD execution function called from within lc3_execute.
*/
void lc3_ld(lc3machine* state, int dr, int offset);

/* lc3_ldi
   LDI execution function called from within lc3_execute.
*/
void lc3_ldi(lc3machine* state, int dr, int offset);

/* lc3_ldr
   LDR execution function called from within lc3_execute.
*/
void lc3_ldr(lc3machine* state, int dr, int baser, int offset);

/* lc3_lea
   LEA execution function called from within lc3_execute.
*/
void lc3_lea(lc3machine* state, int dr, int offset);

/* lc3_not
   NOT execution function called from within lc3_execute.
*/
void lc3_not(lc3machine* state, int dr, int sr);

/* lc3_st
   ST execution function called from within lc3_execute.
*/
void lc3_st(lc3machine* state, int sr, int offset);

/* lc3_sti
   STI execution function called from within lc3_execute.
*/
void lc3_sti(lc3machine* state, int sr, int offset);

/* lc3_str
   STR execution function called from within lc3_execute.
*/
void lc3_str(lc3machine* state, int sr, int baser, int offset);

/* lc3_update_cc
    Update the condition code of the machine
*/
void lc3_update_cc(lc3machine* state, int codeVal);

int lc3_sign_extend(int value, int startSize, int endSize);
