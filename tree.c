#include "parser.h"
#include "tree.h"


struct tree_node* tree_init(struct tree_node* parent, int symbol) {
	struct tree_node* root = (struct tree_node*) malloc(sizeof(struct tree_node));
	root->children_count = 0;
	root->parent = parent;
	root->symbol = symbol;

	for(int i = 0; i < RULE_MAX_SYMBOLS; i++)
		root->children[i] = NULL;
	return root;
}

struct tree_node* tree_traverse(struct tree_node* root) {
	if(root->symbol >= 100 && root->children_count == 0) return root;

	int i = 0;
	struct tree_node* tr1;
	while(i < root->children_count) {
		if(root->children[i]->symbol < 100) {
			i++;
			continue;
		}
		tr1 = tree_traverse(root->children[i]);
		if(tr1 != NULL)
			return tr1;
		else
			i++;
	}
	return NULL;
}



void tree_print(struct tree_node* root) {
	static int tabcount = 0;
	if(root == NULL) return;

	int temptab = tabcount;
	while(temptab--)
		printf("\t");

	print_symbol(root->symbol);

	printf("\n");

	for(int i = 0; i < root->children_count; i++) {
		tabcount++;
		tree_print(root->children[i]);
	}
	tabcount--;
	return;
}
