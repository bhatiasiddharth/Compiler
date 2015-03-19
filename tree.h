#ifndef _TREE_H
#define _TREE_H
#include "parser.h"

struct tree_node {
   int symbol;
   union value value;
   int children_count;
   struct tree_node* parent;
   struct tree_node* children[RULE_MAX_SYMBOLS];
};

extern struct tree_node* tree_init(struct tree_node* parent, int symbol, union value value);
extern struct tree_node* tree_traverse(struct tree_node* root);
extern void tree_print(struct tree_node* root, FILE* fp, int flag);
extern void tree_write(struct tree_node* root, char* filename);


#endif