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

/**
 * execute a MIPS instruction
 *
 * This function should:
 *
 * get register values by calling `get_register(register_type register_number)`
 * change registers by calling `set_register(register_type register_number, uint32_t value)`
 *
 * get memory values by `calling get_byte(uint32_t address)`
 * changes memory by `set_byte(uint32_t address, uint8_t value)`
 *
 * updates program_counter to address of next instruction
 *
 * returns 1 if an exit syscall is executed, 0 otherwise
 */

 // all the helper function is stored in header.* and option.*
 // struct is stored in mips_strict.*
int execute_instruction(uint32_t instruction, uint32_t *program_counter) {
    // since the different bits of instruction will be used, therefore I create a file to break the value of the instruction into different bits.
    mips_structure_t inp = make_bits(instruction);
    // The mips commands will be printed in two parts. 
    // Printing out the word/string at the beginning of command and the option will be used for separating different format of the register or value.
    int option = 0;

    // details of the function can be found in header.c or option.c
    if (option == 0) option = case_opcode_immed_zero(inp, 2, 0, 0, 0);
    if (option == 0) option = spec_case_mul(inp, 2, 0, 0, 0);
    if (option == 0) option = case_basic_immediate(inp, 2, 0, 0, 0);
    if (option == 0) option = case_array(inp, 2, 0, 0, 0);
    if (option == 0) option = case_branch(inp, 2, 0, 0, 0, 0);
    if (option == 0) option = case_jump(inp, 2, 0, 0);
    if (option == 0) case_syscall(inp, 2, 0, 0, 0);


    // Using the same variable for command will decrease the number of variable that needed to name differently according to command
    uint32_t a = 0, b = 0, c = 0;
    switch (option) {
        case 1:
            Op1(inp, 2, &a);
            case_jump(inp, 22, a, program_counter);
            break;

         case 11:
            Op11(inp, 2, &a);
            case_jump(inp, 22, a, program_counter);
            break; 

        case 2:
            Op2(inp, 2, &a, &b, &c);
            case_array(inp, 22, a, b, c);
            break;

        case 3:
            Op3(inp, 2, &a, &b);
            option = case_branch(inp, 22, a, b, c, program_counter);
            break;

        case 31:
            Op31(inp, 2, &a, &b);
            case_basic_immediate(inp, 2, a, b, c);
            break;

        case 4:
            Op4(inp, 2, &a, &b, &c);
            case_opcode_immed_zero(inp, 2, a, b, c);
            spec_case_mul(inp, 2, a, b, c);
            break;

        case 41:
            Op41(inp, 2, &a, &b, &c);
            case_opcode_immed_zero(inp, 2, a, b, c);
            break;
        
        case 5:
            Op5(inp, 2, &a, &b, &c);
            case_basic_immediate(inp, 2, a, b, c);
            break;

        case 51:
            Op51(inp, 2, &a, &b, &c);
            case_basic_immediate(inp, 2, a, b, c);
            break;
        
        case 52:
            Op52(inp, 2, &a, &b, &c);
            option = case_branch(inp, 22, a, b, c, program_counter);
            break;
    }


    // Most instructions will simply do the following:
    // special cases will return -1 so it will escape the following eg Jump & Branch instructions
    if (option != -1) (*program_counter) += 4;

    // 0 should be returned, unless an exit syscall is executed
    return 0;
}
