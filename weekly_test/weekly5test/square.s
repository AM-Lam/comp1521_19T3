main:
    li $v0, 5           #   scanf("%d", &x);
    syscall             #
    move $t0, $v0

    li $t1, 0           #   i = 0

loop:
    bge $t1, $t0, end   #   while (i < x) {
    li $t2, 0           #   j = 0
    bge $t2, $t0, endInnerLoop#  while (j < x) {
    b inLoop

inLoop:
    bge $t2, $t0, endInnerLoop#  while (j < x) {
    li $a0, '*'         #   printf("*");
    li $v0, 11
    syscall
    addi $t2, $t2, 1
    b inLoop

endInnerLoop:
    addi $t1, $t1, 1
    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall
    b loop


end:

    li $v0, 0           # return 0
    jr $31
