#include "parser.h"
#include "tree.h"

struct tree_node* tree_init(struct tree_node* parent, int symbol, union value value) {
	struct tree_node* root = (struct tree_node*) malloc(sizeof(struct tree_node));
	root->children_count = 0;
	root->parent = parent;
	root->symbol = symbol;
	root->value = value;

	for(int i = 0; i < RULE_MAX_SYMBOLS; i++)
		root->children[i] = NULL;
	return root;
}

struct tree_node* tree_traverse(struct tree_node* root) {
	if(root->symbol >= 100 && root->children_count == 0)
		return root;

	struct tree_node* tnode;
	for(int i = 0; i < root->children_count; i++) {
		if(root->children[i]->symbol < 100)
			continue;
		tnode = tree_traverse(root->children[i]);
		if(tnode != NULL)
			return tnode;
	}
	return NULL;
}

void tree_print(struct tree_node* root, FILE* fp, int reset) {
	static int tabcount = 0;
	if(reset) tabcount = 0;
	if(root == NULL) return;

	int temptab = tabcount;
	while(temptab--)
		fprintf(fp, "\t");

	print_symbol(fp, root->symbol, root->value);
	fprintf(fp, "\n");

	for(int i = 0; i < root->children_count; i++) {
		tabcount++;
		tree_print(root->children[i], fp, 0);
	}
	tabcount--;
	return;
}

void tree_write(struct tree_node* root, char* filename) {
	FILE *fp = fopen(filename, "w+");
	tree_print(root, fp, 1);
	fclose(fp);
}
