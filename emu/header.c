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

// $v0 = 2
#define vZero 2
// $a0 = 4
#define aZero 4
// $a1 = 5
#define aOne 5

// This file will be used for both part 1 and part 2 
// so option will be part of the input to distinguish parts

// In part 1, this will be used to print out the first half of the command string
// and will return a value to guide the second half of part 1 for printing out the correct register or immediate

// In part 2, this will handle most of the operation of executing the command after the respective value/register passed in from options file

// for input value a, b, c, they are named according to the column of the instruction
// for example, in "add", a = $d, b = $s, c = $t
// in "addi", a = $t, b = $s, c = I

// print register command including "sllv" and "srlv"
int case_opcode_immed_zero(mips_structure_t inp , int mode, uint32_t a, uint32_t b, uint32_t c) {
    if (inp->opcode == 0 && inp->immed == 0) {
        switch (inp->func) {
            case 4:
                if (mode == 1) print_header("sllv");
                if (mode == 2) set_register(a, get_register(b) << get_register(c));
                return 41;
            case 6:
                if (mode == 1) print_header("srlv");
                if (mode == 2) set_register(a, get_register(b) >> get_register(c));
                return 41;
            case 32:
                if (mode == 1) print_header("add");
                if (mode == 2) set_register(a, get_register(b) + get_register(c));
                return 4;
            case 34:
                if (mode == 1) print_header("sub");
                if (mode == 2) set_register(a, get_register(b) - get_register(c));
                return 4;
            case 36:
                if (mode == 1) print_header("and");
                if (mode == 2) set_register(a, get_register(b) & get_register(c));
                return 4;
            case 37: 
                if (mode == 1) print_header("or");
                if (mode == 2) set_register(a, get_register(b) | get_register(c));
                return 4;
            case 38:
                if (mode == 1) print_header("xor");
                if (mode == 2) set_register(a, get_register(b) ^ get_register(c));
                return 4;
            case 42:
                if (mode == 1) print_header("slt");
                if (mode == 2) set_register(a, (get_register(b) < get_register(c)));
                return 4;
        }
    }
    return 0;
}

int spec_case_mul(mips_structure_t inp , int mode, uint32_t a, uint32_t b, uint32_t c) {
    if (inp->opcode == 28 && inp->immed == 0 && inp->func == 2) {
        if (mode == 1) print_header("mul");
        if (mode == 2) set_register(a, get_register(b) * get_register(c));

        return 4;        
    }
    return 0;
}

// print immediate command including "lui" 
int case_basic_immediate(mips_structure_t inp , int mode, uint32_t a, uint32_t b, uint32_t c) {
    switch(inp->opcode) {
        case 0:
            if (inp->source1 == 0 && inp->func == 0) {
                        if (mode == 1) print_header("sll");
                        if (mode == 2) set_register(a, get_register(b) << c);
                        return 51;
            } else if (inp->source1 == 0 && inp->func == 2) {
                        if (mode == 1) print_header("srl");
                        if (mode == 2) set_register(a, get_register(b) >> c);
                        return 51;
            }
            return 0;

        case 8:
            if (mode == 1) print_header("addi");
            if (mode == 2) set_register(a, get_register(b) + c);
            return 5;

        case 10:
            if (mode == 1) print_header("slti");
            if (mode == 2) set_register(a, twos_complement(get_register(b)) < c);
            return 5;

        case 12:
            if (mode == 1) print_header("andi");
            if (mode == 2) set_register(a, get_register(b) & c);
            return 5;

        case 13:
            if (mode == 1) print_header("ori");
            if (mode == 2) set_register(a, get_register(b) | c);
            return 5;

        case 14:
            if (mode == 1) print_header("xori");
            if (mode == 2) set_register(a, get_register(b) ^ c);
            return 5;

        case 15:
            if (inp->source1 == 0) {
                if (mode == 1) print_header("lui");
                if (mode == 2) set_register(a, b << 16);
                return 31;
            }
    }
    return 0;
}

// for array command
// all the load and save goes here
int case_array(mips_structure_t inp , int mode, uint32_t a, uint32_t b, uint32_t c) {
    uint32_t temp = 0;
    switch (inp->opcode) {
        case 32:
            if (mode == 1) print_header("lb");
            if (mode == 22) set_register(a, (int8_t)get_byte(get_register(c) + b));
            return 2;

        case 33:
            if (mode == 1) print_header("lh");
            if (mode == 22) {
                temp |= get_byte(get_register(c) + b);
                temp |= get_byte(get_register(c) + b + 1) << 8;
                set_register(a, (int16_t)temp);
            }
            return 2;

        case 35:
            if (mode == 1) print_header("lw");
            if (mode == 22) {
                temp |= get_byte(get_register(c) + b);
                for (int i = 1; i < 4; i++) {
                    temp |= (uint32_t) get_byte(get_register(c) + b + i) << (8 * i);
                }
                set_register(a, (int32_t)temp);
            }
            return 2;

        case 40:
            if (mode == 1) print_header("sb");
            if (mode == 22) {
                set_byte(get_register(c) + b, (uint8_t)get_register(a) & 0xff);
            }
            return 2;

        case 41:
            if (mode == 1) print_header("sh");
            if (mode == 22) {
                set_byte(get_register(c) + b, (uint16_t)get_register(a) & 0xffff);
                set_byte(get_register(c) + b + 1, (uint16_t)get_register(a) >> 8);
            }
            return 2;

        case 43:
            if (mode == 1) print_header("sw");
            if (mode == 22) {
                set_byte(get_register(c) + b, (uint32_t)get_register(a));
                for (int i = 1; i < 4; i++) {
                    set_byte(get_register(c) + b + i, (uint32_t)get_register(a) >> 8 * i);
                }  
            }
            
            return 2;
    }
    return 0;
}

