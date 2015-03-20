#include "parser.h"
#include "stack.h"

int parse_table[NONTERM_COUNT][TERM_COUNT];

const char* nonterm_names[] = {"Program", "Functions", "FunctionDef", "fnReturn", "Statements", "moreStmts", "Stmt", "ReturnStmt", "BreakStmt", "DeclarationStmt", "moreDeclarations", "mutMod", "Declaration", "moreTypes", "AssignStmtType2", "listTypes", "typeList", "moreList", "singleAssn", "multAssn", "moreAssn", "IDStmts", "IDStmts2", "Index", "moreIndex", "AssignStmtType1", "FunCall", "MethodCall", "FunCallStmt", "MethodStmt", "Type", "parameterList", "remainingList", "IfStmt", "ElseStmt", "IStmt", "OStmt", "value", "array", "IDList", "moreIds", "arithExpn", "moreTerms", "arithTerm", "moreFactors", "factor", "opLow", "relType", "opHigh", "boolExpn", "logicalOp", "relationalOp", "LoopStmt", "grid", "rows", "moreRows", "row", "moreNums", "boolean"};

const int rules[RULE_COUNT][RULE_MAX_SYMBOLS] = {
	{ Functions, MAIN, OPAREN, CPAREN, OBRACE, Statements, CBRACE },
	{ FunctionDef, Functions },
	{ EPSILON },
	{ FN, ID, OPAREN, parameterList, CPAREN, fnReturn, OBRACE, Statements, CBRACE },
	{ RARROW, Type },
	{ EPSILON },
	{ Stmt , moreStmts },
	{ Stmt , moreStmts },
	{ EPSILON },
	{ COMMENT },
	{ ID , IDStmts },
	{ OStmt },
	{ LoopStmt },
	{ AssignStmtType2 },
	{ DeclarationStmt },
	{ IStmt },
	{ IfStmt },
	{ ReturnStmt },
	{ BreakStmt },
	{ RETURN, relType, SEMICOLON },
	{ BREAK, SEMICOLON },
	{ LET, mutMod, moreDeclarations },
	{ AssignStmtType2 },
	{ Declaration },
	{ MUT },
	{ EPSILON },
	{ ID, ASSIGNOP, arithExpn, moreTypes, SEMICOLON },
	{ COMMA, Declaration },
	{ EPSILON },
	{ listTypes, ASSIGNOP, OPAREN, multAssn, CPAREN, SEMICOLON },
	{ OPAREN, typeList, CPAREN },
	{ ID, moreList },
	{ COMMA, typeList },
	{ EPSILON },
	{ arithExpn },
	{ singleAssn, moreAssn },
	{ COMMA, multAssn },
	{ COLON, multAssn },
	{ EPSILON },
	{ OSQUARE, ID, CSQUARE, OSQUARE, ID, CSQUARE, AssignStmtType1 },
	{ MethodStmt },
	{ AssignStmtType1 },
	{ FunCallStmt },
	{ Index },
	{ MethodCall },
	{ FunCall },
	{ EPSILON },
	{ OSQUARE, relType, CSQUARE, moreIndex },
	{ relType },
	{ NUM },
	{ OSQUARE, ID, CSQUARE },
	{ EPSILON },
	{ ASSIGNOP, singleAssn, SEMICOLON },
	{ OPAREN, IDList ,CPAREN },
	{ DOT, ID, FunCall },
	{ FunCall, SEMICOLON },
	{ MethodCall, SEMICOLON },
	{ BOOL },
	{ F32 },
	{ I32 },
	{ STRING },
	{ ID, COLON, Type, remainingList },
	{ COMMA, parameterList },
	{ EPSILON },
	{ IF, OPAREN, boolExpn, CPAREN, OBRACE, Statements, CBRACE, ElseStmt },
	{ ELSEIF, OPAREN, boolExpn, CPAREN, OBRACE, Statements, CBRACE, ElseStmt },
	{ ELSE, OBRACE , Statements, CBRACE },
	{ EPSILON },
	{ SCAN, OPAREN, ID, CPAREN, SEMICOLON },
	{ PRINT, OPAREN, relType, CPAREN, SEMICOLON },
	{ CHARL },
	{ NUM },
	{ STRL },
	{ grid },
	{ array },
	{ boolean },
	{ OSQUARE, multAssn, CSQUARE },
	{ relType, moreIds },
	{ EPSILON },
	{ COMMA, IDList },
	{ EPSILON },
	{ arithTerm , moreTerms },
	{ opLow , arithExpn },
	{ EPSILON },
	{ factor , moreFactors },
	{ opHigh , arithTerm },
	{ EPSILON },
	{ OPAREN , arithExpn, CPAREN },
	{ relType },
	{ MINUS },
	{ PLUS },
	{ ID, IDStmts2 },
	{ value },
	{ DIV },
	{ MUL },
	{ OPAREN, boolExpn, CPAREN, logicalOp, OPAREN, boolExpn, CPAREN },
	{ relType, relationalOp, relType },
	{ AND },
	{ NOT },
	{ OR },
	{ EQ },
	{ GT },
	{ GE },
	{ LT },
	{ LE },
	{ NE },
	{ WHILE, OPAREN, boolExpn, CPAREN, OBRACE, Statements, CBRACE },
	{ OBRACE, rows, CBRACE },
	{ row , moreRows },
	{ COLON, rows },
	{ EPSILON },
	{ NUM, moreNums },
	{ COMMA, NUM, moreNums },
	{ EPSILON },
	{ FALSE },
	{ TRUE }
};

