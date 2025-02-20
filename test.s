		section .text,code

		xdef _test_add_mem_to_reg
		xdef _test_add_mem_to_reg_unrolled_4x
		xdef _test_add_mem_to_reg_unrolled_4x_pf

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

; iterations in d0, step in d1
_test_add_mem_to_reg_unrolled_4x_pf::
		lsr.l	#2,d0
		move.l	d1,step_long
		clr.l	d1
		movem.l d2-d5,-(sp)
.loop:
		move.l	step_long,d2 ; load to use stall...
		move.l	step_long,d3 ; load to use stall...
		move.l	step_long,d4 ; load to use stall...
		move.l	step_long,d5 ; load to use stall...
		add.l	d2,d1
		add.l	d3,d1
		add.l	d4,d1
		add.l	d5,d1
		subq.l	#1,d0
		bne.s	.loop

		movem.l (sp)+,d2-d5

		move.l	d1,d0
		rts


		align 4
step_long:
		dc.l 1
