# Read numbers into an array until a negative number is entered
# then print the numbers in reverse order

# i in register $t0
# registers $t1, $t2 & $t3 used to hold temporary results

main:
    li $t0, 0           # i = 0
loop0:
    bge $t0, 1000, end0 # while (i < 1000) {
    li $v0, 5           #   scanf("%d", &numbers[i]);
    syscall             #
    move $t4, $v0           # $t4 = int x;

    blt $t4, 0, end0
    mul $t1, $t0, 4     #   calculate &numbers[i]
    la $t2, numbers     #
    add $t3, $t1, $t2   #
    sw $v0, ($t3)       #   store entered number in array

    addi $t0, $t0, 1        #   i++;
    b loop0                 # }

end0:

    ble $t0, 0, end1        # while (i > 0) {
    addi $t0, $t0, -1       #   i--;

    mul $t1, $t0, 4     #   calculate &numbers[i]
    la $t2, numbers     #
    add $t3, $t1, $t2   #
    lw $a0, ($t3)       #   store entered number in array

    li $v0, 1
    syscall

    li $a0, '\n'
    li $v0, 11
    syscall
    b end0

end1:
    li $v0, 0           # return 0
    jr $31              #

.data
numbers:
    .space 4000