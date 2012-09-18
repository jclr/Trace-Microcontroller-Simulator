all : trace shopper

inst_exec.o : inst_exec.h inst_exec.c
	clang -c inst_exec.c

obj_file_handler.o : obj_file_handler.h obj_file_handler.c
	clang -c obj_file_handler.c 

trace : trace.c inst_exec.o obj_file_handler.o
	clang trace.c inst_exec.o obj_file_handler.o -o trace

shopper : shopper.c
	clang shopper.c -o shopper