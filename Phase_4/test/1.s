# SPL compiler generated assembly
.data
_prmpt: .asciiz "Enter an integer: "
_eol: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prmpt
  syscall
  li $v0, 5
  syscall
  jr $ra
write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _eol
  syscall
  move $v0, $0
  jr $ra
hanoi:
  lw $t0, -0($sp)
  move $t0, $a0
  move $v1, $sp
  lw $t1, -4($sp)
  move $t1, $a1
  lw $t2, -8($sp)
  move $t2, $a2
  lw $t3, -12($sp)
  move $t3, $a3
  sw $v1, 0($sp)
  move $v1, $sp
  addi $sp, $sp, -4
  lw $t4, -16($sp)
  move $t4, $t3
  lw $t5, -20($sp)
  li $t5, 1
  beq $t4, $t5, label0
  j label1
label0:
  lw $t6, -24($sp)
  move $t6, $t2
  lw $t7, -28($sp)
  li $t7, 10000
  lw $s0, -32($sp)
  mul $s0, $t6, $t7
  lw $s1, -36($sp)
  move $s1, $t0
  lw $s2, -40($sp)
  add $s2, $s0, $s1
  move $a0, $s2
  addi $sp, $sp, -48
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 48
  j label2
label1:
  lw $s3, -44($sp)
  move $s3, $t3
  lw $s4, -48($sp)
  li $s4, 1
  lw $s5, -52($sp)
  sub $s5, $s3, $s4
  lw $s6, -56($sp)
  move $s6, $t2
  lw $s7, -60($sp)
  move $s7, $t0
  sw $t1, -4($sp)
  lw $t1, -64($sp)
  sw $t4, -16($sp)
  lw $t4, -4($sp)
  move $t1, $t4
  addi $sp, $sp, -68
  move $a0, $t1
  move $a1, $s7
  move $a2, $s6
  move $a3, $s5
  jal hanoi
  addi $sp, $sp, 68
  sw $t5, -20($sp)
  lw $t5, -68($sp)
  move $t5, $v0
  sw $t6, -24($sp)
  lw $t6, -72($sp)
  move $t6, $t2
  sw $t7, -28($sp)
  lw $t7, -76($sp)
  li $t7, 10000
  sw $s0, -32($sp)
  lw $s0, -80($sp)
  mul $s0, $t6, $t7
  sw $s1, -36($sp)
  lw $s1, -84($sp)
  move $s1, $t0
  sw $s2, -40($sp)
  lw $s2, -88($sp)
  add $s2, $s0, $s1
  move $a0, $s2
  addi $sp, $sp, -96
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 96
  sw $t3, -12($sp)
  lw $t3, -92($sp)
  sw $s3, -44($sp)
  lw $s3, -12($sp)
  move $t3, $s3
  sw $s4, -48($sp)
  lw $s4, -96($sp)
  li $s4, 1
  lw $t4, -100($sp)
  sub $t4, $t3, $s4
  sw $t1, -64($sp)
  lw $t1, -104($sp)
  sw $s7, -60($sp)
  lw $s7, -4($sp)
  move $t1, $s7
  sw $s6, -56($sp)
  lw $s6, -108($sp)
  move $s6, $t2
  sw $s5, -52($sp)
  lw $s5, -112($sp)
  move $s5, $t0
  addi $sp, $sp, -116
  move $a0, $s5
  move $a1, $s6
  move $a2, $t1
  move $a3, $t4
  jal hanoi
  addi $sp, $sp, 116
  sw $t5, -68($sp)
  lw $t5, -116($sp)
  move $t5, $v0
label2:
  sw $t6, -72($sp)
  lw $t6, -120($sp)
  li $t6, 0
  move $v0, $t6
  move $sp, $v1
  lw $v1, 0($sp)
  jr $ra
main:
  addi $fp, $gp, -32768
  sw $v1, 0($sp)
  move $v1, $sp
  addi $sp, $sp, -4
  sw $t7, -76($sp)
  lw $t7, -0($sp)
  li $t7, 3
  sw $s0, -80($sp)
  lw $s0, -4($sp)
  move $s0, $t7
  sw $s1, -84($sp)
  lw $s1, -8($sp)
  move $s1, $s0
  sw $s2, -88($sp)
  lw $s2, -12($sp)
  li $s2, 1
  lw $s3, -16($sp)
  li $s3, 2
  sw $t3, -92($sp)
  lw $t3, -20($sp)
  li $t3, 3
  addi $sp, $sp, -24
  move $a0, $t3
  move $a1, $s3
  move $a2, $s2
  move $a3, $s1
  jal hanoi
  addi $sp, $sp, 24
  sw $s4, -96($sp)
  lw $s4, -24($sp)
  move $s4, $v0
  lw $s7, -28($sp)
  li $s7, 0
  move $v0, $s7
  move $sp, $v1
  lw $v1, 0($sp)
  jr $ra
