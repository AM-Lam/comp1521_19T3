// If you use C library functions add includes here.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "emu.h"
#include "ram.h"
#include "registers.h"

#include "mips_struct.h"
#include "header.h"
#include "option.h"


/*
 * print a MIPS instruction (no newline)
 * run reference implementation if unsure what to print
 */

void print_instruction(uint32_t instruction) {
    // since the different bits of instruction will be used, therefore I create a file to break the value of the instruction into different bits.
    mips_structure_t inp = make_bits(instruction);
    // The mips commands will be printed in two parts. 
    // Printing out the word/string at the beginning of command and the option will be used for separating different format of the register or value.
    int option = 0;

    // details of the function can be found in header.c or option.c
    if (option == 0) option = case_opcode_immed_zero(inp, 1, 0, 0, 0);
    if (option == 0) option = spec_case_mul(inp, 1, 0, 0, 0);
    if (option == 0) option = case_basic_immediate(inp, 1, 0, 0, 0);
    if (option == 0) option = case_array(inp, 1, 0, 0, 0);
    if (option == 0) option = case_branch(inp, 1, 0, 0, 0, 0);
    if (option == 0) option = case_jump(inp, 1, 0, 0);
    if (option == 0) case_syscall(inp, 1, 0, 0, 0);

    switch (option) {
        case 1:
            Op1(inp, 1, 0);
            break;

         case 11:
            Op11(inp, 1, 0);
            break; 

        case 2:
            Op2(inp, 1, 0, 0, 0);
            break;

        case 3:
            Op3(inp, 1, 0, 0);
            break;

        case 31:
            Op31(inp, 1, 0, 0);
            break;

        case 4:
            Op4(inp, 1, 0, 0, 0);
            break;

        case 41:
            Op41(inp, 1, 0, 0, 0);
            break;
        
        case 5:
            Op5(inp, 1, 0, 0, 0);
            break;

        case 51:
            Op51(inp, 1, 0, 0, 0);
            break;
        
        case 52:
            Op52(inp, 1, 0, 0, 0);
            break;
    }
    
}
