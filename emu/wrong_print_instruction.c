// If you use C library functions add includes here.

#include "emu.h"
#include "ram.h"
#include "registers.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define NUM 8
#define MAXSIZE 10
typedef struct mips_structure_t {
    uint32_t all;
    uint32_t opcode;
    uint32_t source1;
    uint32_t source2;
    uint32_t dest;
    uint32_t immed;
    uint32_t func;
    uint32_t rest;
} mips_structure_t;

mips_structure_t make_bits(uint32_t);
void stringCopy (char str[NUM][MAXSIZE], char *);

void Op1(mips_structure_t, char str[NUM][MAXSIZE]);
void Op2(mips_structure_t, char str[NUM][MAXSIZE]);
void Op3(mips_structure_t, char str[NUM][MAXSIZE]);
void Op4(mips_structure_t, char str[NUM][MAXSIZE]);
void Op5(mips_structure_t, char str[NUM][MAXSIZE]);
void printStr (char str[NUM][MAXSIZE]);


void case_opcode_immed_zero(char str[NUM][MAXSIZE], mips_structure_t);
int case_basic_immediate(char str[NUM][MAXSIZE], mips_structure_t);
int case_array(char str[NUM][MAXSIZE], mips_structure_t);

/*
 * print a MIPS instruction (no newline)
 * run reference implementation if unsure what to print
 */
void print_instruction(uint32_t instruction) {
    mips_structure_t inp = make_bits(instruction);
    char str[NUM][MAXSIZE];

    int option = case_basic_immediate(str, inp);
    if (option == 0) {
        option = case_array(str, inp);
    }
    
    switch (inp.opcode) {
        case 0:
            if (inp.immed == 0) {
                option = 4;
                switch (inp.func) {
                    case 4:
                        stringCopy(str, "sllv");
                        // bubble sort
                        uint32_t temp = inp.source1;
                        inp.source1 = inp.source2;
                        inp.source2 = temp;
                        break;
                    case 6:
                        stringCopy(str, "srlv");
                        // bubble sort
                        temp = inp.source1;
                        inp.source1 = inp.source2;
                        inp.source2 = temp;
                        break;
                }

                case_opcode_immed_zero(str, inp);
            }
            
            if (inp.source2 == 0 && inp.func == 8 && inp.dest == 0 && inp.immed == 0) {
                option = 1;
                stringCopy(str, "jr");
                break;
            }

        case 2:
            option = 1;
            stringCopy(str, "j");
            break;
        
        case 3:
            option = 1;
            stringCopy(str, "jal");
            break;

        case 4:
            option = 5;
            stringCopy(str, "beq");
            // bubble sort
            uint32_t temp = inp.source1;
            inp.source1 = inp.source2;
            inp.source2 = temp;
            break;
        case 5:
            option = 5;
            stringCopy(str, "bne");
            // bubble sort
            temp = inp.source1;
            inp.source1 = inp.source2;
            inp.source2 = temp;
            break;

        case 28:
            if (inp.immed == 0) {
                if (inp.func == 2) {
                    stringCopy(str, "mul");
                    option = 4;
                }
            }
            break;
    }
    
    if (instruction == 12) {
        option = 6;
    }

    // print the output according to type
    switch (option) {
        // for jump
        case 1:
            Op1(inp,str);
            printStr(str);
            break;
        // for array
        case 2:
            Op2(inp, str);
            printStr(str);
            break;
        // branch immediate 
        case 3:
            Op3(inp, str);
            printStr(str);
            break;
        // register type
        case 4:
            Op4(inp, str);
            printStr(str);
            break;
        // immediate type
        case 5:
            Op5(inp, str);
            printStr(str);
            break;
        case 6:
            printf("syscall");
            break;
    }


}

void stringCopy (char str[NUM][MAXSIZE], char *string) {
    strncpy(str[0], string, MAXSIZE);
}

void Op1(mips_structure_t inp, char str[NUM][MAXSIZE]) {
    uint32_t imed = inp.all & 0xFFFFFF;

        for (int i = 0; i < 6; i++) {
            int num = ((5 - i) == 0) ? 15 : 15 << (4 * (5 - i));
            str[2 + i][0] = imed & num;
            snprintf(str[2 + i], MAXSIZE, "%d", str[2 + i][0]);
        }

        strncpy(str[1], " 0x", MAXSIZE);

        // for "jr"
        if (inp.opcode == 0 && inp.func == 8 && inp.source2 == 0 
            && inp.dest == 0 && inp.immed == 0) {
            str[2][0] = inp.source1;
            snprintf(str[2], MAXSIZE, "%x", str[2][0]);
            strncpy(str[1], " $", MAXSIZE);
        }
}

