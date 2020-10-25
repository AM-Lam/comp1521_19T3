# Sieve of Eratosthenes
# https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
main:

    li $t0, 0           # $t0 = i = 0

    li $t1, 1
loop0:
    bge $t0, 1000, endLoop0 # while (i < 1000) {
    sb $t1, prime($t0)      # prime[i] = 1;
    addi $t0, $t0, 1        # i++;
    b loop0

endLoop0:
    li $t0, 2

Loop1:
    bge $t0, 1000, endLoop1 # while (i < 1000) {
    beq $t0, $t1, primeOp   # if (prime[i]) {
    b endLoop11

primeOp:
    li $t3, 0
    move $a0, $t0
    li $v0, 1
    syscall

    mul $t2, $t0, 2
    blt $t2, 1000, endLoop12    # while (j < 1000) {
    b endLoop11

endLoop12:
    bgt $t2, 1000, endLoop11    # while (j < 1000) {
    sb $t3, prime($t2)          # prime[i] = 1;
    add $t2, $t2, $t0          # j = j + i;

    b endLoop12

endLoop11:
    addi $t0, $t0, 1
    b Loop1

endLoop1:
    li $v0, 0           # return 0
    jr $31

.data
prime:
    .space 1000