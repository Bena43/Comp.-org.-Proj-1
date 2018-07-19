	lw $t3, $imm, $zero, 1023 	# t3=n=R[1023]
	add $t3, $t3, $imm, 1	# t3=n++

INIT:
	sw $zero, $imm, $t3, 1024	# R[1024+n]=A[n]=0
	sub $t3, $t3, $imm, 1	# n--	
	bgt $imm, $t3, $zero, INIT	# if n>0 jump tp init
	add $t2, $zero, $imm, 1	# t2=1
	sw $t2, $imm, $zero, 1024	# R[1024] = A[0] = 1
	lw $t3, $imm, $zero, 1023	# n=R[1023], loading again because we used n for the intialzation
L1:				# This loop checks if we're finished and initiates the L2 loop
	lw $t2, $imm, $t3, 1024	# t2 = A[t3] = A[n]
	bne $imm, $zero, $t2, END	# if A[n] !=0 jump to end (finished calculating the nth line of pascal triangle)
	add $t2, $zero, $imm, 1	# i=1
	lw $t0, $imm, $zero, 1024	# t0 = A[0] = R[1024]
	lw $t1, $imm, $zero, 1025	# t1=A[1]=R[1025]
L2:				# every run of this loop calcuates the next line of Pascal
	add $t0, $t1, $t0		# t0=t0+t1
	sw $t0, $imm, $t2, 1024	# t0= A[t2]
	add $t0, $t1, $zero		# t0=t1
	add $t2, $t2, $imm, 1	# i++
	lw $t1, $imm, $t2, 1024	# A[1024+i]=t1
	bgt $imm, $t2, $t3, L1	# if i>n, go the L1
	beq $imm, $zero, $zero, L2	# else, do another iteration to calculate the next line of pascal
END:
	halt $zero, $zero, $zero #finished
.word 1023 5