void set_parse_table_cell(int terminal, int token, int rule) {
	parse_table[terminal - 100][token] = rule;
}

void init_parse_table(){
    // initialize undefined positions with -1
    for(int i = 0; i < NONTERM_COUNT; i++)
        for(int j = 0; j < TERM_COUNT; j++)
            parse_table[i][j] = -1;

    set_parse_table_cell(Program, FN, 0);
    set_parse_table_cell(Program, MAIN, 0);
    set_parse_table_cell(Functions, FN, 1);
    set_parse_table_cell(Functions, EPSILON, 2);
    set_parse_table_cell(Functions, MAIN, 2);
    set_parse_table_cell(FunctionDef, FN, 3);
    set_parse_table_cell(fnReturn, RARROW, 4);
    set_parse_table_cell(fnReturn, EPSILON, 5);
    set_parse_table_cell(fnReturn, OBRACE, 5);
    set_parse_table_cell(Statements, LET, 6);
    set_parse_table_cell(Statements, COMMENT, 6);
    set_parse_table_cell(Statements, ID, 6);
    set_parse_table_cell(Statements, IF, 6);
    set_parse_table_cell(Statements, OPAREN, 6);
    set_parse_table_cell(Statements, WHILE, 6);
    set_parse_table_cell(Statements, RETURN, 6);
    set_parse_table_cell(Statements, BREAK, 6);
    set_parse_table_cell(Statements, SCAN, 6);
    set_parse_table_cell(Statements, PRINT, 6);
    set_parse_table_cell(moreStmts, LET, 7);
    set_parse_table_cell(moreStmts, COMMENT, 7);
    set_parse_table_cell(moreStmts, ID, 7);
    set_parse_table_cell(moreStmts, IF, 7);
    set_parse_table_cell(moreStmts, OPAREN, 7);
    set_parse_table_cell(moreStmts, WHILE, 7);
    set_parse_table_cell(moreStmts, RETURN, 7);
    set_parse_table_cell(moreStmts, BREAK, 7);
    set_parse_table_cell(moreStmts, SCAN, 7);
    set_parse_table_cell(moreStmts, PRINT, 7);
    set_parse_table_cell(moreStmts, EPSILON, 8);
    set_parse_table_cell(moreStmts, CBRACE, 8);
    set_parse_table_cell(Stmt, COMMENT, 9);
    set_parse_table_cell(Stmt, ID, 10);
    set_parse_table_cell(Stmt, PRINT, 11);
    set_parse_table_cell(Stmt, WHILE, 12);
    set_parse_table_cell(Stmt, OPAREN, 13);
    set_parse_table_cell(Stmt, LET, 14);
    set_parse_table_cell(Stmt, SCAN, 15);
    set_parse_table_cell(Stmt, IF, 16);
    set_parse_table_cell(Stmt, RETURN, 17);
    set_parse_table_cell(Stmt, BREAK, 18);
    set_parse_table_cell(ReturnStmt, RETURN, 19);
    set_parse_table_cell(BreakStmt, BREAK, 20);
    set_parse_table_cell(DeclarationStmt, LET, 21);
    set_parse_table_cell(moreDeclarations, OPAREN, 22);
    set_parse_table_cell(moreDeclarations, ID, 23);
    set_parse_table_cell(mutMod, MUT, 24);
    set_parse_table_cell(mutMod, EPSILON, 25);
    set_parse_table_cell(mutMod, ID, 25);
    set_parse_table_cell(mutMod, OPAREN, 25);
    set_parse_table_cell(Declaration, ID, 26);
    set_parse_table_cell(moreTypes, COMMA, 27);
    set_parse_table_cell(moreTypes, EPSILON, 28);
    set_parse_table_cell(moreTypes, SEMICOLON, 28);
    set_parse_table_cell(AssignStmtType2, OPAREN, 29);
    set_parse_table_cell(listTypes, OPAREN, 30);
    set_parse_table_cell(typeList, ID, 31);
    set_parse_table_cell(moreList, COMMA, 32);
    set_parse_table_cell(moreList, EPSILON, 33);
    set_parse_table_cell(moreList, CPAREN, 33);
    set_parse_table_cell(singleAssn, TRUE, 34);
    set_parse_table_cell(singleAssn, OBRACE, 34);
    set_parse_table_cell(singleAssn, NUM, 34);
    set_parse_table_cell(singleAssn, ID, 34);
    set_parse_table_cell(singleAssn, OSQUARE, 34);
    set_parse_table_cell(singleAssn, STRL, 34);
    set_parse_table_cell(singleAssn, OPAREN, 34);
    set_parse_table_cell(singleAssn, CHARL, 34);
    set_parse_table_cell(singleAssn, FALSE, 34);
    set_parse_table_cell(multAssn, TRUE, 35);
    set_parse_table_cell(multAssn, OBRACE, 35);
    set_parse_table_cell(multAssn, NUM, 35);
    set_parse_table_cell(multAssn, ID, 35);
    set_parse_table_cell(multAssn, OSQUARE, 35);
    set_parse_table_cell(multAssn, STRL, 35);
    set_parse_table_cell(multAssn, OPAREN, 35);
    set_parse_table_cell(multAssn, CHARL, 35);
    set_parse_table_cell(multAssn, FALSE, 35);
    set_parse_table_cell(moreAssn, COMMA, 36);
    set_parse_table_cell(moreAssn, COLON, 37);
    set_parse_table_cell(moreAssn, EPSILON, 38);
    set_parse_table_cell(moreAssn, CSQUARE, 38);
    set_parse_table_cell(moreAssn, CPAREN, 38);
    set_parse_table_cell(IDStmts, OSQUARE, 39);
    set_parse_table_cell(IDStmts, DOT, 40);
    set_parse_table_cell(IDStmts, ASSIGNOP, 41);
    set_parse_table_cell(IDStmts, OPAREN, 42);
    set_parse_table_cell(IDStmts2, NUM, 43);
    set_parse_table_cell(IDStmts2, ID, 43);
    set_parse_table_cell(IDStmts2, OSQUARE, 43);
    set_parse_table_cell(IDStmts2, DOT, 44);
    set_parse_table_cell(IDStmts2, OPAREN, 45);
    set_parse_table_cell(IDStmts2, EPSILON, 46);
    set_parse_table_cell(IDStmts2, PLUS, 46);
    set_parse_table_cell(IDStmts2, GE, 46);
    set_parse_table_cell(IDStmts2, DIV, 46);
    set_parse_table_cell(IDStmts2, LE, 46);
    set_parse_table_cell(IDStmts2, CSQUARE, 46);
    set_parse_table_cell(IDStmts2, MUL, 46);
    set_parse_table_cell(IDStmts2, CPAREN, 46);
    set_parse_table_cell(IDStmts2, NE, 46);
    set_parse_table_cell(IDStmts2, EQ, 46);
    set_parse_table_cell(IDStmts2, GT, 46);
    set_parse_table_cell(IDStmts2, LT, 46);
    set_parse_table_cell(IDStmts2, COMMA, 46);
    set_parse_table_cell(IDStmts2, COLON, 46);
    set_parse_table_cell(IDStmts2, SEMICOLON, 46);
    set_parse_table_cell(IDStmts2, MINUS, 46);
    set_parse_table_cell(Index, OSQUARE, 47);
    set_parse_table_cell(Index, ID, 48);
    set_parse_table_cell(Index, NUM, 49);
    set_parse_table_cell(moreIndex, OSQUARE, 50);
    set_parse_table_cell(moreIndex, EPSILON, 51);
    set_parse_table_cell(moreIndex, PLUS, 51);
    set_parse_table_cell(moreIndex, GE, 51);
    set_parse_table_cell(moreIndex, DIV, 51);
    set_parse_table_cell(moreIndex, LE, 51);
    set_parse_table_cell(moreIndex, CSQUARE, 51);
    set_parse_table_cell(moreIndex, MUL, 51);
    set_parse_table_cell(moreIndex, CPAREN, 51);
    set_parse_table_cell(moreIndex, NE, 51);
    set_parse_table_cell(moreIndex, EQ, 51);
    set_parse_table_cell(moreIndex, GT, 51);
    set_parse_table_cell(moreIndex, LT, 51);
    set_parse_table_cell(moreIndex, COMMA, 51);
    set_parse_table_cell(moreIndex, COLON, 51);
    set_parse_table_cell(moreIndex, SEMICOLON, 51);
    set_parse_table_cell(moreIndex, MINUS, 51);
    set_parse_table_cell(AssignStmtType1, ASSIGNOP, 52);
    set_parse_table_cell(FunCall, OPAREN, 53);
    set_parse_table_cell(MethodCall, DOT, 54);
    set_parse_table_cell(FunCallStmt, OPAREN, 55);
    set_parse_table_cell(MethodStmt, DOT, 56);
    set_parse_table_cell(Type, BOOL, 57);
    set_parse_table_cell(Type, F32, 58);
    set_parse_table_cell(Type, I32, 59);
    set_parse_table_cell(Type, STRING, 60);
    set_parse_table_cell(parameterList, ID, 61);
    set_parse_table_cell(remainingList, COMMA, 62);
    set_parse_table_cell(remainingList, EPSILON, 63);
    set_parse_table_cell(remainingList, CPAREN, 63);
    set_parse_table_cell(IfStmt, IF, 64);
    set_parse_table_cell(ElseStmt, ELSEIF, 65);
    set_parse_table_cell(ElseStmt, ELSE, 66);
    set_parse_table_cell(ElseStmt, EPSILON, 67);
    set_parse_table_cell(ElseStmt, CBRACE, 67);
    set_parse_table_cell(ElseStmt, LET, 67);
    set_parse_table_cell(ElseStmt, COMMENT, 67);
    set_parse_table_cell(ElseStmt, ID, 67);
    set_parse_table_cell(ElseStmt, IF, 67);
    set_parse_table_cell(ElseStmt, OPAREN, 67);
    set_parse_table_cell(ElseStmt, WHILE, 67);
    set_parse_table_cell(ElseStmt, RETURN, 67);
    set_parse_table_cell(ElseStmt, BREAK, 67);
    set_parse_table_cell(ElseStmt, SCAN, 67);
    set_parse_table_cell(ElseStmt, PRINT, 67);
    set_parse_table_cell(IStmt, SCAN, 68);
    set_parse_table_cell(OStmt, PRINT, 69);
    set_parse_table_cell(value, CHARL, 70);
    set_parse_table_cell(value, NUM, 71);
    set_parse_table_cell(value, STRL, 72);
    set_parse_table_cell(value, OBRACE, 73);
    set_parse_table_cell(value, OSQUARE, 74);
    set_parse_table_cell(value, TRUE, 75);
    set_parse_table_cell(value, FALSE, 75);
    set_parse_table_cell(array, OSQUARE, 76);
    set_parse_table_cell(IDList, ID, 77);
    set_parse_table_cell(IDList, NUM, 77);
    set_parse_table_cell(IDList, CHARL, 77);
    set_parse_table_cell(IDList, STRL, 77);
    set_parse_table_cell(IDList, OBRACE, 77);
    set_parse_table_cell(IDList, OSQUARE, 77);
    set_parse_table_cell(IDList, TRUE, 77);
    set_parse_table_cell(IDList, FALSE, 77);
    set_parse_table_cell(IDList, EPSILON, 78);
    set_parse_table_cell(IDList, CPAREN, 78);
    set_parse_table_cell(moreIds, COMMA, 79);
    set_parse_table_cell(moreIds, EPSILON, 80);
    set_parse_table_cell(moreIds, CPAREN, 80);
    set_parse_table_cell(arithExpn, TRUE, 81);
    set_parse_table_cell(arithExpn, OBRACE, 81);
    set_parse_table_cell(arithExpn, NUM, 81);
    set_parse_table_cell(arithExpn, ID, 81);
    set_parse_table_cell(arithExpn, OSQUARE, 81);
    set_parse_table_cell(arithExpn, STRL, 81);
    set_parse_table_cell(arithExpn, OPAREN, 81);
    set_parse_table_cell(arithExpn, CHARL, 81);
    set_parse_table_cell(arithExpn, FALSE, 81);
    set_parse_table_cell(moreTerms, PLUS, 82);
    set_parse_table_cell(moreTerms, MINUS, 82);
    set_parse_table_cell(moreTerms, EPSILON, 83);
    set_parse_table_cell(moreTerms, CSQUARE, 83);
    set_parse_table_cell(moreTerms, CPAREN, 83);
    set_parse_table_cell(moreTerms, COMMA, 83);
    set_parse_table_cell(moreTerms, COLON, 83);
    set_parse_table_cell(moreTerms, SEMICOLON, 83);
    set_parse_table_cell(arithTerm, TRUE, 84);
    set_parse_table_cell(arithTerm, OBRACE, 84);
    set_parse_table_cell(arithTerm, NUM, 84);
    set_parse_table_cell(arithTerm, ID, 84);
    set_parse_table_cell(arithTerm, OSQUARE, 84);
    set_parse_table_cell(arithTerm, STRL, 84);
    set_parse_table_cell(arithTerm, OPAREN, 84);
    set_parse_table_cell(arithTerm, CHARL, 84);
    set_parse_table_cell(arithTerm, FALSE, 84);
    set_parse_table_cell(moreFactors, DIV, 85);
    set_parse_table_cell(moreFactors, MUL, 85);
    set_parse_table_cell(moreFactors, EPSILON, 86);
    set_parse_table_cell(moreFactors, PLUS, 86);
    set_parse_table_cell(moreFactors, CSQUARE, 86);
    set_parse_table_cell(moreFactors, CPAREN, 86);
    set_parse_table_cell(moreFactors, COMMA, 86);
    set_parse_table_cell(moreFactors, COLON, 86);
    set_parse_table_cell(moreFactors, SEMICOLON, 86);
    set_parse_table_cell(moreFactors, MINUS, 86);
    set_parse_table_cell(factor, OPAREN, 87);
    set_parse_table_cell(factor, TRUE, 88);
    set_parse_table_cell(factor, OBRACE, 88);
    set_parse_table_cell(factor, NUM, 88);
    set_parse_table_cell(factor, ID, 88);
    set_parse_table_cell(factor, OSQUARE, 88);
    set_parse_table_cell(factor, STRL, 88);
    set_parse_table_cell(factor, CHARL, 88);
    set_parse_table_cell(factor, FALSE, 88);
    set_parse_table_cell(opLow, MINUS, 89);
    set_parse_table_cell(opLow, PLUS, 90);
    set_parse_table_cell(relType, ID, 91);
    set_parse_table_cell(relType, TRUE, 92);
    set_parse_table_cell(relType, OBRACE, 92);
    set_parse_table_cell(relType, NUM, 92);
    set_parse_table_cell(relType, OSQUARE, 92);
    set_parse_table_cell(relType, STRL, 92);
    set_parse_table_cell(relType, CHARL, 92);
    set_parse_table_cell(relType, FALSE, 92);
    set_parse_table_cell(opHigh, DIV, 93);
    set_parse_table_cell(opHigh, MUL, 94);
    set_parse_table_cell(boolExpn, OPAREN, 95);
    set_parse_table_cell(boolExpn, TRUE, 96);
    set_parse_table_cell(boolExpn, OBRACE, 96);
    set_parse_table_cell(boolExpn, NUM, 96);
    set_parse_table_cell(boolExpn, ID, 96);
    set_parse_table_cell(boolExpn, OSQUARE, 96);
    set_parse_table_cell(boolExpn, STRL, 96);
    set_parse_table_cell(boolExpn, CHARL, 96);
    set_parse_table_cell(boolExpn, FALSE, 96);
    set_parse_table_cell(logicalOp, AND, 97);
    set_parse_table_cell(logicalOp, NOT, 98);
    set_parse_table_cell(logicalOp, OR, 99);
    set_parse_table_cell(relationalOp, EQ, 100);
    set_parse_table_cell(relationalOp, GT, 101);
    set_parse_table_cell(relationalOp, GE, 102);
    set_parse_table_cell(relationalOp, LT, 103);
    set_parse_table_cell(relationalOp, LE, 104);
    set_parse_table_cell(relationalOp, NE, 105);
    set_parse_table_cell(LoopStmt, WHILE, 106);
    set_parse_table_cell(grid, OBRACE, 107);
    set_parse_table_cell(rows, NUM, 108);
    set_parse_table_cell(moreRows, COLON, 109);
    set_parse_table_cell(moreRows, EPSILON, 110);
    set_parse_table_cell(moreRows, CBRACE, 110);
    set_parse_table_cell(row, NUM, 111);
    set_parse_table_cell(moreNums, COMMA, 112);
    set_parse_table_cell(moreNums, EPSILON, 113);
    set_parse_table_cell(moreNums, CBRACE, 113);
    set_parse_table_cell(moreNums, COLON, 113);
    set_parse_table_cell(boolean, FALSE, 114);
    set_parse_table_cell(boolean, TRUE, 115);
}

