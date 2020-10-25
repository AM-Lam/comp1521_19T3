########################################################################
# COMP1521 20T2 --- assignment 1: a cellular automaton renderer
#
# Written by <<YOU>>, July 2020.


# Maximum and minimum values for the 3 parameters.

MIN_WORLD_SIZE	=    1
MAX_WORLD_SIZE	=  128
MIN_GENERATIONS	= -256
MAX_GENERATIONS	=  256
MIN_RULE	=    0
MAX_RULE	=  255

# Characters used to print alive/dead cells.

ALIVE_CHAR	= '#'
DEAD_CHAR	= '.'

# Maximum number of bytes needs to store all generations of cells.

MAX_CELLS_BYTES	= (MAX_GENERATIONS + 1) * MAX_WORLD_SIZE

	.data

# `cells' is used to store successive generations.  Each byte will be 1
# if the cell is alive in that generation, and 0 otherwise.

cells:	.space MAX_CELLS_BYTES


# Some strings you'll need to use:

prompt_world_size:	.asciiz "Enter world size: "
error_world_size:	.asciiz "Invalid world size\n"
prompt_rule:		.asciiz "Enter rule: "
error_rule:		.asciiz "Invalid rule\n"
prompt_n_generations:	.asciiz "Enter how many generations: "
error_n_generations:	.asciiz "Invalid number of generations\n"

	.text

	#
	# REPLACE THIS COMMENT WITH A LIST OF THE REGISTERS USED IN
	# `main', AND THE PURPOSES THEY ARE ARE USED FOR
	#
	# YOU SHOULD ALSO NOTE WHICH REGISTERS DO NOT HAVE THEIR
	# ORIGINAL VALUE WHEN `run_generation' FINISHES
	#

main:
	#
	# REPLACE THIS COMMENT WITH YOUR CODE FOR `main'.
	#
	la $a0, prompt_world_size
	li $v0, 4
	syscall							# printf("Enter world size: ");

	li $t0, 0						# int world_size = 0;
	li $v0, 5
	syscall							# scanf("%d", &world_size);
	move $t0, $v0					# $t0 = world_size

	li $t7, MIN_WORLD_SIZE			# $t7 = temp
	la $a0, error_world_size		# printf("Invalid world size\n");
	blt $t0, $t7, exitWarn			# world_size < MIN_WORLD_SIZE
	li $t7, MAX_WORLD_SIZE			
	bgt $t0, $t7, exitWarn			# world_size > MAX_WORLD_SIZE

	la $a0, prompt_rule
	li $v0, 4
	syscall							# printf("Enter rule: ");

	li $t1, 0						# int rule = 0;
	li $v0, 5
	syscall							# scanf("%d", &rule);
	move $t1, $v0					# $t1 = rule

	li $t7, MIN_RULE					# $t7 = temp
	la $a0, error_rule				# printf("Invalid rule\n");
	blt $t1, $t7, exitWarn			# rule < MIN_RULE
	li $t7, MAX_RULE			
	bgt $t1, $t7, exitWarn			# rule > MAX_RULE

	la $a0, prompt_n_generations
	li $v0, 4
	syscall							# printf("Enter how many generations: ");

	li   $t2, 0						# int n_generations = 0;
	li 	 $v0, 5
	syscall							# scanf("%d", &n_generations);
	move $t2, $v0					# $t2 = n_generations

	li   $t7, MIN_GENERATIONS		# $t7 = temp
	la   $a0, error_n_generations	# printf("Invalid number of generations\n");
	blt  $t2, $t7, exitWarn			# n_generations < MIN_GENERATIONS
	li   $t7, MAX_GENERATIONS
	bgt  $t2, $t7, exitWarn			# n_generations > MAX_GENERATIONS

	li   $a0, '\n'    				# putchar('\n');
    li   $v0, 11
    syscall

	li $t3, 0						# $t3 = int reverse = 0;
	bltz $t3, reverse
	b afterReverse

# wack1
reverse: 
	li $t3, 1						# reverse = 1;
	neg $t2, $t2					# n_generations = -n_generations;
	b afterReverse

