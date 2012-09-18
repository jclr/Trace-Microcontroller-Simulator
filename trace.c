/*
 *  trace.c
 *  
 *
 *  Created by Jacy Clare on 11/28/11.
 *  Copyright 2011 Jacy Clare. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "obj_file_handler.h"
#include "inst_exec.h"

#define MEMORY_LENGTH 65536

int main(int argc, char** argv){
	lc4_inst memory[MEMORY_LENGTH], old_pc;
	char* output_file_name;
	lc4_inst write_val[2];
	FILE* current_file;
	FILE* output_file;
	int i;
	pc = 0;
	psr = 0x2;
	last_inst = 0;
	if (argc < 4) {
		fprintf(stderr, "Incorrect arguments. Please try again.\n");
		exit(1);
	}
	output_file_name = argv[1];
	last_inst = atoi(argv[2]);
	if (last_inst == 0) {
		fprintf(stderr, "Incorrect arguments. Please try again.\n");
		exit(1);
	}
	for (i = 0; i < NUMBER_OF_REGISTERS; i++) {
		registers[i] = 0;
	}
	for (i = 0; i < MEMORY_LENGTH; i++) {
		memory[i] = 0;
	}
	for (i = 3; i < argc ; i++) {
		current_file = fopen(argv[i], "r");
		if (current_file == NULL) {
			fprintf(stderr, "Could not open specified file. Please try again.\n");
			exit(1);
		}
		read_file(current_file, memory);
		fclose(current_file);
	}
	
	output_file = fopen(output_file_name, "wb");
	if (output_file == NULL) {
		fprintf(stderr, "Could not open output file. Please try again.\n");
		exit(1);
	}

	do {
		write_val[0] = pc;
		write_val[1] = memory[pc];
		fwrite(write_val, 2, 2, output_file);
		printf("PC = %X; Instruction = %X\n", pc, memory[pc]);
		old_pc = pc;
		parse_instruction(memory[pc], memory);
		printf("\n");
	} while (old_pc != last_inst);
	fclose(output_file);
	return 0;
}