struct symbol_tree {
    int symbol;
    struct tree_node* root;
};

static void stack_pushval(struct stack* stack, struct symbol_tree value) {
    struct symbol_tree* value_ptr = (struct symbol_tree *) malloc(sizeof(struct symbol_tree));
    *value_ptr = value;
    return stack_push(stack, value_ptr);
}

static int stack_topval(struct stack* stack) {
    struct symbol_tree* top = stack_top(stack);
    return top->symbol;
}

void print_symbol(FILE* fp, int symbol, union value value) {
  if(symbol >= 100)
    fprintf(fp, "%s ", nonterm_names[symbol - 100]);
  else{
    if(token_hasvalue(symbol))
      print_value(fp, symbol, value);
    else
      fprintf(fp, "%s ", token_names[symbol]);
  }
}

struct tree_node* create_parsetree(FILE* fp) {
	struct stack* stack = stack_init();
	struct token token;
    struct symbol_tree stree;
    struct symbol_tree* stree_ptr;

	int status = gettok(fp, &token);
    union value zero_value;

    stree.symbol = DOLLAR;
	stack_pushval(stack, stree);
    stree.symbol = Program;
	stack_pushval(stack, stree);

	struct tree_node* root = tree_init(NULL, Program, zero_value);
	struct tree_node* temptree = root;

