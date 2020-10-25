# read a line and print whether it is a palindrom

main:
    la   $a0, str0       # printf("Enter a line of input: ");
    li   $v0, 4
    syscall

    la   $a0, line
    la   $a1, 256
    li   $v0, 8          # fgets(buffer, 256, stdin)
    syscall              #

    li $t0, 0
while:
    la $t1, line
    add $t2, $t1, $t0
    lb $t3, ($t2)
    beq $t3, 0, next
    addi $t0, $t0, 1
    b while

next:
    li $t2, 0
    sub $t3, $t0, 2

nwhile:
    bge $t2, $t3, yes

    add $t4, $t1, $t2
    add $t5, $t1, $t3
    lb $t6, ($t4)
    lb $t7, ($t5)
    bne $t6, $t7, no
    addi $t2, $t2, 1
    addi $t3, $t3, -1

    b nwhile

no:
    la   $a0, not_palindrome
    li   $v0, 4
    syscall

    b nend

yes:
    la   $a0, palindrome
    li   $v0, 4
    syscall

nend:
    li   $v0, 0          # return 0
    jr   $31


.data
str0:
    .asciiz "Enter a line of input: "
palindrome:
    .asciiz "palindrome\n"
not_palindrome:
    .asciiz "not palindrome\n"


# line of input stored here
line:
    .space 256

