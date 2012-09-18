/*
 *  obj_file_handler.c
 *  
 *
 *  Created by Jacy Clare on 11/29/11.
 *  Copyright 2011 Jacy Clare. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "obj_file_handler.h"
#include "inst_exec.h"

void read_file(FILE* current_file, obj_dir* memory){
	int count, i;
	obj_dir current_addr, current_inst;
	obj_dir header[2];
	while (fread(&current_inst, 1, 2, current_file) == 2) {
		current_inst = FLIP_BYTES(current_inst);
		if (current_inst == 0xCADE || current_inst == 0xDADA) {
			if(fread(&current_addr, 1, 2, current_file) != 2){
				return;
			}
			current_addr = FLIP_BYTES(current_addr);
			if (current_inst == 0xCADE && (current_addr >= 0x2000 && (current_addr < 0x8000 || current_addr >= 0xA000))) {
			 fprintf(stderr, "IllegalMemoryAccessException at %X\n", current_addr);
				exit(1);
			 }
			if (current_inst == 0xDADA && (current_addr < 0x2000 || (current_addr >= 0x8000 && current_addr < 0xA000))) {
				fprintf(stderr, "IllegalMemoryAccessException at %X\n", current_addr);
				exit(1);
			}
			if(fread(&current_inst, 1, 2, current_file) != 2){
				return;
			}
			current_inst = FLIP_BYTES(current_inst);
			count = current_inst;
			for (i = 0; i < count; i++) {
				if(fread(&current_inst, 1, 2, current_file) != 2){
					return;
				}
				current_inst = FLIP_BYTES(current_inst);
				memory[current_addr] = current_inst;
				current_addr++;
			}
		}
	}
}
