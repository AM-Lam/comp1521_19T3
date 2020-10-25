main:
    li $v0, 5           #   scanf("%d", &x);
    syscall             #
    move $t0, $v0

    li $v0, 5           #   scanf("%d", &y);
    syscall             #
    move $t1, $v0

    li $t3, 13
    addi $t2, $t0, 1    #   int i = x + 1;
loop: 
    bge $t2, $t1, end   #    while (i < y) {
    bne $t2, $t3, print #    if (i != 13)
    addi $t2, $t2, 1    #    i = i + 1;
    b loop

print:
    move $a0, $t2       #    printf("%d", i);
    li $v0, 1
    syscall

    addi $t2, $t2, 1    #    i = i + 1;

    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall


    b loop

end:

    li $v0, 0           # return 0
    jr $31