// For branch command
int case_branch (mips_structure_t inp , int mode, uint32_t a, uint32_t b, uint32_t c, uint32_t *program_counter) {
    switch(inp->opcode) {
        case 1:
            if (inp->source2 == 0 && inp->rest != 8) {
                if (mode == 1) print_header("bltz");
                if (mode == 22 && twos_complement(get_register(a)) < 0) {
                    (*program_counter) += twos_complement(b) << 2;
                    return -1;
                }
                return 3;
            } else if (inp->source2 == 1) {
                if (mode == 1) print_header("bgez");
                if (mode == 22 && twos_complement(get_register(a)) >= 0) {
                    (*program_counter) += twos_complement(b) * 4;
                    return -1;
                }
                return 3;
            }
            return 0;
        case 4:
            if (mode == 1) print_header("beq");
            if (mode == 22 && get_register(a) == get_register(b)) {
                (*program_counter) += twos_complement(c) << 2;
                return -1;
            }
            return 52;

        case 5:
            if (mode == 1) print_header("bne");
            if (mode == 22 && get_register(a) != get_register(b)) {
                (*program_counter) += twos_complement(c) << 2;
                return -1;
            }
            return 52;

        case 6:
            if (inp->source2 == 0) {
                if (mode == 1) print_header("blez");
                if (mode == 22 && twos_complement(get_register(a)) <= 0) {
                    (*program_counter) += twos_complement(b) << 2;
                    return -1;
                }
                return 3;
            }
            return 0;

        case 7:
            if (inp->source2 == 0) {
                if (mode == 1) print_header("bgtz");
                if (mode == 22 && twos_complement(get_register(a)) > 0) {
                    (*program_counter) += twos_complement(b) << 2;
                    return -1;
                }
                return 3;
            }
    }
    return 0;
}

// For the jump command
int case_jump(mips_structure_t inp , int mode, uint32_t a, uint32_t *program_counter) {
    switch(inp->opcode) {
        case 0:
            if (inp->source2 == 0 && inp->dest == 0 && inp->immed == 0 && inp->func == 8) {
                if (mode == 1) print_header("jr");
                if (mode == 22) {
                    (*program_counter) = get_register(a);
                    return -1;
                }
                return 11;
            }
            return 0;
        case 2:
            if (mode == 1) print_header("j");
            if (mode == 22) {
                (*program_counter) = ((*program_counter) & 0xF0000000) | (a << 2);
                return -1;
            }
            return 1;
        
        case 3:
            if (mode == 1) print_header("jal");
            if (mode == 22) {
                (*program_counter) = ((*program_counter) & 0xF0000000) | (a << 2);
                set_byte(get_register(31), (*program_counter) + 4);
                return -1;
            }
            return 1;
    }
    return 0;
}

// This is for part 2 mostly
int case_syscall (mips_structure_t inp , int mode, uint32_t a, uint32_t b, uint32_t c) {
    uint32_t test = (inp->all >> 6) & 0xFFFFFF;
    if (inp->func == 12  && test == 0) {
        if (mode == 1) printf("syscall");
        if (mode == 2) {
            int var;
            char ch;
            char str[get_register(aOne)];
            switch(get_register(vZero)) {
                // print integer
                case 1:
                    printf("%d", get_register(aZero));
                    break;
                // print string
                case 4:
                    for (int i = 0; get_byte(get_register(aZero) + i) != '\0'; i++) {
                        printf("%c", get_byte(get_register(aZero) + i));
                    }
                    break;
                // read integer	
                case 5:
                    scanf("%d", &var);
                    set_register(2, (uint8_t)var);
                    break;
                // read string
                case 8:
                    for (int i = 0; i < get_register(aOne); i++) {
                        scanf("%c", str[i]);
                        set_byte(get_register(vZero) + i, (uint8_t)str[i]);
                    }
                    break;
                // exit
                case 10:
                    return -1;
                // print character	
                case 11:
                    printf("%c", twos_complement(get_register(aZero)));
                    break;
                // read character
                case 12:
                    scanf("%c", &ch);
                    set_register(vZero, ch);
                    break;
            }
        } 
        return 6;
    }
    return 0;
}

// print out the instruction word
void print_header(char *string) {
    printf("%s ", string);
}