	while(status != -1){
		//stack_print(stack,fp);

		// return root node when succesful
		if(status == 0 && stack_topval(stack) == DOLLAR) {
			return root;
		}

		if(stack_topval(stack) == EPSILON) {
			stack_pop(stack);
		}else if(stack_topval(stack) < 100) {
            stree_ptr = (struct symbol_tree*) stack_top(stack);
			if(stree_ptr->symbol == token.type) {
				
                if(token_hasvalue(token.type))
                {
                    stree_ptr->root->value = token.value;
                }
                stack_pop(stack);
                
				status = gettok(fp, &token);  
			}
			else {
				printf("Error in Terminal for Token %s. Expected Token %s %d:%d\n",token_names[token.type],token_names[stack_topval(stack)],token.linenum,token.colnum);
				return NULL;
			}
		} else {
			int rule = parse_table[stack_topval(stack) - 100][token.type];

			int i = 0;
			if(rule == -1) {
				printf("Error in Nonterminal for Token %s. Expected Token %s %d:%d\n",token_names[token.type],nonterm_names[stack_topval(stack)-100],token.linenum,token.colnum);
				return NULL;
			}

			stack_pop(stack);
			temptree = tree_traverse(root);

			// End of Rule is marked by zero, since no rule has DOLLAR on RHS
            int flag = 0;
			while(rules[rule][i] != 0) {
				temptree->children[i] = tree_init(temptree, rules[rule][i], zero_value);
				i++;
			}
			
			temptree->children_count = i;
			i--;
			
			while(i != -1) {
                stree.symbol = rules[rule][i];
                stree.root = temptree->children[i];
				stack_pushval(stack, stree);
				i--;
			}
		}
	}
	if(status == -1) {
		if(token.type >= 100)
			printf("%s %d %d",nonterm_names[token.type-100],token.linenum,token.colnum);
		else
			printf("%s %d %d", token_names[token.type],token.linenum,token.colnum);
	}
    return NULL;
}