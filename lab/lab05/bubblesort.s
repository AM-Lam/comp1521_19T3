# read 10 numbers into an array
# bubblesort them
# then print the 10 numbers

# i in register $t0
# registers $t1, $t2 & $t3 used to hold temporary results

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

    add $t0, $t0, 1     #   i++;
    b loop0             # }
end0:
                            # initialise the variables
    li $t6, 1               # swapped = $t6 = 1

swapDo: 
    beq $t6, 0, endInput   # while (swapped) {
    sub $t6, $t6, 1        # swapped = 0
    li $t0, 1              # i = 1

    blt $t0, 10, inputLoop  # while (i < 10) {
    b swapDo

inputLoop:
    bge $t0, 10, swapDo # while (i < 10) {
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

    blt $t4, $t5, sort 

    b endLoop         # }

sort:
    sw $t4, ($t7)       # $t4 = $t5
    sw $t5, ($t3)       # $t5 = temp = $t1
    addi $t6, $t6, 1    # swapped = 1;

    b endLoop
    
endLoop:
    addi $t0, $t0, 1    # i++
    b inputLoop

endInput:

    li $t0, 0           # i = 0
loop1:
    bge $t0, 10, end1   # while (i < 10) {

    mul $t1, $t0, 4     #   calculate &numbers[i]
    la $t2, numbers     #
    add $t3, $t1, $t2   #
    lw $a0, ($t3)       #   load numbers[i] into $a0
    li $v0, 1           #   printf("%d", numbers[i])
    syscall

    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall

    add $t0, $t0, 1     #   i++
    b loop1             # }
end1:

    jr $31              # return

.data

numbers:
    .word 0 0 0 0 0 0 0 0 0 0  # int numbers[10] = {0};

