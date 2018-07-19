
	add $t3, $imm, $zero, 1			# SWAPPED=1
L1:
	add $t2, $zero, $zero 			# i=0
	beq $imm, $t3, $zero, END   		# if swapped = 0, array sorted, jump to end
	add $t3, $zero, $zero 			# set swapped=0            
L2:
	add $t0, $zero, $imm, 15			# set t0 = 15
	beq $imm, $t0, $t2, L1			# array size is 16, so when i=16 do not enter the loop.
	lw $t0, $t2, $imm, 1024			# t0 = A[i]
	lw $t1, $t2, $imm, 1025	 		# t1= A[i+1]
	bgt $imm, $t0, $t1, SWP			# if A[i]>A[i+1], jump to SWP
	add $t2, $imm, $t2, 1			# i++
	beq $imm, $zero, $zero, L2 		# jump to L2

SWP:

	sw $t0, $imm, $t2, 1025			# A[i+1]=t0 (A[i])
	sw $t1, $imm, $t2, 1024			# A[i]=t1    (A[i+1])
	add $t3, $imm, $zero, 1			# SWAPPED=1
	add $t2, $imm, $t2, 1			# i++
	beq $imm, $zero, $zero, L2 		# jump to L2
END:
	halt $zero, $zero, $zero			# return
.word 1024 0
.word 1025 15
.word 1026 -43
.word 1027 0xFFFD
.word 0x0404 0X8D33 #1028
.word 1029 0x64
.word 1030 0x3500
.word 1031 0X0322
.word 1032 0x1
.word 0x0409 55  #1033
.word 0X040A 32 #1034
.word 0x040B -41 #1035
.word 0X040C -55 #1036
.word 0x040D 9 #1037
.word 0X040E -200 #1038
.word 0x040F 0x5000 #1039