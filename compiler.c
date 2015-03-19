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
	char tokens_file[MAX_LEN], parse_file[MAX_LEN], syntax_file[MAX_LEN];
	strcpy(tokens_file, src_file);
	strcpy(parse_file, src_file);
	strcpy(syntax_file, src_file);

	char* idx = strstr(parse_file, ".c");
	strcpy(idx, "-parse.tree");
	idx = strstr(syntax_file, ".c");
	strcpy(idx, "-syntax.tree");
	idx = strstr(tokens_file, ".c");
	strcpy(idx, ".tokens");

	// int status;
	// struct token token;
	// while((status = gettok(fp, &token))) {
	//   if(status == -1) {
	//     printf("Error - Invalid token\n");
	//     break;
	//   }
	//   print_value(stdout,token.type, token.value);
	//   printf("(%2d, %2d)\t %10s\t %s %s \n", token.linenum, token.colnum, token_names[token.type], (token.type >= KWRD_BEGIN ? "Keyword - " : ""), token.lexeme);
	//   bzero(&token, sizeof(token));
	// }
	struct tree_node* parse_tree = create_parsetree(fp);
	if(parse_tree == NULL) return -1;
	tree_write(parse_tree, parse_file);

	struct tree_node* syntax_tree = create_ast(parse_tree);
	tree_write(syntax_tree, syntax_file);
	//tree_print(syntax_tree, stdout, 1);

	return 0;
}