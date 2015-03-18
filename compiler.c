#include "lexer.h"
#include "parser.h"
#include "ast.h"

int main(int argc, char const *argv[]) {
	// if(argc < 2) {
	// 	printf("usage: compiler <src>");
	// 	return -1;
	// }	
	const char* src_file = argv[1];
	// FILE* fp = fopen(src_file, "r");
	init_parse_table();
	struct tree_node* parse_tree = create_parsetree(stdin);
    tree_print(parse_tree, stdout, 1);

    struct tree_node* syntax_tree = create_ast(parse_tree);
    tree_print(syntax_tree, stdout, 1);

	return 0;
}