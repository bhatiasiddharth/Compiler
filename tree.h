#ifndef _TREE_H
#define _TREE_H
#include "parser.h"

struct tree_node {
   int symbol;
   int children_count;
   struct tree_node* parent;
   struct tree_node* children[RULE_MAX_SYMBOLS];
};

extern struct tree_node* tree_init(struct tree_node* parent, int symbol);
extern struct tree_node* tree_traverse(struct tree_node* tnode);
extern void tree_print(struct tree_node* tnode, FILE* fp, int flag);

#endif