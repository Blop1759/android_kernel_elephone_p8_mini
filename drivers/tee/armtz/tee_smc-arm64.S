/*
 * Copyright (c) 2014, Linaro Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/linkage.h>
#include "../include/arm_common/wdteesmc.h"
	.text

#define SMC_PARAM_X0_OFFS	0
#define SMC_PARAM_X2_OFFS	16
#define SMC_PARAM_X4_OFFS	32
#define SMC_PARAM_X6_OFFS	48

/* void tee_smc_call(struct smc_param *param); */
	.globl	tee_smc_call
ENTRY(tee_smc_call)
	stp	x28, x30, [sp, #-16]!
	mov	x28, x0
	ldp	x0, x1, [x28, #SMC_PARAM_X0_OFFS]
	ldp	x2, x3, [x28, #SMC_PARAM_X2_OFFS]
#	ldp	x4, x5, [x28, #SMC_PARAM_X4_OFFS]
#	ldp	x6, x7, [x28, #SMC_PARAM_X6_OFFS]
	mov x4,x0
	ldr x0, =SMC_CMD_NORMAL
	smc	#0
	stp	x0, x1, [x28, #SMC_PARAM_X0_OFFS]
	stp	x2, x3, [x28, #SMC_PARAM_X2_OFFS]
	ldp	x28, x30, [sp], #16
	ret
ENDPROC(tee_smc_call)

#if 0
	.globl __flush_dcache_area
__flush_dcache_area:
  mrs x3, ctr_el0
  ubfx    x3, x3, #16, #4
  mov x2, #0x4                    // #4
  lsl x2, x2, x3
  add x1, x0, x1
  sub x3, x2, #0x1
  bic x0, x0, x3
1: dc  civac, x0
  add x0, x0, x2
  cmp x0, x1
  b.cc 1b
  dsb sy
  ret
#endif
