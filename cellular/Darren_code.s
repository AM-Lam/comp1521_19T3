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
	# s0 = world_size
	# s2 = rule
	# t2 = n_generations
	# s3 = reverse
	# YOU SHOULD ALSO NOTE WHICH REGISTERS DO NOT HAVE THEIR
	# ORIGINAL VALUE WHEN `run_generation' FINISHES
	#
	#
	#
	#
	#

main:
	li      $v0, 4
    la      $a0, prompt_world_size          #print "Enter world size: "
    syscall

worldSizeCheck:    
    li      $s0, 0                          # int world_size 
    li      $v0, 5                          # scanf of world size
    syscall
    
    move    $s0, $v0                        # world size is now s0

    blt     $s0, MIN_WORLD_SIZE, worldSizeError #end program if fail
    bgt     $s0, MAX_WORLD_SIZE, worldSizeError #end program if fail

ruleCheck:
    li      $v0, 4
    la      $a0, prompt_rule                # print "Enter rule: " 
    syscall
        
    li      $s2, 0                          # int rule = 0
    li      $v0, 5                          # scanf rule
    syscall
    
    move    $s2, $v0                        # rule is now s2

    blt     $s2, MIN_RULE , ruleError       #end program if fail
    bgt     $s2, MAX_WORLD_SIZE, ruleError  #end program if fail
    
generationCheck:
    li      $v0, 4
    la      $a0, prompt_n_generations       # print "Enter how many generations"
    syscall
    
    li      $t2, 0                          # int generation = 0
    li      $v0, 5                          # scanf generation
    syscall
    
    move    $t2, $v0                        # generation is now t2

    blt     $t2, MIN_GENERATIONS, generationError   #end program if fail 
    bgt     $t2, MAX_GENERATIONS, generationError   #end program if fail
    
    li      $a0, '\n'                       # putchar('\n');
    li      $v0, 11
    syscall

checkNegative:    
    li      $t4, 0                          # int reverse = 0
    bltz    $t2, reverseGeneration          # check if generation is negative 
                                           

startingPoint:                              # make the starting cell
    div     $t0, $s0, 2                     # "world_size / 2" into a register    
    li      $t1, 1                          
    mul     $t2, $t0, 4                     #   find location in array
    la      $t3, cells                      #   
    add     $t4, $t2, $t3                   #
    sb      $t1, ($t4)                      #   store "1" into array

runCall:                                    # calling run_generation function
    li      $s1, 1                          # int g = 1
    bgt     $s1, $t2, printReverse          # go print after running
    #jal    run_generation
    addi    $s1, $s1, 1                     # g++
    
    b       runCall                         # loop back until condition is false
    
printReverse:                               # print the world in reverse
    beq     $s3, 1, print                   # print normally if not in reverse
    move    $s1, $t2                        # int g = n_generations
    bltz    $s1, end                        # finish program after loop         
    #jal    print_generation
    sub     $s1, $s1, 1                     # g--   
    
print:                                      # print the world         
    li      $s1, 0                          # int g = 0    
    bgt     $s1, $t2, end                   # finish program after loop
    #jal    print_generation        
    addi    $s1, $s1, 1                     # g++
    
    b       print                           # loop back until condition is false
    
reverseGeneration:                          
    addi    $s3, $s3, 1                     # reverse = 1
    mul     $t2, $t2, -1                    # n_generations = -n_generations
    b       startingPoint                   # looping back to startingPoint
    
worldSizeError:                             # print "Invalid world size"
    la      $a0, error_world_size
    li      $v0, 4
    syscall
    b       end                             # end the program
    
ruleError:                                  # print "Invalid rule"
    la      $a0, error_rule
    li      $v0, 4
    syscall
    b       end                             # end the program
    
generationError:                            # print "Invalid no. of generations"
    la      $a0, error_n_generations
    li      $v0, 4
    syscall
    b       end                             # end the program

end: 
    li   $v0, 0          # return 0
    jr   $ra
    
    
    
    #la      $a0 
    
    
	# replace the syscall below with
	#
	# li	$v0, 0
	# jr	$ra
	#
	# if your code for `main' preserves $ra by saving it on the
	# stack, and restoring it after calling `print_world' and
	# `run_generation'.  [ there are style marks for this ]

	#li	$v0, 10
	#syscall



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

	#
	# REPLACE THIS COMMENT WITH YOUR CODE FOR `print_generation'.
	#

	jr	$ra