/* LC3 simulator file.
 * Complete the functions!
 * This is a front-end to the code you wrote in lc3.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lc3.h"
#include "help.h"

void cmd_registers(lc3machine* mach);
void cmd_dump(lc3machine* mach, int start, int end);
void cmd_setaddr(lc3machine* mach, int address, short value);
void cmd_setreg(lc3machine* mach, int reg, short value);

#define PROMPT "(ASM):"

int main(int argc, char **argv) {
	FILE *prog;
	lc3machine mach;

	/* We expect only one argument for the program... */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s file.obj\n", argv[0]);
		return 1;
	}
	/* We want to open the file and make sure that it exists. */
	prog = fopen(argv[1], "rb");
	if (!prog) {
		fprintf(stderr, "Could not find file %s\n", argv[1]);
		return 2;
	}

	/* Make a call to lc3_init to initialize your lc3machine */
    lc3_init(&mach);
	/* Make a call to lc3_load to load the program */
    lc3_load(&mach, prog);

	printf("LC-3 simulator and debugger\n");
	printf("Written by Robert Thackston\n");
    printf("---With spiritual guidance from the Academic Walrus---\n");
	/* Run this loop until we are told to stop debugging. */
    char str[20];
    char* temp;
    char lastCommand[20];
	while (1) {
		/* FIXME: Add some necessary variables here */
		printf("%s", PROMPT);
        temp = fgets(str, 20, stdin);
        if (strcmp(temp, "\n") == 0) {
            strcpy(temp, lastCommand);
        } else {
            strcpy(lastCommand, str);
        }
        temp = strtok(str, " ");
        // Parse the first token and see what instruction, if any, we are using.
        if ((strcmp(temp, "help") == 0) || (strcmp(temp, "help\n") == 0)) {
            printf("%s", HELP_MESSAGE);
        } else if ((strcmp(temp, "quit\n") == 0) || (strcmp(temp, "quit") == 0)) {
            return 0;
        } else if ((strcmp(temp, "continue") == 0) || (strcmp(temp, "continue\n") == 0)) {
            lc3_run(&mach, 65536 - (&mach)->pc);
        } else if ((strcmp(temp, "registers") == 0) || (strcmp(temp, "registers\n") == 0)) {
            cmd_registers(&mach);
        } else if ((strcmp(temp, "step") == 0) || (strcmp(temp, "step\n") == 0)) {
            if (strcmp(temp, "step\n") == 0) {
                lc3_run(&mach, 1);
            } else {
                temp = strtok(NULL, " ");
                lc3_run(&mach, atoi(temp));
            }
        } else if ((strcmp(temp, "dump") == 0)) {
            temp = strtok(NULL, " ");
            int start = strtoul(temp, NULL, 16);
            temp = strtok(NULL, " ");
            if (temp != NULL) {
                int end = strtoul(temp, NULL, 16);
                cmd_dump(&mach, start, end);
            } else {
                cmd_dump(&mach, start, 65536);
            }
        } else if ((strcmp(temp, "setaddr") == 0)) {
            temp = strtok(NULL, " ");
            int addr = strtoul(temp, NULL, 16);
            temp = strtok(NULL, " ");
            int value = atoi(temp);
            cmd_setaddr(&mach, addr, value);
        } else if ((strcmp(temp, "setreg") == 0)) {
            temp = strtok(NULL, " ");
            int reg = atoi(temp);
            temp = strtok(NULL, " ");
            int value = atoi(temp);
            cmd_setreg(&mach, reg, value);
        } else {
            printf("Invalid Command.  Type 'help' for a list of commands.\n");
        }
	}

	return 0;
}

/* cmd_step and cmd_continue 's functionality are provided in lc3_run
Therefore to execute the step and coninute commands you can just call lc3_run with the correct parameters*/

/* cmd_registers
Should print out all of the registers and the PC and CC in both hex and signed decimal.
*/
void cmd_registers(lc3machine* mach)
{
    printf("R0:\n");
    printf("    Hex: 0x%x Decimal: %d Binary: "SHORTTOBINARYPATTERN"\n", (u16)mach->regs[0], (s16)mach->regs[0], SHORTTOBINARY((u16)mach->regs[0]));
    printf("R1:\n");
    printf("    Hex: 0x%x Decimal: %d Binary: "SHORTTOBINARYPATTERN"\n", (u16)mach->regs[1], (s16)mach->regs[1], SHORTTOBINARY((u16)mach->regs[1]));
    printf("R2:\n");
    printf("    Hex: 0x%x Decimal: %d Binary: "SHORTTOBINARYPATTERN"\n", (u16)mach->regs[2], (s16)mach->regs[2], SHORTTOBINARY((u16)mach->regs[2]));
    printf("R3:\n");
    printf("    Hex: 0x%x Decimal: %d Binary: "SHORTTOBINARYPATTERN"\n", (u16)mach->regs[3], (s16)mach->regs[3], SHORTTOBINARY((u16)mach->regs[3]));
    printf("R4:\n");
    printf("    Hex: 0x%x Decimal: %d Binary: "SHORTTOBINARYPATTERN"\n", (u16)mach->regs[4], (s16)mach->regs[4], SHORTTOBINARY((u16)mach->regs[4]));
    printf("R5:\n");
    printf("    Hex: 0x%x Decimal: %d Binary: "SHORTTOBINARYPATTERN"\n", (u16)mach->regs[5], (s16)mach->regs[5], SHORTTOBINARY((u16)mach->regs[5]));
    printf("R6:\n");
    printf("    Hex: 0x%x Decimal: %d Binary: "SHORTTOBINARYPATTERN"\n", (u16)mach->regs[6], (s16)mach->regs[6], SHORTTOBINARY((u16)mach->regs[6]));
    printf("R7:\n");
    printf("    Hex: 0x%x Decimal: %d Binary: "SHORTTOBINARYPATTERN"\n", (u16)mach->regs[7], (s16)mach->regs[7], SHORTTOBINARY((u16)mach->regs[7]));
    printf("PC:\n");
    printf("    Hex: 0x%x Decimal: %d Binary: "SHORTTOBINARYPATTERN"\n", (u16)mach->pc, (s16)mach->pc, SHORTTOBINARY((u16)mach->pc));
    printf("CC:\n");
    printf("    Hex: 0x%x Decimal: %d Binary: "SHORTTOBINARYPATTERN"\n", (u16)mach->cc, (s16)mach->cc, SHORTTOBINARY((u16)mach->cc));
}

/* cmd_dump
Should print out the contents of memory from start to end
If end is -1 then just print out memory[start]*/
void cmd_dump(lc3machine* mach, int start, int end)
{
    if (end != -1) {
        for (int i = start; i < end; i++) {
            printf(SHORTTOBINARYPATTERN"\n", SHORTTOBINARY(mach->mem[i]));
        }
    } else {
        printf(SHORTTOBINARYPATTERN"\n", SHORTTOBINARY(mach->mem[start]));
    }
}

/* cmd_setaddr
 Should set a memory address to some value
*/
void cmd_setaddr(lc3machine* mach, int address, short value)
{
    mach->mem[address] = value;
}

/* cmd_setreg
  Should set a register to some value passed in
*/
void cmd_setreg(lc3machine* mach, int reg, short value)
{
    mach->regs[reg] = value;
}
