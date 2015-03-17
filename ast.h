#ifndef _AST_H
#define _AST_H
#include "parser.h"
#include "tree.h"
struct tree_node* removeTerm(struct tree_node* tr);
struct tree_node* remove_Chaining(struct tree_node* tr);
struct tree_node* removeExtra(struct tree_node* tr);
struct tree_node* arithmeticPass(struct tree_node* tr);
#endif