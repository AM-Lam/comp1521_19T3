int case_opcode_immed_zero(mips_structure_t, int, uint32_t, uint32_t, uint32_t);
int spec_case_mul(mips_structure_t, int, uint32_t, uint32_t, uint32_t);
int case_basic_immediate(mips_structure_t, int, uint32_t, uint32_t, uint32_t);
int case_array(mips_structure_t, int, uint32_t, uint32_t, uint32_t);
int case_branch(mips_structure_t, int, uint32_t, uint32_t, uint32_t, uint32_t *);
int case_jump(mips_structure_t, int, uint32_t, uint32_t *);
int case_syscall(mips_structure_t, int, uint32_t, uint32_t, uint32_t);

void print_header(char *);