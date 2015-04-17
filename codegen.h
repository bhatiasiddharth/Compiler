#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "lexer.h"
#include "analyzer.h"
void dataseg_add(char *identifier, int scope, int type, union value* value);
extern void codeseg_add(char *fmt, ...);
extern void codgen_write(char *codegen_file, struct tree_node* ast);

enum registers{AX, BX, CX, DX };

// todo : Use dynamic string
#define DATA_SIZE 10*1024
#define CODE_SIZE 10*1024
#define REG_COUNT 4
#define MACROS_FILE "codegen/utils.asm"

int reg[REG_COUNT];
char* reg_names[];
char dataseg[DATA_SIZE], codeseg[DATA_SIZE];
#endif
