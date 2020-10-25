# Read a number and print positive multiples of 7 or 11 < n

main:                  # int main(void) {

    la $a0, prompt     # printf("Enter a number: ");
    li $v0, 4
    syscall

    li $v0, 5           # scanf("%d", number);
    syscall
    move $t0, $v0

    li $t1, 2           # counter $t1
    li $t5, 7
    li $t6, 11

loop:
    
    beq $t1, $t0 end

    rem $t2, $t1, $t5      # prob cannot use 7
    beq $t2, 0, print

    rem $t2, $t1, $t6      # prob cannot use 11
    beq $t2, 0, print

    add $t1, $t1, 1

    b loop

print:

    move $a0, $t1         # printf("%d", ) after check;
    li   $v0, 1
    syscall

    li   $a0, '\n'        # printf("%c", '\n');
    li   $v0, 11
    syscall

    add $t1, $t1, 1

    j loop

end:
    jr   $ra           # return

    .data
prompt:
    .asciiz "Enter a number: "