// array form
void Op2(mips_structure_t inp, char str[NUM][MAXSIZE]) {
    // save the number
    str[2][0] = inp.source2;
    str[4][0] = inp.rest;
    str[6][0] = inp.source1;

    // save the format
    strncpy(str[1], " $", MAXSIZE);
    strncpy(str[3], ", ", MAXSIZE);
    strncpy(str[5], "($", MAXSIZE);
    strncpy(str[7], ")", MAXSIZE);

    // convert int to string
    for (int i = 0; i < NUM; i++) {
        if (i == 2 || i == 4 || i == 6) 
            snprintf(str[i], MAXSIZE, "%d", str[i][0]);
    }
}

// branch immediate
void Op3(mips_structure_t inp, char str[NUM][MAXSIZE]) {
    // copy the same style in previous code
    Op4(inp, str);

    // modify
    str[2][0] = (inp.source1 == 0) ? inp.source2 : inp.source1;
    str[4][0] = inp.rest;

    strncpy(str[3], ", ", MAXSIZE);
    str[5][0] = '\0';

    // convert int to string
    for (int i = 0; str[i][0] != '\0'; i++) {
        if (i == 2 || i == 4) 
            snprintf(str[i], MAXSIZE, "%d", str[i][0]);
    }

}

// register form
void Op4(mips_structure_t inp, char str[NUM][MAXSIZE]) {
    // save the number
    str[2][0] = inp.dest;
    str[4][0] = inp.source1;
    str[6][0] = inp.source2;

    // save the format
    strncpy(str[1], " $", MAXSIZE);
    strncpy(str[3], ", $", MAXSIZE);
    strncpy(str[5], ", $", MAXSIZE);

    // convert int to string
    for (int i = 0; i < NUM; i++) {
        if (i == 2 || i == 4 || i == 6) 
            snprintf(str[i], MAXSIZE, "%d", str[i][0]);
    }
}

// immediate
void Op5(mips_structure_t inp, char str[NUM][MAXSIZE]) {
    // copy the same style in previous code
    Op4(inp, str);

    // modify
    str[2][0] = inp.source2;
    str[4][0] = inp.source1;
    str[6][0] = inp.rest;
    strncpy(str[5], ", ", MAXSIZE);

    if (inp.opcode == 0 && inp.source1 == 0) {
        str[2][0] = inp.dest;
        str[4][0] = inp.source2;
        str[6][0] = inp.immed;
    }

    // convert int to string
    for (int i = 0; i < NUM; i++) {
        if (i == 2 || i == 4 || i == 6) 
            snprintf(str[i], MAXSIZE, "%d", str[i][0]);
    }

}

void printStr (char str[NUM][MAXSIZE]) {
    for (int i = 0; i < NUM; i++) {
        printf("%s", str[i]);
    }
}

void case_opcode_immed_zero(char str[NUM][MAXSIZE], mips_structure_t inp) {
    switch (inp.func) {
        case 32:
            stringCopy(str, "add");
            break;
        case 34:
            stringCopy(str, "sub");
            break;
        case 36:
            stringCopy(str, "and");
            break;
        case 37: 
            stringCopy(str, "or");
            break;
        case 38:
            stringCopy(str, "xor");
            break;
        case 42:
            stringCopy(str, "slt");
            break;
    }
}

int case_basic_immediate(char str[NUM][MAXSIZE], mips_structure_t inp) {
    switch(inp.opcode) {
        case 0:
            if (inp.source1 == 0) {
                switch (inp.func) {
                    case 0:
                        stringCopy(str, "sll");
                        return 5;
                    case 2:
                        stringCopy(str, "srl");
                        return 5;
                }
            }

        case 1:
            if (inp.source2 == 0) {
                if (inp.rest == 8) {
                    stringCopy(str, "jal");
                } else {
                    stringCopy(str, "bltz");
                }
            } else if (inp.source2 == 1) {
                stringCopy(str, "bgez");
            }
            return 3;

        if (inp.source2 == 0) {
            case 6:
                stringCopy(str, "blez");
                return 3;

            case 7:
                stringCopy(str, "bgtz");
                return 3;
        }
        case 8:
            stringCopy(str, "addi");
            return 5;

        case 10:
            stringCopy(str, "slti");
            return 5;

        case 12:
            stringCopy(str, "andi");
            return 5;

        case 13:
            stringCopy(str, "ori");
            return 5;

        case 14:
            stringCopy(str, "xori");
            return 5;

        case 15:
            if (inp.source1 == 0) {
                stringCopy(str, "lui");
                return 3;
            }
    }
    return 0;
}

int case_array(char str[NUM][MAXSIZE], mips_structure_t inp) {
    switch (inp.opcode) {
        case 32:
            stringCopy(str, "lb");
            return 2;

        case 33:
            stringCopy(str, "lh");
            return 2;

        case 34:
            stringCopy(str, "lw");
            return 2;

        case 40:
            stringCopy(str, "sb");
            return 2;

        case 41:
            stringCopy(str, "sh");
            return 2;

        case 43:
            stringCopy(str, "sw");
            return 2;
    }
    return 0;
}