#include "lexer.h"
#include "parser.h"
#include "ast.h"
//#include "symboltable.h"

int main(int argc, char const *argv[]) {
	if(argc < 2) {
		printf("usage: compiler <src>");
		return -1;
	}
	const char* src_file = argv[1];
	FILE* fp = fopen(src_file, "r");
	init_parse_table();
	char parse_file[80], syntax_file[80];
	strcpy(parse_file, src_file);
	strcpy(syntax_file, src_file);
	char* idx = strstr(parse_file, ".c");
	strcpy(idx, "-parse.tree");
	idx = strstr(syntax_file, ".c");
	strcpy(idx, "-syntax.tree");

	struct tree_node* parse_tree = create_parsetree(fp);
	tree_write(parse_tree, parse_file);

	struct tree_node* syntax_tree = create_ast(parse_tree);
	tree_write(syntax_tree, syntax_file);
	// tree_print(syntax_tree, stdout, 1);

	return 0;
}