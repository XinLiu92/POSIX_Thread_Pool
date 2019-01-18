/*
 * thr_asm.a - Linux Intel 64 assembler portion of the thread library
 *             implementation.
 *
 *             This just provides a stub for the asm_yield function!
 */

	.text
	.align	8
	.globl	asm_yield
asm_yield:
	
	pushq	 	%rbp
	movq		%rsp,%rbp
#
	cmpq		$0,%rdi
	je			.branchOver
	jmp			.L1


#
.branchOver:
	
	movq		0(%rsi),%rsp
	movq		8(%rsi),%rbx
	movq		16(%rsi),%r12
	movq		24(%rsi),%r13
	movq		32(%rsi),%r14
	movq		40(%rsi),%r15
	movq		48(%rsi),%rdi
	movq		56(%rsi),%rsi

	popq 	 %rbp
	ret
#
.L1:
	movq		%rsp,0(%rdi)
	movq		%rbx,8(%rdi)
	movq		%r12,16(%rdi)
	movq		%r13,24(%rdi)
	movq		%r14,32(%rdi)
	movq		%r15,40(%rdi)
	movq		%rdi,48(%rdi)
	movq		%rsi,56(%rdi)

	movq		0(%rsi),%rsp
	movq		8(%rsi),%rbx
	movq		16(%rsi),%r12
	movq		24(%rsi),%r13
	movq		32(%rsi),%r14
	movq		40(%rsi),%r15
	movq		48(%rsi),%rdi
	movq		56(%rsi),%rsi

	popq 	 %rbp
	ret
