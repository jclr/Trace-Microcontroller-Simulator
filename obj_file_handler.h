/*
 *  obj_file_handler.h
 *  
 *
 *  Created by Jacy Clare on 11/29/11.
 *  Copyright 2011 Jacy Clare. All rights reserved.
 *
 */

typedef unsigned short int obj_dir;

#define FLIP_BYTES(X)(((X) >> 8) | ((X) << 8))

void read_file(FILE* current_file, obj_dir* memory);