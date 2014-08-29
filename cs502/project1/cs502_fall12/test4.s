	.file	"test4.c"
	.section	.rodata
.LC2:
	.string	"%f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	.LC0(%rip), %eax
	movl	%eax, -4(%rbp)
	movl	.LC1(%rip), %eax
	movl	%eax, -8(%rbp)
	movss	-4(%rbp), %xmm0
	addss	-8(%rbp), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC2, %edi
	movl	$1, %eax
	call	printf
	movss	-4(%rbp), %xmm0
	subss	-8(%rbp), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC2, %edi
	movl	$1, %eax
	call	printf
	movss	-4(%rbp), %xmm0
	mulss	-8(%rbp), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC2, %edi
	movl	$1, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1094923059
	.align 4
.LC1:
	.long	1093140480
	.ident	"GCC: (GNU) 4.7.0"
	.section	.note.GNU-stack,"",@progbits
