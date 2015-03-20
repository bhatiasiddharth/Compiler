#ifndef _ANALYZER_H
#include "parser.h"
#include "ast.h"
#include "symtab.h"

extern void st_fill(struct tree_node* tr, int scope, struct symbol_table* tables,char* func_name,FILE* fp);



#endif