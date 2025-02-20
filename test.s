		section .text,code

		xdef _test_add_mem_to_reg
		xdef _test_add_mem_to_reg_unrolled_2x
		xdef _test_add_mem_to_reg_unrolled_4x
		xdef _test_add_mem_to_reg_unrolled_8x
		xdef _test_add_mem_to_reg_unrolled_2x_pf
		xdef _test_add_mem_to_reg_unrolled_4x_pf
		xdef _test_add_mem_to_reg_unrolled_8x_pf

; iterations in d0, step in d1
_test_add_mem_to_reg::
		move.l	d1,step_long
		clr.l	d1
.loop:
		add.l	step_long,d1
		subq.l	#1,d0
		bne.s	.loop

		move.l	d1,d0
		rts

; iterations in d0, step in d1
_test_add_mem_to_reg_unrolled_2x::
		lsr.l	#1,d0
		move.l	d1,step_long
		clr.l	d1
.loop:
		add.l	step_long,d1
		add.l	step_long,d1
		subq.l	#1,d0
		bne.s	.loop

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
		bne.s	.loop

		move.l	d1,d0
		rts

; iterations in d0, step in d1
_test_add_mem_to_reg_unrolled_8x::
		lsr.l	#3,d0
		move.l	d1,step_long
		clr.l	d1
.loop:
		add.l	step_long,d1
		add.l	step_long,d1
		add.l	step_long,d1
		add.l	step_long,d1
		add.l	step_long,d1
		add.l	step_long,d1
		add.l	step_long,d1
		add.l	step_long,d1
		subq.l	#1,d0
		bne.s	.loop

		move.l	d1,d0
		rts

; iterations in d0, step in d1
_test_add_mem_to_reg_unrolled_2x_pf::
		lsr.l	#1,d0
		move.l	d1,step_long
		clr.l	d1
		movem.l d2/d3,-(sp)
.loop:
		move.l	step_long,d2 ; load to use stall...
		move.l	step_long,d3 ; load to use stall...
		add.l	d2,d1
		add.l	d3,d1
		subq.l	#1,d0
		bne.s	.loop

		movem.l (sp)+,d2/d3

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

; iterations in d0, step in d1
_test_add_mem_to_reg_unrolled_8x_pf::
		lsr.l	#3,d0
		move.l	d1,step_long
		clr.l	d1
		movem.l d2-d7,-(sp)
.loop:
		move.l	step_long,d2 ; load to use stall...
		move.l	step_long,d3 ; load to use stall...
		move.l	step_long,d4 ; load to use stall...
		move.l	step_long,d5 ; load to use stall...
		move.l	step_long,d6 ; load to use stall...
		move.l	step_long,d7 ; load to use stall...
		move.l	step_long,a0 ; load to use stall...
		move.l	step_long,a1 ; load to use stall...

		add.l	d2,d1
		add.l	d3,d1
		add.l	d4,d1
		add.l	d5,d1
		add.l	d6,d1
		add.l	d7,d1
		add.l	a0,d1
		add.l	a1,d1

		subq.l	#1,d0
		bne.s	.loop

		movem.l (sp)+,d2-d7

		move.l	d1,d0
		rts


		align 4
step_long:
		dc.l 1
