/*
 *  inst_exec.c
 *  
 *
 *  Created by Jacy Clare on 12/1/11.
 *  Copyright 2011 Jacy Clare. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "inst_exec.h"

void branch_inst(lc4_inst inst){
	lc4_inst sub_op_code;
	lc4_val imm;
	printf("Branching\n");
	printf("PSR = %X\n", psr);
	sub_op_code = INSN_11_9(inst);
	imm = INSN_8_0(inst);
	if ((imm >> 8) & 0x1) {
		imm = imm | 0xFFFFFE00;
	}
	if ((pc + 1 + imm >= 0x2000 && pc + 1 + imm < 0x8000) || pc + 1 + imm >= 0xA000) {
		fprintf(stderr, "IllegalMemoryAccessException at %X\n", pc + 1 + imm);
		exit(1);
	}
	if (pc + 1 + imm >= 0x8000 && psr < 0x8000) {
		fprintf(stderr, "IllegalMemoryAccessException at %X\n", pc + 1 + imm);
		exit(1);
	}
	switch (sub_op_code) {
		case 0:
			pc++;
			break;
		case 4:
			if ((psr & 0x4)) {
				pc = pc + 1 + imm;
			} else {
			pc++;
			}
			break;
		case 6:
			if ((psr & 0x6)) {
				pc = pc + 1 + imm;
			} else {
				pc++;
			}
			break;
		case 5:
			if ((psr & 0x5)) {
				pc = pc + 1 + imm;
			} else {
				pc++;
			}
			break;
		case 2:
			if ((psr & 0x2)) {
				pc = pc + 1 + imm;
			} else {
				pc++;
			}
			break;
		case 3:
			if ((psr & 0x3)) {
				pc = pc + 1 + imm;
			} else {
				pc++;
			}
			break;
		case 1:
			if ((psr & 0x1)) {
				pc = pc + 1 + imm;
			} else {
				pc++;
			}
			break;
		case 7:
			pc = pc + 1 + imm;
			break;
		default:
			printf("Invalid instruction at %x. Skipping instruction.\n", pc);
			pc++;
			break;
	}
}

void arith_inst(lc4_inst inst){
	lc4_inst sub_op_code, rd, rs, rt;
	lc4_val imm;
	printf("Arithmetic\n");
	sub_op_code = INSN_5_3(inst);
	rd = INSN_11_9(inst);
	rs = INSN_8_6(inst);
	rt = INSN_2_0(inst);
	imm = INSN_4_0(inst);
	if ((imm >> 4) & 0x1) {
		imm = imm | 0xFFFFFFE0;
	}
	switch (sub_op_code) {
		case 0:
			registers[rd] = registers[rs] + registers[rt];
			pc++;
			break;
		case 1:
			registers[rd] = registers[rs] * registers[rt];
			pc++;
			printf("RD = %d; RS = %d; RT = %d\n", registers[rd], registers[rs], registers[rt]);
			break;
		case 2:
			registers[rd] = registers[rs] - registers[rt];
			pc++;
			break;
		case 3:
			registers[rd] = registers[rs] / registers[rt];
			pc++;
			break;
		default:
			registers[rd] = registers[rs] + imm;
			pc++;
			break;
	}
	if (!registers[rd]){
		psr = (psr | 0x2);
		psr = (psr & 0x8002);
	}
	else if (registers[rd] > 0){
		psr = (psr | 0x1);
		psr = (psr & 0x8001);
	}
	else {
		printf("Negative");
		psr = (psr | 0x4);
		psr = (psr & 0x8004);
	}
	printf("PSR = %X\n", psr);
}

void cmp_inst(lc4_inst inst){
	lc4_inst sub_op_code, uimm, rs, rt;
	lc4_val imm, cmp;
	printf("Comparing\n");
	sub_op_code = INSN_8_7(inst);
	rs = INSN_11_9(inst);
	rt = INSN_2_0(inst);
	imm = INSN_6_0(inst);
	uimm = INSN_6_0(inst);
	if ((imm >> 6) & 0x1) {
		imm = imm | 0xFFFFFF80;
	}
	switch (sub_op_code) {
		case 0:
			cmp = registers[rs] - registers[rt];
			if (!cmp){
				psr = (psr | 0x2);
				psr = (psr & 0x8002);
			}
			else if (cmp > 0){
				psr = (psr | 0x1);
				psr = (psr & 0x8001);
			}
			else {
				psr = (psr | 0x4);
				psr = (psr & 0x8004);
			}
			pc++;
			break;
		case 1:
			cmp = (lc4_inst)registers[rs] - (lc4_inst)registers[rt];
			if (!cmp){
				psr = (psr | 0x2);
				psr = (psr & 0x8002);
			}
			else if (cmp > 0){
				psr = (psr | 0x1);
				psr = (psr & 0x8001);
			}
			else {
				
				psr = (psr | 0x4);
				psr = (psr & 0x8004);
			}			
			pc++;
			break;
		case 2:
			cmp = registers[rs] - imm;
			if (!cmp){
				psr = (psr | 0x2);
				psr = (psr & 0x8002);
			}
			else if (cmp > 0){
				psr = (psr | 0x1);
				psr = (psr & 0x8001);
			}
			else {
				psr = (psr | 0x4);
				psr = (psr & 0x8004);
			}
			pc++;
			break;
		case 3:
			cmp = (lc4_inst)registers[rs] - uimm;
			if (!cmp){
				psr = (psr | 0x2);
				psr = (psr & 0x8002);
			}
			else if (cmp > 0){
				psr = (psr | 0x1);
				psr = (psr & 0x8001);
			}
			else {
				psr = (psr | 0x4);
				psr = (psr & 0x8004);
			}
			pc++;
			break;
		default:
			printf("Invalid instruction at %x. Skipping instruction.\n", pc);
			pc++;
			break;
	}
	
}

void subroutine_inst(lc4_inst inst){
	lc4_inst sub_op_code, rs;
	lc4_val imm;
	printf("Subroutine\n");
	sub_op_code = INSN_11(inst);
	rs = INSN_8_6(inst);
	imm = INSN_10_0(inst);
	if ((imm >> 10) & 0x1) {
		imm = imm | 0xFFFFFC00;
	}
	switch (sub_op_code) {
		case 0:
			registers[7] = pc + 1;
			pc = ((pc & 0x8000) | (imm << 4));
			break;
		default:
			registers[7] = pc + 1;
			pc = registers[rs];
			break;
	}
	psr = (psr | 0x1);
	psr = (psr & 0x8001);
}

void logic_inst(lc4_inst inst){
	lc4_inst sub_op_code, rd, rs, rt;
	lc4_val imm;
	printf("Logic\n");
	sub_op_code = INSN_5_3(inst);
	rd = INSN_11_9(inst);
	rs = INSN_8_6(inst);
	rt = INSN_2_0(inst);
	imm = INSN_4_0(inst);
	if ((imm >> 4) & 0x1) {
		imm = imm | 0xFFFFFFE0;
	}
	switch (sub_op_code) {
		case 0:
			registers[rd] = registers[rs] & registers[rt];
			pc++;
			break;
		case 1:
			registers[rd] = ~registers[rs];
			pc++;
			break;
		case 2:
			registers[rd] = registers[rs] | registers[rt];
			pc++;
			break;
		case 3:
			registers[rd] = registers[rs] ^ registers[rt];
			pc++;
			break;
		default:
			registers[rd] = registers[rs] & imm;
			pc++;
			break;
	}
	if (!registers[rd]){
		psr = (psr | 0x2);
		psr = (psr & 0x8002);
	}
	else if (registers[rd] > 0){
		psr = (psr | 0x1);
		psr = (psr & 0x8001);
	}
	else {
		psr = (psr | 0x4);
		psr = (psr & 0x8004);
	}
}

void ldr_inst(lc4_inst inst, lc4_inst* memory){
	lc4_inst rd, rs;
	lc4_val imm;
	printf("LDR\n");
	rd = INSN_11_9(inst);
	rs = INSN_8_6(inst);
	imm = INSN_5_0(inst);
	if ((imm >> 5) & 0x1) {
		imm = imm | 0xFFFFFFC0;
	}
	if ((lc4_inst)registers[rs] + imm < 0x2000 || ((lc4_inst)registers[rs] + imm >= 0xA000 && psr < 0x8000)) {
		fprintf(stderr, "IllegalMemoryAccessException at %X\n", (lc4_inst)registers[rs] + imm);
		exit(1);
	}
	if ((lc4_inst)registers[rs] + imm >= 0x8000 && (lc4_inst)registers[rs] + imm < 0xA000) {
		fprintf(stderr, "IllegalMemoryAccessException at %X\n", (lc4_inst)registers[rs] + imm);
		exit(1);
	}
	registers[rd] = (lc4_val)memory[registers[rs] + imm];
	pc++;
	if (!registers[rd]){
		psr = (psr | 0x2);
		psr = (psr & 0x8002);
	}
	else if (registers[rd] > 0){
		psr = (psr | 0x1);
		psr = (psr & 0x8001);
	}
	else {
		psr = (psr | 0x4);
		psr = (psr & 0x8004);
	}
}

void str_inst(lc4_inst inst, lc4_inst* memory){
	lc4_inst rd, rs;
	lc4_val imm;
	printf("STR\n");
	rd = INSN_11_9(inst);
	rs = INSN_8_6(inst);
	imm = INSN_5_0(inst);
	if ((imm >> 5) & 0x1) {
		imm = imm | 0xFFFFFFC0;
	}
	if ((lc4_inst)registers[rs] + imm < 0x2000 || ((lc4_inst)registers[rs] + imm >= 0x8000 && (lc4_inst)registers[rs] + imm < 0xA000)) {
		fprintf(stderr, "IllegalMemoryAccessException at %X\n", (lc4_inst)registers[rs] + imm);
		exit(1);
	}
	if ((lc4_inst)registers[rs] + imm >= 0xA000 && psr < 0x8000) {
		fprintf(stderr, "IllegalMemoryAccessException at %X\n", (lc4_inst)registers[rs] + imm);
		exit(1);
	}
	memory[registers[rs] + imm] = registers[rd];
	pc++;
}

void rti_inst(){
	printf("RTI\n");
	pc = registers[7];
	psr = psr & 0x7fff;
}

void const_inst(lc4_inst inst){
	lc4_inst rd;
	lc4_val imm;
	printf("Constant\n");
	rd = INSN_11_9(inst);
	imm = INSN_8_0(inst);
	if ((imm >> 8) & 0x1) {
		imm = imm | 0xFFFFFE00;
	}
	registers[rd] = imm;
	pc++;
	if (!registers[rd]){
		psr = (psr | 0x2);
		psr = (psr & 0x8002);
	}
	else if (registers[rd] > 0){
		psr = (psr | 0x1);
		psr = (psr & 0x8001);
	}
	else {
		psr = (psr | 0x4);
		psr = (psr & 0x8004);
	}
}

void shift_inst(lc4_inst inst){
	lc4_inst sub_op_code, uimm, rd, rs, rt;
	lc4_val imm;
	printf("Shifting\n");
	sub_op_code = INSN_5_4(inst);
	rd = INSN_11_9(inst);
	rs = INSN_8_6(inst);
	rt = INSN_2_0(inst);
	uimm = INSN_3_0(inst);
	switch (sub_op_code) {
		case 0:
			registers[rd] = registers[rs] << uimm;
			pc++;
			break;
		case 1:
			registers[rd] = (lc4_val)registers[rs] >> uimm;
			pc++;
			break;
		case 2:
			registers[rd] = registers[rs] >> uimm;
			pc++;
			break;
		case 3:
			registers[rd] = registers[rs] % registers[rt];
			pc++;
			break;
		default:
			printf("Invalid instruction at %x. Skipping instruction.\n", pc);
			pc++;
			break;
	}
	if (!registers[rd]){
		psr = (psr | 0x2);
		psr = (psr & 0x8002);
	}
	else if (registers[rd] > 0){
		psr = (psr | 0x1);
		psr = (psr & 0x8001);
	}
	else {
		psr = (psr | 0x4);
		psr = (psr & 0x8004);
	}
}

void jmp_inst(lc4_inst inst){
	lc4_inst sub_op_code, rs;
	lc4_val imm;
	printf("Jumping\n");
	sub_op_code = INSN_11(inst);
	rs = INSN_8_6(inst);
	imm = INSN_10_0(inst);
	if ((imm >> 10) & 0x1) {
		imm = imm | 0xFFFFFC00;
	}
	if ((pc + 1 + imm >= 0x2000 && pc + 1 + imm < 0x8000) || pc + 1 + imm >= 0xA000) {
		fprintf(stderr, "IllegalMemoryAccessException at %X\n", pc + 1 + imm);
		exit(1);
	}
	if (pc + 1 + imm >= 0x8000 && psr < 0x8000) {
		fprintf(stderr, "IllegalMemoryAccessException at %X\n", pc + 1 + imm);
		exit(1);
	}
	switch (sub_op_code) {
		case 0:
			pc = registers[rs];
			break;
		default:
			pc = pc + 1 + imm;
			break;
	}
}

void hiconst_inst(lc4_inst inst){
	lc4_inst uimm, rd;
	printf("HiConstant\n");
	rd = INSN_11_9(inst);
	uimm = INSN_7_0(inst);
	registers[rd] = (registers[rd] & 0xff) | (uimm << 8);
	pc++;
	if (!registers[rd]){
		psr = (psr | 0x2);
		psr = (psr & 0x8002);
	}
	else if (registers[rd] > 0){
		psr = (psr | 0x1);
		psr = (psr & 0x8001);
	}
	else {
		psr = (psr | 0x4);
		psr = (psr & 0x8004);
	}
}

void trap_inst(lc4_inst inst){
	lc4_inst uimm;
	printf("TRAP\n");
	uimm = INSN_7_0(inst);
	registers[7] = pc + 1;
	pc = (0x8000 | uimm);
	psr = (psr | 0x8001);
	psr = (psr & 0x8001);
	printf("PSR = %X\n", psr);
}

void parse_instruction(lc4_inst inst, lc4_inst* memory){
	lc4_inst op_code;
	op_code = INSN_OP(inst);
	switch (op_code) {
		case 0:
			branch_inst(inst);
			break;
		case 1:
			arith_inst(inst);
			break;
		case 2:
			cmp_inst(inst);
			break;
		case 4:
			subroutine_inst(inst);
			break;
		case 5:
			logic_inst(inst);
			break;
		case 6:
			ldr_inst(inst, memory);
			break;
		case 7:
			str_inst(inst, memory);
			break;
		case 8:
			rti_inst();
			break;
		case 9:
			const_inst(inst);
			break;
		case 10:
			shift_inst(inst);
			break;
		case 12:
			jmp_inst(inst);
			break;
		case 13:
			hiconst_inst(inst);
			break;
		case 15:
			trap_inst(inst);
			break;
		default:
			printf("Invalid instruction at %x. Skipping instruction.\n", pc);
			pc++;
			break;
	}
	
}