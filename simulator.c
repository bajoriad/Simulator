/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure *not* to modify printState or any of the associated functions
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Machine Definitions
#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8       /* number of machine registers */

// File
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct stateStruct
{
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

int convertNum(int);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2)
    {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL)
    {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++)
    {
        if (sscanf(line, "%d", state.mem + state.numMemory) != 1)
        {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    state.pc = 0;
    for (int i = 0; i < NUMREGS; ++i)
    {
        state.reg[i] = 0;
    }

    printState(&state);
    // int i = 1;

    while (1)
    {
        int command = state.mem[state.pc];
        int opcode = command >> 22;
        // beq operation
        if (opcode == 0b100)
        {
            int regAvalue = (command >> 19) & 0b111;
            int regBvalue = (command >> 16) & 0b111;
            int offsetvalue = command & 0b1111111111111111;
            offsetvalue = convertNum(offsetvalue);
            if (state.reg[regAvalue] == state.reg[regBvalue])
            {
                state.pc = state.pc + 1 + offsetvalue;
            }
            else
            {
                state.pc = state.pc + 1;
            }
            printState(&state);
        }
        // lw operation
        else if (opcode == 0b010)
        {
            int regAvalue = (command >> 19) & 0b111;
            int regBvalue = (command >> 16) & 0b111;
            int offsetvalue = command & 0b1111111111111111;
            offsetvalue = convertNum(offsetvalue);
            int address = state.reg[regAvalue] + offsetvalue;
            state.reg[regBvalue] = state.mem[address];
            ++state.pc;
            printState(&state);
        }
        // sw operation
        else if (opcode == 0b011)
        {
            int regAvalue = (command >> 19) & 0b111;
            int regBvalue = (command >> 16) & 0b111;
            int offsetvalue = command & 0b1111111111111111;
            offsetvalue = convertNum(offsetvalue);
            int address = state.reg[regAvalue] + offsetvalue;
            state.mem[address] = state.reg[regBvalue];
            ++state.pc;
            printState(&state);
        }
        // jalr operation
        else if (opcode == 0b101)
        {
            int regAvalue = (command >> 19) & 0b111;
            int regBvalue = (command >> 16) & 0b111;
            state.reg[regBvalue] = state.pc + 1;
            state.pc = state.reg[regAvalue];
            printState(&state);
        }
        // add operation
        else if (opcode == 0b000)
        {
            int regAvalue = (command >> 19) & 0b111;
            int regBvalue = (command >> 16) & 0b111;
            int destvalue = command & 0b111;
            state.reg[destvalue] = state.reg[regAvalue] + state.reg[regBvalue];
            ++state.pc;
            printState(&state);
        }
        // nor operation
        else if (opcode == 0b001)
        {
            int regAvalue = (command >> 19) & 0b111;
            int regBvalue = (command >> 16) & 0b111;
            int destvalue = command & 0b111;
            state.reg[destvalue] = ~((state.reg[regAvalue]) | (state.reg[regBvalue]));
            ++state.pc;
            printState(&state);
        }
        // noop operation
        else if (opcode == 0b111)
        {
            ++state.pc;
            printState(&state);
        }
        // halt operation
        else if (opcode == 0b110)
        {
            ++state.pc;
            printState(&state);
            break;
        }
        // ++i;
    }

    return (0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++)
    {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++)
    {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1 << 15))
    {
        num -= (1 << 16);
    }
    return (num);
}
