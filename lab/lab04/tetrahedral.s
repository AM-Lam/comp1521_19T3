# Read a number n and print the first n tetrahedral numbers
# https://en.wikipedia.org/wiki/Tetrahedral_number

main:                  # int main(void) {

    la $a0, prompt     # printf("Enter how many: ");
    li $v0, 4
    syscall

    li $v0, 5           # scanf("%d", number);
    syscall
    move $t0, $v0

    #add $t0, $t0, 1
    li $t1, 1           # counter n $t1
    li $t2, 0           # sum/ total

loop:
    bgt $t1, $t0 end

    addi $t4, $t1, 1    #$t4 = n+1
    mul $t3, $t4, $t1   #$t3 = (n+1)*n
    div $t3, $t3, 2     #$t3 = (n+1)*n/2              
    add $t2, $t2, $t3   #$t2 += (n+1)*n/2

    move $a0, $t2       #   printf("%d", );
    li   $v0, 1
    syscall

    li   $a0, '\n'      # printf("%c", '\n');
    li   $v0, 11
    syscall

    addi $t1, $t1, 1    #n++

    b loop

end:
    jr   $ra           # return

    .data
prompt:
    .asciiz "Enter how many: "
