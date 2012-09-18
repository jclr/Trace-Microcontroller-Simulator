/*
 *  inst_exec.h
 *  
 *
 *  Created by Jacy Clare on 12/1/11.
 *  Copyright 2011 Jacy Clare. All rights reserved.
 *
 */

typedef unsigned short int lc4_inst;
typedef signed short int lc4_val;

#define NUMBER_OF_REGISTERS 8

#define INSN_OP(X)((X) >> 12)
#define INSN_11_9(X)(((X) >> 9) & 0x7)
#define INSN_8_6(X)(((X) >> 6) & 0x7)
#define INSN_5_3(X)(((X) >> 3) & 0x7)
#define INSN_2_0(X)((X) & 0x7)
#define INSN_4_0(X)((X) & 0x1f)
#define INSN_8_7(X)(((X) >> 7) & 0x3)
#define INSN_6_0(X)((X) & 0x7f)
#define INSN_11(X)(((X) >> 11) & 0x1)
#define INSN_10_0(X)((X) & 0x7ff)
#define INSN_5_0(X)((X) & 0x3f)
#define INSN_8_0(X)((X) & 0x1ff)
#define INSN_5_4(X)(((X) >> 4) & 0x3)
#define INSN_3_0(X)((X) & 0xf)
#define INSN_7_0(X)((X) & 0xff)

lc4_val registers[NUMBER_OF_REGISTERS];
lc4_inst pc;
lc4_inst psr;
lc4_inst last_inst;

void branch_inst(lc4_inst inst);
void arith_inst(lc4_inst inst);
void cmp_inst(lc4_inst inst);
void subroutine_inst(lc4_inst inst);
void logic_inst(lc4_inst inst);
void ldr_inst(lc4_inst inst, lc4_inst* memory);
void str_inst(lc4_inst inst, lc4_inst* memory);
void rti_inst();
void const_inst(lc4_inst inst);
void shift_inst(lc4_inst inst);
void jmp_inst(lc4_inst inst);
void hiconst_inst(lc4_inst inst);
void trap_inst(lc4_inst inst);

void parse_instruction(lc4_inst inst, lc4_inst* memory);