afterReverse:
# wack 2
	move $t4, $t0					# $t4 = address of cells #li? sw?
	li $t7, 2
	div $t4, $t7
	mflo $t4
	mul $t4, $t4, 4						
	li $t7, 1						# g = 1
	sw $t7, cells($t4)				# cells[0][world_size / 2] = 1;

	li $t5, 1						# $t5 = 1
gLoop:
	bgt $t5, $t2, isReverse
	addi $sp, $sp, -16
	sw $ra, 12($sp)
	sw $s0, 8($sp)
	sw $s1, 4($sp)
	sw $s2, 0($sp)

	move  $s0, $t0
	move  $s1, $t5
	move  $s2, $t1

	move  $a0, $s0					# run_generation(world_size, g, rule);
	jal run_generation
	addi $t5, $t5, 1				# g++;

	lw $s2, 0($sp)
	lw $s1, 4($sp)
	lw $s0, 8($sp)
	lw $ra, 12($sp)
	addi $sp, $sp, 16

isReverse:
	beq $t3, 0, decreasing
	jal increasing

increasing:
	move $t5, $t2					# int g = n_generations
	li $t5, 0
	blt $t5, 0, exit				# g >= 0
	jal callPrint					# print_generation(world_size, g);

	addi $t5, $t5, -1				# g--

decreasing:
	li $t5, 0						# int g = 0
	bgt $t5, $t2, exit				# g <= n_generations
	jal callPrint					# print_generation(world_size, g);

	addi $t5, $t5, 1				# g++;

callPrint:
	addi $sp, $sp, -12
	sw $ra, 8($sp)
	sw $s0, 4($sp)
	sw $s1, 0($sp)

	move  $s0, $t0
	move  $s1, $t5

	move  $a0, $s0					# print_generation(world_size, g);
	jal print_generation


exitWarn:
	li    $v0, 4
    syscall            				# printf("%s", warning message);
	li	$v0, 0

	jr	$ra

exit:
	# replace the syscall below with
	#
	lw $s1, 0($sp)
	lw $s0, 4($sp)
	lw $ra, 8($sp)
	addi $sp, $sp, 12

	li	$v0, 0
	jr	$ra
	#
	# if your code for `main' preserves $ra by saving it on the
	# stack, and restoring it after calling `print_world' and
	# `run_generation'.  [ there are style marks for this ]

	# li	$v0, 10
	# syscall



	#
	# Given `world_size', `which_generation', and `rule', calculate
	# a new generation according to `rule' and store it in `cells'.
	#

	#
	# REPLACE THIS COMMENT WITH A LIST OF THE REGISTERS USED IN
	# `run_generation', AND THE PURPOSES THEY ARE ARE USED FOR
	#
	# YOU SHOULD ALSO NOTE WHICH REGISTERS DO NOT HAVE THEIR
	# ORIGINAL VALUE WHEN `run_generation' FINISHES
	#

run_generation:

	#
	# REPLACE THIS COMMENT WITH YOUR CODE FOR `run_generation'.
	#
	addi $sp, $sp, -20
	sw $ra, 16($sp)
	sw $s0, 12($sp)
	sw $s1, 8($sp)
	sw $s2, 4($sp)
	sw $a0, 0($sp)

	li 	$t0, 0							# $t0 = x
bigLoopRun:
	bge $t0, $s0, exitRun 				# x < world_size
	li 	$t1, 0							# $t1 = left
	bgt $t0, 0, loadleft

	addi $t0, $t0, 1					# x++;

loadleft:

	jr $ra


exitRun:
	jr	$ra


	#
	# Given `world_size', and `which_generation', print out the
	# specified generation.
	#

	#
	# REPLACE THIS COMMENT WITH A LIST OF THE REGISTERS USED IN
	# `print_generation', AND THE PURPOSES THEY ARE ARE USED FOR
	#
	# YOU SHOULD ALSO NOTE WHICH REGISTERS DO NOT HAVE THEIR
	# ORIGINAL VALUE WHEN `print_generation' FINISHES
	#

print_generation:
	addi $sp, $sp, -16
	sw $ra, 12($sp)
	sw $s0, 8($sp)
	sw $s1, 4($sp)
	sw $a0, 0($sp)
	#
	# REPLACE THIS COMMENT WITH YOUR CODE FOR `print_generation'.
	#

	jr	$ra
