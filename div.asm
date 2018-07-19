	lw $t0, $imm, $zero, 1024			# load the value of x
	
	lw $t1, $imm, $zero, 1025	 		# load the value of y

	add $t3, $zero, $imm, 1				# t3 = 1 

	bne $imm, $t1, $t3, NOT1			# check if y = 1 in order to shorten running time

	add $t2, $t0, $zero					# the result is equal to t0, x

	add $t0, $zero, $zero				# the reminder is 0

	beq $imm, $zero, $zero, END			# go to the end
	
NOT1:
	
	add $t2, $zero, $zero          		# t2=0
	
	bgt $imm, $t0, $zero, L1			#positive number, go to "normal loop"	

	add $t0, $t0, $imm, 0x7FFF			#\
										# \
	add $t0, $t0, $imm, 0x7FFF			#  }    -    -     t0 += MEM_SIZE
										# /
	add $t0, $t0, $imm, 2				#/

L1:

	bgt $imm, $t1, $t0, END				# if x>y, leave the while loop

	sub $t0, $t0, $t1					# x=x-y

	add $t2, $t2, $imm, 1				# t2++

	beq $imm, $zero, $zero, L1 			# jump to L1

END:

	sw $t0, $imm, $zero, 1027			# reminder saved to 1027

	sw $t2, $imm, $zero, 1026			# division result saved to 1026

	halt $zero, $zero, $zero




.word 1024 0xFFFF

.word 1025 0x1

