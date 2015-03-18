#ifndef _AST_H
#define _AST_H
#include "parser.h"
#include "tree.h"

extern struct tree_node* removeTerm(struct tree_node* tr);

extern struct tree_node* remove_Chaining(struct tree_node* tr);

extern struct tree_node* removeExtra(struct tree_node* tr);

extern struct tree_node* arithmeticPass(struct tree_node* tr);

extern struct tree_node* create_ast(struct tree_node* tr);
#endif