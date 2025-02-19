		section .text,code

		xdef _test_add_mem_to_reg
		xdef _test_add_mem_to_reg_unrolled_4x

; iterations in d0, step in d1
_test_add_mem_to_reg::
		move.l	d1,step_long
		clr.l	d1
.loop:
		add.l	step_long,d1
		subq.l	#1,d0
		bgt.s	.loop

		move.l	d1,d0
		rts


; iterations in d0, step in d1
_test_add_mem_to_reg_unrolled_4x::
		lsr.l	#2,d0
		move.l	d1,step_long
		clr.l	d1
.loop:
		add.l	step_long,d1
		add.l	step_long,d1
		add.l	step_long,d1
		add.l	step_long,d1
		subq.l	#1,d0
		bgt.s	.loop

		move.l	d1,d0
		rts

		align 4
step_long:
		dc.l 1
