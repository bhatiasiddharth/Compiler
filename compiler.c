#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "analyzer.h"
#include "codegen.h"

int main(int argc, char const *argv[]) {
	if(argc < 2) {
		printf("usage: compiler <src>");
		return -1;
	}
	const char* src_file = argv[1];
	init_parse_table();
	char tokens_file[MAX_LEN], parse_file[MAX_LEN], syntax_file[MAX_LEN], symbols_file[MAX_LEN], codegen_file[MAX_LEN];
	strcpy(tokens_file, src_file);
	strcpy(parse_file, src_file);
	strcpy(syntax_file, src_file);
	strcpy(symbols_file, src_file);
	strcpy(codegen_file, src_file);

	char* idx = strstr(parse_file, ".c");
	strcpy(idx, "-parse.tree");
	idx = strstr(syntax_file, ".c");
	strcpy(idx, "-syntax.tree");
	idx = strstr(tokens_file, ".c");
	strcpy(idx, ".tokens");
	idx = strstr(symbols_file, ".c");
	strcpy(idx, ".symbols");
	idx = strstr(codegen_file, ".c");
	strcpy(idx, ".asm");

	// create parse tree and write tokens also
	struct tree_node* parse_tree = create_parsetree(src_file, tokens_file);
	if(parse_tree == NULL) return -1;
	tree_write(parse_tree, parse_file);

	struct tree_node* syntax_tree = create_ast(parse_tree);
	tree_write(syntax_tree, syntax_file);
	//tree_print(syntax_tree, stdout, 1);

  //printSymTab(tables, stdout);
	symtab_write(symbols_file, syntax_tree);
	codegen_write(codegen_file, syntax_tree);
	return 0;
}
