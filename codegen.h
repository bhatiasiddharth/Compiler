#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "lexer.h"
#include "analyzer.h"
void dataseg_add(char *identifier, int scope, int type, union value* value, int size);
extern void codeseg_add(char *fmt, ...);
extern void tempstr_add(char *str);
extern void codegen_write(char *codegen_file, struct tree_node* ast);
extern void tempentry_add(char *identifier, union value* value, int type, int size);

enum registers{AX, BX, CX, DX };

// todo : Use dynamic string
#define DATA_SIZE 10*1024
#define CODE_SIZE 10*1024
#define REG_COUNT 4
#define MACROS_FILE "codegen/utils.asm"
#define TMPPREFIX "_TMPD"

int reg[REG_COUNT];
char* reg_names[4];
char dataseg[DATA_SIZE], codeseg[DATA_SIZE];
#endif
