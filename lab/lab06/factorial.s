# Recursive factorial function
# n < 1 yields n! = 1
# $s0 is used for n
# we use an s register because the convention is their value
# is preserved across function calls
# f is in $t0

# DO NOT CHANGE THE CODE IN MAIN

main:
    addi $sp, $sp, -8  # create stack frame
    sw   $ra, 4($sp)   # save return address
    sw   $s0, 0($sp)   # save $s0

    li   $s0, 0
    la   $a0, msg1
    li   $v0, 4
    syscall            # printf(Enter n: ")

    li    $v0, 5
    syscall            # scanf("%d", &n)
    move  $s0, $v0

    move  $a0, $s0     # factorial(n)
    jal   factorial    #
    move  $t0, $v0     #

    move  $a0, $s0
    li    $v0, 1
    syscall            # printf ("%d", n)

    la    $a0, msg2
    li    $v0, 4
    syscall            # printf("! = ")

    move  $a0, $t0
    li    $v0, 1
    syscall            # printf ("%d", f)

    li   $a0, '\n'     # printf("%c", '\n');
    li   $v0, 11
    syscall

                       # clean up stack frame
    lw   $s0, 0($sp)   # restore $s0
    lw   $ra, 4($sp)   # restore $ra
    addi $sp, $sp, 8   # restore sp

    li  $v0, 0         # return 0
    jr  $ra

    .data
msg1:   .asciiz "Enter n: "
msg2:   .asciiz "! = "


# DO NOT CHANGE CODE ABOVE HERE


    .text
factorial:
    addi $sp, $sp, -12  # create stack frame 
    sw   $ra, 8($sp)   # save return address
    sw   $s0, 4($sp)   # save $s0
    sw   $a0, 0($sp)   # save $a0

    li $t1, 1
    bgt $a0, $t1, else
    li $v0, 1
    
    j endif
else:

    addi $a0, $a0, -1
    jal   factorial    # recursive function
    move $t2, $v0
    lw $a0, 0($sp)
    mul $v0, $v0, $a0
        
endif:

    lw   $a0, 0($sp)
    lw   $s0, 4($sp)   # restore $s0
    lw   $ra, 8($sp)   # restore $ra
    addi $sp, $sp, 12   # restore sp
    jr  $ra
