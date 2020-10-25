# Read 10 numbers into an array
# print 0 if they are in non-decreasing order
# print 1 otherwise

# i in register $t0

main:

    li $t0, 0           # i = 0
loop0:
    bge $t0, 10, end0   # while (i < 10) {

    li $v0, 5           #   scanf("%d", &numbers[i]);
    syscall             #

    mul $t1, $t0, 4     #   calculate &numbers[i]
    la $t2, numbers     #
    add $t3, $t1, $t2   #
    sw $v0, ($t3)       #   store entered number in array

    addi $t0, $t0, 1    #   i++;
    b loop0             # }
end0:

    li $t6, 0               # swapped = $t6 = 0
    li $t0, 1               # i = 1

loop1:
    bge $t0, 10, end1   # while (i < 10) {

    mul $t1, $t0, 4     # calculate &numbers[i]
    la $t2, numbers     #
    add $t3, $t1, $t2   #
    lw $t4, ($t3)       # x = &numbers[i]

                        # make a new variable for i - 1 in $t7
    move $t7, $t0       # t7 = i
    sub $t7, $t7, 1     # $t7 = $t7 - 1
    mul $t7, $t7, 4     # calculate &numbers[i - 1] # bits: t7 = t7 * 4
    add $t7, $t7, $t2   # address $t3 = $t7 (array position) + $t2 (address)
    lw $t5, ($t7)       # y = &numbers[i - 1]
    
    blt $t4, $t5, swipped 

    addi $t0, $t0, 1        # i++
    b loop1

swipped:
    addi $t6, $t6, 1         # swapped = 1;
    j end1

end1:
    move $a0, $t6           # printf("%d", swapped)
    li $v0, 1               #
    syscall

    li   $a0, '\n'          # printf("%c", '\n');
    li   $v0, 11
    syscall

    jr $31


.data

numbers:
    .word 0 0 0 0 0 0 0 0 0 0  # int numbers[10] = {0};

