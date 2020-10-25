main:
    la $a0, string2 # get addr of string
    li $v0, 4 # 4 is print string syscall
    syscall

    jr $ra #return

    .data
string2: .asciiz "I MIPS you!\n"